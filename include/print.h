# pragma once

#include <type_traits>
#include <iostream>
#include <cstdint>
#include <vector>
#include <list>

/**
 * @brief Печать IP-адреса, представленного целочисленным типом.
 *
 * Интерпретирует переданное целочисленное значение как набор байтов
 * в сетевом формате (big-endian) и выводит их через точку.
 *
 * @tparam T Целочисленный тип.
 * @param value Значение для печати.
 */
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

/**
 * @brief Печать строки как IP-адреса.
 *
 * Специализация для std::string — просто выводит строку.
 *
 * @tparam T std::string
 * @param value Строка для вывода.
 */
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
print_ip(const T& value)
{
    std::cout << value << std::endl;
}

/**
 * @brief Type trait: является ли тип контейнером.
 */
template<typename T>
struct is_container : std::false_type {};

/// Специализация для std::vector

template<typename T, typename A>
struct is_container<std::vector<T, A>> : std::true_type {};

/// Специализация для std::list

template<typename T, typename A>
struct is_container<std::list<T, A>> : std::true_type {};

/**
 * @brief Печать элементов контейнера как IP-адреса.
 *
 * Элементы выводятся через точку в порядке следования.
 *
 * @tparam Container Тип контейнера: std::vector или std::list.
 * @param container Контейнер значений.
 */
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

/**
 * @brief Проверка, что все типы одинаковы.
 */
template<typename...> struct all_same;

/// База: один тип — всегда true
template<typename T>
struct all_same<T> : std::true_type {};

/// Рекурсивная проверка

template<typename T, typename U, typename... Ts>
struct all_same<T, U, Ts...> : std::conditional<
    std::is_same<T, U>::value,
    all_same<T, Ts...>,
    std::false_type
>::type {};

/**
 * @brief Печать последнего элемента tuple как завершение IP-адреса.
 *
 * @tparam Index Индекс последнего элемента.
 * @tparam Ts Пакет типов. Все типы должны совпадать.
 * @param t Кортеж значений.
 */
template<size_t Index, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index == sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << std::endl;
}

/**
 * @brief Рекурсивная печать элементов tuple как IP-адреса.
 *
 * Каждый элемент выводится, за исключением последнего, после него ставится точка.
 *
 * @tparam Index Текущий индекс.
 * @tparam Ts Пакет типов. Все типы должны совпадать.
 * @param t Кортеж значений.
 */
template<size_t Index = 0, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index < sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << ".";
    print_ip<Index + 1>(t);
}
