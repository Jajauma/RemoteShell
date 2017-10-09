#include "MultiplexIOLoop.hpp"
#include "System/FileDescriptor.hpp"
#include "System/IO.hpp"
#include "System/Shutdown.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include <sys/select.h>
#include <unistd.h>

void
ShellClient::multiplexIOLoop(const System::FileDescriptor& conn)
{
    bool standardInputEOF{false};
    fd_set rset;
    FD_ZERO(&rset);

    while (true)
    {
        if (!standardInputEOF)
            FD_SET(STDIN_FILENO, &rset);
        FD_SET(conn.toNative(), &rset);

        auto maxFD1 = std::max<int>(STDIN_FILENO, conn.toNative()) + 1;
        auto ret    = ::select(maxFD1, &rset, nullptr, nullptr, nullptr);

        if (ret == -1)
            switch (errno)
            {
            case EINTR:
                continue;
            default:
                throw std::system_error{errno, std::system_category()};
            }
        else if (ret > 0)
        {
            if (FD_ISSET(conn.toNative(), &rset))
            {
                System::IOBuffer buf;
                auto socketRead = System::read(conn, buf);
                if (socketRead.isInterrupted())
                    continue;
                if (socketRead.count() == 0)
                {
                    if (standardInputEOF)
                        return;
                    else
                        throw std::runtime_error{
                            "Server terminated prematurely"};
                }
                else
                {
                    std::cerr << "Socket: +" << socketRead.count() << " bytes"
                              << std::endl;
                    System::IOResult stdoutWrite;
                    do
                    {
                        stdoutWrite = System::write(
                            STDOUT_FILENO, buf, socketRead.count());
                    } while (stdoutWrite.isInterrupted());
                }
            }
            if (FD_ISSET(STDIN_FILENO, &rset))
            {
                System::IOBuffer buf;
                auto stdinRead = System::read(STDIN_FILENO, buf);
                if (stdinRead.isInterrupted())
                    continue;
                if (stdinRead.count() == 0)
                {
                    std::cerr << "Standard input: EOF" << std::endl;
                    standardInputEOF = true;
                    System::shutdownWrite(conn);
                    FD_CLR(STDIN_FILENO, &rset);
                    continue;
                }
                else
                {
                    std::cerr << "Standard input: +" << stdinRead.count()
                              << " bytes" << std::endl;
                    System::IOResult socketWrite;
                    do
                    {
                        socketWrite
                            = System::write(conn, buf, stdinRead.count());
                    } while (socketWrite.isInterrupted());
                }
            }
        }
    }
}
