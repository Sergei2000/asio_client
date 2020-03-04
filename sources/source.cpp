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
    ip::tcp::endpoint ep( ip::address::from_string("192.168.1.2"), std::move(port_num));
    sock->connect(ep);
    char buff[200];
    int counter=0;
   // if (sock->available())
      while (true)
    {

        counter=sock->read_some(boost::asio::buffer(buff,200));
        sock->write_some(boost::asio::buffer(msg));
        std::cout<<"accessable"<<boost::this_thread::get_id()<<" "+msg+std::string(buff)<<std::endl;
        boost::this_thread::sleep(boost::posix_time::millisec(4));
    }

   //

    std::cout<<buff;
    sock->close();


}
int main(int argc, char* argv[])
{
    std::string msg="ebana rot\n";
    std::string msg2="nihuya sebe\n";
    std::string msg3="we fucking done it\n";
    boost::thread first(sync_echo,msg,8001);
    boost::thread second(sync_echo,msg2,8001);
    boost::thread third(sync_echo,msg3,8001);


    first.join();
    second.join();
    third.join();
    return 0;

}

