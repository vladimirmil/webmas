#include "ambient.h"

static int AMBIENT_VCC_3_3 = 3300;
static int AMBIENT_RES_12_BIT = 4096;
static int AMBIENT_GAIN = 7;

uint16_t ambient_calculate_light_intensity(uint16_t adc_data){
    uint32_t temp;
    uint16_t light_intensity;
    temp = adc_data;
    temp *= AMBIENT_VCC_3_3;
    temp /= AMBIENT_RES_12_BIT;
    temp /= AMBIENT_GAIN;
    
    light_intensity = (uint16_t) temp;
    return light_intensity;
}