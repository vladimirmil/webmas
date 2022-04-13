#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0

static int TEMPHUM16_ADDRESS = 0x5F;
static uint8_t device_id;

static int TEMPHUM16_REG_AVERAGE = 0x10; 
static int TEMPHUM16_REG_CTRL_1 = 0x20;    
static int TEMPHUM16_REG_CTRL_2 = 0x21;    
static int TEMPHUM16_REG_CTRL_3 = 0x22;    
static int TEMPHUM16_REG_STATUS = 0x27;    
static int TEMPHUM16_REG_H_OUT_L = 0x28;   
static int TEMPHUM16_REG_H_OUT_H = 0x29;   
static int TEMPHUM16_REG_T_OUT_L = 0x2A;   
static int TEMPHUM16_REG_T_OUT_H = 0x2B;   
static int TEMPHUM16_REG_H0_RH_X2 = 0x30;      
static int TEMPHUM16_REG_H1_RH_X2 = 0x31;      
static int TEMPHUM16_REG_T0_DEGC_X8 = 0x32;    
static int TEMPHUM16_REG_T1_DEGC_X8 = 0x33;    
static int TEMPHUM16_REG_T0_T1_DEGC_H2 = 0x35; 
static int TEMPHUM16_REG_H0_T0_OUT_L = 0x36;   
static int TEMPHUM16_REG_H0_T0_OUT_H = 0x37;   
static int TEMPHUM16_REG_H1_T0_OUT_L = 0x3A;   
static int TEMPHUM16_REG_H1_T0_OUT_H = 0x3B;   
static int TEMPHUM16_REG_T0_OUT_L = 0x3C;      
static int TEMPHUM16_REG_T0_OUT_H = 0x3D;      
static int TEMPHUM16_REG_T1_OUT_L = 0x3E;      
static int TEMPHUM16_REG_T1_OUT_H = 0x3F;

static int TEMPHUM16_PWR_MODE_POWER_DOWN = 0x00;
static int TEMPHUM16_PWR_MODE_CONTINUOUS = 0x01;
    
static int TEMPHUM16_BDU_CONTINUOUS_UPDATE = 0x00;
static int TEMPHUM16_BDU_NOT_UPDAT_UNTIL_READ = 0x01;
    
static int TEMPHUM16_ODR_ONE_SHOT_MODE = 0x00;
static int TEMPHUM16_ODR_1_Hz = 0x01;
static int TEMPHUM16_ODR_7_Hz = 0x02;
static int TEMPHUM16_ODR_12_5_Hz = 0x03;
    
static int TEMPHUM16_HEATER_DISABLE = 0x00;
static int TEMPHUM16_HEATER_ENABLE = 0x01;

static int TEMPHUM16_REG_DEVICE_ID = 0x0F;
static int TEMPHUM16_DEVICE_ID = 0xBC;
static int TEMPHUM16_DRDY = 1;


static int CONTINUOUS_MODE_BIT_MASK = 0x80;
static int BDU_CONTINUOUS_UPDATE_BIT_MASK = 0xFB;
static int ODR_BIT_MASK = 0x03;
static int HEATER_BIT_MASK = 0x02;
static int DATA_READY_INTERRUPT_BIT_MASK = 0x04;
static int I2C_AUTO_INCREMENT_RW_BIT = 0x80;


void set_temphum16_ready_pin(uint pin);
void temphum16_init();
void temphum16_get_device_id(uint8_t *device_id );
static void temphum16_i2c_write(uint8_t reg, uint8_t *data_in, uint8_t len);
static void temphum16_i2c_read(uint8_t reg, uint8_t *data_out, uint8_t len);
void temphum16_power_mode(uint8_t pwr_mode);
void temphum16_bdu_mode(uint8_t bdu_mode);
void temphum16_set_odr(uint8_t odr);
void temphum16_heater(uint8_t heater_mode);
void temphum16_enable_data_ready_interrupt();
void temphum16_get_status (uint8_t *status );
uint8_t temphum16_check_ready();
static uint16_t temphum16_get_h_t_out();
static uint16_t temphum16_get_h0_rh();
static uint16_t temphum16_get_h1_rh();
static uint16_t temphum16_get_h0_t0_out();
static uint16_t temphum16_get_h1_t0_out();
static uint16_t temphum16_get_t0_out();
static uint16_t temphum16_get_t1_out();
static uint16_t temphum16_get_t_out();
void temphum16_default_cfg();
void temphum16_get_temperature(float *temperature);
void temphum16_get_humidity (float *humidity );