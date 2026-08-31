#include "../ST7735S/ST7735S.hpp"

ST7735S::ST7735S(
    SPI_HandleTypeDef* spi,
    GPIO_TypeDef* sdaPort, uint16_t sdaPin,
    GPIO_TypeDef* rsPort,  uint16_t rsPin,
    GPIO_TypeDef* rstPort, uint16_t rstPin,
    GPIO_TypeDef* csPort,  uint16_t csPin
)
    : m_spi(spi),
      m_SdaPort(sdaPort),
      m_SdaPin(sdaPin),
      m_RsPort(rsPort),
      m_RsPin(rsPin),
      m_RstPort(rstPort),
      m_RstPin(rstPin),
      m_CsPort(csPort),
      m_CsPin(csPin)
{
	Utils::init();
}

ST7735S::~ST7735S()
{
	setSelected(false);
}

void ST7735S::hardwareReset() const
{
    HAL_GPIO_WritePin(m_RstPort, m_RstPin, GPIO_PIN_RESET);
    Utils::delayUs(10);
	HAL_GPIO_WritePin(m_RstPort, m_RstPin, GPIO_PIN_SET);
	HAL_Delay(120);
}

void ST7735S::init()
{
    uint8_t cmd;

    // ============================================================
    // HARDWARE RESET
    // ============================================================
    hardwareReset();

    setSelected(true);


    // ============================================================
    // SOFTWARE RESET
    // ============================================================
    setTransferMode(TransferType::CMD);

    cmd = static_cast<uint8_t>(Command::SWRESET);
    transport(&cmd, 1);

    HAL_Delay(120);


    // ============================================================
    // SLEEP OUT
    // ============================================================
    setTransferMode(TransferType::CMD);

    cmd = static_cast<uint8_t>(Command::SLPOUT);
    transport(&cmd, 1);

    HAL_Delay(120);


    // ============================================================
    // FRAME RATE CONTROL 1
    // ============================================================
    {
        uint8_t data[] =
        {
            0x01, 0x2C, 0x2D
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::FRMCTR1);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // FRAME RATE CONTROL 2
    // ============================================================
    {
        uint8_t data[] =
        {
            0x01, 0x2C, 0x2D
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::FRMCTR2);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // FRAME RATE CONTROL 3
    // ============================================================
    {
        uint8_t data[] =
        {
            0x01, 0x2C, 0x2D,
            0x01, 0x2C, 0x2D
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::FRMCTR3);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // INVERSION CONTROL
    // ============================================================
    {
        uint8_t data = 0x07;

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::INVCTR);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(&data, 1);
    }


    // ============================================================
    // POWER CONTROL 1
    // ============================================================
    {   // from LCD.c / LCD_WR_REG(0xC0)
        uint8_t data[] =
        {
            0xA2, 0x02, 0x84
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::PWCTR1);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // POWER CONTROL 2
    // ============================================================
    {
        uint8_t data = 0xC5;

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::PWCTR2);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(&data, 1);
    }


    // ============================================================
    // POWER CONTROL 3
    // ============================================================
    {
        uint8_t data[] =
        {
            0x0A, 0x00
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::PWCTR3);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // POWER CONTROL 4
    // ============================================================
    {
        uint8_t data[] =
        {
            0x8A, 0x2A
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::PWCTR4);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // POWER CONTROL 5
    // ============================================================
    {
        uint8_t data[] =
        {
            0x8A, 0xEE
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::PWCTR5);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // VCOM CONTROL
    // ============================================================
    {
        uint8_t data = 0x0E;

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::VMCTR1);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(&data, 1);
    }


    // ============================================================
    // DISPLAY INVERSION OFF
    // ============================================================
    setTransferMode(TransferType::CMD);

    cmd = static_cast<uint8_t>(Command::INVOFF);
    transport(&cmd, 1);


    // ============================================================
    // MEMORY ACCESS CONTROL
    // ============================================================
    {
    	setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::MADCTL);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(&m_madctl, 1);
    }


    // ============================================================
    // PIXEL FORMAT
    //
    // RGB565
    // ============================================================
    setPixelFormat(PixelFormat::RGB565);

    // ============================================================
    // ORIENTATION
    // ============================================================
    setOrientation(Orientation::Portrait);

    // ============================================================
    // POSITIVE GAMMA
    // ============================================================
    {
        uint8_t data[] =
        {
			0x0F, 0x1A, 0x0F, 0x18,
			0x2F, 0x28, 0x20, 0x22,
			0x1F, 0x1B, 0x23, 0x37,
			0x00, 0x07, 0x02, 0x10
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::GMCTRP1);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // NEGATIVE GAMMA
    // ============================================================
    {
        uint8_t data[] =
        {
			0x0F, 0x1B, 0x0F, 0x17,
			0x33, 0x2C, 0x29, 0x2E,
			0x30, 0x30, 0x39, 0x3F,
			0x00, 0x07, 0x03, 0x10
        };

        setTransferMode(TransferType::CMD);

        cmd = static_cast<uint8_t>(Command::GMCTRN1);
        transport(&cmd, 1);

        setTransferMode(TransferType::DATA);
        transport(data, sizeof(data));
    }


    // ============================================================
    // NORMAL DISPLAY MODE
    // ============================================================
    setTransferMode(TransferType::CMD);

    cmd = static_cast<uint8_t>(Command::NORON);
    transport(&cmd, 1);

    // ============================================================
    // DISPLAY ON
    // ============================================================
    setTransferMode(TransferType::CMD);

    cmd = static_cast<uint8_t>(Command::DISPON);
    transport(&cmd, 1);

    setSelected(false);
}

static volatile bool dmaFinished = false;

void ST7735S::transport(const uint8_t* value, size_t size) const
{
    while (dmaFinished)
    {
    }

    dmaFinished = true;

    volatile HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(
        m_spi,
        value,
        size
    );
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
        dmaFinished = false;
    }
}

void ST7735S::setColorOrder(ColorOrder order)
{
	uint8_t cmd = static_cast<uint8_t>(Command::MADCTL);

	switch(order)
	{
	case ColorOrder::BGR:
	{
		m_madctl |= 0x08;
		break;
	}
	case ColorOrder::RGB:
	{
		m_madctl &= ~0x08;
		break;
	}
	}

    setTransferMode(TransferType::CMD);
    transport(&cmd, sizeof(cmd));

    setTransferMode(TransferType::DATA);
    transport(&m_madctl, sizeof(m_madctl));
}

void ST7735S::setOrientation(Orientation orientation)
{
	m_orientation = orientation;
	uint8_t cmd = static_cast<uint8_t>(Command::MADCTL);

	constexpr uint8_t MY = 0x80;
	constexpr uint8_t MX = 0x40;
	constexpr uint8_t MV = 0x20;

	m_madctl &= ~(MY | MX | MV);

	switch (m_orientation)
	{
	case Orientation::Portrait:
		m_displayInfo.width = 128;
		m_displayInfo.height = 160;
		break;

	case Orientation::Landscape:
		m_madctl |= MX | MV;

		m_displayInfo.width = 160;
		m_displayInfo.height = 128;
		break;

	case Orientation::PortraitFlipped:
		m_madctl |= MY | MX;

		m_displayInfo.width = 128;
		m_displayInfo.height = 160;
		break;

	case Orientation::LandscapeFlipped:
		m_madctl |= MY | MV;

		m_displayInfo.width = 160;
		m_displayInfo.height = 128;
		break;
	}

    setTransferMode(TransferType::CMD);
    transport(&cmd, sizeof(cmd));

    setTransferMode(TransferType::DATA);
    transport(&m_madctl, sizeof(m_madctl));
}

void ST7735S::setPixelFormat(const PixelFormat format)
{
	if (m_pixelFormat == format)
	{
		return;
	}

	m_pixelFormat = format;

    uint8_t cmd = static_cast<uint8_t>(Command::COLMOD);

    uint8_t value = static_cast<uint8_t>(getPixelFormatValue(m_pixelFormat));

    if (!value)
    {
    	return;
    }

    setTransferMode(TransferType::CMD);
    transport(&cmd, 1);

    setTransferMode(TransferType::DATA);
    transport(&value, 1);

    switch (m_pixelFormat)
    {
        case PixelFormat::RGB444:
        {
        	configureRgb444Lut();
			return;

        }
        case PixelFormat::RGB565:
        {
        	configureRgb565Lut();
        	return;

        }
        default:
        {
    		// LUT no need
    		break;
        }
    }
}

PixelFormat ST7735S::getPixelFormat() const
{
	return m_pixelFormat;
}

void ST7735S::setDrawingArea(const Rect& rect) const
{
	uint8_t cmd;

	setTransferMode(TransferType::CMD);
	cmd = static_cast<uint8_t>(Command::CASET);
	transport(&cmd, sizeof(cmd));

	setTransferMode(TransferType::DATA);
	transport(rect.x.data, rect.x.size);

	setTransferMode(TransferType::CMD);
	cmd = static_cast<uint8_t>(Command::RASET);
	transport(&cmd, sizeof(cmd));

	setTransferMode(TransferType::DATA);
	transport(rect.y.data, rect.y.size);
}

void ST7735S::setSelected(const bool selected) const
{
    HAL_GPIO_WritePin(m_CsPort, m_CsPin, (selected) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void ST7735S::setTextScale(size_t scale)
{
    m_textScale = scale;
}

void ST7735S::setTextColor(const Color& color)
{
	m_textColor = color;
}

void ST7735S::setTextSpacing(const Spacing& spacing)
{
	m_textSpacing = spacing;
}

void ST7735S::drawText(const Point& point, const char* text, size_t size)
{
	static uint8_t lineBreak = 0;

	Point calculatedPoint;

	calculatedPoint.y = point.y;

	for(uint8_t i = 0; size > i; ++i)
	{
		if ((point.x + ((i - lineBreak) * (m_textScale * 5 + m_textSpacing.horizontal))) >=
				((m_displayInfo.width - m_textScale * 5) - m_textSpacing.horizontal))
		{
			lineBreak = i;

			calculatedPoint.y += (m_textScale * 7 + m_textSpacing.vertical);

			if (calculatedPoint.y > ((m_displayInfo.height - m_textScale * 7) - m_textSpacing.vertical))
			{
				return;
			}
			else if (text[i] == ' ')
			{
				++lineBreak;
				continue;
			}
		}

		calculatedPoint.x = point.x + ((i - lineBreak) * (m_textScale * 5 + m_textSpacing.horizontal));

		drawChar(calculatedPoint, text[i]);
	}

	lineBreak = 0;
}

void ST7735S::drawChar(const Point& point, char character)
{
    if (character < 32 || character > 126)
    {
    	return;
    }

	const uint8_t* glyph = Font5x7::DATA[character - 32];

	for(uint8_t x = 0; 5 > x; ++x)
	{
        const uint8_t column = glyph[x];

        for (uint8_t y = 0; y < 7; ++y)
        {
            if (column & (1U << y))
            {
            	fillRect({point.x + (x * m_textScale), point.y + (y * m_textScale)},
            			{m_textScale, m_textScale}, m_textColor);
            }
        }
	}
}

void ST7735S::draw(const Rect& rect, const Color& color)
{
	uint8_t cmd;

	setDrawingArea(rect);

	setTransferMode(TransferType::CMD);
	// 0x2C = Memory Write
	cmd =  static_cast<uint8_t>(Command::RAMWR);
	transport(&cmd, sizeof(cmd));

	setTransferMode(TransferType::DATA);

	uint8_t data[3] {};
	const uint16_t pixels = rect.size.width * rect.size.height;
	uint32_t frameBufferSize = 0;

	if (pixels > MAX_PIXELS)
	{
		return;
	}

	switch(m_pixelFormat)
	{
	case PixelFormat::RGB444:
	{
	    uint8_t r = color.r >> 4;
	    uint8_t g = color.g >> 4;
	    uint8_t b = color.b >> 4;

	    // 2 RGB444 pixels = 3 bytes
	    data[0] = (r << 4) | g;
	    data[1] = (b << 4) | r;
	    data[2] = (g << 4) | b;

	    frameBufferSize = (pixels / 2) * 3 + (pixels % 2) * 2;

	    size_t i = 0;
	    for (; i < (pixels / 2) * 3; i += 3)
	    {
	        m_frameBuffer[i]     = data[0];
	        m_frameBuffer[i + 1] = data[1];
	        m_frameBuffer[i + 2] = data[2];
	    }

	    if (pixels % 2 != 0)
	    {
	        m_frameBuffer[i]     = (r << 4) | g;
	        m_frameBuffer[i + 1] = b << 4;
	    }

	    transport(m_frameBuffer, frameBufferSize);

	    break;
	}
	case PixelFormat::RGB565:
	{
		uint16_t rgb565 =
		    ((color.r >> 3) << 11) |
		    ((color.g >> 2) << 5)  |
		    (color.b >> 3);

		data[0] = rgb565 >> 8;
		data[1] = rgb565 & 0xFF;

		frameBufferSize = pixels * 2;

		for (size_t i = 0; i < frameBufferSize; i += 2)
		{
			m_frameBuffer[i] 	 = data[0];
			m_frameBuffer[i + 1] = data[1];
		}

		transport(m_frameBuffer, frameBufferSize);

		break;
	}
	case PixelFormat::RGB666:
	{
	    uint8_t r = color.r >> 2;
	    uint8_t g = color.g >> 2;
	    uint8_t b = color.b >> 2;

	    data[0] = r << 2;
	    data[1] = g << 2;
	    data[2] = b << 2;

		frameBufferSize = pixels * 3;

		for (size_t i = 0; i < frameBufferSize; i += 3)
		{
			m_frameBuffer[i] 	 = data[0];
			m_frameBuffer[i + 1] = data[1];
			m_frameBuffer[i + 2] = data[2];
		}

		transport(m_frameBuffer, frameBufferSize);

	    break;
	}
	default:
	{
		break;
	}
	}
}

uint16_t ST7735S::getSize(Coordinates coordinates) const
{
	if (coordinates.size != 2)
	{
		return 0;
	}

    uint16_t start =
        (static_cast<uint16_t>(coordinates.data[0]) << 8) |
        coordinates.data[1];

    uint16_t end =
        (static_cast<uint16_t>(coordinates.data[2]) << 8) |
        coordinates.data[3];

    return end - start + 1;
}


void ST7735S::setTransferMode(const TransferType mode) const
{
    HAL_GPIO_WritePin(
        m_RsPort,
        m_RsPin,
        (mode == TransferType::CMD)
            ? GPIO_PIN_RESET
            : GPIO_PIN_SET
    );
}


uint8_t ST7735S::getPixelFormatValue(PixelFormat format) const
{
    switch (format)
    {
        case PixelFormat::RGB444:
            return 0x03;

        case PixelFormat::RGB565:
            return 0x05;

        case PixelFormat::RGB666:
            return 0x06;

        case PixelFormat::Invalid:
        default:
            return 0x00;
    }
}

void ST7735S::configureRgb444Lut() const
{
	// 9.17.2 4096 Color to 262,144 Color
	static constexpr uint8_t rgbLut[128] =
	{
	    // RED 0..15
	    0x00, 0x04, 0x08, 0x0C,
	    0x11, 0x15, 0x19, 0x1D,
	    0x22, 0x26, 0x2A, 0x2E,
	    0x33, 0x37, 0x3B, 0x3F,

	    // RED unused 16..31
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,

	    // GREEN 0..15
	    0x00, 0x04, 0x08, 0x0C,
	    0x11, 0x15, 0x19, 0x1D,
	    0x22, 0x26, 0x2A, 0x2E,
	    0x33, 0x37, 0x3B, 0x3F,

	    // GREEN unused 16..63
	    // 48 bytes
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,

	    // BLUE 0..15
	    0x00, 0x04, 0x08, 0x0C,
	    0x11, 0x15, 0x19, 0x1D,
	    0x22, 0x26, 0x2A, 0x2E,
	    0x33, 0x37, 0x3B, 0x3F,

	    // BLUE unused 16..31
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00
	};

	setTransferMode(TransferType::CMD);

	uint8_t cmd = static_cast<uint8_t>(Command::RGBSET);
	transport(&cmd, 1);

	setTransferMode(TransferType::DATA);

	transport(rgbLut, sizeof(rgbLut));
}

void ST7735S::configureRgb565Lut() const
{
	// 9.17.1 65536 Color to 262,144 Color
	static constexpr uint8_t rgbLut[128] =
	{
		// ----------------------------------------------------
		// RED
		// 5-bit -> 6-bit
		// ----------------------------------------------------
		0x00, 0x02, 0x04, 0x06,
		0x08, 0x0A, 0x0C, 0x0E,
		0x10, 0x12, 0x14, 0x16,
		0x18, 0x1A, 0x1C, 0x1E,

		0x21, 0x23, 0x25, 0x27,
		0x29, 0x2B, 0x2D, 0x2F,
		0x31, 0x33, 0x35, 0x37,
		0x39, 0x3B, 0x3D, 0x3F,

		// ----------------------------------------------------
		// GREEN
		// 6-bit -> 6-bit
		// ----------------------------------------------------
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B,
		0x0C, 0x0D, 0x0E, 0x0F,

		0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B,
		0x1C, 0x1D, 0x1E, 0x1F,

		0x20, 0x21, 0x22, 0x23,
		0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B,
		0x2C, 0x2D, 0x2E, 0x2F,

		0x30, 0x31, 0x32, 0x33,
		0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x3A, 0x3B,
		0x3C, 0x3D, 0x3E, 0x3F,

		// ----------------------------------------------------
		// BLUE
		// 5-bit -> 6-bit
		// ----------------------------------------------------
		0x00, 0x02, 0x04, 0x06,
		0x08, 0x0A, 0x0C, 0x0E,
		0x10, 0x12, 0x14, 0x16,
		0x18, 0x1A, 0x1C, 0x1E,

		0x21, 0x23, 0x25, 0x27,
		0x29, 0x2B, 0x2D, 0x2F,
		0x31, 0x33, 0x35, 0x37,
		0x39, 0x3B, 0x3D, 0x3F
	};

	setTransferMode(TransferType::CMD);

	uint8_t cmd = static_cast<uint8_t>(Command::RGBSET);
	transport(&cmd, 1);

	setTransferMode(TransferType::DATA);

	transport(rgbLut, sizeof(rgbLut));
}

void ST7735S::drawLine(const Point& start, const Point& end, uint8_t thickness, const Color& color)
{
    const uint16_t width = m_displayInfo.width;
    const uint16_t height = m_displayInfo.height;

    int16_t x0 = start.x;
    int16_t y0 = start.y;

    const int16_t x1 = end.x;
    const int16_t y1 = end.y;

    const int16_t dx =
        (x1 >= x0) ? (x1 - x0) : (x0 - x1);

    const int16_t dy =
        (y1 >= y0) ? (y1 - y0) : (y0 - y1);

    const int16_t sx =
        (x0 < x1) ? 1 : -1;

    const int16_t sy =
        (y0 < y1) ? 1 : -1;

    int16_t error = dx - dy;

    while (true)
    {
        const int16_t half = thickness / 2;

        int16_t xStart = x0 - half;
        int16_t yStart = y0 - half;

        int16_t xEnd = xStart + thickness - 1;
        int16_t yEnd = yStart + thickness - 1;

        // Clip to logical display dimensions
        xStart = (xStart < 0)
            ? 0
            : xStart;

        yStart = (yStart < 0)
            ? 0
            : yStart;

        xEnd = (xEnd >= width)
            ? width - 1
            : xEnd;

        yEnd = (yEnd >= height)
            ? height - 1
            : yEnd;

        if (xStart <= xEnd && yStart <= yEnd)
        {
            const uint8_t xData[] =
            {
                static_cast<uint8_t>(xStart >> 8),
                static_cast<uint8_t>(xStart & 0xFF),

                static_cast<uint8_t>(xEnd >> 8),
                static_cast<uint8_t>(xEnd & 0xFF)
            };

            const uint8_t yData[] =
            {
                static_cast<uint8_t>(yStart >> 8),
                static_cast<uint8_t>(yStart & 0xFF),

                static_cast<uint8_t>(yEnd >> 8),
                static_cast<uint8_t>(yEnd & 0xFF)
            };

            draw(
                {
                    {xData, sizeof(xData)},
                    {yData, sizeof(yData)},
                    static_cast<uint16_t>(xEnd - xStart + 1),
                    static_cast<uint16_t>(yEnd - yStart + 1)
                },
                color
            );
        }

        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        const int16_t error2 = 2 * error;

        if (error2 > -dy)
        {
            error -= dy;
            x0 += sx;
        }

        if (error2 < dx)
        {
            error += dx;
            y0 += sy;
        }
    }
}

void ST7735S::fillBackground(const Color& color)
{
	const uint8_t x[] =
	{
	    0x00, 0x00,
	    static_cast<uint8_t>((m_displayInfo.width - 1) >> 8),
	    static_cast<uint8_t>((m_displayInfo.width - 1) & 0xFF)
	};

	const uint8_t y[] =
	{
	    0x00, 0x00,
	    static_cast<uint8_t>((m_displayInfo.height - 1) >> 8),
	    static_cast<uint8_t>((m_displayInfo.height - 1) & 0xFF)
	};

	draw({{x, sizeof(x)},{y, sizeof(y)}, m_displayInfo.width, m_displayInfo.height}, color);
}

void ST7735S::drawRect(const Point& point, const Size& size, const Color& color)
{
    if (size.width == 0 || size.height == 0)
    {
        return;
    }

    const uint16_t xEnd = point.x + size.width - 1;
    const uint16_t yEnd = point.y + size.height - 1;

    drawLine(
        {point.x, point.y},
        {xEnd, point.y},
        1,
        color
    );

    drawLine(
        {xEnd, point.y},
        {xEnd, yEnd},
        1,
        color
    );

    drawLine(
        {xEnd, yEnd},
        {point.x, yEnd},
        1,
        color
    );

    drawLine(
        {point.x, yEnd},
        {point.x, point.y},
        1,
        color
    );
}

void ST7735S::fillRect(const Point& point, const Size& size, const Color& color)
{
    if (size.width == 0 || size.height == 0)
    {
        return;
    }

    const uint16_t xEnd = point.x + size.width - 1;
    const uint16_t yEnd = point.y + size.height - 1;

    const uint8_t x[] =
    {
        static_cast<uint8_t>(point.x >> 8),
        static_cast<uint8_t>(point.x & 0xFF),
        static_cast<uint8_t>(xEnd >> 8),
        static_cast<uint8_t>(xEnd & 0xFF)
    };

    const uint8_t y[] =
    {
        static_cast<uint8_t>(point.y >> 8),
        static_cast<uint8_t>(point.y & 0xFF),
        static_cast<uint8_t>(yEnd >> 8),
        static_cast<uint8_t>(yEnd & 0xFF)
    };

    draw(
        {
            {x, sizeof(x)},
            {y, sizeof(y)},
            size.width,
            size.height
        },
        color
    );
}

void ST7735S::drawCircle(const Point& center, uint8_t radius, const Color& color)
{
    int16_t x = 0;
    int16_t y = radius;

    int16_t decision = 1 - radius;

    while (x <= y)
    {
        const Point points[] =
        {
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y - y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y - y)},
            {static_cast<uint16_t>(center.x + y), static_cast<uint16_t>(center.y + x)},
            {static_cast<uint16_t>(center.x - y), static_cast<uint16_t>(center.y + x)},
            {static_cast<uint16_t>(center.x + y), static_cast<uint16_t>(center.y - x)},
            {static_cast<uint16_t>(center.x - y), static_cast<uint16_t>(center.y - x)}
        };

        for (const Point& point : points)
        {
            if (point.x < m_displayInfo.width &&
                point.y < m_displayInfo.height)
            {
                drawRect(point, {1, 1}, color);
            }
        }

        ++x;

        if (decision < 0)
        {
            decision += 2 * x + 1;
        }
        else
        {
            --y;
            decision += 2 * (x - y) + 1;
        }
    }
}

void ST7735S::fillCircle(const Point& center, uint8_t radius, const Color& color)
{
	int16_t x = 0;
	int16_t y = radius;

	int16_t decision = 1 - radius;

	while (x <= y)
	{
		const int16_t yPositions[] =
		{
			static_cast<int16_t>(center.y + y),
			static_cast<int16_t>(center.y - y),
			static_cast<int16_t>(center.y + x),
			static_cast<int16_t>(center.y - x)
		};

		const int16_t xStarts[] =
		{
			static_cast<int16_t>(center.x - x),
			static_cast<int16_t>(center.x - x),
			static_cast<int16_t>(center.x - y),
			static_cast<int16_t>(center.x - y)
		};

		const int16_t xEnds[] =
		{
			static_cast<int16_t>(center.x + x),
			static_cast<int16_t>(center.x + x),
			static_cast<int16_t>(center.x + y),
			static_cast<int16_t>(center.x + y)
		};

		for (uint8_t i = 0; i < 4; ++i)
		{
			const int16_t yPos = yPositions[i];

			if (yPos < 0 || yPos >= m_displayInfo.height)
			{
				continue;
			}

			const int16_t xStart = xStarts[i];
			const int16_t xEnd = xEnds[i];

			if (xEnd < 0 || xStart >= m_displayInfo.width)
			{
				continue;
			}

			const int16_t clippedStart =
				(xStart < 0) ? 0 : xStart;

			const int16_t clippedEnd =
				(xEnd >= m_displayInfo.width)
					? m_displayInfo.width - 1
					: xEnd;

			if (clippedStart > clippedEnd)
			{
				continue;
			}

			const uint8_t xData[] =
			{
				static_cast<uint8_t>(clippedStart >> 8),
				static_cast<uint8_t>(clippedStart & 0xFF),
				static_cast<uint8_t>(clippedEnd >> 8),
				static_cast<uint8_t>(clippedEnd & 0xFF)
			};

			const uint8_t yData[] =
			{
				static_cast<uint8_t>(yPos >> 8),
				static_cast<uint8_t>(yPos & 0xFF),
				static_cast<uint8_t>(yPos >> 8),
				static_cast<uint8_t>(yPos & 0xFF)
			};

			draw(
				{
					{xData, sizeof(xData)},
					{yData, sizeof(yData)},
					static_cast<uint16_t>(clippedEnd - clippedStart + 1),
					1
				},
				color
			);
		}

		++x;

		if (decision < 0)
		{
			decision += 2 * x + 1;
		}
		else
		{
			--y;
			decision += 2 * (x - y) + 1;
		}
	}
}

void ST7735S::drawEllipse(const Point& center, uint8_t radiusX, uint8_t radiusY, const Color& color)
{
    if (radiusX == 0 || radiusY == 0)
    {
        return;
    }

    const int32_t rx = radiusX;
    const int32_t ry = radiusY;

    const int32_t rx2 = rx * rx;
    const int32_t ry2 = ry * ry;

    int32_t x = 0;
    int32_t y = ry;

    int32_t px = 0;
    int32_t py = 2 * rx2 * y;

    int32_t decision =
        ry2 - rx2 * ry + rx2 / 4;

    while (px < py)
    {
        const Point points[] =
        {
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y - y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y - y)}
        };

        for (const Point& point : points)
        {
            if (point.x < m_displayInfo.width &&
                point.y < m_displayInfo.height)
            {
                drawRect(point, {1, 1}, color);
            }
        }

        if (decision < 0)
        {
            ++x;
            px += 2 * ry2;
            decision += px + ry2;
        }
        else
        {
            ++x;
            --y;
            px += 2 * ry2;
            py -= 2 * rx2;
            decision += px - py + ry2;
        }
    }

    decision =
        ry2 * (x * x + x) +
        rx2 * (y - 1) * (y - 1) -
        rx2 * ry2;

    while (y >= 0)
    {
        const Point points[] =
        {
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y + y)},
            {static_cast<uint16_t>(center.x + x), static_cast<uint16_t>(center.y - y)},
            {static_cast<uint16_t>(center.x - x), static_cast<uint16_t>(center.y - y)}
        };

        for (const Point& point : points)
        {
            if (point.x < m_displayInfo.width &&
                point.y < m_displayInfo.height)
            {
                drawRect(point, {1, 1}, color);
            }
        }

        if (decision > 0)
        {
            --y;
            py -= 2 * rx2;
            decision += rx2 - py;
        }
        else
        {
            ++x;
            --y;
            px += 2 * ry2;
            py -= 2 * rx2;
            decision += px - py + rx2;
        }
    }
}

