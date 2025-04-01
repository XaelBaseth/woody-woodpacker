#ifndef WOODY_WOODYPACKER_H
# define WOODY_WOODPACKER_H

/*-----------------------------------------------------------------------------
                                LIBRAIRIES
-----------------------------------------------------------------------------*/

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/libft.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/ft_gc_alloc.h"

# include "elf.h"
# include "encryption.h"

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/mman.h>
# include <string.h>
# include <stdint.h>
# include <linux/elf.h>

/*-----------------------------------------------------------------------------
                                MACROS
-----------------------------------------------------------------------------*/

# define RESET   "\x1b[0m"
# define RED     "\x1b[31m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"

/*-----------------------------------------------------------------------------
                                STRUCTURES
-----------------------------------------------------------------------------*/
typedef struct s_elf		t_elf;
typedef struct s_file		t_file;
typedef struct s_payload	t_payload;

typedef struct s_woody {
	void			*ptr;
	size_t			size;
	Elf64_Phdr		*data;
	Elf64_Ehdr		*last;
	Elf32_Ehdr		*data_32;
	Elf32_Ehdr		*last_32;
}				t_woody;


/*-----------------------------------------------------------------------------
                                FUNCTIONS
-----------------------------------------------------------------------------*/

void	panic(const char *msg);

#endif