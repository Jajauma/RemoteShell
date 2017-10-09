#ifndef HEADER_160FDCDC_AA92_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_160FDCDC_AA92_11E7_A376_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
class GetAddrInfo;
} /* namespace System  */

namespace ShellClient {
System::FileDescriptor connect(const System::GetAddrInfo& addrList);
System::FileDescriptor connect(const char* host, const char* port);
System::FileDescriptor connect(const char* port);
} /* namespace ShellClient  */

#endif /* end of the header guard */
