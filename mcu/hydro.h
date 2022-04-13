#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0

static int HYDRO_ADDRESS = 0x4D;

void hydroprobe_init(void);
void hydroprobe_calib(uint16_t*, uint16_t*);
uint16_t hydroprobe_read_raw();
uint16_t hydroprobe_avg_val();
uint16_t hydroprobe_max_val();
uint16_t hydroprobe_min_val();
uint8_t hydroprobe_rel_env_hum (uint16_t, uint16_t);