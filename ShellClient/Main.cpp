#include <array>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>

#include <sys/select.h>
#include <unistd.h>

#include "Connect.hpp"
#include "System/FileDescriptor.hpp"
#include "System/IO.hpp"

int
main(int argc, char* argv[])
{
    System::FileDescriptor conn;

    try
    {
        switch (argc)
        {
        case 2:
            conn = ShellClient::connect(argv[1]);
            break;

        case 3:
            conn = ShellClient::connect(argv[1], argv[2]);
            break;

        default:
            std::cerr << "Usage: " << argv[0] << " [<addr>] <port>"
                      << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    bool checkStandardInput{true};
    bool checkSocket{true};

    fd_set rset;
    FD_ZERO(&rset);
    while (true)
    {
        if (checkStandardInput)
            FD_SET(STDIN_FILENO, &rset);
        if (checkSocket)
            FD_SET(conn.toNative(), &rset);
        auto maxFD1 = std::max(static_cast<int>(STDIN_FILENO),
                               static_cast<int>(conn.toNative()))
                      + 1;
        auto ret = ::select(maxFD1, &rset, nullptr, nullptr, nullptr);
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
                if (!socketRead)
                    continue;
                std::cerr << "Socket: +" << socketRead.count() << " bytes"
                          << std::endl;
                if (!socketRead.count())
                {
                    std::cerr << "Socket: EOF" << std::endl;
                    FD_CLR(conn.toNative(), &rset);
                    checkSocket = false;
                }
                if (socketRead.count() > 0)
                {
                    System::IOResult stdoutWrite;
                    do
                    {
                        stdoutWrite = System::write(
                            STDOUT_FILENO, buf, socketRead.count());
                    } while (!stdoutWrite);
                }
            }
            if (FD_ISSET(STDIN_FILENO, &rset))
            {
                System::IOBuffer buf;
                auto stdinRead = System::read(STDIN_FILENO, buf);
                if (!stdinRead)
                    continue;
                std::cerr << "Standard input: +" << stdinRead.count()
                          << " bytes" << std::endl;
                if (!stdinRead.count())
                {
                    std::cerr << "Standard input: EOF" << std::endl;
                    FD_CLR(STDIN_FILENO, &rset);
                    checkStandardInput = false;
                }
                if (stdinRead.count() > 0)
                {
                    System::IOResult socketWrite;
                    do
                    {
                        socketWrite
                            = System::write(conn, buf, stdinRead.count());
                    } while (!socketWrite);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
