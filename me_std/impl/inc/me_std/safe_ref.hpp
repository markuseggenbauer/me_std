#ifndef ME_STD_SAFE_REF_HPP
#define ME_STD_SAFE_REF_HPP

#include <cassert>
#include <memory>
#include <type_traits>

namespace me_std {

template <typename T>
class safe_ref {
  static_assert(std::is_lvalue_reference<T>::value == true,
                "Template argument T must be a reference type.");

 public:
  using value_type = std::decay_t<T>;
  using reference_type = T;

  safe_ref(reference_type ref) : m_ref{ref} {}

  safe_ref(safe_ref<T> const &other)
      : m_store{std::make_unique<value_type>(*other)}, m_ref{*m_store} {}
  safe_ref<T> &operator=(safe_ref<T> const &) = delete;
  ~safe_ref() = default;

  reference_type operator*() const noexcept { return m_ref; }

  auto operator==(safe_ref<reference_type> other) const noexcept { return m_ref == other.m_ref; }
  auto operator<(safe_ref<reference_type> other) const noexcept { return m_ref < other.m_ref; }

 private:
  std::unique_ptr<value_type> m_store{};
  reference_type m_ref;
};

template <typename T>
bool operator==(T const &lhs, safe_ref<T &> rhs) {
  return (lhs == *rhs);
}

template <typename T>
bool operator==(T const &lhs, safe_ref<T const &> rhs) {
  return (lhs == *rhs);
}

template <typename T>
bool operator==(safe_ref<T &> lhs, T const &rhs) {
  return (*lhs == rhs);
}

template <typename T>
bool operator==(safe_ref<T const &> lhs, T const &rhs) {
  return (*lhs == rhs);
}

template <typename T>
bool operator<(T const &lhs, safe_ref<T &> rhs) {
  return (lhs < *rhs);
}
template <typename T>
bool operator<(T const &lhs, safe_ref<T const &> rhs) {
  return (lhs < *rhs);
}

template <typename T>
bool operator<(safe_ref<T &> lhs, T const &rhs) {
  return (*lhs < rhs);
}
template <typename T>
bool operator<(safe_ref<T const &> lhs, T const &rhs) {
  return (*lhs < rhs);
}

template <typename T>
bool operator!=(safe_ref<T> lhs, safe_ref<T> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(T const &lhs, safe_ref<T &> rhs) {
  return !(lhs == rhs);
}
template <typename T>
bool operator!=(T const &lhs, safe_ref<T const &> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(safe_ref<T &> lhs, T const &rhs) {
  return !(lhs == rhs);
}
template <typename T>
bool operator!=(safe_ref<T const &> lhs, T const &rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator<=(safe_ref<T> lhs, safe_ref<T> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(T const &lhs, safe_ref<T &> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}
template <typename T>
bool operator<=(T const &lhs, safe_ref<T const &> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(safe_ref<T &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs < rhs);
}
template <typename T>
bool operator<=(safe_ref<T const &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator>(safe_ref<T> lhs, safe_ref<T> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(T const &lhs, safe_ref<T &> rhs) {
  return !(lhs <= rhs);
}
template <typename T>
bool operator>(T const &lhs, safe_ref<T const &> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(safe_ref<T &> lhs, T const &rhs) {
  return !(lhs <= rhs);
}
template <typename T>
bool operator>(safe_ref<T const &> lhs, T const &rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>=(safe_ref<T> lhs, safe_ref<T> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(T const &lhs, safe_ref<T &> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}
template <typename T>
bool operator>=(T const &lhs, safe_ref<T const &> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(safe_ref<T &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs > rhs);
}
template <typename T>
bool operator>=(safe_ref<T const &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

}  // namespace me_std

#endif  // ME_STD_SAFE_REF_HPP