void ST7735S::fillEllipse(const Point& center, uint8_t radiusX, uint8_t radiusY, const Color& color)
{
    if (radiusX == 0 || radiusY == 0)
    {
        return;
    }

    const int32_t rx = radiusX;
    const int32_t ry = radiusY;

    const int32_t rx2 = rx * rx;
    const int32_t ry2 = ry * ry;

    int32_t x = 0;
    int32_t y = ry;

    int32_t px = 0;
    int32_t py = 2 * rx2 * y;

    int32_t decision =
        ry2 - rx2 * ry + rx2 / 4;

    while (px < py)
    {
        const int16_t xStart =
            static_cast<int16_t>(center.x - x);

        const int16_t xEnd =
            static_cast<int16_t>(center.x + x);

        const int16_t yTop =
            static_cast<int16_t>(center.y - y);

        const int16_t yBottom =
            static_cast<int16_t>(center.y + y);

        if (yTop >= 0 && yTop < m_displayInfo.height)
        {
            const int16_t clippedStart =
                (xStart < 0) ? 0 : xStart;

            const int16_t clippedEnd =
                (xEnd >= m_displayInfo.width)
                    ? m_displayInfo.width - 1
                    : xEnd;

            if (clippedStart <= clippedEnd)
            {
                const uint8_t xData[] =
                {
                    static_cast<uint8_t>(clippedStart >> 8),
                    static_cast<uint8_t>(clippedStart & 0xFF),
                    static_cast<uint8_t>(clippedEnd >> 8),
                    static_cast<uint8_t>(clippedEnd & 0xFF)
                };

                const uint8_t yData[] =
                {
                    static_cast<uint8_t>(yTop >> 8),
                    static_cast<uint8_t>(yTop & 0xFF),
                    static_cast<uint8_t>(yTop >> 8),
                    static_cast<uint8_t>(yTop & 0xFF)
                };

                draw(
                    {
                        {xData, sizeof(xData)},
                        {yData, sizeof(yData)},
                        static_cast<uint16_t>(
                            clippedEnd - clippedStart + 1),
                        1
                    },
                    color
                );
            }
        }

        if (yBottom >= 0 &&
            yBottom < m_displayInfo.height &&
            yBottom != yTop)
        {
            const int16_t clippedStart =
                (xStart < 0) ? 0 : xStart;

            const int16_t clippedEnd =
                (xEnd >= m_displayInfo.width)
                    ? m_displayInfo.width - 1
                    : xEnd;

            if (clippedStart <= clippedEnd)
            {
                const uint8_t xData[] =
                {
                    static_cast<uint8_t>(clippedStart >> 8),
                    static_cast<uint8_t>(clippedStart & 0xFF),
                    static_cast<uint8_t>(clippedEnd >> 8),
                    static_cast<uint8_t>(clippedEnd & 0xFF)
                };

                const uint8_t yData[] =
                {
                    static_cast<uint8_t>(yBottom >> 8),
                    static_cast<uint8_t>(yBottom & 0xFF),
                    static_cast<uint8_t>(yBottom >> 8),
                    static_cast<uint8_t>(yBottom & 0xFF)
                };

                draw(
                    {
                        {xData, sizeof(xData)},
                        {yData, sizeof(yData)},
                        static_cast<uint16_t>(
                            clippedEnd - clippedStart + 1),
                        1
                    },
                    color
                );
            }
        }

        if (decision < 0)
        {
            ++x;

            px += 2 * ry2;

            decision += px + ry2;
        }
        else
        {
            ++x;
            --y;

            px += 2 * ry2;
            py -= 2 * rx2;

            decision += px - py + ry2;
        }
    }

    decision =
        ry2 * (x * x + x) +
        rx2 * (y - 1) * (y - 1) -
        rx2 * ry2;

    while (y >= 0)
    {
        const int16_t xStart =
            static_cast<int16_t>(center.x - x);

        const int16_t xEnd =
            static_cast<int16_t>(center.x + x);

        const int16_t yTop =
            static_cast<int16_t>(center.y - y);

        const int16_t yBottom =
            static_cast<int16_t>(center.y + y);

        if (yTop >= 0 && yTop < m_displayInfo.height)
        {
            const int16_t clippedStart =
                (xStart < 0) ? 0 : xStart;

            const int16_t clippedEnd =
                (xEnd >= m_displayInfo.width)
                    ? m_displayInfo.width - 1
                    : xEnd;

            if (clippedStart <= clippedEnd)
            {
                const uint8_t xData[] =
                {
                    static_cast<uint8_t>(clippedStart >> 8),
                    static_cast<uint8_t>(clippedStart & 0xFF),
                    static_cast<uint8_t>(clippedEnd >> 8),
                    static_cast<uint8_t>(clippedEnd & 0xFF)
                };

                const uint8_t yData[] =
                {
                    static_cast<uint8_t>(yTop >> 8),
                    static_cast<uint8_t>(yTop & 0xFF),
                    static_cast<uint8_t>(yTop >> 8),
                    static_cast<uint8_t>(yTop & 0xFF)
                };

                draw(
                    {
                        {xData, sizeof(xData)},
                        {yData, sizeof(yData)},
                        static_cast<uint16_t>(
                            clippedEnd - clippedStart + 1),
                        1
                    },
                    color
                );
            }
        }

        if (yBottom >= 0 &&
            yBottom < m_displayInfo.height &&
            yBottom != yTop)
        {
            const int16_t clippedStart =
                (xStart < 0) ? 0 : xStart;

            const int16_t clippedEnd =
                (xEnd >= m_displayInfo.width)
                    ? m_displayInfo.width - 1
                    : xEnd;

            if (clippedStart <= clippedEnd)
            {
                const uint8_t xData[] =
                {
                    static_cast<uint8_t>(clippedStart >> 8),
                    static_cast<uint8_t>(clippedStart & 0xFF),
                    static_cast<uint8_t>(clippedEnd >> 8),
                    static_cast<uint8_t>(clippedEnd & 0xFF)
                };

                const uint8_t yData[] =
                {
                    static_cast<uint8_t>(yBottom >> 8),
                    static_cast<uint8_t>(yBottom & 0xFF),
                    static_cast<uint8_t>(yBottom >> 8),
                    static_cast<uint8_t>(yBottom & 0xFF)
                };

                draw(
                    {
                        {xData, sizeof(xData)},
                        {yData, sizeof(yData)},
                        static_cast<uint16_t>(
                            clippedEnd - clippedStart + 1),
                        1
                    },
                    color
                );
            }
        }

        if (decision > 0)
        {
            --y;

            py -= 2 * rx2;

            decision += rx2 - py;
        }
        else
        {
            ++x;
            --y;

            px += 2 * ry2;
            py -= 2 * rx2;

            decision += px - py + rx2;
        }
    }
}

