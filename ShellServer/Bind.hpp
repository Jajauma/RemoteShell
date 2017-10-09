#ifndef HEADER_F5F0B2C4_ACE8_11E7_BB02_74D435E5BEA6_INCLUDED
#define HEADER_F5F0B2C4_ACE8_11E7_BB02_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
class GetAddrInfo;
} /* namespace System  */

namespace ShellServer {
System::FileDescriptor bind(const System::GetAddrInfo& addrList);
System::FileDescriptor bind(const char* host, const char* port);
System::FileDescriptor bind(const char* port);
} /* namespace ShellServer  */

#endif /* end of the header guard */
