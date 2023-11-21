#include <autocheck/arbitrary.hpp>
#include <autocheck/tuple.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace ac = autocheck;

static const size_t limit = 20;

TEST(ArbitraryBool, Size) {
  auto arb = ac::make_arbitrary<bool>();
  std::clog << "sizeof(arb) = " << sizeof(arb) << std::endl;
}

TEST(ArbitraryInt, Size) {
  auto arb = ac::make_arbitrary<int>();
  std::clog << "sizeof(arb) = " << sizeof(arb) << std::endl;
}

TEST(Arbitrary, Generating) {
  auto arb = ac::make_arbitrary<int>();

  ac::value<std::tuple<int>> args;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(arb(args));
    if (i > 0) std::clog << ", ";
    std::clog << args;
  }
  std::clog << std::endl;

  ASSERT_TRUE(arb(args));
}

TEST(Arbitrary, Only) {
  auto arb = ac::make_arbitrary<bool>();
  arb.discard_if([] (bool b) { return b; });

  ac::value<std::tuple<bool>> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    arb(b);
    ASSERT_FALSE(std::get<0>(b.cref()));
    if (i > 0) std::clog << ", ";
    std::clog << b;
  }
  std::clog << std::endl;
}

TEST(Arbitrary, AtMost) {
  auto arb = ac::make_arbitrary<bool>();
  arb.discard_if([] (bool b) { return b; });
  arb.discard_at_most(limit);

  ac::value<std::tuple<bool>> b;
  std::clog << std::boolalpha;
  bool first = true;
  while (arb(b)) {
    if (first) {
      first = false;
    } else {
      std::clog << ", ";
    }
    ASSERT_FALSE(std::get<0>(b.cref()));
    std::clog << b;
  }
  std::clog << std::endl;

  ASSERT_FALSE(arb(b));
}

TEST(Arbitrary, Combinator) {
  auto arb =
    ac::discard_if([] (bool b) { return !b; },
    ac::discard_at_most(limit,
    ac::make_arbitrary<bool>()));

  ac::value<std::tuple<bool>> b;
  while (arb(b)) {
    ASSERT_TRUE(std::get<0>(b.cref()));
  }

  ASSERT_FALSE(arb(b));
}

