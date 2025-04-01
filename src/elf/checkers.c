#include "../../inc/woody_woodpacker.h"

/* *
 * Parses the ELF header of a given file to determine its validity 
 * and architecture details.
 *
 * @param file t_file*: Pointer to the structure containing the mapped file.
 *
 * @return int: 
 *         `EXIT_SUCCESS` if the ELF header is valid and supported.
 *         `EXIT_FAILURE` if the ELF file is invalid or unsupported.
 *
 * */
static int	parse_header(t_file *file) {
	unsigned char	*ident;
	Elf64_Ehdr		*header;
	char			magic[4] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	ident = file->ptr;
	header = file->ptr;
	file->arch = ident[EI_CLASS];
	file->endian = ident[EI_DATA];
	errno = ENOEXEC;
	if (ft_memcmp(magic, ident, 4) || ident[EI_CLASS] == ELFCLASSNONE
		|| ident[EI_DATA] == ELFDATANONE || ident[EI_VERSION] == EV_NONE
		|| !(get_uint16(header->e_machine, file->endian) == EM_386
			|| get_uint16(header->e_machine, file->endian) == EM_X86_64)
		|| get_uint32(header->e_version, file->endian) == EV_NONE
		|| get_uint16(header->e_phnum, file->endian) == 0)
		return (EXIT_FAILURE);
	if (header->e_type != ET_EXEC && header->e_type != ET_DYN)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* *
 * Checks if the given file can be opened, read and mapped in
 * the memory of the application.
 *
 * @param `*filename` const char: given file.
 * @param `file` t_file*: Pointer to a structure that will store 
 *        the file descriptor, size, and memory-mapped pointer.
 *
 * @return int: success or failure of further file processing.
 * 
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

