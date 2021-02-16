#ifndef ATAPI_H_
#define ATAPI_H_

# include <k/compiler.h>
# include <k/types.h>
# include <stdbool.h>

/*
    --- IDE Controller ---
    1F0 (Read and Write): Data Register
    1F1 (Read): Error Register
    1F1 (Write): Features Register
    1F2 (Read and Write): Sector Count Register
    1F3 (Read and Write): LBA Low Register
    1F4 (Read and Write): LBA Mid Register
    1F5 (Read and Write): LBA High Register
    1F6 (Read and Write): Drive/Head Register
    1F7 (Read): Status Register
    1F7 (Write): Command Register
    3F6 (Read): Alternate Status Register
    3F6 (Write): Device Control Register
    --- Status register ---
    BSY (busy)
    DRDY (device ready)
    DF (Device Fault)
    DSC (seek complete)
    DRQ (Data Transfer Requested)
    CORR (data corrected)
    IDX (index mark)
    ERR (error)

    Primary IDE controller: 1F0h to 1F7h and 3F6h to 3F7h
    Secondary IDE controller: 170h to 177h and 376h to 377h
*/


/* Device Control Registers */
# define PRIMARY_DCR 0x3F6
# define SECONDARY_DCR 0x376

/* DCR bits */
# define DISABLE_IRQ (1 << 1)
# define SRST (1 << 2)

/* Drive selection bytes */
# define ATA_PORT_MASTER 0x00
# define ATA_PORT_SLAVE 0x10

/* ATA bus IO ports */
# define PRIMARY_REG 0x1F0
# define SECONDARY_REG 0x170

/* ATA I/O registers */
# define ATA_REG_DATA(PORT) (PORT)
# define ATA_REG_FEATURES(PORT) ((PORT) + 1)
# define ATA_REG_ERROR_INFO(PORT) ((PORT) + 1)
# define ATA_REG_SECTOR_COUNT(PORT) ((PORT) + 2)
# define ATA_REG_SECTOR_NB(PORT) ((PORT) + 3)
# define ATA_REG_LBA_LO(PORT) ((PORT) + 3)
# define ATA_REG_CYLINDER_LOW(PORT) ((PORT) + 4)
# define ATA_REG_LBA_MI(PORT) ((PORT) + 4)
# define ATA_REG_CYLINDER_HIGH(PORT) ((PORT) + 5)
# define ATA_REG_LBA_HI(PORT) ((PORT) + 5)
# define ATA_REG_DRIVE(PORT) ((PORT) + 6)
# define ATA_REG_HEAD(PORT) ((PORT) + 6)
# define ATA_REG_COMMAND(PORT) ((PORT) + 7)
# define ATA_REG_STATUS(PORT) ((PORT) + 7)

/* Status bits */
# define ERR (1 << 0)
# define DRQ (1 << 3)
# define SRV (1 << 4)
# define DF (1 << 5)
# define RDY (1 << 6)
# define BSY (1 << 7)

# define ABRT (1 << 2)

/* ATAPI signature */
# define ATAPI_SIG_SC 0x01
# define ATAPI_SIG_LBA_LO 0x01
# define ATAPI_SIG_LBA_MI 0x14
# define ATAPI_SIG_LBA_HI 0xEB

/* ATA commands */
# define IDENTIFY_PACKET_DEVICE 0xA1
# define PACKET 0xA0

/* SCSI commands */
# define READ_12 0xA8

# define DPO (1 << 4)

# define ATAPI_BLK_CACHE_SZ 256
# define CD_BLOCK_SZ 2048
# define PACKET_SZ 12

# define PACKET_AWAIT_COMMAND 1
# define PACKET_DATA_TRANSMIT 2
# define PACKET_COMMAND_COMPLETE 3

struct SCSI_packet {
	u8 op_code;
	u8 flags_lo;
	u8 lba_hi;
	u8 lba_mihi;
	u8 lba_milo;
	u8 lba_lo;
	u8 transfer_length_hi;
	u8 transfer_length_mihi;
	u8 transfer_length_milo;
	u8 transfer_length_lo;
	u8 flags_hi;
	u8 control;
} __packed;


static inline void enable_interrupts(void) {
    __asm__ volatile("sti");
}

static inline void disable_interrupts(void) {
    __asm__ volatile("cli");
}

void atapi_polling();
bool is_atapi_drive(u16 bus, u8 drive);
void select_drive(u16 bus, u8 drive);
void busy_wait(u16 drive);
void wait_device_selection();
void wait_packet_request(u16 drive);
int send_packet(struct SCSI_packet *pkt, u16 drive, u16 size);
u16 *read_block(size_t lba);


#endif /* !ATAPI_H_ */
