#include "../../inc/woody_woodpacker.h"

/* *
* Checks if the given file can be opened, read and mapped in
* the memory of the application.
*
* @param *filename const char: given file.
* @param file t_file*: Pointer to a structure that will store 
*        the file descriptor, size, and memory-mapped pointer.
*
* @return int: success or failure of further file processing.
* */
int	check_file(const char *filename, t_file *file) {
    if ((file->fd = open(filename, O_RDONLY)) == -1)
        panic("Error while opening the file");
    if ((file->size = lseek(file->fd, 0, SEEK_END)) == (off_t)-1)
        panic("Error while reading the file");
    if ((file->ptr = mmap(NULL, file->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file->fd, 0)) == MAP_FAILED)
        panic("Error while mapping the file");
    file->end = file->ptr + file->size;
    return (parse_header(file));
}

