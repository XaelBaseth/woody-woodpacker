#include "../../inc/woody_woodpacker.h"

/* *
 * Saves the given data to a file named "woody" by temporarily writing to a file named "woody.tmp" and then renaming it to "woody" upon success.
 * 
 * @param woody char*: Pointer to the data to be written to the file.
 * @param size size_t: The size of the data to be written.
 * 
 * @return int: 
 *         - `EXIT_SUCCESS` if the file was successfully saved.
 *         - `EXIT_FAILURE` if any error occurred during the file operations.
 * */
static int	save_file(char *woody, size_t size) {
	int	fd;
	
	errno = 0;
	if ((fd = open("woody.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0755)) == -1)
		return (EXIT_FAILURE);
	if (syscall(SYS_rename, "woody.tmp", "woody"))
		return (EXIT_FAILURE);
	write(fd, woody, size);
	close(fd);
	syscall(SYS_unlink, "woody.tmp");
	return (EXIT_SUCCESS);
}

/* *
 * Formats the payload for a 32-bit ELF file by calculating relative addresses and embedding necessary data into the payload.
 *
 * @param file t_file*: Pointer to the structure containing the mapped ELF file.
 * @param payload t_payload*: Pointer to the structure that will store the formatted payload.
 * @param entry_addr Elf32_Addr: The entry point address used for calculating relative addresses.
 * */
static void	format_payload_32(t_file *file, t_payload *payload, 	Elf32_Addr entry_addr) {
	uint32_t	last_entry;
	int32_t		rel_entry;
	int32_t		rel_text;
	uint32_t	key_size;
	uint32_t	filesz;
	Elf32_Ehdr	*hdr;

	hdr = file->ptr;
	last_entry = get_uint32(hdr->e_entry, file->endian);
	rel_entry = last_entry - (entry_addr + payload->i_jmp + sizeof(int32_t));
	rel_text = get_uint32(entry_addr + payload->i_text - 2
		- get_uint32(((Elf32_Phdr *)file->text)->p_vaddr, file->endian), file->endian);
	filesz = get_uint32(((Elf32_Phdr *)file->text)->p_filesz, file->endian);
	key_size = KEY_SIZE;
	ft_memcpy(payload->code + payload->i_tsize, &filesz, sizeof(uint32_t));
	ft_memcpy(payload->code + payload->i_ksize, &key_size, sizeof(uint32_t));
	ft_memcpy(payload->code + payload->i_key, &file->key, KEY_SIZE);
	ft_memcpy(payload->code + payload->i_text, &rel_text, sizeof(int32_t));
	ft_memcpy(payload->code + payload->i_jmp, &rel_entry, sizeof(int32_t));
}

/* *
 * Formats the payload for a 64-bit ELF file by calculating relative addresses and embedding necessary data into the payload.
 *
 * @param file t_file*: Pointer to the structure containing the mapped ELF file.
 * @param payload t_payload*: Pointer to the structure that will store the formatted payload.
 * @param entry_addr Elf64_Addr: The entry point address used for calculating relative addresses.
 * */
static void	format_payload(t_file *file, t_payload *payload, Elf64_Addr entry_addr)
{
	uint32_t	last_entry;
	int32_t		rel_entry;
	int32_t		rel_text;
	uint32_t	key_size;
	uint64_t	filesz;
	Elf64_Ehdr	*hdr;

	hdr = file->ptr;
	last_entry = get_uint64(hdr->e_entry, file->endian);
	rel_entry = last_entry - (entry_addr + payload->i_jmp + sizeof(int32_t));
	rel_text = get_uint64(((Elf64_Phdr *)file->text)->p_vaddr, file->endian)
		- (entry_addr + payload->i_text + sizeof(int32_t));
	filesz = get_uint64(((Elf64_Phdr *)file->text)->p_filesz, file->endian);
	key_size = KEY_SIZE;
	ft_memcpy(payload->code + payload->i_tsize, &filesz, sizeof(uint32_t));
	ft_memcpy(payload->code + payload->i_ksize, &key_size, sizeof(uint32_t));
	ft_memcpy(payload->code + payload->i_key, &file->key, KEY_SIZE);
	ft_memcpy(payload->code + payload->i_text, &rel_text, sizeof(int32_t));
	ft_memcpy(payload->code + payload->i_jmp, &rel_entry, sizeof(int32_t));
}

/* *
 * Modifies the given ELF file to create a "woody" file by injecting data into the specified sections, updating the section headers, and ensuring that the file's contents are correctly adjusted.
 *
 * @param file t_file*: Pointer to the structure containing the ELF file data.
 * @param woody t_woody*: Pointer to the structure representing the modified ELF file.
 * @param inject_index size_t*: Pointer to the index at which the injection will occur, updated during the process.
 * */
static void	create_woody_32(t_file *file, t_woody *woody, size_t *inject_index) {
	size_t		file_index;

	*inject_index = get_uint32(woody->data_32->p_offset, file->endian)
		+ get_uint32(woody->data_32->p_filesz, file->endian);
	file_index = *inject_index;
	ft_memcpy(woody->ptr, file->ptr, *inject_index);
	if (woody->data_32 && get_uint32(woody->data_32->p_filesz, file->endian)
		!= get_uint32(woody->data_32->p_memsz, file->endian))
	{
		ft_bzero(woody->ptr + *inject_index, get_uint32(woody->data_32->p_memsz,
			file->endian) - get_uint32(woody->data_32->p_filesz, file->endian));
		*inject_index += get_uint32(woody->data_32->p_memsz, file->endian)
			- get_uint32(woody->data_32->p_filesz, file->endian);
		woody->data_32 = woody->ptr + ((void *)woody->data_32 - file->ptr);
		woody->data_32->p_filesz = woody->data_32->p_memsz;
	}
	if (woody->last_32 != woody->data_32)
	{
		*inject_index += get_uint32(woody->last_32->p_offset, file->endian)
			+ get_uint32(woody->last_32->p_filesz, file->endian) - file_index;
		ft_memcpy(woody->ptr + *inject_index, file->ptr + file_index,
			get_uint32(woody->last_32->p_offset, file->endian)
			+ get_uint32(woody->last_32->p_filesz, file->endian) - file_index);
	}
	woody->last_32 = woody->ptr + ((void *)woody->last_32 - file->ptr);
}

/* *
 * Modifies the given ELF file to create a "woody" file by injecting data into the specified sections, updating the section headers, and ensuring that the file's contents are correctly adjusted for 64-bit architecture.
 *
 * @param file t_file*: Pointer to the structure containing the ELF file data.
 * @param woody t_woody*: Pointer to the structure representing the modified ELF file.
 * @param inject_index size_t*: Pointer to the index at which the injection will occur, updated during the process.
 *
 * */
static void	create_woody(t_file *file, t_woody *woody, size_t *inject_index) {
	size_t		file_index;

	*inject_index = get_uint64(woody->data->p_offset, file->endian)
		+ get_uint64(woody->data->p_filesz, file->endian);
	file_index = *inject_index;
	ft_memcpy(woody->ptr, file->ptr, *inject_index);
	if (woody->data && get_uint64(woody->data->p_filesz, file->endian)
		!= get_uint64(woody->data->p_memsz, file->endian))
	{
		ft_bzero(woody->ptr + *inject_index, get_uint64(woody->data->p_memsz,
			file->endian) - get_uint64(woody->data->p_filesz, file->endian));
		*inject_index += get_uint64(woody->data->p_memsz, file->endian)
			- get_uint64(woody->data->p_filesz, file->endian);
		woody->data = woody->ptr + ((void *)woody->data - file->ptr);
		woody->data->p_filesz = woody->data->p_memsz;
	}
	if (woody->last != woody->data)
	{
		*inject_index += get_uint64(woody->last->p_offset, file->endian)
			+ get_uint64(woody->last->p_filesz, file->endian) - file_index;
		ft_memcpy(woody->ptr + *inject_index, file->ptr + file_index,
			get_uint64(woody->last->p_offset, file->endian)
			+ get_uint64(woody->last->p_filesz, file->endian) - file_index);
	}
	woody->last = woody->ptr + ((void *)woody->last - file->ptr);
}

/* *
 * Injects the given payload into a 32-bit ELF file by modifying the necessary
 * segments, updating program headers, and saving the modified ELF to a new file.
 *
 * @param file t_file*: Pointer to the structure containing the original ELF file data.
 * @param payload t_payload*: Pointer to the payload data that will be injected into the ELF file.
 * */
static void	inject_32(t_file *file, t_payload *payload) {
	t_woody		woody;
	size_t		inject_index;

	if (!(woody.last_32 = get_last_load_segment_32(file)))
		panic("Program header table is truncated.");
	woody.data_32 = get_segment_32(file, is_data_32);
	if (!woody.data_32)
		woody.data_32 = woody.last_32;
	woody.size = get_uint32(woody.last_32->p_offset, file->endian)
		+ get_uint32(woody.last_32->p_filesz, file->endian) + payload->size
		+ (woody.data_32->p_memsz - woody.data_32->p_filesz);
	errno = 0;
	if (!(woody.ptr = malloc(woody.size)))
		panic("Memory allocation failed.");
	format_payload_32(file, payload, get_uint32(woody.last_32->p_vaddr, file->endian)
		+ get_uint32(woody.last_32->p_memsz, file->endian));
	create_woody_32(file, &woody, &inject_index);
	ft_memcpy(woody.ptr + inject_index, payload->code, payload->size);
	((Elf32_Ehdr *)woody.ptr)->e_entry = (get_uint32(woody.last_32->p_vaddr,
		file->endian) + get_uint32(woody.last_32->p_memsz, file->endian));
	woody.last_32->p_filesz += get_uint32(payload->size, file->endian);
	woody.last_32->p_memsz += get_uint32(payload->size, file->endian);
	woody.last_32->p_flags |= PF_X;
	if (save_file(woody.ptr, woody.size) == EXIT_FAILURE)
		panic("Failed to save the modified ELF file.");
	free(woody.ptr);
}

/* *
 * Injects the given payload into an ELF file by modifying the necessary segments, updating program headers, and saving the modified ELF to a new file.
 *
 * @param file t_file*: Pointer to the structure containing the original ELF file data.
 * @param payload t_payload*: Pointer to the payload data that will be injected into the ELF file..
 * */
void	inject(t_file *file, t_payload *payload) {
	t_woody		woody;
	size_t		inject_index;

	if (file->arch == ELFCLASS32)
		return (inject_32(file, payload));
	if (!(woody.last = get_last_load_segment(file)))
		panic("Program header table is truncated.");
	woody.data = get_segment(file, is_data);
	if (!woody.data)
		woody.data = woody.last;
	woody.size = get_uint64(woody.last->p_offset, file->endian)
		+ get_uint64(woody.last->p_filesz, file->endian) + payload->size
		+ (woody.data->p_memsz - woody.data->p_filesz);
	errno = 0;
	if (!(woody.ptr = malloc(woody.size)))
		panic("Memory allocation failed.");
	format_payload(file, payload, get_uint64(woody.last->p_vaddr, file->endian)
		+ get_uint64(woody.last->p_memsz, file->endian));
	create_woody(file, &woody, &inject_index);
	ft_memcpy(woody.ptr + inject_index, payload->code, payload->size);
	((Elf64_Ehdr *)woody.ptr)->e_entry = (get_uint64(woody.last->p_vaddr,
		file->endian) + get_uint64(woody.last->p_memsz, file->endian));
	woody.last->p_filesz += get_uint64(payload->size, file->endian);
	woody.last->p_memsz += get_uint64(payload->size, file->endian);
	woody.last->p_flags |= PF_X;
	if (save_file(woody.ptr, woody.size) == EXIT_FAILURE)
		panic("Failed to save the modified ELF file.");
	free(woody.ptr);
}