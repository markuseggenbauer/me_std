#ifndef ME_STD_OPTIONAL_REF_HPP
#define ME_STD_OPTIONAL_REF_HPP

#include <cassert>
#include <optional>
#include <type_traits>

namespace me_std {

template <typename T>
class optional_ref {
  static_assert(std::is_lvalue_reference<T>::value == true,
                "Template argument T must be a reference type.");
  static_assert(std::is_const<std::remove_reference_t<T>>::value == true,
                "Template argument T must be a constant qualified type.");

 public:
  using value_type = std::decay_t<T>;
  using reference_type = T;

  optional_ref() = default;
  optional_ref(reference_type value) : m_value{&value} {}
  optional_ref(std::optional<value_type> const &optional)
      : m_value{optional.has_value() ? optional.operator->() : nullptr} {}

  auto has_value() const noexcept { return m_value != nullptr; }

  auto operator*() const noexcept {
    assert(has_value());
    return *m_value;
  }

  auto operator->() const noexcept {
    assert(has_value());
    return m_value;
  }

  auto value() const {
    if (!has_value()) {
      throw std::bad_optional_access{};
    }
    return *m_value;
  }

  operator std::optional<value_type>() const noexcept {
    if (has_value()) {
      return std::optional<value_type>{value()};
    }
    return std::optional<value_type>{};
  }

  auto operator==(optional_ref<reference_type> other) const noexcept {
    if (m_value == other.m_value) {
      return true;
    }

    if ((m_value != nullptr) && (other.m_value != nullptr)) {
      return *m_value == *other.m_value;
    }

    return false;
  }

  auto operator<(optional_ref<reference_type> other) const noexcept {
    if (m_value == other.m_value) {
      return false;
    }

    if (m_value == nullptr) {
      return true;
    }

    if (other.m_value == nullptr) {
      return false;
    }

    return *m_value < *other.m_value;
  }

 private:
  value_type const *m_value{nullptr};
};  // namespace me_std

template <typename T>
bool operator==(T const &lhs, optional_ref<T const &> rhs) {
  return rhs.has_value() && (lhs == *rhs);
}

template <typename T>
bool operator==(optional_ref<T const &> lhs, T const &rhs) {
  return lhs.has_value() && (*lhs == rhs);
}

template <typename T>
bool operator<(T const &lhs, optional_ref<T const &> rhs) {
  return rhs.has_value() && (lhs < *rhs);
}

template <typename T>
bool operator<(optional_ref<T const &> lhs, T const &rhs) {
  return !lhs.has_value() || (*lhs < rhs);
}

template <typename T>
bool operator!=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(T const &lhs, optional_ref<T const &> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(optional_ref<T const &> lhs, T const &rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator<=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(T const &lhs, optional_ref<T const &> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(optional_ref<T const &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator>(optional_ref<T> lhs, optional_ref<T> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(T const &lhs, optional_ref<T const &> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(optional_ref<T const &> lhs, T const &rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(T const &lhs, optional_ref<T const &> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(optional_ref<T const &> lhs, T const &rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

}  // namespace me_std

#endif  // ME_STD_OPTIONAL_REF_HPP
