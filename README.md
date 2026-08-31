# 🚀 STM32 ST7735S TFT Display Driver

An example of developing a **C++ TFT display driver** for STM32 using the HAL library and SPI communication.

This project demonstrates how to create a reusable hardware driver for the **ST7735S TFT display controller**, hiding low-level SPI, GPIO, reset, and display configuration operations behind a simple C++ class.

## 🎬 Demo

* First
<img width="320" height="250" alt="Screenshot_20260831_044302_Gallery" src="https://github.com/user-attachments/assets/bb1747e7-d0dc-4eba-883f-771ab11cd5e8" />

* Second
<img width="220" height="350" alt="20260831_024707" src="https://github.com/user-attachments/assets/22ac9ef5-bf77-4cdb-8b88-650d70548c9c" />


## ✨ Features

* 🔧 Object-oriented **C++ driver design**
* ⚡ **STM32 HAL** based implementation
* 🔗 **SPI communication**
* 🔄 Hardware display reset
* 🎨 RGB color support
* 🧭 Display orientation configuration
* 🖼️ Pixel format configuration
* 📝 Text rendering with **5x7 font**
* 🔍 Text scaling
* ↔️ Custom text spacing
* ▫️ Rectangle drawing and filling
* ⚪ Circle drawing and filling
* 🥚 Ellipse drawing and filling
* 🔺 Triangle drawing and filling
* 📏 Line drawing with configurable thickness
* 🌈 Background filling
* 🔌 Configurable GPIO pins
* ♻️ Reusable display driver architecture

## 🛠️ Hardware

* 🧠 **MCU:** STM32F446RE
* 🔌 **Board:** NUCLEO-F446RE
* 🖥️ **Display:** ST7735S TFT
* 🔗 **Interface:** SPI

## 📌 Default Configuration

By default, the example uses **SPI1** and the following GPIO configuration:

```cpp
ST7735S display(
    &hspi1,

    GPIOA, GPIO_PIN_7,   // SDA
    GPIOA, GPIO_PIN_9,   // RS
    GPIOA, GPIO_PIN_8,   // RST
    GPIOA, GPIO_PIN_10   // CS
);
```

### 🔌 Default Pinout

| Signal | Pin    | Description                    |
| ------ | ------ | ------------------------------ |
| 🔗 SPI | `SPI1` | SPI peripheral                 |
| 📤 SDA | `PA7`  | SPI MOSI / Display Data        |
| 🎛️ RS | `PA9`  | Register Select / Data-Command |
| 🔄 RST | `PA8`  | Hardware Reset                 |
| 🔒 CS  | `PA10` | Chip Select                    |

> 💡 **Note:** The pin configuration above is the default configuration used in the example. It can be changed by passing different GPIO ports and pins to the `ST7735S` constructor.

## 📂 Project Structure

```text
Project/
├── Drivers/
│   └── ST7735S/
│       ├── ST7735S.hpp
│       ├── ST7735S.cpp
│       ├── Font5x7.hpp
│       └── Types.hpp
│
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── spi.h
│   │   └── gpio.h
│   │
│   └── Src/
│       ├── main.cpp
│       ├── spi.c
│       └── gpio.c
├── Domain/
│   └── Display/
│       └── Types.hpp
│   
├── Utils/
│   ├── Utils.hpp
│   └── Utils.cpp
│   
└── README.md
```

## ⚙️ Technologies

* **C++17**
* **STM32 HAL**
* **SPI**
* **GPIO**
* **TFT Graphics**
* **Embedded C++**

## 🚀 Initialization

Before communicating with the display, the STM32 HAL, GPIO, and SPI peripherals must be initialized.

The correct initialization sequence is:

```cpp
HAL_Init();

SystemClock_Config();

MX_GPIO_Init();
MX_SPI1_Init();

display.init();
display.setSelected(true);
```

### 🖥️ Display Initialization

