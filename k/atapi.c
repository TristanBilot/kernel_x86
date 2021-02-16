#include <k/atapi.h>
#include <stdbool.h>
#include <k/types.h>
#include <k/iso9660.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "memory.h"
#include "init_tables.h"
#include <k/fs.h>

#define NB_DRIVES 2
#define NB_REGISTERS 2

u16 main_register;

void *memcpy(void *dest, const void *src, size_t n)
{
	const char *s = src;
	char *d = dest;

	for (size_t i = 0; i < n; i++)
		*d++ = *s++;

	return dest;
}

void init_atapi(multiboot_info_t *info) {
    memory_init(info);
    atapi_polling();

    open("/bin/hunter", 0);
}


/// Loop to find enabled atapi drives and set the main register
/// when found.
void atapi_polling(void) {
    u8 drives[NB_DRIVES] = {ATA_PORT_MASTER, ATA_PORT_SLAVE};
    u16 registers[NB_REGISTERS] = {PRIMARY_REG, SECONDARY_REG};
    for (u8 i = 0; i < NB_DRIVES; i++) {
        for (u8 j = 0; j < NB_DRIVES; j++)
            if (is_atapi_drive(registers[i], drives[j]))
           {
               main_register = registers[i];
               //printf("main register -> %d\n", main_register);
               return;
           }
    }
}

/// Return whether an entry set (bus/drive) is a valid drive.
bool is_atapi_drive(u16 bus, u8 drive) {
  select_drive(bus, drive);
  u16 sig[4];
  sig[0] = inb(ATA_REG_SECTOR_COUNT(bus));
  sig[1] = inb(ATA_REG_LBA_LO(bus));
  sig[2] = inb(ATA_REG_LBA_MI(bus));
  sig[3] = inb(ATA_REG_LBA_HI(bus));
  u16 cmp[4];
  cmp[0] = ATAPI_SIG_SC;
  cmp[1] = ATAPI_SIG_LBA_LO;
  cmp[2] = ATAPI_SIG_LBA_MI;
  cmp[3] = ATAPI_SIG_LBA_HI;
  return !memcmp(sig, cmp, sizeof(sig));
}

/// Setup the right drive (primary or secondary) with the
/// associated values.
void select_drive(u16 bus, u8 drive) {
  u16 dcr = bus == PRIMARY_REG ? PRIMARY_DCR : SECONDARY_DCR;
  outb(dcr, SRST);
  outb(dcr, DISABLE_IRQ);
  outb(ATA_REG_DRIVE(bus), drive);
  wait_device_selection();
}

/// Wait while the busy status on the input drive is not
/// set to true.
void busy_wait(u16 drive) {
    u8 status;
    do
    {
        status = inb(ATA_REG_STATUS(drive));
    }
    while(status & BSY);
}

/// Wait 400 ns.
void wait_device_selection() {
    inb(PACKET);
    inb(PACKET);
    inb(PACKET);
    inb(PACKET);
}

// Wait for BSY cleared and DRQ set.
void wait_packet_request(u16 drive) {
    u8 status;
    do
    {
        status = inb(ATA_REG_STATUS(drive));
    }
    while((status & BSY) || !(status & DRQ));
}

/// quand on recoit le paquet, il faut l'ecrire dans le data register word by word with outw
/// jusqu'à la sizeof d'un paquet SCSI
/// Puis read sector count register while != DATA_TRANSMIT 0X2 (attendre que le packet s'envoie)

// lecture du paquet:1h25
// boucle de 1 à 1024 => inw(DATA_REGISTER(port)) while doesn't return PACKET_COMMAND_COMPLETE
// allouer un buffer de 2048 pour stocker le message



// Sends a request in order to read the packet later.
int send_packet(struct SCSI_packet *pkt, u16 drive, u16 size) {
    busy_wait(main_register);
    
    // Setup registers.
    outb(ATA_REG_FEATURES(main_register), 0); /* No overlap/no DMA */
    outb(ATA_REG_SECTOR_COUNT(main_register), 0); /* No queuing */
    outb(ATA_REG_LBA_MI(main_register), CD_BLOCK_SZ);
    outb(ATA_REG_LBA_HI(main_register), CD_BLOCK_SZ >> 8);
    outb(ATA_REG_COMMAND(main_register), 0xa0); /* PACKET */
    wait_packet_request(main_register);

    // Send the packet word by word (2Bytes).
    for (size_t i = 0; i < sizeof(*pkt) / 2; i++)
        outw(ATA_REG_DATA(main_register), ((u16*)pkt)[i]);

    // Wait the packet to be sent.
    u8 status;
    do {
        /* Read Sector Count Reg while it doesn’t return DATA TRANSMIT */
        status = inb(ATA_REG_SECTOR_COUNT(main_register));
    } while(status != PACKET_DATA_TRANSMIT);
    // printf("Packet transmitted !\n");
    return 0;
}

// return a buffer with data fetched from the given lba.
u16 *read_block(size_t lba)
{
    // Init of the sent packet.
    int i = 0;
    void *_low_limit = memory_reserve(CD_BLOCK_SZ);
    struct cache *sentPacketCache = cache_new(_low_limit, 1, CD_BLOCK_SZ);
    struct SCSI_packet *sentpacket = cache_alloc(sentPacketCache);
    sentpacket->op_code = READ_12;
    sentpacket->flags_lo = DPO;
    sentpacket->lba_hi = lba >> 24; // Numéro du premier bloc à lire
    sentpacket->lba_mihi = lba >> 16;
    sentpacket->lba_milo = lba >> 8;
    sentpacket->lba_lo = lba;
    sentpacket->transfer_length_hi = 0; // Nombre de blocs contigus à lire
    sentpacket->transfer_length_mihi = 0;
    sentpacket->transfer_length_milo = 0;
    sentpacket->transfer_length_lo = 1;

    send_packet(sentpacket, ATA_PORT_MASTER, 0);


    // init received packet.
    // _low_limit = memory_reserve(CD_BLOCK_SZ);
    // struct cache *receivedPacketCache = cache_new(_low_limit, 1, CD_BLOCK_SZ);
    // printf("hup");
    u16 *receivedpacket = memory_reserve(CD_BLOCK_SZ);// cache_alloc(receivedPacketCache);
    // read packet.
    for (; i < CD_BLOCK_SZ / 2; i++)
    {
        u16 aux = inw(ATA_REG_DATA(main_register));
        receivedpacket[i] = aux;
    }
    u8 status;
    while ((status = inb(ATA_REG_SECTOR_COUNT(main_register))) != PACKET_COMMAND_COMPLETE)
    {
        u16 aux = inw(ATA_REG_DATA(main_register));
        receivedpacket[i++] = aux;
    }
    // printf("Packet received: %s\n", receivedpacket);
    return receivedpacket;
}
