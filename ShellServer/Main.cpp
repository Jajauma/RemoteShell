#include "Bind.hpp"
#include "ProcessClientsLoop.hpp"
#include "System/FileDescriptor.hpp"
#include "System/Listen.hpp"

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    System::FileDescriptor serverSocket;
    try
    {
        switch (argc)
        {
        case 2:
            serverSocket = ShellServer::bind(argv[1]);
            break;
        case 3:
            serverSocket = ShellServer::bind(argv[1], argv[2]);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [<addr>] <port>"
                      << std::endl;
            return EXIT_FAILURE;
        }
        System::listen(serverSocket);
        ShellServer::processClientsLoop(serverSocket);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
