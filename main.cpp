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
    std::cout << "file name: " << so_file_name << " file size: " << so_file_size << std::endl;

    // read so
    u1* so = new u1[so_file_size];
    int so_fd = open(so_file_name, O_RDONLY);
    read(so_fd, so, so_file_size);
    close(so_fd);
    std::cout << "read so success" << std::endl;

    
    // release
    delete[] so;
    return 0;
}