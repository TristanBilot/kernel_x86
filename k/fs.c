#include <k/iso9660.h>
#include <k/atapi.h>
#include <stdbool.h>
#include <k/types.h>
#include <stdio.h>
#include "io.h"
#include "memory.h"
#include "init_tables.h"
#include <k/fs.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>




struct iso_dir *descriptor_table[2048];

bool is_delimiter(char c) {
    return c == '/' || c == '\\';
}

void reverse(char *s)
{
   int length, c;
   char *begin, *end, temp;
 
   length = strlen(s);
   begin  = s;
   end    = s;
 
   for (c = 0; c < length - 1; c++)
      end++;
 
   for (c = 0; c < length/2; c++)
   {        
      temp   = *end;
      *end   = *begin;
      *begin = temp;
 
      begin++;
      end--;
   }
}

char **split(const char *pathname) {
    size_t len = strlen(pathname);
    char **splitted = memory_reserve(len * MAX_FILE_LENGTH);
    char tmp[MAX_FILE_LENGTH];
    memset(tmp, 0, MAX_FILE_LENGTH);
    int index_count = 0;
    int total = 0;
    for (int i = len - 1; i >= 0; i--) {
        /*if (i == 0)
            while (is_delimiter(pathname[i]))
                i++;*/
        if (is_delimiter(pathname[i]) || i == 0) {
            if (i == 0 && !is_delimiter(pathname[i]))
                tmp[index_count++] = pathname[i];
            char *subpath = memory_reserve(strlen(tmp) + 1);
            
            strncpy(subpath, tmp, strlen(tmp) + 1);
            reverse(subpath);
            splitted[total++] = subpath;
            memset(tmp, 0, MAX_FILE_LENGTH);
            index_count = 0;
        }
        else {
            tmp[index_count++] = pathname[i];
        }
    }
    splitted[total] = NULL;
    return splitted;
}

void dump_file(struct iso_dir *file) {
    if (file->idf_len == 1 && file->idf[0] == (char)1)
        return;
    
    for (u8 j = 0; j < file->idf_len; j++)
        printf("%c", file->idf[j]);
    printf("%s%s\n", file->idf_len < 8 ? "\t\t" : "\t", file->type == ISO_FILE_ISDIR ? "(dir)" : "(file)");
}

void dump_splitted(char **splitted, char *pathname) {
    printf("splitted of: %s\n", pathname);
    while (*splitted) {
        printf("%s ", *splitted);
        *splitted++;
    }
    printf("\n");
}

char *get_file_name(char *idf, u8 idf_len) {
    char *buffer = memory_reserve(idf_len) + 1;
    strncpy(buffer, idf, idf_len);
    buffer[idf_len] = '\0';
    return buffer;
}

struct iso_dir *get_file(char **path, struct iso_dir *file, int index) {
    char *next_subpath = path[index];
    bool is_end = !path[index + 1];
    printf("[+] Enter in get_file() `%s` compared to `%s` dir at index %d\n", next_subpath, get_file_name(file->idf, file->idf_len), index);
    printf('Current path: ');
    for (int i = 0; i < index; i++)
        printf("%s/", path[i]);
    printf("\n\n");

    /* files */
    if (file->type != ISO_FILE_ISDIR) {
        if (is_end && strcmp(next_subpath, get_file_name(file->idf, file->idf_len)) == 0)
            return file;
        return NULL;
    }
    
    /* directories */
    while (file->dir_size > 0) {
        file = (struct iso_dir *)((char *)file + file->dir_size);
        printf("comparing %s (%d) and %s (%d): \t\t%s\n", next_subpath, strlen(next_subpath), get_file_name(file->idf, file->idf_len), strlen(get_file_name(file->idf, file->idf_len)), strcmp(next_subpath, get_file_name(file->idf, file->idf_len)) == 0 ? "true" : "false");
        if (strcmp(next_subpath, get_file_name(file->idf, file->idf_len)) == 0) {
            if (is_end) {
                printf("--> MATCH: `%s` at index %d\n", next_subpath, index);
                return file; /* check later if its dir of file */
            }                   
            else {
                printf("--> ENTER DIR `%s` at index %d\n", next_subpath, index);
                file = (struct iso_dir *)((char *)file + file->dir_size);
                file = (struct iso_dir *)((char *)file + file->dir_size);
                return get_file(path, file, index + 1);
            }
        }
    }
    return NULL;
}

