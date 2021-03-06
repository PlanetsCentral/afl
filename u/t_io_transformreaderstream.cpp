/**
  *  \file u/t_io_transformreaderstream.cpp
  *  \brief Test for afl::io::TransformReaderStream
  */

#include "afl/io/transformreaderstream.hpp"

#include "t_io.hpp"
#include "afl/io/constmemorystream.hpp"
#include "afl/io/transform.hpp"
#include "afl/except/fileproblemexception.hpp"
#include "afl/io/filemapping.hpp"

namespace {
    // A sample transform.
    // Transforms input by adding one to each byte.
    // When given a flush(), writes a '!'.
    // We also want to check that flush() is called only once, and the data generated by it is requested.
    class TestTransform : public afl::io::Transform {
     public:
        TestTransform()
            : m_flushPost(0),
              m_flushAck(0)
            { }
        ~TestTransform()
            {
                if (m_flushPost > 0) {
                    TS_ASSERT_EQUALS(m_flushPost, 1);
                    TS_ASSERT_EQUALS(m_flushAck, 1);
                }
            }
        void transform(afl::base::ConstBytes_t& in, afl::base::Bytes_t& out)
            {
                afl::base::Bytes_t outSave = out;
                while (!in.empty() && !out.empty()) {
                    *out.eat() = uint8_t(*in.eat() + 1);
                }
                while (m_flushPost > m_flushAck && !out.empty()) {
                    *out.eat() = '!';
                    ++m_flushAck;
                }
                out = outSave.subrange(0, outSave.size() - out.size());
            }
        void flush()
            { ++m_flushPost; }
     private:
        int m_flushPost;
        int m_flushAck;
    };
}

/** Basic tests. */
void
TestIoTransformReaderStream::testIt()
{
    // Create testee
    static const uint8_t data[] = {'h','e','l','l','o','w','o','r','l','d'};
    afl::io::ConstMemoryStream in(data);
    TestTransform tx;
    afl::io::TransformReaderStream testee(in, tx);

    // Read data
    uint8_t result[30];
    size_t n = testee.read(result);
    TS_ASSERT_EQUALS(n, 11U);
    TS_ASSERT_SAME_DATA(result, "ifmmpxpsme!", 11);
}

/** Test other methods. */
void
TestIoTransformReaderStream::testExtra()
{
    // Create testee
    static const uint8_t data[] = {'h','e','l','l','o','w','o','r','l','d'};
    afl::io::ConstMemoryStream in(data);
    TestTransform tx;
    afl::io::TransformReaderStream testee(in, tx);

    // Check initial state
    TS_ASSERT_EQUALS(testee.getPos(), afl::io::Stream::FileSize_t(0));
    TS_ASSERT_EQUALS(testee.getName(), in.getName());
    TS_ASSERT_EQUALS(testee.getCapabilities(), afl::io::Stream::CanRead);
    TS_ASSERT_EQUALS(testee.getSize(), afl::io::Stream::FileSize_t(-1));

    // Must be able to seek to current position, fail to seek elsewhere
    testee.setPos(0);
    TS_ASSERT_THROWS(testee.setPos(1), afl::except::FileProblemException);

    // Read a byte and recheck
    uint8_t tmp[1];
    testee.fullRead(tmp);
    TS_ASSERT_EQUALS(tmp[0], 'i');
    TS_ASSERT_EQUALS(testee.getPos(), afl::io::Stream::FileSize_t(1));
    testee.setPos(1);
    TS_ASSERT_THROWS(testee.setPos(10), afl::except::FileProblemException);

    // Writing fails
    TS_ASSERT_THROWS(testee.write(tmp), afl::except::FileProblemException);

    // Null operations
    testee.createFileMapping(10);
    testee.flush();
}

/** Test position behaviour.
    There used to be a bug in TransformReaderStream to return wrong positions
    when a read operation was built from multiple transform() invocations. */
void
TestIoTransformReaderStream::testPosition()
{
    // A test transformation that produces single bytes.
    class Tester : public afl::io::Transform {
     public:
        virtual void transform(afl::base::ConstBytes_t& in, afl::base::Bytes_t& out)
            {
                if (in.size() > 0 && out.size() > 0) {
                    out.trim(1);
                    out.copyFrom(in.split(1));
                } else {
                    out.trim(0);
                }
            }
        virtual void flush()
            { }
    };

    // Create a stream
    Tester t;
    afl::io::ConstMemoryStream ms(afl::string::toBytes("hello"));
    afl::io::TransformReaderStream testee(ms, t);

    // Initial position must be 0
    TS_ASSERT_EQUALS(testee.getPos(), 0U);

    // Read the content
    uint8_t data[10];
    size_t n = testee.read(data);
    TS_ASSERT_EQUALS(n, 5U);

    // Final position must be 5
    TS_ASSERT_EQUALS(testee.getPos(), 5U);
}

