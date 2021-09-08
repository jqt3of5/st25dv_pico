//
// Created by jqt3o on 9/6/2021.
//

#include <hardware/gpio.h>
#include "hardware/spi.h"
#include "max31855.h"


#define PIN_MISO 4
//#define PIN_CS   5
#define PIN_SCK  6
#define PIN_MOSI 7

#define SPI_PORT spi0
#define READ_BIT 0x80


static inline void cs_select(int csPin) {
    asm volatile("nop \n nop \n nop");
    gpio_put(csPin, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect(int csPin) {
    asm volatile("nop \n nop \n nop");
    gpio_put(csPin, 1);
    asm volatile("nop \n nop \n nop");
}

void max31855_init(int count, uint csPins[])
{
#if !UNIT_TESTS
    // This example will use SPI0 at 0.5MHz.
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    for (int i = 0; i < count; ++i)
    {
        // Chip select is active-low, so we'll initialise it to a driven-high state
        gpio_init(csPins[i]);
        gpio_set_dir(csPins[i], GPIO_OUT);
        gpio_put(csPins[i], 1);
    }

#endif
}

enum Max31855_Result max31855_read(int csPin, int16_t * probeTemp, int16_t * coldTemp)
{
    struct max31855_data data = {0};
    cs_select(csPin);
    //TODO: Test loading the data like this. Might work, who knows
    int n = spi_read_blocking(SPI_PORT, 0, (uint8_t*)&data, 4);
    cs_deselect(csPin);

    if (n != 4)
    {
       return TransferFailed;
    }

    *coldTemp = data.coldTemp;
    *probeTemp =  data.thermocoupleTemp;

    if (data.OC)
    {
        return OC;
    }
    if (data.SCG)
    {
        return SCG;
    }
    if (data.SCV)
    {
        return SCV;
    }
    return SuccessfulRead;
}
