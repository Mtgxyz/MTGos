#include <base/output.hpp>
#include <stdint.h>
namespace MTGos {
namespace Base {
auto Output::putChar(char c) -> void {
    putChar((int)c);
}
auto Output::puts(const char* s) -> void {
    int i=0;
    while(s[i])
        putChar(s[i++]);
}
template <>
auto Output::operator<<<Base>(Base output) -> Output & {
    base=static_cast<int>(output);
    return *this;
}
template <>
auto Output::operator<<<int>(int output) -> Output & {
    return *this << (unsigned int) output;
}
template <>
auto Output::operator<<<long long int>(long long int output) -> Output & {
    return *this << (unsigned long long int) output;
}
template <>
auto Output::operator<<<unsigned int>(unsigned int output) -> Output & {
    return *this << (unsigned long long int) output;
}
template <>
auto Output::operator<<<unsigned long long int>(unsigned long long int output) -> Output & {
    const char* chars="01234567890ABCDEF";
    char buf[65];
    buf[64]='\0';
    char* ptr=buf+63;
    do {
        *(ptr--)=chars[output%base];
        output/=base;
    } while (output && (ptr!=buf));
    puts(ptr+1);
    return *this;
}
template <>
auto Output::operator<<<char>(char output) -> Output & {
    putChar(output);
    return *this;
}
}
}