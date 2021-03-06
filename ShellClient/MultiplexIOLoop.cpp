#include "MultiplexIOLoop.hpp"
#include "System/FileDescriptor.hpp"
#include "System/IO.hpp"
#include "System/Shutdown.hpp"

#include <sys/select.h>
#include <unistd.h>

#include <cerrno>
#include <stdexcept>
#include <system_error>

void
ShellClient::multiplexIOLoop(const System::FileDescriptor& clientSocket)
{
    bool standardInputEOF{false};
    fd_set rset;
    FD_ZERO(&rset);

    while (true)
    {
        if (!standardInputEOF)
            FD_SET(STDIN_FILENO, &rset);
        FD_SET(clientSocket.toNative(), &rset);

        auto maxFD1 = std::max<int>(STDIN_FILENO, clientSocket.toNative()) + 1;
        auto ret    = ::select(maxFD1, &rset, nullptr, nullptr, nullptr);

        if (ret == -1)
            throw std::system_error{errno, std::generic_category()};
        else if (ret > 0)
        {
            if (FD_ISSET(clientSocket.toNative(), &rset))
            {
                System::IOBuffer buf;
                auto socketRead = System::read(clientSocket, buf);
                if (socketRead.count() == 0)
                {
                    if (standardInputEOF)
                        return;
                    else
                        throw std::runtime_error{
                            "Server terminated prematurely"};
                }
                else
                    (void)System::write(STDOUT_FILENO, buf, socketRead.count());
            }
            if (FD_ISSET(STDIN_FILENO, &rset))
            {
                System::IOBuffer buf;
                auto stdinRead = System::read(STDIN_FILENO, buf);
                if (stdinRead.count() == 0)
                {
                    standardInputEOF = true;
                    System::shutdownWrite(clientSocket);
                    FD_CLR(STDIN_FILENO, &rset);
                }
                else
                {
                    auto errorCode
                        = System::write(clientSocket, buf, stdinRead.count())
                              .errorCode();
                    if (errorCode)
                        throw std::system_error{errorCode};
                }
            }
        }
    }
}
