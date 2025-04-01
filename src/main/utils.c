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
