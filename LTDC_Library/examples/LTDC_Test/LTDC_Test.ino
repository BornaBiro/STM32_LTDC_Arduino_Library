// Include the library.
#include "STM32_LTDC_Arduino_Library.h"

// Include the image bitmap data.
#include "image1.h"

// Create the lcd object for LTDC library.
LTDCDriver lcd;

//Create the object for UART (needed for the STM32f7508-DK Board).
HardwareSerial myUART(USART1);

void setup()
{
    // Hot fix for now.
    SCB_DisableICache();
    SCB_DisableDCache();
    
    // Init the serial communication.
    myUART.begin(115200);
    myUART.println("Code has started!");

    // Init the library for the LCD.
    lcd.begin();

    // Fill the screen with blue color (32bit color - ARGB).
    lcd.fill(0x000000FF);

    // Draw a bitmap at X = 75 and Y = 75.
    lcd.drawRGBBitmap(75, 75, img1, img1_w, img1_h);

    // Set text to white color and background to black color.
    lcd.setTextColor(0xFFFF, 0x0000);

    // Set text size multiplier to 5.
    lcd.setTextSize(5);
}

int i = 0;
void loop()
{
    lcd.setCursor(0, 0);
    lcd.printf("Test %d", i++);
}

// Custom clock config - TODO: Put this into the libary somehow.
extern "C" void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}