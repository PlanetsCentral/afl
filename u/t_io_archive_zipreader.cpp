/**
  *  \file u/t_io_archive_zipreader.cpp
  *  \brief Test for afl::io::archive::ZipReader
  */

#include "afl/io/archive/zipreader.hpp"

#include "t_io_archive.hpp"
#include "afl/io/constmemorystream.hpp"
#include "afl/io/internalstream.hpp"
#include "afl/except/fileproblemexception.hpp"
#include "afl/except/fileformatexception.hpp"
#include "afl/io/directoryentry.hpp"
#include "afl/io/filemapping.hpp"

namespace {
    const uint8_t SAMPLE_ZIP[] = {
        0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x6b,
        0x57, 0x49, 0x53, 0x74, 0x24, 0xf4, 0x0d, 0x00, 0x00, 0x00, 0x0d, 0x00,
        0x00, 0x00, 0x09, 0x00, 0x1c, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e,
        0x74, 0x78, 0x74, 0x55, 0x54, 0x09, 0x00, 0x03, 0xa3, 0x9e, 0x0c, 0x58,
        0xa3, 0x9e, 0x0c, 0x58, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f,
        0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x0a, 0x50, 0x4b, 0x03, 0x04,
        0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x79, 0x6b, 0x57, 0x49, 0xe0, 0x42,
        0x28, 0x88, 0x1c, 0x00, 0x00, 0x00, 0x0f, 0x02, 0x00, 0x00, 0x08, 0x00,
        0x1c, 0x00, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x6c, 0x6f, 0x67, 0x55, 0x54,
        0x09, 0x00, 0x03, 0xb5, 0x9e, 0x0c, 0x58, 0xaf, 0x9e, 0x0c, 0x58, 0x75,
        0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x0b, 0x2a, 0xcd, 0xcb, 0xcb, 0xcc, 0x4b, 0x57, 0x30, 0x35,
        0x30, 0x53, 0x28, 0x49, 0x2d, 0x2e, 0x29, 0xd6, 0x1b, 0x05, 0x23, 0x14,
        0xf8, 0x7b, 0x2b, 0x72, 0x01, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03,
        0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x6b, 0x57, 0x49, 0x53, 0x74,
        0x24, 0xf4, 0x0d, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x09, 0x00,
        0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81,
        0x00, 0x00, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e, 0x74, 0x78,
        0x74, 0x55, 0x54, 0x05, 0x00, 0x03, 0xa3, 0x9e, 0x0c, 0x58, 0x75, 0x78,
        0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00,
        0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x14, 0x00, 0x00, 0x00, 0x08,
        0x00, 0x79, 0x6b, 0x57, 0x49, 0xe0, 0x42, 0x28, 0x88, 0x1c, 0x00, 0x00,
        0x00, 0x0f, 0x02, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x50, 0x00, 0x00, 0x00, 0x74,
        0x65, 0x73, 0x74, 0x2e, 0x6c, 0x6f, 0x67, 0x55, 0x54, 0x05, 0x00, 0x03,
        0xb5, 0x9e, 0x0c, 0x58, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x05, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x9d, 0x00, 0x00, 0x00, 0xae,
        0x00, 0x00, 0x00, 0x00, 0x00
    };

    const uint8_t UNCOMPR_ZIP[] = {
        0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0x8d,
        0x6f, 0x4a, 0x53, 0x74, 0x24, 0xf4, 0x0d, 0x00, 0x00, 0x00, 0x0d, 0x00,
        0x00, 0x00, 0x09, 0x00, 0x1c, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e,
        0x74, 0x78, 0x74, 0x55, 0x54, 0x09, 0x00, 0x03, 0xc1, 0x6f, 0xc9, 0x58,
        0xc3, 0x6f, 0xc9, 0x58, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f,
        0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x0a, 0x50, 0x4b, 0x01, 0x02,
        0x1e, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0x8d, 0x6f, 0x4a,
        0x53, 0x74, 0x24, 0xf4, 0x0d, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
        0x09, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xa4, 0x81, 0x00, 0x00, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e,
        0x74, 0x78, 0x74, 0x55, 0x54, 0x05, 0x00, 0x03, 0xc1, 0x6f, 0xc9, 0x58,
        0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8,
        0x03, 0x00, 0x00, 0x50, 0x4b, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00,
        0x00
    };

