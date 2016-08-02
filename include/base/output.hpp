#pragma once
namespace MTGos {
namespace Base {
/**
 * \brief Contains the useable bases in the number-printing routine
 */
enum class Base : int {
    BINARY=2,
    TERNARY,
    BASE4,
    BASE5,
    BASE6,
    BASE7,
    OCTAL,
    BASE9,
    DECIMAL,
    BASE11,
    BASE12,
    BASE13,
    BASE14,
    BASE15,
    HEXADECIMAL
};
/**
 * \class Output
 * \brief Base class for output classes
 */
class Output {
private:
    /**
     * \brief Output of a character (UTF-32)
     */
    virtual auto putChar(int) -> void {}
    /**
     * \brief Output of an ASCII-char
     */
    auto putChar(char) -> void;
    /**
     * \brief Contains the base stored for number output
     */
    int base=10;
public:
    Output() {}
    /**
     * \brief Outputs an generic null-terminated ASCII string
     */
    auto puts(const char*) -> void;
    /**
     * \brief Outputs an object of any type
     */
    template <typename T> auto operator<<(T object) -> Output & {
        puts(object);
        return *this;
    }
};
template <>
auto Output::operator<<<Base>(Base) -> Output &;
template <>
auto Output::operator<<<int>(int) -> Output&;
template <>
auto Output::operator<<<long long int>(long long int) -> Output&;
template <>
auto Output::operator<<<unsigned int>(unsigned int) -> Output&;
template <>
auto Output::operator<<<unsigned long long int>(unsigned long long int) -> Output&;
template <>
auto Output::operator<<<char>(char) -> Output&;
}
}