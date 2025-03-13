#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <string.h>
#include <time.h>

/* Generate a random encryption key */
unsigned char generate_key() {
    srand(time(NULL));
    return rand() % 256;
}

/* XOR encryption (placeholder for stronger encryption like AES) */
void encrypt_section(unsigned char *data, size_t size, unsigned char key) {
    for (size_t i = 0; i < size; i++)
        data[i] ^= key;
}

/* Read ELF file, encrypt the executable section, and write a new ELF */
void process_elf(const char *filename) {
    int fd;
    struct stat st;
    Elf64_Ehdr *elf_header;
    Elf64_Phdr *program_header;
    unsigned char key = generate_key();

    /* Open file */
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Get file size */
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Map ELF file into memory */
    void *map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    elf_header = (Elf64_Ehdr *)map;

    /* Validate ELF file */
    if (memcmp(elf_header->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not a valid ELF file\n");
        munmap(map, st.st_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Locate executable section */
    printf("Processing ELF file: %s\n", filename);
    printf("Encryption key: 0x%X\n", key);

    for (int i = 0; i < elf_header->e_phnum; i++) {
        program_header = (Elf64_Phdr *)((char *)map + elf_header->e_phoff + (i * elf_header->e_phentsize));

        /* Look for executable segment */
        if (program_header->p_type == PT_LOAD && (program_header->p_flags & PF_X)) {
            printf("Found executable segment at offset 0x%lx\n", program_header->p_offset);

            /* Encrypt the segment */
            encrypt_section((unsigned char *)map + program_header->p_offset, program_header->p_filesz, key);
        }
    }

    /* Create new output file (woody) */
    int out_fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (out_fd < 0) {
        perror("open woody");
        munmap(map, st.st_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Write modified ELF to new file */
    if (write(out_fd, map, st.st_size) != st.st_size) {
        perror("write");
        munmap(map, st.st_size);
        close(fd);
        close(out_fd);
        exit(EXIT_FAILURE);
    }

    printf("Encrypted file saved as 'woody'\n");

    /* Cleanup */
    munmap(map, st.st_size);
    close(fd);
    close(out_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <64-bit ELF binary>\n", argv[0]);
        return EXIT_FAILURE;
    }

    process_elf(argv[1]);
    return EXIT_SUCCESS;
}
