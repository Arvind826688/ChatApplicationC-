#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <cstdlib>

#define SERVER_PORT 5208 // The server's port number
#define IP_ADDRESS "127.0.0.1"

using boost::asio::ip::tcp;

// Global variables
std::string name;
std::string message;

void send_msg(tcp::socket& socket) {
    while (true) {
        std::getline(std::cin, message);
        if (message == "quit") {
            socket.close();
            exit(0);
        }

        std::string full_msg = name + ": " + message;
        boost::asio::write(socket, boost::asio::buffer(full_msg + "\n"));
    }
}

void recv_msg(tcp::socket& socket) {
    while (true) {
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n");
        std::string received_msg{buffers_begin(buf.data()), buffers_end(buf.data())};
        std::cout << received_msg;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Name>" << std::endl;
        return 1;
    }

    name = argv[1];

    try {
        // Create an I/O service
        boost::asio::io_service io_service;

        // Create a socket to connect to the server
        tcp::socket socket(io_service);
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(IP_ADDRESS), SERVER_PORT);

        // Connect to the server
        socket.connect(endpoint);

        // Send a greeting message to the server
        std::string greeting = "#new client: " + name;
        boost::asio::write(socket, boost::asio::buffer(greeting + "\n"));

        // Start the send and receive threads
        std::thread send_thread(send_msg, std::ref(socket));
        std::thread recv_thread(recv_msg, std::ref(socket));

        send_thread.join();
        recv_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
