#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void drop(const char *filename, const char *data, int size) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 064);
    write(fd, data, size);
    close(fd);
    chmod(filename, 075);
}

void launch(const char *filename) {
    execl(filename, filename, NULL);
}

int main() {
    const char *filename = "harmless";
	const char *data = ""; 		// WIP

    int size = strlen(data);

    drop(filename, data, size);
    launch(filename);

    return 0;
}

