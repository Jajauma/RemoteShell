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
                throw std::system_error{errno, std::generic_category()};
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
                    standardInputEOF = true;
                    System::shutdownWrite(conn);
                    FD_CLR(STDIN_FILENO, &rset);
                    continue;
                }
                else
                {
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
