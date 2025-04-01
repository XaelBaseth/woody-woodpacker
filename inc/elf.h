#ifndef ELF_H
# define ELF_H

/*-----------------------------------------------------------------------------
                                STRUCTURES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                                MACROS
-----------------------------------------------------------------------------*/

# define KEY_SIZE	32

/*-----------------------------------------------------------------------------
                                STRUCTURES
-----------------------------------------------------------------------------*/

typedef struct s_payload	t_payload;
typedef	uint16_t			t_arch;
typedef uint16_t			t_endian;

typedef struct s_elf {
	void		*ptr;
	void		*end;
	void		*text;
	void		*note;
	char		key[KEY_SIZE];
	off_t		size;
	t_arch		arch;
	t_endian	endian;
	int			fd;
}				t_elf;

typedef struct s_file {
	void		*ptr;
	void		*end;
	void		*text;
	void		*note;
	char		key[KEY_SIZE];
	off_t		size;
	t_arch		arch;
	t_endian	endian;
	int			fd;
}				t_file;

/*-----------------------------------------------------------------------------
                                FUNCTIONS
-----------------------------------------------------------------------------*/

int		check_file(const char *filename, t_file *file);

#endif