#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // Added this line

#define PIPE_NAME "mypipe"
#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    int fd;

    // Create the named pipe
    if (mkfifo(PIPE_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    printf("Named pipe created: %s\n", PIPE_NAME);

    // Open the named pipe for reading
    if ((fd = open(PIPE_NAME, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    printf("Waiting for data...\n");

    // Read data from the named pipe
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("read");
        exit(1);
    }

    printf("Received data: %.*s\n", (int)bytes_read, buffer);

    // Close the named pipe
    close(fd);

    // Remove the named pipe
    unlink(PIPE_NAME);

    return 0;
}
