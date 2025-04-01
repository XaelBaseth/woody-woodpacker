#ifndef ENCRYPTION_H
# define ENCRYPTION_H

/*-----------------------------------------------------------------------------
                                LIBRARIES
-----------------------------------------------------------------------------*/

# include <elf.h>

/*-----------------------------------------------------------------------------
                                MACROS
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                                STRUCTURES
-----------------------------------------------------------------------------*/

typedef struct s_elf	t_elf;
typedef struct s_woody	t_woody;
typedef struct s_file	t_file;

typedef struct s_payload {
	char		*code;
	size_t		size;
	uint16_t	i_tsize;
	uint16_t	i_ksize;
	uint16_t	i_text;
	uint16_t	i_key;
	uint16_t	i_jmp;
}				t_payload;

/*-----------------------------------------------------------------------------
                                FUNCTIONS
-----------------------------------------------------------------------------*/

void	inject(t_file *file, t_payload *payload);
int		is_text(Elf64_Phdr *phdr);
int		is_data(Elf64_Phdr *phdr);
int		is_text_32(Elf32_Phdr *phdr);
int		is_data_32(Elf32_Phdr *phdr);
Elf64_Phdr	*get_segment(t_file *file, int (*f)(Elf64_Phdr *));
Elf64_Phdr	*get_last_load_segment(t_file *file);
Elf32_Phdr	*get_segment_32(t_file *file, int (*f)(Elf32_Phdr *));
Elf32_Phdr	*get_last_load_segment_32(t_file *file);

#endif