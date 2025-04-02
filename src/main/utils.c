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
 */
void	ft_putstr(char *str) {
	while (*str)
		write(1, str++, 1);
}