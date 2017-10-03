#ifndef HEADER_DDD50F64_A85B_11E7_BC31_74D4359F3068_INCLUDED
#define HEADER_DDD50F64_A85B_11E7_BC31_74D4359F3068_INCLUDED

#include <iterator>

struct addrinfo;

namespace System {
enum class Protocol { TCP, TCP4, TCP6, UDP, UDP4, UDP6 };
enum class SpecialAddress { Loopback, Wildcard };

class GetAddrInfo
{
public:
    GetAddrInfo(Protocol protocol, const char* host, const char* port);
    GetAddrInfo(Protocol protocol, SpecialAddress specialAddress,
                const char* port);
    GetAddrInfo(GetAddrInfo&& other);
    GetAddrInfo& operator=(GetAddrInfo&& other);
    ~GetAddrInfo();

    class Iterator : public std::iterator<std::input_iterator_tag, addrinfo>
    {
    public:
        explicit Iterator(addrinfo* value = nullptr);
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        const addrinfo& operator*() const;

    private:
        addrinfo* mValue;
    };

    Iterator begin() const;
    Iterator end() const;
    int size() const;
    bool isEmpty() const;

private:
    addrinfo* mResult;
};
} /* namespace System  */

#endif /* end of the header guard */
