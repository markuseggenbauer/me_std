#include <array>
#include <me_std/optional_ref.hpp>
#include <string>

#include "gtest/gtest.h"

namespace {
using TestTypes = ::testing::Types<int const&, std::string const&>;

enum class ValueType : int { test, same, larger };

template <typename T>
T get_value(ValueType);

template <>
int const& get_value<int const&>(ValueType type) {
  static std::array<int, 3> const value{42, 42, 43};
  return value[static_cast<std::underlying_type_t<ValueType>>(type)];
}

template <>
std::string const& get_value<std::string const&>(ValueType type) {
  static std::array<std::string, 3> const value{"Hello World", "Hello World",
                                                "Oh, Hello World"};
  return value[static_cast<std::underlying_type_t<ValueType>>(type)];
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
  me_std::optional_ref<TypeParam> object{get_value<TypeParam>(ValueType::test)};
  EXPECT_TRUE(object);
  EXPECT_TRUE(object.has_value());
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefTest, DefaultConstruct, ValueConstruct,
                            DefaultValueConstruct);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefTest, TestTypes);

template <typename T>
class OptionalRefValueTest : public ::testing::Test {
 protected:
  T const test_value = get_value<T>(ValueType::test);
  T const same_value = get_value<T>(ValueType::same);
  T const other_value = get_value<T>(ValueType::larger);
  T const larger_value = get_value<T>(ValueType::larger);

  me_std::optional_ref<T> const test_value_ref{test_value};
  me_std::optional_ref<T> const test_empty_ref{};

  me_std::optional_ref<T> const empty_ref{};
  me_std::optional_ref<T> const same_value_ref{same_value};
  me_std::optional_ref<T> const other_value_ref{other_value};
  me_std::optional_ref<T> const larger_value_ref{larger_value};
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

TYPED_TEST_P(OptionalRefValueTest, OperatorLT) {
  EXPECT_FALSE(this->empty_ref < this->test_empty_ref);
  EXPECT_FALSE(this->test_empty_ref < this->empty_ref);

  EXPECT_TRUE(this->empty_ref < this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref < this->empty_ref);

  EXPECT_FALSE(this->same_value_ref < this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref < this->same_value_ref);

  EXPECT_FALSE(this->same_value < this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref < this->same_value);

  EXPECT_FALSE(this->larger_value_ref < this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref < this->larger_value_ref);

  EXPECT_FALSE(this->larger_value < this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref < this->larger_value);
}

TYPED_TEST_P(OptionalRefValueTest, OperatorLE) {
  EXPECT_TRUE(this->empty_ref <= this->test_empty_ref);
  EXPECT_TRUE(this->test_empty_ref <= this->empty_ref);

  EXPECT_TRUE(this->empty_ref <= this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref <= this->empty_ref);

  EXPECT_TRUE(this->same_value_ref <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->same_value_ref);

  EXPECT_TRUE(this->same_value <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->same_value);

  EXPECT_FALSE(this->larger_value_ref <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->larger_value_ref);

  EXPECT_FALSE(this->larger_value <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->larger_value);
}

TYPED_TEST_P(OptionalRefValueTest, OperatorGT) {
  EXPECT_FALSE(this->empty_ref > this->test_empty_ref);
  EXPECT_FALSE(this->test_empty_ref > this->empty_ref);

  EXPECT_FALSE(this->empty_ref > this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref > this->empty_ref);

  EXPECT_FALSE(this->same_value_ref > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->same_value_ref);

  EXPECT_FALSE(this->same_value > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->same_value);

  EXPECT_TRUE(this->larger_value_ref > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->larger_value_ref);

  EXPECT_TRUE(this->larger_value > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->larger_value);
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefValueTest, Value, OperatorEQ, OperatorNE,
                            OperatorLT, OperatorLE, OperatorGT);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefValueTest, TestTypes);

}  // namespace
