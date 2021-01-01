#include <me_std/optional_ref.hpp>
#include <string>

#include "gtest/gtest.h"

namespace {
using TestTypes = ::testing::Types<int const&, std::string const&>;

template <typename T>
T get_value();

template <>
int const& get_value<int const&>() {
  static int const value = 42;
  return value;
}

template <>
std::string const& get_value<std::string const&>() {
  static std::string const value{"Hello World"};
  return value;
}

template <typename T>
T get_large_value();

template <>
int const& get_large_value<int const&>() {
  static int const value = 43;
  return value;
}

template <>
std::string const& get_large_value<std::string const&>() {
  static std::string const value{"Last Hello World"};
  return value;
}

template <typename T>
class OptionalRefTest : public ::testing::Test {};

TYPED_TEST_SUITE_P(OptionalRefTest);

TYPED_TEST_P(OptionalRefTest, DefaultConstruct) {
  me_std::optional_ref<TypeParam> object{};
  EXPECT_FALSE(object);
  EXPECT_FALSE(object.has_value());
}

TYPED_TEST_P(OptionalRefTest, DefaultValueConstruct) {
  me_std::optional_ref<TypeParam> object{TypeParam{}};
  EXPECT_TRUE(object);
  EXPECT_TRUE(object.has_value());
}

TYPED_TEST_P(OptionalRefTest, ValueConstruct) {
  me_std::optional_ref<TypeParam> object{get_value<TypeParam>()};
  EXPECT_TRUE(object);
  EXPECT_TRUE(object.has_value());
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefTest, DefaultConstruct, ValueConstruct,
                            DefaultValueConstruct);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefTest, TestTypes);

template <typename T>
class OptionalRefValueTest : public ::testing::Test {
 protected:
  T test_value = get_value<T>();
  T same_value = get_value<T>();
  T other_value = get_large_value<T>();

  me_std::optional_ref<T> test_value_ref{test_value};
  me_std::optional_ref<T> test_empty_ref{};

  me_std::optional_ref<T> empty_ref{};
  me_std::optional_ref<T> same_value_ref{same_value};
  me_std::optional_ref<T> other_value_ref{other_value};
};

TYPED_TEST_SUITE_P(OptionalRefValueTest);

TYPED_TEST_P(OptionalRefValueTest, Value) {
  EXPECT_EQ(this->test_value_ref.value(), this->test_value);
}

TYPED_TEST_P(OptionalRefValueTest, OperatorEQ) {
  EXPECT_TRUE(this->empty_ref == this->test_empty_ref);
  EXPECT_TRUE(this->test_empty_ref == this->empty_ref);

  EXPECT_FALSE(this->empty_ref == this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref == this->empty_ref);

  EXPECT_TRUE(this->same_value_ref == this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref == this->same_value_ref);

  EXPECT_TRUE(this->same_value == this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref == this->same_value);

  EXPECT_FALSE(this->other_value_ref == this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref == this->other_value_ref);

  EXPECT_FALSE(this->other_value == this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref == this->other_value);
}

TYPED_TEST_P(OptionalRefValueTest, OperatorNE) {
  EXPECT_FALSE(this->empty_ref != this->test_empty_ref);
  EXPECT_FALSE(this->test_empty_ref != this->empty_ref);

  EXPECT_TRUE(this->empty_ref != this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref != this->empty_ref);

  EXPECT_FALSE(this->same_value_ref != this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref != this->same_value_ref);

  EXPECT_FALSE(this->same_value != this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref != this->same_value);

  EXPECT_TRUE(this->other_value_ref != this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref != this->other_value_ref);

  EXPECT_TRUE(this->other_value != this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref != this->other_value);
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefValueTest, Value, OperatorEQ,
                            OperatorNE);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefValueTest, TestTypes);

}  // namespace