void ST7735S::drawTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color)
{
    drawLine(p1, p2, 1, color);
    drawLine(p2, p3, 1, color);
    drawLine(p3, p1, 1, color);
}

void ST7735S::fillTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color)
{
    Point a = p1;
    Point b = p2;
    Point c = p3;

    // Sort vertices by Y coordinate:
    // a.y <= b.y <= c.y
    if (a.y > b.y)
    {
        const Point temp = a;
        a = b;
        b = temp;
    }

    if (b.y > c.y)
    {
        const Point temp = b;
        b = c;
        c = temp;
    }

    if (a.y > b.y)
    {
        const Point temp = a;
        a = b;
        b = temp;
    }

    // Degenerate triangle: all three points have the same Y.
    // Draw a horizontal line from min X to max X.
    if (a.y == c.y)
    {
        const uint16_t minX =
            (a.x < b.x)
                ? ((a.x < c.x) ? a.x : c.x)
                : ((b.x < c.x) ? b.x : c.x);

        const uint16_t maxX =
            (a.x > b.x)
                ? ((a.x > c.x) ? a.x : c.x)
                : ((b.x > c.x) ? b.x : c.x);

        fillRect(
            {minX, a.y},
            {
                static_cast<uint16_t>(maxX - minX + 1),
                1
            },
            color
        );

        return;
    }

    for (int16_t y = a.y; y <= c.y; ++y)
    {
        int32_t xStart;
        int32_t xEnd;

        if (y < b.y)
        {
            // Upper part of the triangle.
            //
            // a -> b
            // a -> c

            const int32_t dy1 = b.y - a.y;
            const int32_t dy2 = c.y - a.y;

            // dy2 cannot be zero here because a.y != c.y.
            xEnd =
                a.x +
                static_cast<int32_t>(c.x - a.x) *
                (y - a.y) / dy2;

            // If a.y == b.y, there is no upper part.
            // Therefore this branch can only be entered
            // when dy1 > 0.
            if (dy1 != 0)
            {
                xStart =
                    a.x +
                    static_cast<int32_t>(b.x - a.x) *
                    (y - a.y) / dy1;
            }
            else
            {
                xStart = a.x;
            }
        }
        else
        {
            // Lower part of the triangle.
            //
            // b -> c
            // a -> c

            const int32_t dy1 = c.y - b.y;
            const int32_t dy2 = c.y - a.y;

            // dy2 cannot be zero because a.y != c.y.
            xEnd =
                a.x +
                static_cast<int32_t>(c.x - a.x) *
                (y - a.y) / dy2;

            // Horizontal lower edge:
            // b.y == c.y
            //
            // There is no b -> c slope to calculate.
            if (dy1 != 0)
            {
                xStart =
                    b.x +
                    static_cast<int32_t>(c.x - b.x) *
                    (y - b.y) / dy1;
            }
            else
            {
                xStart = b.x;
            }
        }

        if (xStart > xEnd)
        {
            const int32_t temp = xStart;
            xStart = xEnd;
            xEnd = temp;
        }

        // Clip Y.
        if (y < 0 || y >= m_displayInfo.height)
        {
            continue;
        }

        // Clip X.
        if (xStart < 0)
        {
            xStart = 0;
        }

        if (xEnd >= m_displayInfo.width)
        {
            xEnd = m_displayInfo.width - 1;
        }

        if (xStart > xEnd)
        {
            continue;
        }

        const uint8_t xData[] =
        {
            static_cast<uint8_t>(xStart >> 8),
            static_cast<uint8_t>(xStart & 0xFF),

            static_cast<uint8_t>(xEnd >> 8),
            static_cast<uint8_t>(xEnd & 0xFF)
        };

        const uint8_t yData[] =
        {
            static_cast<uint8_t>(y >> 8),
            static_cast<uint8_t>(y & 0xFF),

            static_cast<uint8_t>(y >> 8),
            static_cast<uint8_t>(y & 0xFF)
        };

        draw(
            {
                {xData, sizeof(xData)},
                {yData, sizeof(yData)},
                static_cast<uint16_t>(xEnd - xStart + 1),
                1
            },
            color
        );
    }
}
