#ifndef ME_STD_OPTIONAL_REF_HPP
#define ME_STD_OPTIONAL_REF_HPP

#include <cassert>
#include <type_traits>

namespace me_std {

template <typename T>
class optional_ref {
  static_assert(std::is_lvalue_reference<T>::value == true,
                "Template argument T must be a reference type.");
  static_assert(std::is_const<std::remove_reference_t<T>>::value == true,
                "Template argument T must be a constant qualified type.");

 public:
  optional_ref() = default;
  optional_ref(T value) : m_value{&value} {}

  bool has_value() const noexcept { return m_value != nullptr; }
  T value() const noexcept {
    assert(has_value());
    return *m_value;
  }

  operator bool() const noexcept { return has_value(); }

  bool operator==(optional_ref<T> other) const noexcept {
    if (m_value == other.m_value) {
      return true;
    }

    if ((m_value != nullptr) && (other.m_value != nullptr)) {
      return *m_value == *other.m_value;
    }

    return false;
  }

  bool operator<(optional_ref<T> other) const noexcept {
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
  std::decay_t<T> const* m_value{nullptr};
};

template <typename T>
bool operator==(T const& lhs, optional_ref<T const&> rhs) {
  return rhs && (lhs == rhs.value());
}

template <typename T>
bool operator==(optional_ref<T const&> lhs, T const& rhs) {
  return lhs && (lhs.value() == rhs);
}

template <typename T>
bool operator<(T const& lhs, optional_ref<T const&> rhs) {
  return rhs && (lhs < rhs.value());
}

template <typename T>
bool operator<(optional_ref<T const&> lhs, T const& rhs) {
  return !lhs || (lhs.value() < rhs);
}

template <typename T>
bool operator!=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(T const& lhs, optional_ref<T const&> rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(optional_ref<T const&> lhs, T const& rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator<=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(T const& lhs, optional_ref<T const&> rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator<=(optional_ref<T const&> lhs, T const& rhs) {
  return (lhs == rhs) || (lhs < rhs);
}

template <typename T>
bool operator>(optional_ref<T> lhs, optional_ref<T> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(T const& lhs, optional_ref<T const&> rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>(optional_ref<T const&> lhs, T const& rhs) {
  return !(lhs <= rhs);
}

template <typename T>
bool operator>=(optional_ref<T> lhs, optional_ref<T> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(T const& lhs, optional_ref<T const&> rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

template <typename T>
bool operator>=(optional_ref<T const&> lhs, T const& rhs) {
  return (lhs == rhs) || (lhs > rhs);
}

}  // namespace me_std

#endif  // ME_STD_OPTIONAL_REF_HPP
