#include "tlv_connection_res.hpp"
#include "tlv_manager.hpp"

using namespace TLV;

TLV::TLV_Connection_Res::TLV_Connection_Res(boost::asio::io_context &context, tcpEndpoint::protocol_type::socket &&sock)
    : socket_(std::make_shared<tcpEndpoint::protocol_type::socket>(std::move(sock)))
{

}

void TLV_Connection_Res::do_read()
{
    Login_Send_TLV send{};
    boost::asio::async_read(*socket_, boost::asio::buffer(&send, sizeof(Login_Send_TLV)), [&send, self = shared_from_this(), this](boost::system::error_code, std::size_t)
                            {
                                if(strncmp(send.user_name ,std::get<0>(TLV_Manager::getInstance()->tlv_handler.handles).value.user_name, sizeof(send.user_name)) == 0)
                                {
                                    if(strncmp(send.password ,std::get<0>(TLV_Manager::getInstance()->tlv_handler.handles).value.password, sizeof(send.password)) == 0)
                                    {
                                        Login_rcv_TLV tlv;
                                        tlv.authenticationStatus = true;
                                        write(tlv);
                                        TLV_Manager::getInstance()->allowed_ips_.push_back(socket_->remote_endpoint().address());
                                        TLV_Manager::getInstance()->appendSocket(std::move(socket_));
                                        std::cout << "allowed ips are : " <<TLV_Manager::getInstance()->allowed_ips_.size() << std::endl;
                                    }else
                                    {
                                        Login_rcv_TLV tlv;
                                        tlv.authenticationStatus = false;
                                        write(tlv);
                                    }
                                }
                            });
}

void TLV::TLV_Connection_Res::write(const Login_rcv_TLV &tlv)
{
    boost::asio::async_write(*socket_, boost::asio::buffer(&tlv,sizeof(Login_rcv_TLV)),
                             [&, self = shared_from_this()](boost::system::error_code ec, std::size_t)
                             {
                                 if(ec)
                                     socket_->close();
                             });
}
