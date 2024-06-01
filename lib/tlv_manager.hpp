#ifndef TLV_MANAGER_H
#define TLV_MANAGER_H
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include "tlv_connection_req.hpp"
#include "base_tlv.hpp"

namespace TLV {

#define max_buffer_size 1024

// template <typename Var>
// concept IS_TLV = requires(Var var)
// {
//     { std::derived_from<Var, Base_TLV> };
//     // { std::shared_ptr<Var>(var) } -> std::convertible_to<std::shared_ptr<Var>>;
// };

template <typename T>
concept IS_TLV = std::derived_from<T, Base_TLV>;

template <IS_TLV T>
struct TLV_Object
{
    TLV_Object<T> &operator=(const TLV_Object<T> &) = default;
    T value;
    std::function<void(const T&)> impl;

    void run()
    {
        std::invoke(impl, value);
    }
};

class TLV_Manager;

struct TLV_Handler
{
    template <IS_TLV T>
    void add_tlv(T &input, std::function<void(const T&)> function)
    {
        auto check = [&]<IS_TLV is_tlv>(TLV_Object<is_tlv> &Var){
            if constexpr (std::is_same<is_tlv, T>::value)
            {
                Var.impl = function;
            }
        };

        std::apply([&](auto&&... args) {(check(args), ...);}, handles);
    }

    std::tuple<TLV_Object<Login_Send_TLV>,
               TLV_Object<Login_rcv_TLV>,
               TLV_Object<Test_TLV>>

            handles;
};


class TLV_Manager
{
public:

    TLV_Manager();

    template <IS_TLV Var>
    bool Send(const Var &value, std::string_view ip, const uint16_t &port)
    {
        std::cout << "come in here \n";
        boost::system::error_code ec;

        auto it = std::find_if(m_sockets.begin(), m_sockets.end(), [&](const std::shared_ptr<boost::asio::ip::tcp::socket> &sock){
            return sock->remote_endpoint().address().to_string(ec) == ip;
        });

        if(it == m_sockets.end())
        {
            // create new socket connection and get its response and add it to the socket vector
            auto tlv_connection_instance = std::make_shared<TLV_Connection_Req>(io_context_, std::string{ip.begin(), ip.end()}, port);
            tlv_connection_instance->connect(io_context_, std::string{ip.begin(), ip.end()}, port);
            return false;
        }

        boost::asio::async_write(*(*it), boost::asio::buffer(&value,sizeof(value)),
                                 [&](boost::system::error_code ec, std::size_t)
                                 {
                                     if(ec)
                                     {
                                         std::cout << ec.message() << std::endl;
                                     }
                                 });

        return true;
    }

    static TLV_Manager* getInstance();

    void setBufferSize(const uint32_t &value);

    void appendSocket(std::shared_ptr<boost::asio::ip::tcp::socket> &&socket);



    TLV_Handler tlv_handler;
    std::vector<boost::asio::ip::address> allowed_ips_;
    boost::asio::io_context io_context_;
private:
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> m_sockets;

    static TLV_Manager* m_ptr;

    void start_accept();


    tcp::acceptor acceptor_;
    std::vector<uint8_t> buffer_;

    char userName[16]{"salam"};
    char password[16]{"chetory"};
};


struct AsyncReader : public std::enable_shared_from_this<AsyncReader> {

    AsyncReader(std::shared_ptr<boost::asio::ip::tcp::socket> &socket):
        socket_(socket){}

    void do_read();

    std::shared_ptr<boost::asio::ip::tcp::socket> socket_{nullptr};
    std::array<char, max_buffer_size> data_;
    std::array<char, max_buffer_size>::iterator iter{data_.begin()};
};
}
#endif // TLV_MANAGER_H
