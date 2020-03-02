// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::asio;
static const int buf_size=500;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
void clear_buf(char * buf,int size)
{
    for (int i=0;i<size;++i)
    {
        buf[i]='/0';
    }
}
io_service service;


std::string receive_message(socket_ptr& sock)
{
    char buf[buf_size];
    clear_buf(buf,buf_size);
    int length;
    sock->read_some(buffer(&length,4));
    sock->read_some(buffer(buf,length));
    return std::string(buf,length);
}
void send_string(socket_ptr& sock,std::string& msg)
{
    int length=msg.size();
    sock->write_some(boost::asio::buffer(&length,4));
    sock->write_some(boost::asio::buffer(msg,length));
}
void sync_echo(std::string& msg,int port_num)
{
    socket_ptr sock(new ip::tcp::socket(service));
    ip::tcp::endpoint ep( ip::address::from_string("enter address"), std::move(port_num));
    sock->connect(ep);


     for (int i=0;i<5;++i)
     {
         std::cout<<receive_message(sock)<<std::endl;
         send_string(sock,msg);
         std::cout<<receive_message(sock)<<std::endl;
     }

    sock->close();

}
int main(int argc, char* argv[])
{
    std::string msg="ebana rot\n";
    std::string msg2="nihuya sebe\n";

    boost::thread first(sync_echo,msg,8001);
    boost::thread second(sync_echo,msg2,8001);


    first.join();
    second.join();
    return 0;

}
