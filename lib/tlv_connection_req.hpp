#ifndef TLV_CONNECTION_REQ_H
#define TLV_CONNECTION_REQ_H
#include "base_tlv.hpp"
#include <boost/asio.hpp>

namespace TLV
{

using boost::asio::ip::tcp;
using tcpEndpoint = boost::asio::ip::tcp::endpoint;


class TLV_Connection_Req : public std::enable_shared_from_this<TLV_Connection_Req>
{
public:
    TLV_Connection_Req(boost::asio::io_context &context, const std::string &ip, const uint16_t &port);

    void connect(boost::asio::io_context& context, const std::string &ip, const uint16_t &port);
private:
    std::shared_ptr<tcpEndpoint::protocol_type::socket> socket_{nullptr};


    void write(boost::asio::io_context& context);


    void read(boost::asio::io_context& context);
};

}
#endif // TLV_CONNECTION_REQ_H
