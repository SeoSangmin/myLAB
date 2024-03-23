#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pipe_path>\n", argv[0]);
        exit(1);
    }

    const char *pipe_path = argv[1];
    char buffer[BUFFER_SIZE];
    int fd;

    // Open the named pipe for reading
    if ((fd = open(pipe_path, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    printf("Reading data from named pipe: %s\n", pipe_path);

    // Read data from the named pipe
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("read");
        exit(1);
    }

    printf("Received data: %.*s\n", (int)bytes_read, buffer);

    // Close the named pipe
    close(fd);

    return 0;
}
