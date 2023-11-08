// Include the library.
#include "STM32_LTDC_Arduino_Library.h"

// Include the image bitmap data.
#include "image1.h"

// Include icons.
#include "icons.h"

// Create the lcd object for LTDC library.
LTDCDriver lcd;

// Create the object for UART (needed for the STM32f7508-DK Board).
HardwareSerial myUART(USART1);

// Variable that holds the state of the user button.
uint8_t userButtonState = 0;

void setup()
{
    // Init the serial communication.
    myUART.begin(115200);
    myUART.println("Code has started!");

    // Set the pin for the "high beams icon" (USER BUTTON).
    pinMode(PI11, INPUT);

    // Init the library for the LCD.
    lcd.begin();

    // Set main layer (backgorund layer)
    lcd.setCurrentLayer(0);

    // Fill the screen with purple color (32bit color - ARGB).
    lcd.fill(0xFF900CC2);

    // Set addidional layer (backgorund layer)
    lcd.setCurrentLayer(1);

    // Set all to transparent.
    lcd.fill(0x00000000);

    // Switch back to main layer.
    lcd.setCurrentLayer(0);

    // Draw a bitmap at X = 75 and Y = 75.
    lcd.drawRGBBitmap(75, 75, img1, img1_w, img1_h);

    // Set text to white color and background to black color.
    lcd.setTextColor(0xFFFF, 0x0000);

    // Select addtional layer.
    // lcd.setCurrentLayer(1);

    // Draw low beam icon at X = 20, Y = 220.
    // lcd.drawBitmap32Bit(20, 220, (uint32_t*)lowBeamIcon, lowBeamIcon_w, lowBeamIcon_h);

    // Go back to the main layer.
    lcd.setCurrentLayer(0);

    // Set text size multiplier to 5.
    lcd.setTextSize(5);
}

int i = 0;
void loop()
{
    // Print variable on the LCD.
    lcd.setCursor(0, 0);
    lcd.printf("Test %d", i++);

    // Check the state of the user button.
    int userButton = digitalRead(PI11);

    // If there is change detected, update the icon state (visable or not).
    if (userButton != userButtonState)
    {
        // Set the current layer to the addtional layer.
        lcd.setCurrentLayer(1);

        // If the button is pressed, icon should be visable.
        if (userButton)
        {
            // Set the alpha to visible.
            lcd.setAdafruitGFXAlpha(0xFF);

            // Draw the icon.
            lcd.drawBitmap32Bit(100, 220, (uint32_t *)highBeamIcon, highBeamIcon_w, highBeamIcon_h);
        }
        else
        {
            // Otherwise, icon should not be visable (set alpha to 0).
            lcd.setAdafruitGFXAlpha(0x00);

            // Draw a "white" rectangle.
            lcd.fillRect(100, 220, highBeamIcon_w, highBeamIcon_h, 0);
        }

        // Set everything back to normal.
        lcd.setCurrentLayer(0);
        lcd.setAdafruitGFXAlpha(0xFF);
    }

    // Update the user button variable state.
    userButtonState = userButton;
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
    {
        Error_Handler();
    }
}