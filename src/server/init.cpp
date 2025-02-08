#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // file descriptor is -1 if socket creation failed
    if (fd < 0) {
        return 1;
    }

    // setting SO_REUSEADDR to 1
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    // binding to the port defined above in the address struct
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        cerr << "bind failed" << endl;
        exit(1);
    }

    // socket not created yet

    // create socket with listen
    // SOMAXCONN is the maximum number of connections
    // 4096 on Linux
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        cerr << "listen failed" << endl;
        exit(1);
    }

    // accept a connection
    while (1) {
        struct sockaddr_in client_addr = {};
        socklen_t addr_len = sizeof(client_addr);

        // accept requires a pointer to the address length as well
        int conn = accept(fd, (struct sockaddr *)&client_addr, &addr_len);

        if (conn < 0) {
            cerr << "some error while accepting" << endl;
            continue;
        }

        // do something
        auto respond = [conn] (char* msg) {
            char reader[64] = {};
            ssize_t len_read = read(conn, reader, sizeof(reader) - 1);

            if (len_read < 0) {
                cerr << "some error while reading" << endl;
                return;
            }

            cout << "client sent: " << reader << endl;

            write(conn, msg, strlen(msg));
        };

        char msg[] = "hello world from server!";
        respond(msg);

        close(conn);
    }

    return 0;
}
