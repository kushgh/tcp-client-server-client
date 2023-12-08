#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#define PORT 8080
#define INTERVAL 2

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int number = 1;

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    } else {
        std::cout<<"Address Conversion Successful"<<std::endl;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    } else {
        std::cout<<"Connected to Server..."<<std::endl;
    }

    while (1) {
        // Send the incremental number to the server
        std::string message = std::to_string(number++);
        std::cout<<"Client sending: "<<message<<std::endl;
        send(sock, message.c_str(), message.length(), 0);

        // Sleep for 2 seconds
        sleep(INTERVAL);
    }


    return 0;
}