    const uint8_t CRYPT_ZIP[] = {
        0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x09, 0x00, 0x00, 0x00, 0xbb, 0x8d,
        0x6f, 0x4a, 0x53, 0x74, 0x24, 0xf4, 0x19, 0x00, 0x00, 0x00, 0x0d, 0x00,
        0x00, 0x00, 0x09, 0x00, 0x1c, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e,
        0x74, 0x78, 0x74, 0x55, 0x54, 0x09, 0x00, 0x03, 0xc1, 0x6f, 0xc9, 0x58,
        0xc3, 0x6f, 0xc9, 0x58, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x4e, 0xb0, 0x12, 0xd1, 0x6a,
        0xf1, 0x18, 0x53, 0xd1, 0xd8, 0xfa, 0x13, 0x81, 0xae, 0x8c, 0xfb, 0x6f,
        0xa1, 0x4b, 0x96, 0x1a, 0xd6, 0x82, 0xc9, 0x7d, 0x50, 0x4b, 0x07, 0x08,
        0x53, 0x74, 0x24, 0xf4, 0x19, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
        0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x09, 0x00, 0x00, 0x00,
        0xbb, 0x8d, 0x6f, 0x4a, 0x53, 0x74, 0x24, 0xf4, 0x19, 0x00, 0x00, 0x00,
        0x0d, 0x00, 0x00, 0x00, 0x09, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x00, 0x00, 0x00, 0x00, 0x68, 0x65,
        0x6c, 0x6c, 0x6f, 0x2e, 0x74, 0x78, 0x74, 0x55, 0x54, 0x05, 0x00, 0x03,
        0xc1, 0x6f, 0xc9, 0x58, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03,
        0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x05, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x6c,
        0x00, 0x00, 0x00, 0x00, 0x00
    };

