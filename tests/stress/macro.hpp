#pragma once

#include <twist/rt/run.hpp>
#include <twist/rt/run/env.hpp>
#include <wheels/test/framework.hpp>

struct TestEnv : twist::rt::IEnv {

  size_t Seed() const override { return wheels::test::TestHash(); }

  bool KeepRunning() const override {
    static const auto kSafeMargin = 1000ms;
    return wheels::test::TestTimeLeft() > kSafeMargin;
  }

  void PrintLine(std::string message) override {
    std::cout << message << std::endl;
  }

  void Exception() override { wheels::test::FailTestByException(); }

  void Assert(wheels::SourceLocation where, std::string reason) override {
    wheels::test::FailTestByAssert({reason, where});
  }
};

//================================================================================

inline twist::rt::IEnv *TestEnv() {
  static struct TestEnv instance;
  return &instance;
}

#define _TWIST_STRESS_TEST_IMPL(name, options)                                 \
  void TwistTestRoutine##name();                                               \
  TEST(name, options) {                                                        \
    ::twist::rt::Run(TestEnv(), [] { TwistTestRoutine##name(); });             \
  }                                                                            \
  void TwistTestRoutine##name()

//================================================================================

#define TWIST_STRESS_TEST(name, budget)                                        \
  _TWIST_STRESS_TEST_IMPL(name, ::wheels::test::TestOptions().TimeLimit(budget))

#define _TWIST_UNIT_TEST_IMPL(name, options)                                   \
  void TwistTestRoutine##name();                                               \
  TEST(name, options) {                                                        \
    ::twist::rt::Run(TestEnv(), [] { TwistTestRoutine##name(); });             \
  }                                                                            \
  void TwistTestRoutine##name()
