//**********************************************************************************
//  Copyright 2024 Paul Chote
//  This file is part of shtstick, which is free software. It is made
//  available to you under version 3 (or later) of the GNU General Public License,
//  as published by the Free Software Foundation and included in the LICENSE file.
//**********************************************************************************

#include <stdio.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

uint8_t crc(uint8_t *data, int len)
{
	uint8_t crc = 0xFF;
	for (int i = 0; i < len; i++)
	{
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x131 : crc << 1;
	}

    return crc;
}

int main()
{
    stdio_init_all();

    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("USB SHT35 temperature + humidity sensor"));

    i2c_init(i2c_default, 100000);    
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    uint8_t cmd[2] = { 0x24, 0x00 };
    uint8_t data[6];

    while (true) {
        sleep_ms(1000);
        i2c_write_blocking(i2c_default, 0x45, cmd, 2, false);
        sleep_ms(16);
        i2c_read_blocking(i2c_default, 0x45, data, 6, false);

        float temperature = 175 * (data[0] * 256 + data[1]) / 65535. - 45;
        bool temperature_valid = data[2] == crc(data, 2);
        float humidity = 100 * (data[3] * 256 + data[4]) / 65535.;
        bool humidity_valid = data[5] == crc(data + 3, 2);

        if (stdio_usb_connected())
            printf("2R0,Ti=%.1f%s,Ui=%.1f%s\r\n",
                temperature, temperature_valid ? "C" : "#",
                humidity, humidity_valid ? "P" : "#");
    }
}