    // Unsupported methods.
    //  Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
    // --------  ------  ------- ---- ---------- ----- --------  ----
    //      434  Implode     321  26% 1991-07-19 06:01 cb1f7829  DISPLAY.PAS
    //       76  Shrunk       52  32% 1991-07-19 06:01 4d8516a5  TURBOC.CFG
    const uint8_t UNSUPP_ZIP[] = {
        0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x06, 0x00, 0x20, 0x30,
        0xf3, 0x16, 0x29, 0x78, 0x1f, 0xcb, 0x41, 0x01, 0x00, 0x00, 0xb2, 0x01,
        0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x44, 0x49, 0x53, 0x50, 0x4c, 0x41,
        0x59, 0x2e, 0x50, 0x41, 0x53, 0x0d, 0x02, 0x01, 0x12, 0x23, 0x14, 0x15,
        0x36, 0x37, 0x68, 0x89, 0x9a, 0xdb, 0x3c, 0x05, 0x06, 0x12, 0x13, 0x44,
        0xc5, 0xf6, 0x96, 0xf7, 0x1b, 0x2a, 0xdc, 0x0b, 0x72, 0xe8, 0x5b, 0xb8,
        0x79, 0xe5, 0xa6, 0x3d, 0x8b, 0x96, 0x2e, 0x48, 0x94, 0x63, 0x53, 0x82,
        0x8c, 0x99, 0x13, 0x67, 0x4d, 0x96, 0x39, 0x61, 0x82, 0x14, 0x9b, 0x17,
        0xa4, 0xd0, 0xb7, 0x72, 0xd9, 0x86, 0x75, 0x4b, 0x16, 0x64, 0x52, 0xb7,
        0x74, 0xcb, 0xca, 0x75, 0x1b, 0x96, 0x6e, 0xda, 0x37, 0xc4, 0x9b, 0x2d,
        0x0b, 0xe7, 0x1f, 0xeb, 0x12, 0x64, 0x9f, 0xfc, 0x15, 0xbc, 0xeb, 0xba,
        0x4d, 0x43, 0xbf, 0x89, 0xa6, 0x9d, 0x0b, 0xc7, 0x7c, 0xe7, 0xdd, 0xa1,
        0x2c, 0xa7, 0x86, 0x6d, 0xb3, 0xbc, 0xcb, 0x82, 0xb8, 0xee, 0x66, 0xfa,
        0x97, 0xb6, 0x4e, 0x2a, 0x75, 0x68, 0x55, 0xa6, 0x41, 0xa5, 0xba, 0x84,
        0x1a, 0x74, 0x8a, 0x7c, 0x9d, 0x66, 0x59, 0x9a, 0x0d, 0x3b, 0xb6, 0x0c,
        0xf3, 0x84, 0x2b, 0xf7, 0x0d, 0xf2, 0x26, 0x5b, 0x57, 0x4e, 0xfc, 0xbe,
        0x02, 0x74, 0x5d, 0x6f, 0xac, 0x59, 0x51, 0xe2, 0xf1, 0x56, 0xf7, 0x5a,
        0x27, 0x88, 0xf2, 0x79, 0xe6, 0x7c, 0xef, 0x04, 0x39, 0x87, 0xf9, 0xe7,
        0xd2, 0xe1, 0xde, 0x6e, 0xcf, 0xa6, 0xdc, 0xe1, 0xdf, 0xbe, 0x65, 0xdb,
        0x32, 0xde, 0xab, 0x69, 0x0b, 0xfa, 0x73, 0xcb, 0xce, 0x29, 0xde, 0x20,
        0xd0, 0xb5, 0x5c, 0x3a, 0xf9, 0xbb, 0xa8, 0x9c, 0x67, 0x2d, 0xc7, 0xbc,
        0x61, 0x03, 0xb8, 0x62, 0xa8, 0x77, 0x9a, 0x66, 0x3b, 0xd6, 0x69, 0xcd,
        0x10, 0xb9, 0xf1, 0x94, 0x3f, 0xc8, 0xad, 0x31, 0x5d, 0xba, 0xc4, 0x09,
        0xb3, 0xcb, 0x93, 0x0b, 0xc8, 0x89, 0x32, 0x8f, 0x74, 0x2e, 0xb3, 0x86,
        0xf4, 0xd3, 0x45, 0x5b, 0x86, 0xbd, 0xd5, 0xa7, 0x82, 0xf7, 0xd1, 0xb7,
        0x54, 0xdf, 0x94, 0x13, 0xcf, 0x42, 0x7a, 0xe4, 0x73, 0x94, 0x73, 0x9c,
        0x2b, 0xf8, 0x4d, 0xc6, 0xb8, 0xb3, 0x9d, 0x83, 0xd7, 0xba, 0xe7, 0x54,
        0xb4, 0x6f, 0xef, 0xbc, 0xed, 0x28, 0x4f, 0x3c, 0xa9, 0xed, 0x58, 0xa9,
        0xd3, 0x92, 0x51, 0x9b, 0x83, 0x1c, 0xfb, 0xa6, 0x7a, 0x93, 0x4d, 0xf3,
        0xa6, 0x69, 0xd5, 0x27, 0xfa, 0xc7, 0xfe, 0x51, 0x65, 0xc7, 0xef, 0xe2,
        0x8d, 0x0d, 0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x20, 0x30, 0xf3, 0x16, 0xa5, 0x16, 0x85, 0x4d, 0x34, 0x00, 0x00, 0x00,
        0x4c, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x54, 0x55, 0x52, 0x42,
        0x4f, 0x43, 0x2e, 0x43, 0x46, 0x47, 0x2d, 0xc6, 0x34, 0x50, 0xd0, 0x02,
        0xce, 0xc0, 0x16, 0x72, 0x5a, 0x1c, 0xac, 0xa3, 0x90, 0xa0, 0x9e, 0x21,
        0x43, 0x9e, 0x10, 0x29, 0x72, 0x50, 0x0f, 0x94, 0x8a, 0x41, 0x2a, 0x4a,
        0x89, 0xe8, 0x64, 0x0a, 0x95, 0x8a, 0x53, 0x2a, 0x7e, 0x74, 0x48, 0x84,
        0x48, 0x10, 0x2a, 0x19, 0x1d, 0x1e, 0xa9, 0x28, 0x64, 0x20, 0x50, 0x4b,
        0x01, 0x02, 0x0a, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x06, 0x00, 0x20, 0x30,
        0xf3, 0x16, 0x29, 0x78, 0x1f, 0xcb, 0x41, 0x01, 0x00, 0x00, 0xb2, 0x01,
        0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x49, 0x53, 0x50,
        0x4c, 0x41, 0x59, 0x2e, 0x50, 0x41, 0x53, 0x50, 0x4b, 0x01, 0x02, 0x0a,
        0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x30, 0xf3, 0x16, 0xa5,
        0x16, 0x85, 0x4d, 0x34, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x0a,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00,
        0x00, 0x6a, 0x01, 0x00, 0x00, 0x54, 0x55, 0x52, 0x42, 0x4f, 0x43, 0x2e,
        0x43, 0x46, 0x47, 0x50, 0x4b, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x02,
        0x00, 0x02, 0x00, 0x71, 0x00, 0x00, 0x00, 0xc6, 0x01, 0x00, 0x00, 0x00,
        0x00
    };