The display must be initialized before sending drawing commands:

```cpp
display.init();
```

This performs the required **ST7735S initialization sequence** and prepares the display controller for communication.

### 🔓 Select Display

After initialization, the display must be selected:

```cpp
display.setSelected(true);
```

The display is now ready to receive commands and pixel data.

When communication with the display is finished, it can be deselected:

```cpp
display.setSelected(false);
```

### 🔄 Communication Flow

```text
HAL_Init()
    ↓
SystemClock_Config()
    ↓
MX_GPIO_Init()
    ↓
MX_SPI1_Init()
    ↓
display.init()
    ↓
display.setSelected(true)
    ↓
🎨 Draw / Communicate
    ↓
display.setSelected(false)
```

## 🎨 Drawing Example

The driver provides a simple API for drawing graphical primitives:

```cpp
display.fillBackground({0, 0, 255});

display.drawRect(
    {10, 10},
    {50, 30},
    {255, 0, 0}
);

display.fillRect(
    {70, 10},
    {50, 30},
    {0, 255, 0}
);

display.drawCircle(
    {30, 80},
    20,
    {0, 0, 255}
);

display.fillCircle(
    {90, 80},
    20,
    {255, 255, 0}
);

display.drawLine(
    {0, 0},
    {127, 159},
    5,
    {255, 255, 255}
);
```

## 📝 Text Example

The driver includes a built-in **5x7 font** for text rendering.

```cpp
display.setTextScale(1);
display.setTextSpacing({3, 5});
display.setTextColor({0, 255, 0});

char output[] = "Hello, world!";

display.drawText(
    {10, 10},
    output,
    sizeof(output)
);
```

### 🔍 Text Scaling

Text size can be changed using:

```cpp
display.setTextScale(2);
```

### 🎨 Text Color

Text color can be configured using:

```cpp
display.setTextColor({255, 0, 0});
```

### ↔️ Text Spacing

Character spacing can be configured using:

```cpp
display.setTextSpacing({3, 5});
```

## 🧭 Display Orientation

The display orientation can be changed using:

```cpp
display.setOrientation(Orientation::Landscape);
```

For portrait mode:

```cpp
display.setOrientation(Orientation::Portrait);
```

## 🖼️ Pixel Format

The display pixel format can be configured using:

```cpp
display.setPixelFormat(PixelFormat::RGB565);
```

The current pixel format can be retrieved with:

```cpp
PixelFormat format = display.getPixelFormat();
```

## 🎨 Color Order

The display color order can be configured using:

```cpp
display.setColorOrder(ColorOrder::RGB);
```

or:

```cpp
display.setColorOrder(ColorOrder::BGR);
```

## 📐 Graphics API

### ▫️ Rectangle

```cpp
display.drawRect(...);
display.fillRect(...);
```

### ⚪ Circle

```cpp
display.drawCircle(...);
display.fillCircle(...);
```

### 🥚 Ellipse

```cpp
display.drawEllipse(...);
display.fillEllipse(...);
```

### 🔺 Triangle

```cpp
display.drawTriangle(...);
display.fillTriangle(...);
```

### 📏 Line

```cpp
display.drawLine(...);
```

### 🌈 Background

```cpp
display.fillBackground(...);
```

## 🎯 Purpose

The goal of this project is to demonstrate **embedded C++ driver development** for TFT displays and show how to build a reusable graphics driver for **STM32 microcontrollers**.

The driver separates **application logic** from **low-level hardware communication**, making the ST7735S driver easy to reuse in other STM32 projects.

## Display

This driver was developed and tested with the following display module:

**1.8 inch TFT LCD Display Screen Module SPI ST7735S 128x160**

- Driver IC: ST7735S
- Resolution: 128 × 160 pixels
- Interface: 4-wire SPI
- Display: TFT
- Operating voltage: 3.3 V

⭐ If you find this project useful, feel free to give it a **star**!
