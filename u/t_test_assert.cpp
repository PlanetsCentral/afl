/**
  *  \file u/t_test_assert.cpp
  *  \brief Test for afl::test::Assert
  */

#include "afl/test/assert.hpp"

#include "t_test.hpp"
#include "afl/except/assertionfailedexception.hpp"

void
TestTestAssert::testIt()
{
    afl::test::Assert t("z");

    // Success cases
    TS_ASSERT_THROWS_NOTHING(t.check("hi", true));
    TS_ASSERT_THROWS_NOTHING(t.checkEqual("hi", 9, 9));
    TS_ASSERT_THROWS_NOTHING(t.checkEqual("hi", 9, 9L));
    TS_ASSERT_THROWS_NOTHING(t.checkEqual("hi", "a", String_t("a")));
    TS_ASSERT_THROWS_NOTHING(t.checkEqualContent("hi", afl::string::toMemory("axy"), afl::string::toMemory("axy")));

    // Error cases
    TS_ASSERT_THROWS(t.check("hi", false), afl::except::AssertionFailedException);
    TS_ASSERT_THROWS(t.checkEqual("hi", 1, 9), afl::except::AssertionFailedException);
    TS_ASSERT_THROWS(t.checkEqual("hi", 1, 9L), afl::except::AssertionFailedException);
    TS_ASSERT_THROWS(t.checkEqual("hi", "a", String_t("b")), afl::except::AssertionFailedException);
    TS_ASSERT_THROWS(t.checkEqualContent("hi", afl::string::toMemory("azy"), afl::string::toMemory("axy")), afl::except::AssertionFailedException);
}
