#ifndef HEADER_B96B9FB0_AA74_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_B96B9FB0_AA74_11E7_A376_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor
{
public:
    using Handle = int;

    FileDescriptor();
    explicit FileDescriptor(Handle fd);
    FileDescriptor(FileDescriptor&& other);
    FileDescriptor& operator=(FileDescriptor&& other);
    ~FileDescriptor();

    Handle toNative() const;
    void close();

private:
    Handle mHandle;
};
} /* namespace System  */

#endif /* end of the header guard */
