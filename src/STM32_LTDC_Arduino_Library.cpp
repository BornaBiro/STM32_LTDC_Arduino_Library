// Include header file of the library
#include "STM32_LTDC_Arduino_Library.h"

LTDCDriver::LTDCDriver() : Adafruit_GFX(LTDC_LCD_WIDTH, LTDC_LCD_HEIGHT)
{
    // Empty, for now.
}

void LTDCDriver::begin()
{
    // WHY STM32DUINO WHY FOR THE LOVE OF GOD?!?!?!?! It doesn't work without this.
    // Some of the LCD and SDRAM FMC pins are NOT configured?!?!?!?!?!
    // LCD Pins.
    pinMode(PE4, OUTPUT);
    pinMode(PJ13, OUTPUT);
    pinMode(PK7, OUTPUT);
    pinMode(PK6, OUTPUT);
    pinMode(PK5, OUTPUT);
    pinMode(PG12, OUTPUT);
    pinMode(PJ14, OUTPUT);
    pinMode(PI10, OUTPUT);
    pinMode(PK4, OUTPUT);
    pinMode(PJ15, OUTPUT);
    pinMode(PI9, OUTPUT);
    pinMode(PK1, OUTPUT);
    pinMode(PK2, OUTPUT);
    pinMode(PI15, OUTPUT);
    pinMode(PJ11, OUTPUT);
    pinMode(PK0, OUTPUT);
    pinMode(PI14, OUTPUT);
    pinMode(PJ8, OUTPUT);
    pinMode(PJ10, OUTPUT);
    pinMode(PJ7, OUTPUT);
    pinMode(PJ9, OUTPUT);
    pinMode(PJ6, OUTPUT);
    pinMode(PJ4, OUTPUT);
    pinMode(PJ5, OUTPUT);
    pinMode(PJ3, OUTPUT);
    pinMode(PJ2, OUTPUT);
    pinMode(PJ0, OUTPUT);
    pinMode(PJ1, OUTPUT);

    // FMC Pins.
    pinMode(PE1, OUTPUT);
    pinMode(PE0, OUTPUT);
    pinMode(PG15, OUTPUT);
    pinMode(PD0, OUTPUT);
    pinMode(PD1, OUTPUT);
    pinMode(PF0, OUTPUT);
    pinMode(PF1, OUTPUT);
    pinMode(PF2, OUTPUT);
    pinMode(PF3, OUTPUT);
    pinMode(PG8, OUTPUT);
    pinMode(PF4, OUTPUT);
    pinMode(PH5, OUTPUT);
    pinMode(PH3, OUTPUT);
    pinMode(PF5, OUTPUT);
    pinMode(PD15, OUTPUT);
    pinMode(PD10, OUTPUT);
    pinMode(PC3, OUTPUT);
    pinMode(PD14, OUTPUT);
    pinMode(PD9, OUTPUT);
    pinMode(PD8, OUTPUT);
    pinMode(PF12, OUTPUT);
    pinMode(PG1, OUTPUT);
    pinMode(PF15, OUTPUT);
    pinMode(PF13, OUTPUT);
    pinMode(PG0, OUTPUT);
    pinMode(PE8, OUTPUT);
    pinMode(PG5, OUTPUT);
    pinMode(PG4, OUTPUT);
    pinMode(PF14, OUTPUT);
    pinMode(PF11, OUTPUT);
    pinMode(PE9, OUTPUT);
    pinMode(PE11, OUTPUT);
    pinMode(PE14, OUTPUT);
    pinMode(PE7, OUTPUT);
    pinMode(PE10, OUTPUT);
    pinMode(PE12, OUTPUT);
    pinMode(PE15, OUTPUT);
    pinMode(PE13, OUTPUT);

    // Initialize driver for the DMA.
    MX_DMA_Init();

    // Init the FMC (for SDRAM).
    MX_FMC_Init();

    // Init the Parallel LCD Controller.
    MX_LTDC_Init();

    // Link SDRAM with DMA.
    // Get the DMA instance.
    _myDMA = getDMAInstance();

    // Get the SDRAM instance.
    _mySDRAM = getSDRAMInstance();

    // Link SDRAM controller with DMA.
    _mySDRAM->hdma = _myDMA;

    // Enable LCD and backlight.
    pinMode(PI12, OUTPUT);
    pinMode(PK3, OUTPUT);
    digitalWrite(PI12, HIGH);
    digitalWrite(PK3, HIGH);
}

