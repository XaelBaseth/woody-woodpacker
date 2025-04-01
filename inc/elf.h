	#ifndef ELF_H
	# define ELF_H

	/*-----------------------------------------------------------------------------
									LIBRARIES
	-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------
									MACROS
	-----------------------------------------------------------------------------*/

	# define KEY_SIZE	32
	# define LENDIAN	ELFDATA2LSB
	# define BENDIAN	ELFDATA2MSB

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

	int			check_file(const char *filename, t_file *file);
	uint16_t	get_uint16(uint16_t byte, t_endian endian);
	int32_t		get_int32(int32_t byte, t_endian endian);
	uint32_t	get_uint32(uint32_t byte, t_endian endian);
	uint64_t	get_uint64(uint64_t byte, t_endian endian);

	#endif