    const uint8_t HELLO_TXT[] = {
        0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64,
        0x0a
    };

    const uint8_t TEST_LOG[] = {
        0x52, 0x75, 0x6e, 0x6e, 0x69, 0x6e, 0x67, 0x20, 0x35, 0x30, 0x36, 0x20,
        0x74, 0x65, 0x73, 0x74, 0x73, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x4f, 0x4b, 0x21, 0x0a
    };

}

/** Test reading the zip file members. */
void
TestIoArchiveZipReader::testReadMembers()
{
    // Open zip file
    afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(SAMPLE_ZIP), 0));

    // Read member "hello.txt"
    {
        afl::base::Ref<afl::io::Stream> in(testee->openFile("hello.txt", afl::io::FileSystem::OpenRead));
        afl::io::InternalStream out;
        out.copyFrom(*in);
        TS_ASSERT(out.getContent().equalContent(HELLO_TXT));
    }

    // Read member "test.log"
    {
        afl::base::Ref<afl::io::Stream> in(testee->openFile("test.log", afl::io::FileSystem::OpenRead));
        afl::io::InternalStream out;
        out.copyFrom(*in);
        TS_ASSERT(out.getContent().equalContent(TEST_LOG));
    }

    // We are case-sensitive
    TS_ASSERT_THROWS(testee->openFile("HELLO.TXT", afl::io::FileSystem::OpenRead), afl::except::FileProblemException);

    // We cannot write
    TS_ASSERT_THROWS(testee->openFile("hello.txt", afl::io::FileSystem::OpenWrite), afl::except::FileProblemException);
    TS_ASSERT_THROWS(testee->openFile("hello.txt", afl::io::FileSystem::Create), afl::except::FileProblemException);
}

/** Test bad operations. */
void
TestIoArchiveZipReader::testBadOperations()
{
    // Try modifications
    {
        afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(SAMPLE_ZIP), 0));

        // Modify existing
        TS_ASSERT_THROWS(testee->openFile("hello.txt", afl::io::FileSystem::OpenWrite), afl::except::FileProblemException);
        TS_ASSERT_THROWS(testee->openFile("hello.txt", afl::io::FileSystem::Create), afl::except::FileProblemException);

        // Open nonexistant
        TS_ASSERT_THROWS(testee->openFile("bye.txt", afl::io::FileSystem::OpenRead), afl::except::FileProblemException);

        // Delete
        TS_ASSERT_THROWS(testee->erase("hello.txt"), afl::except::FileProblemException);
        TS_ASSERT_THROWS(testee->erase("bye.txt"), afl::except::FileProblemException);
    }

    // Not a zip file
    {
        afl::base::Ptr<afl::io::archive::ZipReader> testee;
        // These errors are currently not detected; if it does not look like a zip, it is treated empty:
        //   TS_ASSERT_THROWS(testee = afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(HELLO_TXT), 0).asPtr(), afl::except::FileFormatException);
        //   TS_ASSERT_THROWS(testee = afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(afl::base::ConstBytes_t()), 0).asPtr(), afl::except::FileFormatException);

        // Truncated zip is detected up to a point:
        for (size_t i = 4; i < 30; ++i) {
            TS_ASSERT_THROWS(testee = afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(afl::base::ConstBytes_t(SAMPLE_ZIP).subrange(0, i)), 0).asPtr(), afl::except::FileFormatException);
        }
    }
}

