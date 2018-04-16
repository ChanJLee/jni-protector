#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "scope_array.h"
#include "elf.h"

typedef unsigned char u1;

#define ALIGN(P, ALIGNBYTES) (((unsigned long)(P) + (ALIGNBYTES)-1) & ~((ALIGNBYTES)-1))

int main(int argc, char *argv[])
{

    const char *so_file_name = argv[1];
    const char *payload_file_name = argv[2];
    const char *dest_file_name = argv[3];
    const char *section_name = argv[4];

    // fetch so file size
    struct stat so_buf;
    stat(so_file_name, &so_buf);
    size_t so_file_size = so_buf.st_size;
    printf("so file name %s, file size: %lu\n", so_file_name, so_file_size);

    struct stat payload_buf;
    stat(payload_file_name, &payload_buf);
    size_t payload_file_size = payload_buf.st_size;
    printf("payload file name %s, file size: %lu\n", payload_file_name, payload_file_size);

    // read so
    u1 *so = new u1[so_file_size * 2 + payload_file_size];
    memset(so, 0, so_file_size * 2 + payload_file_size);
    ScopeArray<u1> so_scope_array(so);
    int so_fd = open(so_file_name, O_RDONLY);
    read(so_fd, so, so_file_size);
    close(so_fd);
    printf("read so success\n");

    u1 *payload = new u1[payload_file_size];
    ScopeArray<u1> payload_scope_array(payload);
    int payload_fd = open(so_file_name, O_RDONLY);
    read(payload_fd, payload, payload_file_size);
    close(payload_fd);
    printf("read payload success\n");

    // find load program
    Elf32_Ehdr *so_header = (Elf32_Ehdr *)(so);
    printf("program header count: %d\n", so_header->e_phnum);
    Elf32_Phdr *program_iterator = (Elf32_Phdr *)(so + so_header->e_phoff);

    Elf32_Phdr *first_load_program = NULL;
    Elf32_Phdr *last_load_program = NULL;

    for (int i = 0; i < so_header->e_phnum; ++i, ++program_iterator)
    {
        if (program_iterator->p_type != PT_LOAD)
        {
            continue;
        }

        if (first_load_program == NULL)
        {
            printf("find first load program\n");
            first_load_program = program_iterator;
        }
        else
        {
            printf("find load program\n");
            last_load_program = program_iterator;
        }
    }

    Elf32_Shdr *section_iterator = (Elf32_Shdr *)(so + so_header->e_shoff);

    Elf32_Addr new_section_address = ALIGN(last_load_program->p_paddr + last_load_program->p_memsz - first_load_program->p_vaddr, last_load_program->p_align);
    if (first_load_program->p_vaddr + first_load_program->p_filesz <
        ALIGN(last_load_program->p_paddr + last_load_program->p_memsz, last_load_program->p_align))
    {
        // if section header is not at the end of file
        if (so_header->e_shoff + sizeof(Elf32_Shdr) * so_header->e_shnum != so_buf.st_size)
        {
            if (so_file_size + sizeof(Elf32_Shdr) * (so_header->e_shnum + 1) > new_section_address)
            {
                printf("can not write section");
                return -1;
            }
            else
            {
                const size_t section_table_size = sizeof(Elf32_Shdr) * so_header->e_shnum;
                memcpy(so + so_file_size, so + so_header->e_shoff, section_table_size);
                so_header->e_shoff = so_file_size;
                so_file_size += section_table_size;
                section_iterator = (Elf32_Shdr *)(so + so_file_size);
            }
        }
    }
    else
    {
        new_section_address = first_load_program->p_filesz;
    }

    // find str table section
    size_t section_name_len = strlen(section_name) + 1;
    size_t write_size = new_section_address + ALIGN(section_name_len, 0x10) + payload_file_size;
    char *cache = new char[write_size];
    ScopeArray<char> cache_scope_array(cache);
    memset(cache, 0, write_size);
    Elf32_Shdr *str_table_section = section_iterator + so_header->e_shstrndx;
    str_table_section->sh_size = new_section_address - str_table_section->sh_offset + strlen(section_name) + 1;
    strcpy(cache + new_section_address, section_name);

    Elf32_Shdr section = {0};
    section.sh_name = new_section_address - str_table_section->sh_offset;
    section.sh_type = SHT_PROGBITS;
    section.sh_flags = SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR;
    new_section_address += ALIGN(section_name_len, 0x10);
    section.sh_size = payload_file_size;
    section.sh_addr = new_section_address;
    memcpy(cache + new_section_address, payload, payload_file_size);
    section.sh_addralign = 0x10;

    first_load_program->p_filesz = write_size;
    first_load_program->p_memsz = new_section_address + payload_file_size;
    first_load_program->p_flags = 7;

    ++so_header->e_shnum;

    memcpy(cache, so, so_file_size);
    memcpy(cache + so_file_size, &section, sizeof(Elf32_Shdr));

    int dest_fd = open(dest_file_name, O_WRONLY | O_CREAT);
    write(dest_fd, cache, write_size);
    close(dest_fd);

    return 0;
}