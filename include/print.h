# pragma once

#include <type_traits>
#include <iostream>
#include <cstdint>
#include <vector>
#include <list>

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_ip(const T& value)
{
    const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
    
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        if (i != 0) {
            std::cout << ".";
        }
        std::cout << static_cast<int>(bytes[sizeof(T) - 1 - i]);
    }
    std::cout << std::endl;
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
print_ip(const T& value)
{
    std::cout << value << std::endl;
}

template<typename T>
struct is_container : std::false_type {};

template<typename T, typename A>
struct is_container<std::vector<T, A>> : std::true_type {};

template<typename T, typename A>
struct is_container<std::list<T, A>> : std::true_type {};

template<typename Container>
typename std::enable_if<is_container<Container>::value>::type
print_ip(const Container& container)
{
    bool first = true;
    for (const auto& p : container)
    {
        if (!first) std::cout << ".";
        std::cout << p;
        first = false;
    }
    std::cout << std::endl;
}

template<typename...> struct all_same;

template<typename T>
struct all_same<T> : std::true_type {};

template<typename T, typename U, typename... Ts>
struct all_same<T, U, Ts...> : std::conditional<std::is_same<T, U>::value, all_same<T, Ts...>, std::false_type>::type {};

template<size_t Index, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index == sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << std::endl;
}

template<size_t Index = 0, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index < sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << ".";
    print_ip<Index + 1>(t);
}