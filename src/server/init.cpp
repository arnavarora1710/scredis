#include <sys/socket.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // file descriptor is -1 if socket creation failed
    if (fd < 0)
    {
        return 1;
    }

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    return 0;
}
