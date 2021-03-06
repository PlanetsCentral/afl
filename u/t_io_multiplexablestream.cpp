/**
  *  \file u/t_io_multiplexablestream.cpp
  *  \brief Test for afl::io::MultiplexableStream
  */

#include "afl/io/multiplexablestream.hpp"

#include "u/t_io.hpp"
#include "afl/io/memorystream.hpp"

/** Test parallel access. */
void
TestIoMultiplexableStream::testAccess()
{
    // We'll be using a MemoryStream as sample implementation.
    // Make sure it actually is derived from MultiplexableStream.
    {
        afl::io::MemoryStream* ms = 0;
        afl::io::MultiplexableStream* muxs = ms;
        (void) muxs;
    }

    // Try it
    {
        // Preinitialize
        static uint8_t data[] = { 1,2,3,4,5 };
        afl::io::MemoryStream ms(data);
        TS_ASSERT_EQUALS(ms.getSize(), 5U);
        ms.setPos(2);

        // Create children
        afl::base::Ref<afl::io::Stream> one = ms.createChild();
        afl::base::Ref<afl::io::Stream> two = ms.createChild();
        TS_ASSERT(&one.get() != 0);
        TS_ASSERT(&two.get() != 0);

        TS_ASSERT_EQUALS(one->getSize(), 5U);
        TS_ASSERT_EQUALS(two->getSize(), 5U);

        TS_ASSERT_EQUALS(one->getPos(), 0U);
        TS_ASSERT_EQUALS(two->getPos(), 0U);

        // Access children
        uint8_t tmp[2];
        size_t n = two->read(tmp);
        TS_ASSERT_EQUALS(n, 2U);
        TS_ASSERT_EQUALS(two->getPos(), 2U);
        TS_ASSERT_EQUALS(one->getPos(), 0U);
        TS_ASSERT_EQUALS(tmp[0], 1U);
        TS_ASSERT_EQUALS(tmp[1], 2U);

        uint8_t tmp2[3];
        n = one->read(tmp2);
        TS_ASSERT_EQUALS(n, 3U);
        TS_ASSERT_EQUALS(one->getPos(), 3U);
        TS_ASSERT_EQUALS(two->getPos(), 2U);
        TS_ASSERT_EQUALS(tmp2[0], 1U);
        TS_ASSERT_EQUALS(tmp2[1], 2U);
        TS_ASSERT_EQUALS(tmp2[2], 3U);
    }
}

/** Test lifetime management. */
void
TestIoMultiplexableStream::testLife()
{
    // Try releasing streams in different orders.
    const size_t COUNT = 4;
    for (size_t access = 0; access < COUNT+1; ++access) {
        for (size_t freeFirst = 0; freeFirst < COUNT; ++freeFirst) {
            for (size_t freeSecond = 0; freeSecond < COUNT; ++freeSecond) {
                // Create it
                static uint8_t data[] = { 1,2,3,4,5 };
                afl::io::MemoryStream ms(data);
                afl::base::Ptr<afl::io::Stream> children[COUNT] = {
                    ms.createChild().asPtr(),
                    ms.createChild().asPtr(),
                    ms.createChild().asPtr(),
                    ms.createChild().asPtr()
                };

                // Access and thus activate one
                if (access < COUNT) {
                    uint8_t tmp[1];
                    children[access]->read(tmp);
                }

                // Release in order
                children[freeFirst] = 0;
                children[freeSecond] = 0;
            }
        }
    }
}

/** Test setPos() behaviour.
    If the first operation on a new stream is setPos(), only one setPos() call should be done. */
void
TestIoMultiplexableStream::testSeek()
{
    class Tester : public afl::io::MultiplexableStream {
     public:
        virtual size_t read(Bytes_t /*m*/)
            { throw std::runtime_error("unexpected"); }
        virtual size_t write(ConstBytes_t /*m*/)
            { throw std::runtime_error("unexpected"); }
        virtual void flush()
            { throw std::runtime_error("unexpected"); }
        virtual void setPos(FileSize_t pos)
            { TS_ASSERT_EQUALS(pos, 23U); }
        virtual FileSize_t getPos()
            { return 42; }
        virtual FileSize_t getSize()
            { throw std::runtime_error("unexpected"); }
        virtual uint32_t getCapabilities()
            { return 0; }
        virtual String_t getName()
            { return "<name>"; }
        virtual afl::base::Ptr<afl::io::FileMapping> createFileMapping(FileSize_t /*limit*/)
            { throw std::runtime_error("unexpected"); }
    };
    Tester t;
    afl::base::Ref<afl::io::Stream> child(t.createChild());
    child->setPos(23);
}

