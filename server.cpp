#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <mutex>

#define SERVER_PORT 5208

using boost::asio::ip::tcp;

std::unordered_map<std::string, tcp::socket> clients;
std::mutex mtx;

void session(tcp::socket sock) {
    try {
        while (true) {
            boost::asio::streambuf buf;
            boost::asio::read_until(sock, buf, "\n");
            std::string message{buffers_begin(buf.data()), buffers_end(buf.data())};
            
            std::cout << message;

            // Broadcast message to all clients
            std::lock_guard<std::mutex> lock(mtx);
            for (auto& client : clients) {
                boost::asio::write(client.second, boost::asio::buffer(message));
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in session: " << e.what() << std::endl;
    }
}

int main() {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), SERVER_PORT));
    
    while (true) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        std::thread(session, std::move(socket)).detach();
    }

    return 0;
}
