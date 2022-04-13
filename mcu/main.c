#include "ambient.h"
#include "hydro.h"
#include "temphum16.h"

#define I2C_PORT i2c0

static float humidity = 0.0;
static float temperature = 0.0;
static uint8_t humy_val = 0;
static uint16_t dry_val = 0;
static uint16_t wet_val = 0;
static uint16_t light_intensity = 0;


int main(){
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    stdio_init_all();
    adc_init();
    adc_select_input(0);
    
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    
    sleep_ms(1000);

    temphum16_init();
    sleep_ms(500);

    hydroprobe_init();
    sleep_ms(500);

    gpio_put(LED_PIN, 1);
    hydroprobe_calib(&dry_val, &wet_val);
    gpio_put(LED_PIN, 0);
    sleep_ms(3000);
    

    while(1){
        gpio_put(LED_PIN, 1);

        // soil humidity
        humy_val = hydroprobe_rel_env_hum(dry_val, wet_val );

        // air humidity and temperature
        if ( temphum16_check_ready() == TEMPHUM16_DRDY ) {
            temphum16_get_humidity(&humidity);
            temphum16_get_temperature(&temperature);
            sleep_ms(5);
            temphum16_get_temperature(&temperature);
        }

        // light intensity
        uint16_t raw = adc_read();
        light_intensity = ambient_calculate_light_intensity(raw);

        printf("%d,%.2f,%.2f,%d,\n", (uint16_t)humy_val, humidity, temperature, light_intensity);

        gpio_put(LED_PIN, 0);
        sleep_ms(4995);
    }
}