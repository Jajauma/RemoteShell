#ifndef HEADER_F4014074_ACF6_11E7_BB02_74D435E5BEA6_INCLUDED
#define HEADER_F4014074_ACF6_11E7_BB02_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
} /* namespace System  */

namespace ShellServer {
void acceptClientsLoop(System::FileDescriptor& serverSocket);
} /* namespace ShellServer  */

#endif /* end of the header guard */
