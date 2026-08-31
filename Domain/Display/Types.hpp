#ifndef DOMAIN_DISPLAY_TYPES_HPP_
#define DOMAIN_DISPLAY_TYPES_HPP_

#include <cstdint>

struct DisplayInfo
{
    uint16_t width  = 0;
    uint16_t height = 0;
};

struct Spacing
{
    uint8_t horizontal = 0;
    uint8_t vertical   = 0;
};

enum class TransferType : uint8_t
{
    CMD,
    DATA
};

enum class PixelFormat : uint8_t
{
	Invalid,
	RGB444,
	RGB565,
	RGB666
};

enum class ColorOrder : uint8_t
{
	RGB,
	BGR
};

enum class Orientation : uint8_t
{
    Portrait,
    Landscape,
    PortraitFlipped,
    LandscapeFlipped
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Coordinates
{
    const uint8_t* data;
    size_t size;
};

struct Size
{
	uint8_t width;
	uint8_t height;
};

struct Point
{
	uint8_t x;
	uint8_t y;
};

struct Rect
{
    Coordinates x;
    Coordinates y;
    Size size;
};

#endif
