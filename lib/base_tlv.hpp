#ifndef BASE_TLV_H
#define BASE_TLV_H


#include <cstdint>
#include <functional>
#include <boost/asio.hpp>
#include <concepts>
#include <tuple>

/*
 *  base tlv class
 *  crtp
 *  std::function handle or friend class
 *
 *  tlv problems :
 *  if instead of switch
 *  using mainwindow as its interface
 *  using while for reading data
 *  using Qt socket for reading data not boost sockets
 *
 */

namespace TLV
{

using boost::asio::ip::tcp;
using tcpEndpoint = boost::asio::ip::tcp::endpoint;

struct Base_TLV
{
    Base_TLV() = default;

    enum class TLV_TYPE : uint8_t {
        none,
        test,
        example,
        login_req,
        login_res,
    };

public:
    uint32_t header{0xffaaddcc};
    TLV_TYPE type{TLV_TYPE::none};
    uint32_t length{sizeof(this)};
    uint8_t  flag{0};
};


struct Test_TLV : Base_TLV
{
    Test_TLV()
    {
        type = Base_TLV::TLV_TYPE::test;
        length = sizeof(Test_TLV);
    }

    uint32_t value{11};
    double   dValue{12.34};
    char     c{'h'};
};

struct Example_TLV : Base_TLV
{
    Example_TLV()
    {
        type = Base_TLV::TLV_TYPE::example;
        length = sizeof(Example_TLV);
    }

    uint32_t value{11};
    double   dValue{12.34};
    char     c{'h'};
};

struct Login_Send_TLV : Base_TLV
{
    Login_Send_TLV()
    {
        type = Base_TLV::TLV_TYPE::login_req;
        length = sizeof(Login_Send_TLV);
    }

    char user_name [16];
    char password  [16];
};

struct Login_rcv_TLV : Base_TLV
{
    Login_rcv_TLV()
    {
        type = Base_TLV::TLV_TYPE::login_res;
    }

    bool authenticationStatus{false};
};

}

#endif // BASE_TLV_H
