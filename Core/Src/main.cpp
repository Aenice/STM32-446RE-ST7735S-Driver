#include "main.h"

#include "../../Drivers/ST7735S/ST7735S.hpp"
#include "spi.h"
#include "gpio.h"

void SystemClock_Config(void);

ST7735S display(
    &hspi1,

    GPIOA, GPIO_PIN_7,   // SDA
    GPIOA, GPIO_PIN_9,   // RS
    GPIOA, GPIO_PIN_8,   // RST
    GPIOA, GPIO_PIN_10   // CS
);

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_SPI1_Init();

	display.init();
	display.setSelected(true);
	/*
	display.fillBackground({18, 12, 30});

	display.drawRect({10, 10}, {50, 30}, {255, 0, 0});
	display.fillRect({70, 10}, {50, 30}, {0, 255, 0});

	display.drawCircle({30, 80}, 20, {0, 0, 255});
	display.fillCircle({90, 80}, 20, {255, 255, 0});

	display.drawEllipse({30, 125}, 20, 12, {255, 0, 255});
	display.fillEllipse({90, 125}, 20, 12, {0, 255, 255});

	display.drawTriangle(
		{10, 150},
		{30, 130},
		{50, 150},
		{255, 255, 255}
	);

	display.fillTriangle(
		{70, 150},
		{90, 130},
		{110, 150},
		{255, 128, 0}
	);

	display.drawLine(
		{0, 0},
		{127, 159},
		5,
		{255, 255, 255}
	);
	*/
	display.setOrientation(Orientation::Landscape);
	display.setTextScale(1);
	display.setTextSpacing({3,5});
	display.setTextColor({0, 255, 0});

	char output[] = "Hello, world!, ST7735S TEST: ABC abc 0123 !@#$%^&*() [] {} <> +-=/!";

	display.fillBackground({0, 0, 255});
	display.drawText({10, 10}, output, sizeof(output));

	display.setSelected(false);

	while (true)
	{
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
