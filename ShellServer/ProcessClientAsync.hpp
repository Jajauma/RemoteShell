#ifndef HEADER_4A305EC4_ADB2_11E7_B109_74D435E5BEA6_INCLUDED
#define HEADER_4A305EC4_ADB2_11E7_B109_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
} /* namespace System */

namespace ShellServer {
void processClientAsync(System::FileDescriptor& serverSocket,
                        System::FileDescriptor& clientSocket);
} /* namespace ShellServer */

#endif /* end of the header guard */
