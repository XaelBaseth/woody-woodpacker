#include "../../inc/woody_woodpacker.h"

/* *
 * Declares an error message and quit the application.
 *
 * @param `*msg` const char : The error message descripting the source of the error.
 * */
void	panic(const char *msg) {
	ft_printf(RED "[Error]:\t%s\n" RESET, msg);
	exit(1);
}

/* *
 * Prints a character to the stdout.
 *
 * @param `c` char : Character to print. 
 * */
void ft_putchar(char c) {
    write(1, &c, 1);
}

/* *
 * Prints a strings to the stdout.
 * 
 * @param `*str`: string to be printed
 * */
void	ft_putstr(char *str) {
	while (*str)
		write(1, str++, 1);
}

/* *
 * @brief Generates the decryption payload based on the ELF architecture.
 *
 * @param file Pointer to the `t_file` struct containing ELF metadata.
 * @return A `t_payload` structure containing:
 *         - `code`: Pointer to dynamically allocated memory holding the decryption shellcode.
 *         - `size`: The total size of the payload, including the key.
 *         - Encryption key values (0x1e, 0x23, etc.).
 * */
t_payload	get_payload(t_file *file) {
	int	size;
	t_payload	payload;

	size = CODE_SIZE;
	if (file->arch == ELFCLASS64) {
		payload = (t_payload){malloc(size), size, 0x1e, 0x23, 0x2a, 0x78, 0x5c};
		ft_memcpy(payload.code, DECRYPTION_CODE, size);
		return (payload);
	}
	size = CODE_SIZE_32;
	payload = (t_payload){malloc(size), size, 0x16, 0x1b, 0x26, 0x67, 0x47};
	return (payload);
}

/**
 * Free the payload and reset its pointer to NULL.
 * 
 * @param `*payload[]`: Adress of the string pointer to free.
 */
void	free_payload(char *payload[]) {
	if (payload == NULL)
		return;
	if (payload && *payload){
		free(*payload);
		*payload = NULL;
	}
}