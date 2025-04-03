#include "../../inc/woody_woodpacker.h"

/* *
 * Checks if the given program header corresponds to a text (code) segment.
 * 
 * @param phdr: A pointer to the program header.
 * @return int: 1 if the segment is a text segment, 0 otherwise.
 * */
int	is_text(Elf64_Phdr *phdr) {
	return (phdr->p_type == PT_LOAD && phdr->p_flags & PF_X && phdr->p_flags & PF_R);
}

/* *
 * Checks if the given program header corresponds to a data segment. 
 * A data segment is loadable and has a file size different from 
 * its memory size, indicating that it contains data (not just code).
 * 
 * @param phdr: A pointer to the program header.
 * @return int: 1 if the segment is a data segment, 0 otherwise.
 * */
int	is_data(Elf64_Phdr *phdr) {
	return (phdr->p_type == PT_LOAD && phdr->p_filesz != phdr->p_memsz);
}

/* *
 * Searches for a program header segment in the ELF file based on a given condition.
 * 
 * @param file A pointer to the file structure that contains the ELF file.
 * @param f A function pointer, This function is used to check whether a segment satisfies a condition.
 * @return A pointer to the program header that satisfies the condition, or NULL if no such segment is found.
 * */
Elf64_Phdr	*get_segment(t_file *file, int (*f)(Elf64_Phdr *)) {
	Elf64_Phdr	*phdrs;
	uint16_t	i;
	uint16_t	size;

	i = -1;
	size = ((Elf64_Ehdr *)file->ptr)->e_phnum;
	phdrs = file->ptr + ((Elf64_Ehdr *)file->ptr)->e_phoff;
	if ((void *)phdrs < file->ptr)
		return (NULL);
	while (++i < size && (void *)(phdrs + i + 1) < file->end)
	{
		if (f(phdrs + i))
			return (phdrs + i);
	}
	return (NULL);
}

/* *
 * Retrieves the last loadable program header segment in the ELF file.
 * 
 * @param file A pointer to the file structure that contains the ELF file.
 * @return A pointer to the last loadable program header, or NULL if no loadable segment is found.
 * */
Elf64_Phdr	*get_last_load_segment(t_file *file) {
	Elf64_Phdr	*phdrs;
	uint16_t	size;

	size = ((Elf64_Ehdr *)file->ptr)->e_phnum;
	phdrs = file->ptr + ((Elf64_Ehdr *)file->ptr)->e_phoff;
	while (--size && (void *)(phdrs + size) < file->end)
	{
		if (phdrs[size].p_type == PT_LOAD)
			return (phdrs + size);
	}
	return (NULL);
}
