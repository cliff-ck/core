#ifndef CORE_UTILITY_HPP
#define CORE_UTILITY_HPP

#include <type_traits>
#include <utility>

#include <cstddef>

namespace core {
inline namespace v1 {
namespace impl {

template <class T, T... I> struct integer_sequence {
  static_assert(
    std::is_integral<T>::value,
    "integer_sequence must use an integral type"
  );

  template <T N> using append = integer_sequence<T, I..., N>;
  static constexpr std::size_t size () noexcept { return sizeof...(I); }
  using next = append<size()>;
  using type = T;
};

template <class T, T Index, std::size_t N>
struct sequence_generator {
  static_assert(Index >= 0, "Index cannot be negative");
  using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
};

template <class T, T Index>
struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

template <std::size_t Index, class T, class U, class... Types>
struct typelist_index {
  using type = typename typelist_index<Index + 1, T, Types...>::type;
  static constexpr std::size_t value = Index;
};

template <std::size_t Index, class T, class... Types>
struct typelist_index<Index, T, T, Types...> {
  using type = typelist_index;
  static constexpr std::size_t value = Index;
};

} /* namespace impl */
template <class T, T... I>
using integer_sequence = impl::integer_sequence<T, I...>;

template <std::size_t... I>
using index_sequence = integer_sequence<std::size_t, I...>;

template <class T, T N>
using make_integer_sequence = typename impl::sequence_generator<T, N, N>::type;

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

/* N3761 */
template <std::size_t N, class T, class... Ts>
struct type_at { using type = typename type_at<N - 1, Ts...>::type; };

template <class T, class... Ts>
struct type_at<0ul, T, Ts...> { using type = T; };

template <std::size_t N, class T, class... Ts>
auto value_at(T&&, Ts&&... values) ->  typename type_at<N - 1, T, Ts...>::type {
  return value_at<N - 1, Ts...>(std::forward<Ts>(values)...);
}

template <class T, class... Ts>
auto value_at(T&& value, Ts&&...) -> decltype(std::forward<T>(value)) {
  return std::forward<T>(value);
}

}} /* namespace core::v1 */

#endif /* CORE_UTILITY_HPP */