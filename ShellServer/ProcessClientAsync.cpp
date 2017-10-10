#include "ProcessClientAsync.hpp"
#include "Cxx/Unused.hpp"

#include <unistd.h>

#include <cerrno>
#include <system_error>

void
ShellServer::processClientAsync(System::FileDescriptor& serverSocket,
                                System::FileDescriptor& clientSocket)
{
    Cxx::Unused(serverSocket);
    Cxx::Unused(clientSocket);
}
