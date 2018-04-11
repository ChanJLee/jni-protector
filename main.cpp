#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "elf.h"

typedef unsigned char u1;

int main(int argc, char* argv[]) {

    const char* so_file_name = "libyasc.so";

    // fetch so file size
    struct stat buf;
    stat(so_file_name, &buf);
    size_t so_file_size = buf.st_size;
    printf("file name %s, file size: %lu\n", so_file_name, so_file_size);

    // read so
    u1* so = new u1[so_file_size];
    int so_fd = open(so_file_name, O_RDONLY);
    read(so_fd, so, so_file_size);
    close(so_fd);
    printf("read so success\n");

    // find load program
    // find dynamic program
    Elf32_Ehdr* so_header = (Elf32_Ehdr*)(so);
    printf("program header count: %d\n", so_header->e_phnum);
    Elf32_Phdr* program_iterator = (Elf32_Phdr*)(so + so_header->e_phoff);

    Elf32_Phdr* first_load_program = NULL;
    Elf32_Phdr* last_load_program = NULL;

    for (int i = 0; i < so_header->e_phnum; ++i) {
        if (program_iterator->p_type != PT_LOAD) {
            continue;
        }
        switch(program_iterator->p_type) {
            case PT_LOAD:
                if (first_load_program == NULL) {
                    printf("find first load program\n");
                    first_load_program = program_iterator;
                } else {
                    printf("find load program\n");
                    last_load_program = program_iterator;
                }
                break;
        }

        ++program_iterator;
    }

    Elf32_Shdr* section_iterator = (Elf32_Shdr*)(so + so_header->e_shoff);
    // TODO find section to write

    // release
    delete[] so;
    return 0;
}