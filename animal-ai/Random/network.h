#ifndef network_h
#define network_h

#include <string>
#include <boost/asio.hpp>

class IO {
public:
    IO(const std::string &host, const std::string &service) : io_service_(), socket_(io_service_)
    {
        boost::asio::ip::tcp::resolver resolver(io_service_);
        boost::asio::ip::tcp::resolver::query query(host, service);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
        boost::asio::connect(socket_, iterator);
    }
    
    void receive(std::string& message)
    {
        boost::asio::read_until(socket_, buffer, '\n');
        std::istream is(&buffer);
        std::getline(is, message);
    }
    
    void send(const std::string &message)
    {
        boost::asio::write(socket_, boost::asio::buffer(message.data(), message.size()));
    }

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer;
};

#endif
