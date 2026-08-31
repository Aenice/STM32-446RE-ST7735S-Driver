#ifndef DRIVERS_ST7735S_ST7735S_HPP_
#define DRIVERS_ST7735S_ST7735S_HPP_

#include "../ST7735S/Font5x7.hpp"
#include "../ST7735S/Types.hpp"
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"

#include "Display/Types.hpp"
#include "Utils.hpp"

class ST7735S
{
public:
	ST7735S(
		SPI_HandleTypeDef* spi,
	    GPIO_TypeDef* sdaPort, uint16_t sdaPin,
	    GPIO_TypeDef* rsPort,  uint16_t rsPin,
	    GPIO_TypeDef* rstPort, uint16_t rstPin,
	    GPIO_TypeDef* csPort,  uint16_t csPin
	);

    ~ST7735S();

    void init();

    // configuration
    void setColorOrder(ColorOrder order);
    void setOrientation(Orientation orientation);
    void setPixelFormat(const PixelFormat format);
    PixelFormat getPixelFormat() const;

    void setSelected(const bool selected) const;

    // Drawing
    void setTextScale(size_t scale);
    void setTextColor(const Color& color);
    void setTextSpacing(const Spacing& spacing);
    void drawChar(const Point& point, char character);
    void drawText(const Point& point, const char* text, size_t size);

    void drawRect(const Point& point, const Size& size, const Color& color);
    void fillRect(const Point& point, const Size& size, const Color& color);
    void drawCircle(const Point& center, uint8_t radius, const Color& color);
    void fillCircle(const Point& center, uint8_t radius, const Color& color);
    void drawEllipse(const Point& center, uint8_t radiusX, uint8_t radiusY, const Color& color);
    void fillEllipse(const Point& center, uint8_t radiusX, uint8_t radiusY, const Color& color);
    void drawTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color);
    void fillTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color);

    void drawLine(const Point& start, const Point& end, uint8_t thickness, const Color& color);

    void fillBackground(const Color& color);

private:
    // Configure
    uint8_t getPixelFormatValue(PixelFormat format) const;

    void hardwareReset() const;

    void configureRgb444Lut() const;
    void configureRgb565Lut() const;

    // Transport
    void setTransferMode(const TransferType mode) const;
    void transport(const uint8_t* value, size_t size) const;

    // Draw
    void draw(const Rect& rect, const Color& color);
    void setDrawingArea(const Rect& rect) const;

    uint16_t getSize(Coordinates coordinates) const;

private:
    GPIO_TypeDef* m_SdaPort;
    uint16_t      m_SdaPin;

    GPIO_TypeDef* m_RsPort;
    uint16_t      m_RsPin;

    GPIO_TypeDef* m_RstPort;
    uint16_t      m_RstPin;

    GPIO_TypeDef* m_CsPort;
    uint16_t      m_CsPin;

    SPI_HandleTypeDef* m_spi;

    DisplayInfo m_displayInfo;

    PixelFormat m_pixelFormat = PixelFormat::Invalid;
    Orientation m_orientation = Orientation::Portrait;

    uint8_t m_madctl = 0xc0;

    Color m_textColor = {255, 255, 255};
    uint8_t m_textScale = 1;
    Spacing m_textSpacing = {1, 1};

    static constexpr size_t MAX_PIXELS = 128 * 160;

    uint8_t m_frameBuffer[MAX_PIXELS * 3];
};

#endif
