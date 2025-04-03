#ifndef ENCRYPTION_H
# define ENCRYPTION_H

/*-----------------------------------------------------------------------------
                                LIBRARIES
-----------------------------------------------------------------------------*/

# include <elf.h>

/*-----------------------------------------------------------------------------
                                MACROS
-----------------------------------------------------------------------------*/

# define DECRYPTION_CODE "\x50\x56\x57\x52\x51\x41\x50\x41\x51\xeb\x55\x5e\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\xba\x0e\x00\x00\x00\x0f\x05\xb9\x2b\x00\x00\x00\xbe\x04\x00\x00\x00\x48\x8d\x15\x09\x00\x00\x00\x4d\x31\xc0\x48\x31\xc0\xeb\x3d\x5f\x42\x8a\x04\x07\x00\xe0\x30\x02\x48\xff\xc2\x49\xff\xc0\x49\x39\xf0\x75\x06\x80\xc4\x2a\x4d\x31\xc0\xe2\xe5\x41\x59\x41\x58\x59\x5a\x5f\x5e\x58\xe9\x26\x00\x00\x00\xe8\xa6\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\xbe\xff\xff\xff"
# define CODE_SIZE sizeof(DECRYPTION_CODE) -1 + KEY_SIZE

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

extern void			encrypt(void *data, uint32_t data_len, void *text, uint32_t text_len);

void	inject(t_file *file, t_payload *payload);
void	encrypt_code(t_file *file);
int		is_text(Elf64_Phdr *phdr);
int		is_data(Elf64_Phdr *phdr);
Elf64_Phdr	*get_segment(t_file *file, int (*f)(Elf64_Phdr *));
Elf64_Phdr	*get_last_load_segment(t_file *file);

#endif