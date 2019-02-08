#ifndef _APP_HALL_HPP__
#define _APP_HALL_HPP__


#include <stdint.h>
#include "nrf_saadc.h"

class hall_sensors_c
{
    public:
        hall_sensors_c();
        void init(uint8_t c1, uint8_t c2, uint8_t c3);
        void convert();
    public:
        float v1,v2,v3;
        nrf_saadc_value_t   adc_values[3];
};

#endif /*_APP_HALL_HPP__*/
