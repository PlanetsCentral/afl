/**
  *  \file u/t_base.hpp
  *  \brief Test for afl::base
  */
#ifndef AFL_U_T_BASE_HPP
#define AFL_U_T_BASE_HPP

#include <cxxtest/TestSuite.h>

class TestBaseBaseWeakLink : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseClonable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseClonableRef : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseClosure : public CxxTest::TestSuite {
 public:
    void testNullary();
    void testUnary();
    void testBinary();
    void testTernary();
    void testQuaternary();
    void testQuinary();
    void testSenary();

    void testNullaryFactory();
    void testUnaryFactory();
    void testBinaryFactory();
    void testTernaryFactory();
    void testQuaternaryFactory();
    void testQuinaryFactory();
    void testSenaryFactory();

    void testSmart();

    void testNull();
};

class TestBaseCountof : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseDeletable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseDeleter : public CxxTest::TestSuite {
 public:
    void testBase();
    void testMove();
};

class TestBaseEnumerator : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseGrowableMemory : public CxxTest::TestSuite {
 public:
    void testInstantiation();
    void testMix();
    void testConversion();
    void testVectorContent();
    void testDuplicate();
    void testAppend();
};

class TestBaseInlineMemory : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseInlineOptional : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseMemory : public CxxTest::TestSuite {
 public:
    void testInstantiation();
    void testMix();
    void testConversion();
    void testVectorConversion();
    void testVectorContent();
    void testSplit();
    void testObject();
    void testFind();
    void testNothing();
    void testFullRead();
    void testFindNot();
    void testCopy();
};

class TestBaseNullEnumerator : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseObservable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseOptional : public CxxTest::TestSuite {
 public:
    void testInt();
    void testString();
    void testClass();
    void testInline();
};

class TestBasePtr : public CxxTest::TestSuite {
 public:
    void testInt();
    void testObj();
    void testConstObj();
    void testRefCountedObj();
    void testDerivation();
    void testCopyDestructor();
    void testAssignDestructor();
};

class TestBaseRef : public CxxTest::TestSuite {
 public:
    void testObj();
    void testDerived();
    void testPtr();
    void testConstObj();
};

class TestBaseRefCounted : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseRunnable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseSignal : public CxxTest::TestSuite {
 public:
    void testFirst();
    void testSecond();
    void testSigDies();
    void testFirstArg();
    void testSecondArg();
    void testSigDiesArg();
    void testMulti();
    void testMultiRec();
    void testManyArgs();
    void testResult();
    void testResult2();
    void testNew();
    void testKillSelf();
};

class TestBaseSignalConnection : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseSignalHandler : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseStaticAssert : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseStoppable : public CxxTest::TestSuite {
 public:
    void testInterface();
};

class TestBaseTypes : public CxxTest::TestSuite {
 public:
    void test8();
    void test16();
    void test32();
    void test64();
    void testSize();
};

class TestBaseUncopyable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestBaseWeakLink : public CxxTest::TestSuite {
 public:
    void testIt();
    void testAssignment();
};

class TestBaseWeakTarget : public CxxTest::TestSuite {
 public:
    void testIt();
};

#endif
