#include "../../inc/woody_woodpacker.h"

/* *
* Declares an error message and quit the application.
*
* @param const char *msg : The error message descripting the
* source of the error
 * */
void    panic(const char *msg) {
    ft_printf(RED "[Error]:\t%s\n" RESET, msg);
    exit(1);
}
/* *
* Checks if the given file can be opened, read and mapped in
* the memory of the application.
*
 * @ param const char *filename : given file.
*  @ param t_file *file: 
 * */
int    check_file(const char *filename, t_file *file) {
    if ((file-fd = open(filename, O_RDONLY)) == -1)
        panic("Error while opening the file");
    if ((file->size = lseek(file->fd, 0, SEEK_END)) == (off_t)-1)
        panic("Error while reading the file");
    if ((file->ptr = mmap(NULL, file->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file->fd, 0)) == MAP_FAILED)
        panic("Error while mapping the file");
    file->end = file->ptr + file->size;
    return (parse_header(file));
}