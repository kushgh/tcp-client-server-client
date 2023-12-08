#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char * address = "127.0.0.1";

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    } else {
        std::cout<<"Connected to "<<address<<":"<<PORT<<std::endl;
    }

    while (1) {
        // Receive the response from the server
        ssize_t bytesRead = read(sock, buffer, sizeof(buffer));

        if (bytesRead <= 0) {
            // Server disconnected or an error occurred
            std::cerr << "Server disconnected" << std::endl;
            close(sock);
            return -1;
        }

        std::cout << "Client received: " << buffer << std::endl;

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
