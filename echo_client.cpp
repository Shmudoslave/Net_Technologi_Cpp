#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/time.h>   

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "socket() error: " << strerror(errno) << std::endl;
        return 1;
    }
    const char* ip = "127.0.0.1";
    int port = 7777;
    
    struct timeval tv;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "connect() error: " << strerror(errno) << std::endl;
        close(sock);
        return 1;
    }
    std::cout << "Connected to " << ip << ":" << port << std::endl;
    
    const char* msg = "Hello, server!";
    send(sock, msg, strlen(msg), 0);
    std::cout << "Sent: " << msg << std::endl;
    std::cout << "TimeOut enabled!" << std::endl;
    shutdown(sock, SHUT_WR);

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    int s = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if(s) std::cout << "Connection close: TimeOut!" << std::endl;

    char buffer[1024];
    int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    buffer[n] = '\0';
    std::cout << "Received: " << buffer << std::endl;
    close(sock);
    return 0;
}