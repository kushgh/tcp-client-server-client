#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <algorithm>

#define PORT 8080

std::vector<int> connectedClients;

void handleClient(int client_socket) {
    char buffer[1024] = {0};
    connectedClients.push_back(client_socket);

    while (1) {
        // Read the number from the client
        ssize_t bytesRead = read(client_socket, buffer, sizeof(buffer));

        if (bytesRead <= 0) {
            // Client disconnected or an error occurred
            close(client_socket);
            std::cout << "Client disconnected" << std::endl;
            
            // Remove the client from the vector
            auto it = std::find(connectedClients.begin(), connectedClients.end(), client_socket);
            if (it != connectedClients.end()) {
                connectedClients.erase(it);
            }

            return;
        }

        std::cout << "Server received: " << buffer << std::endl;

        // Broadcast the response to all connected clients
        for (int client : connectedClients) {
            send(client, buffer, strlen(buffer), 0);
        }

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::vector<std::thread> clientThreads;

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "New client connected" << std::endl;

        // Handle the client in a separate thread
        clientThreads.emplace_back(handleClient, new_socket);
    }

    // The program will never reach this point, but for demonstration purposes:
    // Wait for all threads to finish (this won't happen in this example)
    for (auto& thread : clientThreads) {
        thread.join();
    }

    return 0;
}
