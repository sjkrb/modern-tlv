#include <functional>
#include <iostream>

#include <memory>
#include "lib/tlv_connection_res.hpp"
#include "lib/tlv_manager.hpp"
#include "lib/base_tlv.hpp"

struct TestTester
{
    void impel(const TLV::Test_TLV &test)
    {
        std::cout << test.c << std::endl;
        std::cout << test.dValue << std::endl;
        std::cout << test.value << std::endl;
    }
    void hello()
    {
        std::cout << "hellos \n";
    }
};


int main()
{
    TestTester tester;

    TLV::Test_TLV test{};

    std::function<void(const TLV::Test_TLV&)> function = std::bind(&TestTester::impel, &tester, std::placeholders::_1);
    TLV::TLV_Manager::getInstance()->tlv_handler.add_tlv(test, function);

    boost::asio::ip::tcp::acceptor acs(TLV::TLV_Manager::getInstance()->io_context_, TLV::tcp::endpoint(TLV::tcp::v4(), 1375));
    acs.async_accept([](const boost::system::error_code& ec, TLV::tcpEndpoint::protocol_type::socket client_socket)
                     {
                         if(ec)
                             std::cout << ec.what() << std::endl;
                         std::cout << "i read something" << std::endl;
                         auto tlv_connection_instance = std::make_shared<TLV::TLV_Connection_Res>(TLV::TLV_Manager::getInstance()->io_context_, std::move(client_socket));
                         tlv_connection_instance->do_read();
                     });


    std::cout << TLV::TLV_Manager::getInstance()->Send<TLV::Test_TLV>(test, boost::asio::ip::address_v4::any().to_string(), 1375);
    // TLV::TLV_Manager::getInstance()->tlv_handler.add_tlv(test, std::bind(&TestTester::hello, &tester));


    for (int i = 0; i < 10; ++i) {
        std::cout << TLV::TLV_Manager::getInstance()->Send<TLV::Test_TLV>(test, "0.0.0.0", 1375);
    }

    TLV::TLV_Manager::getInstance()->io_context_.run();


    return 0;
}
