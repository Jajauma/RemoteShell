#include "SystemCall.hpp"

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#include <iostream>
#endif /* BUILD_UNIT_TESTS */

#if defined(BUILD_UNIT_TESTS)
TEST(SystemCall, TcpSocketOptionsPrintDefaults)
{
    auto socket = System::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int intOption{};
    socklen_t intOptionLength{sizeof(intOption)};

    System::getsockopt(socket, SOL_SOCKET, SO_RCVLOWAT, &intOption, &intOptionLength);
    std::cout << "Receive low water mark: " << intOption << std::endl;

    System::getsockopt(socket, SOL_SOCKET, SO_SNDLOWAT, &intOption, &intOptionLength);
    std::cout << "Send low water mark: " << intOption << std::endl;

    System::close(socket);
}
#endif /* BUILD_UNIT_TESTS */
