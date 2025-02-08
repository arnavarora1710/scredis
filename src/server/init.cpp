#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // file descriptor is -1 if socket creation failed
    if (fd < 0)
    {
        return 1;
    }

    // setting SO_REUSEADDR to 1
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        cerr << "bind failed" << endl;
        exit(1);
    }

    return 0;
}
