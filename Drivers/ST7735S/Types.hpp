#ifndef DRIVERS_ST7735S_TYPES_HPP_
#define DRIVERS_ST7735S_TYPES_HPP_

#include <cstdint>

enum class Command : uint8_t
{
    SWRESET   = 0x01,
    SLPOUT    = 0x11,

    SLPIN     = 0x10,
    NORON     = 0x13,
    DISPOFF   = 0x28,
    DISPON    = 0x29,

    INVON     = 0x21,
    INVOFF    = 0x20,

    CASET     = 0x2A,
    RASET     = 0x2B,
    RAMWR     = 0x2C,
    RAMRD     = 0x2E,

    MADCTL    = 0x36,
    COLMOD    = 0x3A,

    RDDMADCTL = 0x0B,
    RDDCOLMOD = 0x0C,

    RGBSET    = 0x2D,

    FRMCTR1   = 0xB1,
    FRMCTR2   = 0xB2,
    FRMCTR3   = 0xB3,

    INVCTR    = 0xB4,

    PWCTR1    = 0xC0,
    PWCTR2    = 0xC1,
    PWCTR3    = 0xC2,
    PWCTR4    = 0xC3,
    PWCTR5    = 0xC4,

    VMCTR1    = 0xC5,

    GMCTRP1   = 0xE0,
    GMCTRN1   = 0xE1
};

#endif
