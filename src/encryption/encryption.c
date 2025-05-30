# include "../../inc/woody_woodpacker.h"

/* *
 * This function reads `KEY_SIZE` bytes from `/dev/urandom` and stores them in `file->key`.
 * It then prints the key in hexadecimal format.
 *
 * @param file Pointer to the `t_file` structure that holds the encryption key.
 * */
static void	get_key(t_file *file) {
	int		fd;
	int		i;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
		panic("Failed to open the file.");
	read(fd, file->key, KEY_SIZE);
	close(fd);
	ft_putstr("encryption key : 0x");
	i = -1;
	while (++i < KEY_SIZE)
		printf("%02hhx", file->key[i]);
	fflush(NULL);
	ft_putchar('\n');
}


/* *
 * This function detects whether the ELF file is 32-bit or 64-bit and calls the
 * appropriate encryption function. If the ELF is 64-bit, it directly encrypts
 * the .text section.
 *
 * @param file Pointer to the `t_file` structure containing the ELF binary.
 * */
void		encrypt_code(t_file *file) {
	void		*text;
	void		*key;
	uint32_t	text_size;
	uint32_t	key_size;

	if (!(file->text = get_segment(file, is_text)))
		panic("Program header table is truncated.");
	((Elf64_Phdr *)file->text)->p_flags |= PF_W;
	text = file->ptr + get_uint64(((Elf64_Phdr *)file->text)->p_offset,
		file->endian);
	text_size = get_uint64(((Elf64_Phdr *)file->text)->p_filesz, file->endian);
	get_key(file);
	key = &file->key;
	key_size = KEY_SIZE;
	if (text < file->ptr || text > file->end || text + text_size > file->end)
		panic("Program header table is truncated.");
	encrypt(key, key_size, text, text_size);
}