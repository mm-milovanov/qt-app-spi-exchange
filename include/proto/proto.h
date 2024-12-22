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
    // Control
    uint16_t cnt;
    uint16_t crc;
    // Footer
    uint16_t footer;
} proto_pkg_t;

#pragma pack(pop)

enum class ExchangeCmdCode {
    ECHO  = 0x00,
    FW_INFO = 0x01,
    SPI_INFO = 0x02,
    GET_VAR = 0x10,
    SET_VAR = 0x11,
    SPI_WRITE = 0x20,
    SPI_READ = 0x21
};

enum class ExchangeVarCode {
    DATA_LEN = 0,
    DATA_ORDER = 1,
    CPOL = 2,
    SPHA = 3,
    FREQ = 4,
};

/* Helper functions */

void preparePkg(proto_pkg_t& pkg);

#endif // PROTO_H
