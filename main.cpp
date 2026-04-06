#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>   // struct sockaddr_in
#include <arpa/inet.h>    // inet_pton
#include <unistd.h>       // close
#include <cerrno>         // errno
#include <cstring>        // strerror
#include <string>

int main()
{
    const char* ips[] = {"8.8.8.8", "142.250.185.46"};
    int ports[] = {80, 443, 53};
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if(sock == -1)
            {
                std::cout << "Socket incorrected!" << std::endl;
                std::cerr << "Socket error: " << strerror(errno) << std::endl;
                return 1;
            }
            std::cout << "Socket " << sock << " comlete enabled!" << std::endl;

            server_addr.sin_port = htons(ports[j]);
            int conv = inet_pton(AF_INET, ips[i], &server_addr.sin_addr);
            if (conv == 0) 
            {
                std::cerr << "inet_pton() invalid IP address" << std::endl;
                close(sock);
                break;
            }
            if (conv == -1) 
            {
                std::cerr << "inet_pton() error: " << strerror(errno) << std::endl;
                close(sock);
                break;
            }

            int conn = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
            std::cout << "Connection server: " << ips[i] << ":" << ports[j] << std::endl; 
            if(conn == -1)
            {
                std::cerr << "Connected error: " << strerror(errno) << std::endl;
                close(sock);
                continue;
            }
            std::cout << "Connection Complete: " << ips[i] << ":" << ports[j] << std::endl; 
            close(sock);
        }
    }
    return 0;
}