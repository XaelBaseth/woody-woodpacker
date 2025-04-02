# include "../../inc/woody_woodpacker.h"

int	main(int argc, char *argv[]) {
	t_file		file;
	t_payload	payload;

	ft_bzero(&file, sizeof(t_file));
	if (argc != 2 || check_file(argv[1], &file) == EXIT_FAILURE)
		panic("Correct usage : ./woody_woodpacker <filename>\nWhere filename is either a Elf64 or Elf32 bits.");
	encrypt_code(&file);
	payload = get_payload(&file);
	inject(&file, &payload);
	free_payload(&payload.code);
	return (0);
}