void dump_path(struct iso_dir *file, struct iso_path_table_le *pte, int files_count) {
    printf("[+] %s n°%d: \t", file->type == ISO_FILE_ISDIR ? "Directory" : "File", files_count);
    for (u8 j = 0; j < pte->idf_len; j++)
        printf("%c", pte->idf[j]);
    printf("\n");
    while (file->dir_size > 0) {
        file = (struct iso_dir *)((char *)file + file->dir_size);
        dump_file(file);
    }
}

int get_splitted_length(char **splitted) {
    int i = 0;
    while (splitted[i])
        i++;
    return i;
}

struct path_table_le *path_table[2048];
int path_table_length = 0;

void fill_path_table() {
    // Path table fetching
    struct iso_prim_voldesc *voldesc = read_block(ISO_PRIM_VOLDESC_BLOCK);
    char *path_table = memory_reserve(voldesc->path_table_size.le);
    char *buffer = read_block(voldesc->le_path_table_blk);
    memcpy(path_table, buffer, voldesc->path_table_size.le);
    struct iso_path_table_le *pte = (struct iso_path_table_le *)path_table;
    int index = 0;

    while ((u32)((char *)pte - (char *)path_table) < voldesc->path_table_size.le) {
        struct iso_path_table_le *copy = pte;
        path_table[index++] = copy;
        pte = (struct iso_path_table_le *)((char *)pte + sizeof(struct iso_path_table_le) + pte->idf_len + pte->idf_len % 2);
    }
    path_table[index] = NULL;
    path_table_length = index;
}

int open(const char *pathname, int flags) {
    // Path table fetching
    fill_path_table();
    
    char **splitted = split(pathname);
    int path_count = get_splitted_length(splitted);
    char *desired_file_name = splitted[0];
    dump_splitted(splitted, pathname);

    struct iso_path_table_le *pte = NULL;
    
    for (int j = 0; j < path_table_length; j++) {
        pte = path_table[j];
        struct iso_dir *dir = read_block(pte->data_blk);
        if (dir->type == ISO_FILE_ISDIR) {
            dump_path(dir, pte, path_table_length);
            
            /* start at 1 because 0 is the file name */
            int i = 1;
            struct iso_path_table_le *pte_copy = pte;
            while (splitted[i] && strcmp(splitted[i], get_file_name(pte_copy->idf, pte_copy->idf_len)) == 0) {
                i++;
                // struct iso_dir *subdir = read_block(pte_copy->data_blk);
                printf("In loop: %s\n", get_file_name(pte_copy->idf, pte_copy->idf_len));

                pte_copy = read_block(pte_copy->parent_dir);

                if (i == path_table_length) { /* arrived at root */
                    while (dir->dir_size > 0) { /* loop over the end of path directory looking for the file */
                        dir = (struct iso_dir *)((char *)dir + dir->dir_size);
                        if (strcmp(get_file_name(dir->idf, dir->idf_len), desired_file_name) == 0) { /* found the file */
                            /* dir is the file we want to store to the array */
                        }
                    }
                }
            }
        } else {
            printf("this is a file: %s\n", get_file_name(dir->idf, dir->idf_len));
        }
    }
    printf("nb directories in path table: %d\n", path_table_length);
}

// size_t read(int fd, void *buf, size_t count)
// {
//      if (count == 0)
//          return NULL;
//      struct iso_dir *file = read_block(descriptor_table[fd]);
//      struct endian32 *ptr= file->data_blk.le;
//      memcpy(buf, ptr, count);
//      size_t len_buf = strlen(buf);
//
// }

// off_t seek(int fd, off_t offset, int whence)
// {
    
// }

// int close(int fd)
// {
    
// }
