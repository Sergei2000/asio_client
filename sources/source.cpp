// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

static const int buf_size = 500;
typedef boost::shared_ptr <boost::asio::ip::tcp::socket> socket_ptr;

void clear_buf(char *buf, int size) {
    for (int i = 0; i < size; ++i) {
        buf[i] = 0;
    }
}
boost::asio::io_service service;
std::string receive_message(socket_ptr &sock) {
    char buf[500];
    clear_buf(buf, buf_size);
    int length;
    sock->read_some(boost::asio::buffer(&length, 4));
    sock->read_some(boost::asio::buffer(buf, length));
    return std::string(buf, length);
}

void send_string(socket_ptr &sock, std::string &msg) {
    int length = msg.size();
    sock->write_some(boost::asio::buffer(&length, 4));
    sock->write_some(boost::asio::buffer(msg, length));
}

void sync_echo(std::string &msg, int port_num) {
    socket_ptr sock(new boost::asio::ip::tcp::socket(service));
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string
    ("192.168.1.2"),
    std::move(port_num));
    sock->connect(ep);
    char buff[200];
    std::string data = "client list\n", data1 = "ping\n";
    bool check = true;
    int c = msg.size();
    sock->write_some(boost::asio::buffer(&c, 4));
    sock->write_some(boost::asio::buffer(msg, c));
    while (true) {
        if (check) {
            int a = data1.size();
            sock->write_some(boost::asio::buffer(&a, 4));
            sock->write_some(boost::asio::buffer(data1, a));
        } else {
            int a = data.size();
            sock->write_some(boost::asio::buffer(&a, 4));
            sock->write_some(boost::asio::buffer(data, a));
        }
        int b;
        sock->read_some(boost::asio::buffer(&b, 4));
        sock->read_some(boost::asio::buffer(buff, b));
        std::cout << " " + std::string(buff) << std::endl;
        boost::this_thread::sleep(boost::posix_time::millisec(4));
        check = (!check);
    }
}

int main() {
    std::string msg = "hello\n";
    std::string msg2 = "hi\n";
    boost::thread first(sync_echo, msg, 8001);
    boost::thread second(sync_echo, msg2, 8001);
    first.join();
    second.join();
    return 0;
}

