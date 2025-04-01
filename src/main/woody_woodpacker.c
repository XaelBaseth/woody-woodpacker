# include "../../inc/woody_woodpacker.h"

int main(int argc, char *argv[]) {
	t_file		file;
	t_payload	payload;
	(void)payload;

	ft_bzero(&file, sizeof(t_file));
	if (argc != 2 || check_file(argv[1], &file)) {
		panic("Correct usage : ./woody_woodpacker <filename>");
	}
	return 0;
}