/** Test metadata access. */
void
TestIoArchiveZipReader::testMeta()
{
    // Open zip file
    afl::base::Ref<afl::io::Stream> in(*new afl::io::ConstMemoryStream(SAMPLE_ZIP));
    afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(in, 0));

    // Check ZipReader metadata
    TS_ASSERT(testee->getParentDirectory().get() == 0);
    TS_ASSERT_EQUALS(testee->getDirectoryName(), "");
    TS_ASSERT_EQUALS(testee->getTitle(), in->getName());

    // Read content
    afl::base::Ref<afl::base::Enumerator<afl::base::Ptr<afl::io::DirectoryEntry> > > content = testee->getDirectoryEntries();
    afl::base::Ptr<afl::io::DirectoryEntry> entry;

    // Verify first element
    TS_ASSERT(content->getNextElement(entry));
    TS_ASSERT(entry.get() != 0);
    TS_ASSERT_EQUALS(entry->getTitle(), "hello.txt");
    TS_ASSERT_EQUALS(entry->getPathName(), "");
    TS_ASSERT_EQUALS(entry->getFileSize(), sizeof(HELLO_TXT));
    TS_ASSERT_EQUALS(&*entry->openContainingDirectory(), &*testee);
    TS_ASSERT_THROWS(entry->openDirectory(), afl::except::FileProblemException);
    TS_ASSERT(entry->openFile(afl::io::FileSystem::OpenRead)->createVirtualMapping()->get().equalContent(HELLO_TXT));

    // Verify second element
    TS_ASSERT(content->getNextElement(entry));
    TS_ASSERT(entry.get() != 0);
    TS_ASSERT_EQUALS(entry->getTitle(), "test.log");
    TS_ASSERT_EQUALS(entry->getPathName(), "");
    TS_ASSERT_EQUALS(entry->getFileSize(), sizeof(TEST_LOG));
    TS_ASSERT_EQUALS(&*entry->openContainingDirectory(), &*testee);
    TS_ASSERT_THROWS(entry->openDirectory(), afl::except::FileProblemException);
    TS_ASSERT(entry->openFile(afl::io::FileSystem::OpenRead)->createVirtualMapping()->get().equalContent(TEST_LOG));

    TS_ASSERT(!content->getNextElement(entry));
}

/** Test uncompressed member. */
void
TestIoArchiveZipReader::testUncompr()
{
    // Open zip file
    afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(UNCOMPR_ZIP), 0));

    // Read member "hello.txt" (uncompressed)
    {
        afl::base::Ref<afl::io::Stream> in(testee->openFile("hello.txt", afl::io::FileSystem::OpenRead));
        afl::io::InternalStream out;
        out.copyFrom(*in);
        TS_ASSERT(out.getContent().equalContent(HELLO_TXT));
        TS_ASSERT_EQUALS(in->getPos(), sizeof(HELLO_TXT));
    }
}

/** Test encrypted member. */
void
TestIoArchiveZipReader::testCrypt()
{
    // Open zip file
    afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(CRYPT_ZIP), 0));

    // Try to read member "hello.txt" (treated as nonexistant because it's encrypted)
    TS_ASSERT_THROWS(testee->openFile("hello.txt", afl::io::FileSystem::OpenRead), afl::except::FileProblemException);

    // Read content
    afl::base::Ref<afl::base::Enumerator<afl::base::Ptr<afl::io::DirectoryEntry> > > content = testee->getDirectoryEntries();
    afl::base::Ptr<afl::io::DirectoryEntry> entry;
    TS_ASSERT(!content->getNextElement(entry));
}

/** Test unsupported members. */
void
TestIoArchiveZipReader::testUnsupported()
{
    // Open zip file
    afl::base::Ref<afl::io::archive::ZipReader> testee(afl::io::archive::ZipReader::open(*new afl::io::ConstMemoryStream(CRYPT_ZIP), 0));

    // Try to read the members (treated as nonexistant because they cannot be decompressed)
    TS_ASSERT_THROWS(testee->openFile("TURBOC.CFG", afl::io::FileSystem::OpenRead), afl::except::FileProblemException);
    TS_ASSERT_THROWS(testee->openFile("DISPLAY.PAS", afl::io::FileSystem::OpenRead), afl::except::FileProblemException);

    // Read content
    afl::base::Ref<afl::base::Enumerator<afl::base::Ptr<afl::io::DirectoryEntry> > > content = testee->getDirectoryEntries();
    afl::base::Ptr<afl::io::DirectoryEntry> entry;
    TS_ASSERT(!content->getNextElement(entry));
}

