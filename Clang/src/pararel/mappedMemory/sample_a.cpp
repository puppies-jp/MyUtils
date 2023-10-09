#include <cstdlib>
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

const unsigned int FILE_LENGTH = 0x100;
const std::string FILE_NAME("./data.dat");

int main()
{
    /* Prepare a file large enough to hold an unsigned integer. */
    auto fd = open(FILE_NAME.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);
    /* Create the memory mapping. */
    char *const file_memory = reinterpret_cast<char *>(mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0));
    // char *const file_memory = reinterpret_cast<char *>(mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    close(fd);
    /* Write a random integer to memory-mapped area. */
    sprintf(file_memory, "%s", "Hello World!!!!");
    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);

    return EXIT_SUCCESS;
}
