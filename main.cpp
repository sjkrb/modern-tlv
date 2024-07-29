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

    // boost::asio::io_context context;
    // TLV::TLV_Manager reader(context, 1375);

    // std::function<void(const TLV::Test_TLV&)> function = std::bind(&TestTester::impel, &tester, std::placeholders::_1);
    // reader.tlv_handler.add_tlv(test, function);


    // context.run();






    std::cout << TLV::TLV_Manager::getInstance()->Send<TLV::Test_TLV>(test, boost::asio::ip::address_v4::loopback().to_string(), 1375);

    auto t = std::thread([&test](){
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << TLV::TLV_Manager::getInstance()->Send<TLV::Test_TLV>(test, boost::asio::ip::address_v4::loopback().to_string(), 1375);
    });

    TLV::TLV_Manager::getInstance()->io_context_.run();
    t.join();

    return 0;
}
