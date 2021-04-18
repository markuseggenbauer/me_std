#include <array>
#include <me_std/safe_ref.hpp>
#include <memory>
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
class SafeRefTest : public ::testing::Test {};

TYPED_TEST_SUITE_P(SafeRefTest);

TYPED_TEST_P(SafeRefTest, DefaultValueConstruct) {
  auto test_value = std::decay_t<TypeParam>{};
  me_std::safe_ref<TypeParam> test_ref{test_value};
  EXPECT_EQ(&test_value, &(*test_ref));
  EXPECT_EQ(test_value, *test_ref);
}

TYPED_TEST_P(SafeRefTest, ValueConstruct) {
  auto test_value = get_value<std::decay_t<TypeParam>>(ValueType::test);
  me_std::safe_ref<TypeParam> test_ref{test_value};
  EXPECT_EQ(&test_value, &(*test_ref));
  EXPECT_EQ(test_value, *test_ref);
}

TYPED_TEST_P(SafeRefTest, CopyConstruct) {
  auto test_value = get_value<std::decay_t<TypeParam>>(ValueType::test);
  std::unique_ptr<me_std::safe_ref<TypeParam>> copy_test_ref{};
  {
    auto test_copy_value = test_value;
    me_std::safe_ref<TypeParam> test_ref{test_copy_value};
    copy_test_ref = std::make_unique<me_std::safe_ref<TypeParam>>(test_ref);
  }
  EXPECT_EQ(test_value, **copy_test_ref);
}

REGISTER_TYPED_TEST_SUITE_P(SafeRefTest, DefaultValueConstruct, ValueConstruct, CopyConstruct);
INSTANTIATE_TYPED_TEST_SUITE_P(ME, SafeRefTest, TestTypes);

template <typename T>
class SafeRefValueTest : public ::testing::Test {
 protected:
  using test_value_type = std::decay_t<T>;
  test_value_type test_value = get_value<test_value_type>(ValueType::test);
  test_value_type same_value = get_value<test_value_type>(ValueType::same);
  test_value_type other_value = get_value<test_value_type>(ValueType::larger);
  test_value_type larger_value = get_value<test_value_type>(ValueType::larger);

  me_std::safe_ref<T> const test_value_ref{test_value};
  me_std::safe_ref<T> const same_value_ref{same_value};
  me_std::safe_ref<T> const other_value_ref{other_value};
  me_std::safe_ref<T> const larger_value_ref{larger_value};
};

TYPED_TEST_SUITE_P(SafeRefValueTest);

TYPED_TEST_P(SafeRefValueTest, Value) {
  EXPECT_EQ(this->test_value_ref, this->test_value);
  EXPECT_EQ(*this->test_value_ref, this->test_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorEQ) {
  EXPECT_TRUE(this->same_value_ref == this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref == this->same_value_ref);

  EXPECT_TRUE(this->same_value == this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref == this->same_value);

  EXPECT_FALSE(this->other_value_ref == this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref == this->other_value_ref);

  EXPECT_FALSE(this->other_value == this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref == this->other_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorNE) {
  EXPECT_FALSE(this->same_value_ref != this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref != this->same_value_ref);

  EXPECT_FALSE(this->same_value != this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref != this->same_value);

  EXPECT_TRUE(this->other_value_ref != this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref != this->other_value_ref);

  EXPECT_TRUE(this->other_value != this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref != this->other_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorLT) {
  EXPECT_FALSE(this->same_value_ref < this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref < this->same_value_ref);

  EXPECT_FALSE(this->same_value < this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref < this->same_value);

  EXPECT_FALSE(this->larger_value_ref < this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref < this->larger_value_ref);

  EXPECT_FALSE(this->larger_value < this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref < this->larger_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorLE) {
  EXPECT_TRUE(this->same_value_ref <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->same_value_ref);

  EXPECT_TRUE(this->same_value <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->same_value);

  EXPECT_FALSE(this->larger_value_ref <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->larger_value_ref);

  EXPECT_FALSE(this->larger_value <= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref <= this->larger_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorGT) {
  EXPECT_FALSE(this->same_value_ref > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->same_value_ref);

  EXPECT_FALSE(this->same_value > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->same_value);

  EXPECT_TRUE(this->larger_value_ref > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->larger_value_ref);

  EXPECT_TRUE(this->larger_value > this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref > this->larger_value);
}

TYPED_TEST_P(SafeRefValueTest, OperatorGE) {
  EXPECT_TRUE(this->same_value_ref >= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref >= this->same_value_ref);

  EXPECT_TRUE(this->same_value >= this->test_value_ref);
  EXPECT_TRUE(this->test_value_ref >= this->same_value);

  EXPECT_TRUE(this->larger_value_ref >= this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref >= this->larger_value_ref);

  EXPECT_TRUE(this->larger_value >= this->test_value_ref);
  EXPECT_FALSE(this->test_value_ref >= this->larger_value);
}

REGISTER_TYPED_TEST_SUITE_P(SafeRefValueTest, Value, OperatorEQ, OperatorNE, OperatorLT, OperatorLE,
                            OperatorGT, OperatorGE);

INSTANTIATE_TYPED_TEST_SUITE_P(ME, SafeRefValueTest, TestTypes);

}  // namespace
