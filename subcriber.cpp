#include <mqtt/async_client.h>
#include <iostream>

int main() {
    std::string ip = "localhost:1883";
    std::string id = "consumer";

    try {
        mqtt::async_client client(ip, id);
        mqtt::connect_options connOpts;
        client.connect(connOpts);

        // Check for connection success
        if (!client.is_connected()) {
            std::cerr << "Failed to connect to the broker." << std::endl;
            return 1;
        }

        client.subscribe("in");

        bool running = true;
        while (running) {
            mqtt::message_ptr msg = client.consume_message();
            if (msg) {
                std::string messageString = msg->get_payload_str();
                std::cout << "Received message: " << messageString << std::endl;

                if (messageString == "quit") {
                    running = false;
                }
            }
        }

        // Disconnect from the broker when done
        client.disconnect();
    } catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Exception: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
