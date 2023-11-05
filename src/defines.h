#ifndef __STM32_LTDC_DEFINES_H__
#define __STM32_LTDC_DEFINES_H__

// Layer pointer addresses
// Main layer (aka. backgorund layer)
#define STM32_LTDC_LAYER1_ADDR (0xC0000000ULL)
// Addidional layer (STM32_LTDC_LAYER1_ADDR + (width * height * 4))
#define STM32_LTDC_LAYER2_ADDR (0xC007F800ULL)

#endif