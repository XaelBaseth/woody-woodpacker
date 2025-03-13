# include "../../inc/woody_woodpacker.h"

int is_elf64(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return 0;
    }

    Elf64_Ehdr ehdr;
    if (fread(&ehdr, 1, sizeof(ehdr), file) != sizeof(ehdr)) {
        perror("fread");
        fclose(file);
        return 0;
    }

    fclose(file);

    // Check ELF magic number
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        return 0;
        }

    // Check if it is a 64-bit ELF file
    if (ehdr.e_ident[EI_CLASS] != ELFCLASS64) {
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    (void) argv;
    // verify the number of args
    if (argc != 2)
        panic("Incorrect number of arguments.");
    if (!is_elf64(argv[1]))
        panic("The given file is not a 64-bit ELF file.");
    // verify the type of files given
    // in case of bonus, check the encryption key
    // verify the file is either elf 64 or elf 32 (bonus)
    // running the encryption
    ft_printf("Hello World!\n");
    return 0;
}