#pragma once

#include <type_traits>
#include <iostream>
#include <cstdint>
#include <vector>
#include <list>
#include <tuple>

/**
 * @brief Печать IP-адреса, представленного целочисленным типом.
 * 
 * Функция интерпретирует целочисленное значение как IP-адрес в сетевом порядке байтов (big-endian)
 * и выводит его в формате "байт1.байт2.байт3.байт4".
 * 
 * @tparam T Целочисленный тип (int8_t, int16_t, int32_t, int64_t, etc.)
 * @param value Значение для печати
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
 * Специализация для std::string. Просто выводит строку без изменений.
 * 
 * @tparam T Тип std::string
 * @param value Строка для вывода
 */
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
print_ip(const T& value)
{
    std::cout << value << std::endl;
}

/**
 * @brief Type trait для проверки, является ли тип контейнером.
 * 
 * По умолчанию возвращает false. Специализируется для поддерживаемых контейнеров.
 */
template<typename T>
struct is_container : std::false_type {};

/**
 * @brief Специализация для std::vector
 */
template<typename T, typename A>
struct is_container<std::vector<T, A>> : std::true_type {};

/**
 * @brief Специализация для std::list
 */
template<typename T, typename A>
struct is_container<std::list<T, A>> : std::true_type {};

/**
 * @brief Печать элементов контейнера как IP-адреса.
 * 
 * Элементы контейнера выводятся через точку в порядке их следования.
 * Поддерживает std::vector и std::list.
 * 
 * @tparam Container Тип контейнера (std::vector или std::list)
 * @param container Контейнер значений
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
 * @brief Проверка, что все типы в пакете одинаковы.
 * 
 * @tparam Ts Пакет типов для проверки
 */
template<typename...> struct all_same;

/**
 * @brief Базовый случай: один тип всегда удовлетворяет условию.
 */
template<typename T>
struct all_same<T> : std::true_type {};

/**
 * @brief Рекурсивная проверка совпадения типов.
 * 
 * Сравнивает первые два типа и рекурсивно проверяет остальные.
 */
template<typename T, typename U, typename... Ts>
struct all_same<T, U, Ts...> : std::conditional<
    std::is_same<T, U>::value,
    all_same<T, Ts...>,
    std::false_type
>::type {};

/**
 * @brief Базовый случай рекурсии для печати tuple.
 * 
 * Выводит последний элемент tuple и завершает строку.
 * 
 * @tparam Index Текущий индекс в tuple
 * @tparam Ts Пакет типов элементов tuple
 * @param t Кортеж значений
 */
template<size_t Index, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index == sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << std::endl;
}

/**
 * @brief Рекурсивная печать элементов tuple как IP-адреса. 
 * 
 * Функция рекурсивно обходит элементы tuple и выводит их как IP-адрес.
 * Между элементами ставятся точки, после последнего элемента - перевод строки.
 * 
 * @tparam Index Текущий индекс (по умолчанию 0)
 * @tparam Ts Пакет типов элементов tuple. Все типы должны совпадать.
 * @param t Кортеж значений
 */
template<size_t Index = 0, typename... Ts>
typename std::enable_if<all_same<Ts...>::value && Index < sizeof...(Ts) - 1>::type
print_ip(const std::tuple<Ts...>& t) {
    std::cout << std::get<Index>(t) << ".";
    print_ip<Index + 1>(t);
}
