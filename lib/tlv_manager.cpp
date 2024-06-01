#include <iostream>

#include "tlv_manager.hpp"
#include "base_tlv.hpp"
#include "tlv_connection_req.hpp"
#include "tlv_connection_res.hpp"

using namespace TLV;

TLV::TLV_Manager *TLV::TLV_Manager::m_ptr {nullptr};

TLV::TLV_Manager::TLV_Manager()
    :acceptor_(io_context_, tcp::endpoint(tcp::v4(), 1376))
{
    memcpy(std::get<0>(tlv_handler.handles).value.password, password, sizeof(password));
    memcpy(std::get<0>(tlv_handler.handles).value.user_name, userName, sizeof(userName));

    start_accept();
}


TLV::TLV_Manager *TLV::TLV_Manager::getInstance()
{
    if(!m_ptr)
    {
        m_ptr = new TLV_Manager();
    }

    return m_ptr;
}

void TLV::TLV_Manager::setBufferSize(const uint32_t &value)
{
    buffer_.resize(value + sizeof(Base_TLV));
}

void TLV_Manager::appendSocket(std::shared_ptr<tcp::socket> &&socket)
{
    m_sockets.push_back(socket);

    std::shared_ptr<AsyncReader> reader = std::make_shared<AsyncReader>(m_sockets.back());
    reader->do_read();
}

void TLV::TLV_Manager::start_accept()
{
    acceptor_.async_accept([this](const boost::system::error_code& ec, tcpEndpoint::protocol_type::socket client_socket)
                           {
                               auto it = std::find_if(allowed_ips_.begin(), allowed_ips_.end(), [&client_socket](const boost::asio::ip::address &ip)
                                                      { return ip == client_socket.remote_endpoint().address();});

                               if(it == std::end(allowed_ips_))
                               {
                                   auto tlv_connection_instance = std::make_shared<TLV_Connection_Res>(io_context_, std::move(client_socket));
                                   tlv_connection_instance->do_read();
                               }
                           });

}


void AsyncReader::do_read()
{
    boost::asio::async_read(*socket_, boost::asio::buffer(iter, std::distance(iter, data_.end())),[this, self = shared_from_this()](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    if(std::distance(data_.begin(), iter) < sizeof(Base_TLV))
                                        self->do_read();

                                    Base_TLV base;
                                    memcpy(&base, data_.cbegin(), sizeof(Base_TLV));

                                    if(length < base.length)
                                        self->do_read();
                                    else
                                    {
                                        auto check = [this]<IS_TLV T>(TLV_Object<T> &Var, Base_TLV::TLV_TYPE &type){
                                            if (Var.value.type == type)
                                            {
                                                memcpy(&Var.value, data_.begin(), sizeof(T));
                                                Var.run();
                                                iter = data_.begin();
                                            }
                                        };

                                        std::apply([&](auto&&... args) {((check(args, base.type)), ...);},  TLV_Manager::getInstance()->tlv_handler.handles);
                                        self->do_read();
                                    }

                                } else if (ec == boost::asio::error::eof) {
                                    std::cout << "Connection closed by peer." << std::endl;
                                    socket_->close();
                                } else if (ec != boost::asio::error::operation_aborted) {
                                    std::cerr << "Error: " << ec.message() << std::endl;
                                    socket_->close();
                                }
                            });
}
