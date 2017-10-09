#include "ProcessClientsLoop.hpp"
#include "Cxx/Unused.hpp"
#include "System/FileDescriptor.hpp"

#include <unistd.h>

void
ShellServer::processClientsLoop(System::FileDescriptor& serverSocket)
{
    Cxx::Unused(serverSocket);
    ::pause();
}
