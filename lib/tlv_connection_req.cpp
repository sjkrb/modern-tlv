#include "tlv_connection_req.hpp"

#include "tlv_manager.hpp"
#include <boost/asio.hpp>

using namespace TLV;

TLV::TLV_Connection_Req::TLV_Connection_Req(boost::asio::io_context &context, const std::string &ip, const uint16_t &port)
    :socket_(std::make_shared<tcpEndpoint::protocol_type::socket>(context))
{
    // connect(context, ip, port);
}

void TLV::TLV_Connection_Req::connect(boost::asio::io_context &context, const std::string &ip, const uint16_t &port)
{
    PRINT_LOCATION(this, std::source_location::current());

    socket_->async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ip.c_str()), port),[&, self = shared_from_this()](const boost::system::error_code& error)
                           {
                               if(!error)
                               {
                                   self->write(context);
                               }else
                               {
                                   std::cout << error.what() << std::endl;
                                   boost::asio::io_context cont;
                                   boost::asio::steady_timer timer(cont);
                                   timer.expires_after(std::chrono::seconds(1));
                                   timer.async_wait([&](const boost::system::error_code& error)
                                                    {
                                                        if (!error)
                                                        {
                                                            self->connect(context, ip, port);
                                                        }
                                                    });
                                   cont.run();
                               }});
}

void TLV::TLV_Connection_Req::write(boost::asio::io_context &context)
{
    PRINT_LOCATION(this, std::source_location::current());

    boost::asio::async_write(*socket_, boost::asio::buffer(&std::any_cast<TLV_Object<Login_Send_TLV>&>(TLV_Manager::getInstance()->tlv_handler.handles[0]).value,
                                                           sizeof(std::any_cast<TLV_Object<Login_Send_TLV>&>(TLV_Manager::getInstance()->tlv_handler.handles[0]).value)),
                             [&, self = shared_from_this()](boost::system::error_code ec, std::size_t)
                             {
                                 if(ec)
                                 {
                                     boost::asio::steady_timer timer(context);
                                     timer.expires_after(std::chrono::seconds(5));
                                     timer.async_wait([&](const boost::system::error_code& error) {
                                         if (!error)
                                         {
                                             self->write(context);
                                         }
                                     });
                                 }else
                                 {
                                     self->read(context);
                                 }
                             });
}

void TLV::TLV_Connection_Req::read(boost::asio::io_context &context)
{
    Login_rcv_TLV rcv;
    boost::asio::async_read(*socket_, boost::asio::buffer(&rcv, sizeof(Login_rcv_TLV)), [&rcv, self = shared_from_this(), this](boost::system::error_code, std::size_t)
                            {
                                if(rcv.authenticationStatus == true)
                                {
                                    TLV_Manager::getInstance()->appendSocket(std::move(socket_));
                                }
                                else
                                {
                                    socket_->close();
                                }
                            });
}
