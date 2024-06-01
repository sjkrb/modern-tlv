#ifndef TLV_CONNECTION_RES_H
#define TLV_CONNECTION_RES_H
#include <boost/asio.hpp>
#include "base_tlv.hpp"
namespace TLV
{
class TLV_Connection_Res : public std::enable_shared_from_this<TLV_Connection_Res>
{
public:
    TLV_Connection_Res(boost::asio::io_context &context, tcpEndpoint::protocol_type::socket&& sock);
    void do_read();
private:
    std::shared_ptr<tcpEndpoint::protocol_type::socket> socket_;

    void write(const Login_rcv_TLV& tlv);
};

}


#endif // TLV_CONNECTION_RES_H
