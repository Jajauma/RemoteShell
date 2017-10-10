#ifndef HEADER_B96B9FB0_AA74_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_B96B9FB0_AA74_11E7_A376_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor
{
public:
    FileDescriptor();
    explicit FileDescriptor(int fd);
    FileDescriptor(FileDescriptor&& other);
    FileDescriptor& operator=(FileDescriptor&& other);
    ~FileDescriptor();

    int toNative() const;
    void close();

private:
    static constexpr int None{-1};
    int mHandle;
};
} /* namespace System  */

#endif /* end of the header guard */
