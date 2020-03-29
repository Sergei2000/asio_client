// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

static boost::asio::io_service service;

struct server {
    std::string _reply;
    boost::asio::ip::tcp::socket _sock{service};

    void connect() {
        boost::asio::
        ip::tcp::endpoint ep(boost::asio::ip::address::
        from_string("192.168.1.2"), std::move(8001));
        _sock.connect(ep);
    }

    void send_name() {
        std::string request;
        std::cin >> request;
        request += "\r\n\r\n";
        std::cout << request;
        _sock.write_some(boost::asio::buffer(request, request.size()));
        read_reply();
    }

    void read_reply() {
        boost::asio::streambuf buffer{};
        read_until(_sock, buffer, "\n");
        std::string reply((std::istreambuf_iterator<char>(&buffer)),
        std::istreambuf_iterator<char>());
        _reply = reply;
    }

    void send_ping() {
        std::string request = "ping\n";
        request += "\r\n";
        _sock.write_some(boost::asio::buffer(request, request.size()));
    }

    void send_list() {
        std::string request = "list\n";
        request += "\r\n";
        _sock.write_some(boost::asio::buffer(request, request.size()));
    }

    void tester() {
        std::string request;
        connect();
        send_name();

        while (true) {
            send_ping();
            read_reply();
            if (_reply == "ping_ok\n\r\n") {
                // std::cout<<_reply;
                continue;
            } else {
                if (_reply == "changed\n\r\n") {
                    send_list();
                    read_reply();
                    std::cout << "list:\n";
                    std::cout << _reply;
                }
            }
        }
    }
};

int main() {
    server one;
    one.tester();
    return 0;
}

