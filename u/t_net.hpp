/**
  *  \file u/t_net.hpp
  *  \brief Test for afl::net
  */
#ifndef AFL_U_T_NET_HPP
#define AFL_U_T_NET_HPP

#include <cxxtest/TestSuite.h>

class TestNetAcceptOperation : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetCommandHandler : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetHeaderConsumer : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetHeaderField : public CxxTest::TestSuite {
 public:
    void testNormal();
    void testSpecial();
    void testNoPrimary();
    void testAddress();
};

class TestNetMimeBuilder : public CxxTest::TestSuite {
 public:
    void testSimple();
    void testBase64();
    void testBoundaryConflict();
};

class TestNetMimeParser : public CxxTest::TestSuite {
 public:
    void testInit();
    void testSimple();
    void testMinLF();
    void testMinCRLF();
    void testFoldedHeader();
    void testGetFileName();
    void testGetFileName1();
    void testGetFileName2();
    void testQPBody();
    void testBase64Body();
    void testEncodedHeader();
    void testEncodedHeader2();
    void testEncodedHeader3();
    void testEncodedHeader4();
    void testTrace();
    void testHeader();
    void testClear();
    void testIncomplete();
    void testMultipart();
    void testMultipartBad();
    void testMultipartModif();
    void testMultipartMissing();
    void testMessage();
};

class TestNetNullCommandHandler : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetNullNetworkStack : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetParameterEncoder : public CxxTest::TestSuite {
 public:
    void testEmpty();
    void testOne();
    void testTwo();
    void testEncoding();
};

class TestNetHeaderParser : public CxxTest::TestSuite {
 public:
    void testPartition();
    void testValid();
    void testInvalid();
};

class TestNetHeaderTable : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetListener : public CxxTest::TestSuite {
 public:
    void testListen();
    void testAsync();
};

class TestNetName : public CxxTest::TestSuite {
 public:
    void testInit();
    void testParse();
    void testCompare();
};

class TestNetNetworkStack : public CxxTest::TestSuite {
 public:
    void testConnect();
    void testBadName();
};

class TestNetProtocolHandler : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetProtocolHandlerFactory : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetReconnectable : public CxxTest::TestSuite {
 public:
    void testInterface();
};

class TestNetSecureContext : public CxxTest::TestSuite {
 public:
    void testMock();
    void testConnect();
    void testTransfer1();
    void testTransfer2();
};

class TestNetSecureNetworkStack : public CxxTest::TestSuite {
 public:
    void testConnect();
    void testTransfer();
};

class TestNetSecureSocket : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetServer : public CxxTest::TestSuite {
 public:
    void testIt();
    void testStop();
};

class TestNetSimpleServer : public CxxTest::TestSuite {
 public:
    void testIt();
};

class TestNetSocket : public CxxTest::TestSuite {
 public:
    void testBidi();
    void testClose();
};

class TestNetUrl : public CxxTest::TestSuite {
 public:
    void testParse();
    void testMerge();
    void testSet();
    void testMatchPath();
    void testMatchArgs();
};

class TestNetInteract : public CxxTest::TestSuite {
 public:
    void testIt();
};

#endif
