#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        cout << "socket() failed" << endl;
        return 1;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    // INADDR_LOOPBACK is 0x7f000001
    // which is 127.0.0.1 (localhost)
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rv = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (rv < 0) {
        cout << "connect() failed" << endl;
        return 1;
    }

    char msg[] = "hello from client";
    write(fd, msg, strlen(msg));

    char reader[64] = {};
    ssize_t len_read = read(fd, reader, sizeof(reader));

    if (len_read < 0) {
        cout << "read() failed" << endl;
        return 1;
    }

    cout << "server sent: " << reader << endl;

    close(fd);

    return 0;
}
