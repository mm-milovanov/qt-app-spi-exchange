#ifndef PROTO_H
#define PROTO_H

#include <QObject>

// Package header and footer from control device to board
#define FROM_CTRL_PACKAGE_HEADER    ((uint16_t)0xCDE6)
#define FROM_CTRL_PACKAGE_FOOTER    ((uint16_t)0xDEC6)

// Package header and footer from board to control device
#define FROM_BOARD_PACKAGE_HEADER   ((uint16_t)0xAFE6)
#define FROM_BOARD_PACKAGE_FOOTER   ((uint16_t)0xFEA6)

#define PROTO_PKG_PAYLOAD_SIZE 128

; /* to fix clang warning */
#pragma pack(push,1)

/*
 * CMD Register - 16 bits
 *  ___________ ___________
 * | CMD[15:8] | DATA[7:0] |
 *  ----------- -----------
 *
 *  CMD[15:8] - 8 bits - CMD code. Can be:
 *  1. 0x00 - ECHO request
 *  2. 0x20 - write data via SPI
 *  3. 0x21 - read data via SPI
 *
 *  DATA[7:0] - 8 bits - CMD data. Can be:
 *  1. RESERVED - if CMD register equals 0x00
 *  2. DATA SIZE - from 0 to 128 (maximum payload size)
 *                 if CMD register equals 0x20 or 0x21.
 */

typedef struct proto_pkg_t {
    // Header
    uint16_t header;
    // Registers
    uint16_t reg1;
    uint16_t reg2;
    uint16_t reg3;
    // Payload
    union {
        char data[PROTO_PKG_PAYLOAD_SIZE];
        uint32_t word;
    } payload;
    // Control fields
    uint16_t cnt;
    uint16_t crc;
    // Footer
    uint16_t footer;
} proto_pkg_t;

#pragma pack(pop)

enum class ExchangeCmdCode {
    ECHO  = 0x00,
    SPI_WRITE = 0x20,
    SPI_READ = 0x21
};

/* Helper functions */

void preparePkg(proto_pkg_t& pkg);

#endif // PROTO_H