void LTDCDriver::drawPixel(int16_t x0, int16_t y0, uint16_t color)
{
    if (x0 > width() - 1 || y0 > height() - 1 || x0 < 0 || y0 < 0)
        return;

    // TODO: Check this if this work at all.
    // switch (rotation)
    // {
    // case 1:
    //     _swap_int16_t(x0, y0);
    //     x0 = height() - x0 - 1;
    //     break;
    // case 2:
    //     x0 = width() - x0 - 1;
    //     y0 = height() - y0 - 1;
    //     break;
    // case 3:
    //     _swap_int16_t(x0, y0);
    //     y0 = width() - y0 - 1;
    //     break;
    // }

    // Upconvert from 565 RGB to 888RGB.
    // Extract the individual color components from 565RGB
    uint8_t _r = (color >> 11) & 0b00011111;
    uint8_t _g = (color >> 5) & 0b00111111;
    uint8_t _b = color & 0b00011111;

    // Upscale them into ()
    _r = ((uint16_t)(_r)*255 + 15) / 31;
    _g = ((uint16_t)(_g)*255 + 31) / 63;
    _b = ((uint16_t)(_b)*255 + 15) / 31;

    // Combine the components into an 888RGB color
    uint32_t _rgb = ((_r << 16) | (_g << 8) | _b) | _gfxAlpha;

    *(uint32_t *)(_fbPtr + ((y0 * LTDC_LCD_WIDTH + x0) * 4)) = _rgb;
}

void LTDCDriver::clearDisplay()
{
    // Use fill to fill buffer with white color.
    fill(0x00000000);
}

void LTDCDriver::fill(uint32_t _color)
{
    // Buffer for one line.
    uint32_t fbData[480];

    // Fill the array with selected color.
    for (int i = 0; i < 480; i++)
    {
        fbData[i] = _color;
    }

    // DCache issue... Thanks chatGPT for the help with this one.
    // Clean and invalidate the entire DCache
    SCB_CleanInvalidateDCache();

    // Data synchronization barrier
    __DSB();

    for (int i = 0; i < 272; i++)
    {
        // Staart the DMA transfer.
        HAL_SDRAM_Write_DMA(_mySDRAM, (uint32_t *)(_fbPtr + (480 * i * 4)), fbData, 480);

        // Wait until the transfer is done.
        HAL_DMA_PollForTransfer(_myDMA, HAL_DMA_FULL_TRANSFER, 0xFFF);

        // Set the state of the SDRAM controller to READY (DMA sets it to BUSY).
        _mySDRAM->State = HAL_SDRAM_STATE_READY;
    }

    // Clean and invalidate the entire DCache
    SCB_CleanInvalidateDCache();

    // Data synchronization barrier
    __DSB();
}

void LTDCDriver::drawBitmap32Bit(int _x, int _y, uint32_t *_bitmap, int _w, int _h)
{
    // Write the pixels to the selected framebuffer.
    // TODO: Add rotation!
    for (int i = 0; i < _w; i++)
    {
        for (int j = 0; j < _h; j++)
        {
            drawPixel32Bit(_x + i, _y + j, _bitmap[(_w * j) + i]);
        }
    }
}

void LTDCDriver::drawPixel32Bit(int _x, int _y, uint32_t _color)
{
    // Calculate the framebuffer start offset.
    // uint32_t _offset = ((width() * _y) + _x) * 4;
    *(uint32_t *)(_fbPtr + (((width() * _y) + _x) * 4)) = _color;
}

void LTDCDriver::setCurrentLayer(uint8_t _layer)
{
    // Limit to olny two layers.
    _layer = _layer & 1;

    // Select the layer.
    if (_layer == 0)
    {
        _fbPtr = STM32_LTDC_LAYER1_ADDR;
    }
    else if (_layer == 1)
    {
        _fbPtr = STM32_LTDC_LAYER2_ADDR;
    }
}

void LTDCDriver::setAdafruitGFXAlpha(uint8_t _alphaLayerValue)
{
    _gfxAlpha = _alphaLayerValue << 24;
}

void LTDCDriver::end()
{
    // TODO: Add de-init.
}