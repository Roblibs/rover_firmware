#include "hall.hpp"

#include "nrf_gpio.h"

#include "nrfx_saadc.h"

#define GPIO_Debug_ADC    NRF_GPIO_PIN_MAP(1,10)

nrf_saadc_input_t an_chan_map[8]={NRF_SAADC_INPUT_AIN0,NRF_SAADC_INPUT_AIN1,NRF_SAADC_INPUT_AIN2,NRF_SAADC_INPUT_AIN3,
                                NRF_SAADC_INPUT_AIN4,NRF_SAADC_INPUT_AIN5,NRF_SAADC_INPUT_AIN6,NRF_SAADC_INPUT_AIN7};

hall_sensors_c *p_sensors = NULL;

void saadc_event_handler(nrfx_saadc_evt_t const * p_evt)
{
    if(p_sensors == NULL)
    {
        return;
    }
    if (p_evt->type == NRFX_SAADC_EVT_DONE)
    {
        nrf_gpio_pin_clear(GPIO_Debug_ADC);
        p_sensors->v1 = p_sensors->adc_values[0] / 4096.0;// 12 bits
        p_sensors->v2 = p_sensors->adc_values[1] / 4096.0;// 12 bits
        p_sensors->v3 = p_sensors->adc_values[2] / 4096.0;// 12 bits
        
    }
}

hall_sensors_c::hall_sensors_c()
{
    p_sensors = this;
}

void hall_sensors_c::init(uint8_t c1, uint8_t c2, uint8_t c3)
{
    nrfx_saadc_config_t init_config = {
        NRF_SAADC_RESOLUTION_12BIT,
        NRF_SAADC_OVERSAMPLE_DISABLED,
        5,                                  //interrup priority
        false                               //low power mode disabled
    };
    nrfx_saadc_init(&init_config,saadc_event_handler);


    nrf_saadc_channel_config_t chan_config = {
        NRF_SAADC_RESISTOR_DISABLED,                //P
        NRF_SAADC_RESISTOR_DISABLED,                //N
        NRF_SAADC_GAIN1_4,
        NRF_SAADC_REFERENCE_VDD4,
        NRF_SAADC_ACQTIME_40US,
        NRF_SAADC_MODE_SINGLE_ENDED,
        NRF_SAADC_BURST_DISABLED,
        NRF_SAADC_INPUT_DISABLED,
        NRF_SAADC_INPUT_DISABLED
    };

    chan_config.pin_p = an_chan_map[c1];
    nrfx_saadc_channel_init(c1,&chan_config);

    chan_config.pin_p = an_chan_map[c2];
    nrfx_saadc_channel_init(c2,&chan_config);

    chan_config.pin_p = an_chan_map[c3];
    nrfx_saadc_channel_init(c3,&chan_config);

}

void hall_sensors_c::convert()
{
    nrf_gpio_pin_set(GPIO_Debug_ADC);
    nrfx_saadc_buffer_convert(adc_values, 3);
    nrfx_saadc_sample();
}
