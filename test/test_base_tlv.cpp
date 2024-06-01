#ifndef TEST_BASE_TLV_H
#define TEST_BASE_TLV_H


#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../lib/tlv_manager.hpp"
#include "../lib/tlv_connection_res.hpp"

TEST_CASE("send auth check")
{

    boost::asio::io_context context;
    boost::asio::ip::tcp::acceptor acs(context, TLV::tcp::endpoint(TLV::tcp::v4(), 1375));

    acs.async_accept([&context](const boost::system::error_code& ec, TLV::tcpEndpoint::protocol_type::socket client_socket)
                     {
                         auto tlv_connection_instance = std::make_shared<TLV::TLV_Connection_Res>(context, std::move(client_socket));
                     });

    // TLV::TLV_Manager::getInstance()->Send("127.0.0.1", 1375);

    // REQUIRE(TLV::TLV_Manager::getInstance()->allowed_ips_.size() == 0);

}

#endif // TEST_BASE_TLV_H
