#ifndef __ARDUINOLTDC_LIBRARY_H__
#define __ARDUINOLTDC_LIBRARY_H__

// Include Arduio header file
#include "Arduino.h"

// Define LCD screen size
#define LTDC_LCD_WIDTH  480
#define LTDC_LCD_HEIGHT 272

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                                                            \
    {                                                                                                                  \
        int16_t t = a;                                                                                                 \
        a = b;                                                                                                         \
        b = t;                                                                                                         \
    }
#endif

// Include header file for addidional HAL modules that needs to be enabled.
// #include "hal_conf_extra.h"

// Include STM32 HAL Files
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_ltdc.h"
#include "stm32f7xx_hal_sram.h"

// Include library defines.
#include "defines.h"

// Include library specific includes for STM32 LTDC Driver.
#include "STM32_System_Init.h"

// Include Adafruit GFX.
#include "Adafruit_GFX.h"

class LTDCDriver : public Adafruit_GFX
{
  public:
    LTDCDriver();
    void begin();
    void drawPixel(int16_t x0, int16_t y0, uint16_t color);
    void clearDisplay();
    void fill(uint32_t _color);
    void drawBitmap32Bit(int _x, int _y, uint32_t *_bitmap, int _w, int _h);
    void setCurrentLayer(uint8_t _layer);
    void setAdafruitGFXAlpha(uint8_t _alphaLayerValue);
    void drawPixel32Bit(int _x, int _y, uint32_t _color);
    void end();

  private:
    DMA_HandleTypeDef *_myDMA;
    SDRAM_HandleTypeDef *_mySDRAM;

    // Poiter to the current framebuffer.
    uint32_t _fbPtr = 0xC0000000;

    // Current value of the alpha layer in the Adafruit GFX (since Adafruit GFX does not support 32bit color).
    uint32_t _gfxAlpha = 0xFF000000;
};

#endif