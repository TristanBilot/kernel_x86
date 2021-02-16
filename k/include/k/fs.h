#ifndef FS_H
# define FS_H

# define MAX_FILE_LENGTH 128

int open(const char *pathname, int flags);
size_t read(int fd, void *buf, size_t count);
// off_t seek(int fd, off_t offset, int whence);
int close(int fd);


#endif

