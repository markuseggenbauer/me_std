#include <array>
#include <me_std/optional_ref.hpp>
#include <optional>
#include <string>

#include "gtest/gtest.h"

namespace {
using TestTypes = ::testing::Types<int const &, std::string const &, int &, std::string &>;

enum class ValueType : int { test, same, larger };

template <typename T>
T get_value(ValueType);

template <>
int get_value<int>(ValueType type) {
  static std::array<int, 3> const value{42, 42, 43};
  return value[static_cast<std::underlying_type_t<ValueType>>(type)];
}

template <>
std::string get_value<std::string>(ValueType type) {
  static std::array<std::string, 3> const value{"Hello World", "Hello World", "Oh, Hello World"};
  return value[static_cast<std::underlying_type_t<ValueType>>(type)];
}

template <typename T>
class OptionalRefTest : public ::testing::Test {};

TYPED_TEST_SUITE_P(OptionalRefTest);

TYPED_TEST_P(OptionalRefTest, DefaultConstruct) {
  me_std::optional_ref<TypeParam> test_ref{};
  EXPECT_FALSE(test_ref.has_value());
}

TYPED_TEST_P(OptionalRefTest, DefaultValueConstruct) {
  me_std::optional_ref<TypeParam> test_ref{TypeParam{}};
  EXPECT_TRUE(test_ref.has_value());
}

TYPED_TEST_P(OptionalRefTest, ValueConstruct) {
  me_std::optional_ref<TypeParam> test_ref{get_value<std::decay_t<TypeParam>>(ValueType::test)};
  EXPECT_TRUE(test_ref.has_value());
}

TYPED_TEST_P(OptionalRefTest, OptionalValueConstruct) {
  std::optional<std::decay_t<TypeParam>> test_value_optional{
      get_value<std::decay_t<TypeParam>>(ValueType::test)};
  me_std::optional_ref<TypeParam> test_ref{test_value_optional};
  EXPECT_TRUE(test_ref.has_value());
  EXPECT_EQ(test_ref.value(), get_value<std::decay_t<TypeParam>>(ValueType::test));
}

TYPED_TEST_P(OptionalRefTest, OptionalEmptyConstruct) {
  std::optional<std::decay_t<TypeParam>> test_value_optional{};
  me_std::optional_ref<TypeParam> test_ref{test_value_optional};
  EXPECT_FALSE(test_ref.has_value());
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefTest, DefaultConstruct, DefaultValueConstruct,
                            ValueConstruct, OptionalValueConstruct, OptionalEmptyConstruct);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefTest, TestTypes);

template <typename T>
class OptionalRefValueTest : public ::testing::Test {
 protected:
  using test_value_type = std::decay_t<T>;
  test_value_type test_value = get_value<test_value_type>(ValueType::test);
  test_value_type same_value = get_value<test_value_type>(ValueType::same);
  test_value_type other_value = get_value<test_value_type>(ValueType::larger);
  test_value_type larger_value = get_value<test_value_type>(ValueType::larger);

  me_std::optional_ref<T> const test_value_ref{test_value};
  me_std::optional_ref<T> const test_empty_ref{};

  me_std::optional_ref<T> const empty_ref{};
  me_std::optional_ref<T> const same_value_ref{same_value};
  me_std::optional_ref<T> const other_value_ref{other_value};
  me_std::optional_ref<T> const larger_value_ref{larger_value};

  void SetUp() override {
    ASSERT_FALSE(test_empty_ref.has_value());
    ASSERT_TRUE(test_value_ref.has_value());
    ASSERT_TRUE(same_value_ref.has_value());
    ASSERT_TRUE(other_value_ref.has_value());
    ASSERT_TRUE(larger_value_ref.has_value());
  }
};

TYPED_TEST_SUITE_P(OptionalRefValueTest);

TYPED_TEST_P(OptionalRefValueTest, NoValue) {
  EXPECT_THROW(this->test_empty_ref.value(), std::bad_optional_access);
}

TYPED_TEST_P(OptionalRefValueTest, Value) {
  EXPECT_EQ(this->test_value_ref.value(), this->test_value);
  EXPECT_EQ(*this->test_value_ref, this->test_value);
  EXPECT_EQ(*(this->test_value_ref.operator->()), this->test_value);
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

TYPED_TEST_P(OptionalRefValueTest, OperatorGE) {
  EXPECT_TRUE(this->empty_ref >= this->test_empty_ref);
  EXPECT_TRUE(this->test_empty_ref >= this->empty_ref);

  EXPECT_FALSE(this->empty_ref >= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref >= this->empty_ref);

  EXPECT_TRUE(this->same_value_ref >= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref >= this->same_value_ref);

  EXPECT_TRUE(this->same_value >= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref >= this->same_value);

  EXPECT_TRUE(this->larger_value_ref >= this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref >= this->larger_value_ref);

  EXPECT_TRUE(this->larger_value >= this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref >= this->larger_value);
}

TYPED_TEST_P(OptionalRefValueTest, EmptyToOptional) {
  std::optional<std::decay_t<TypeParam>> test_value{this->test_empty_ref};
  EXPECT_FALSE(test_value.has_value());
}

TYPED_TEST_P(OptionalRefValueTest, ValueToOptional) {
  std::optional<std::decay_t<TypeParam>> test_value{this->test_value_ref};
  EXPECT_TRUE(test_value.has_value());
  EXPECT_EQ(test_value.value(), get_value<std::decay_t<TypeParam>>(ValueType::test));
}

REGISTER_TYPED_TEST_SUITE_P(OptionalRefValueTest, NoValue, Value, OperatorEQ, OperatorNE,
                            OperatorLT, OperatorLE, OperatorGT, OperatorGE, EmptyToOptional,
                            ValueToOptional);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, OptionalRefValueTest, TestTypes);

}  // namespace
