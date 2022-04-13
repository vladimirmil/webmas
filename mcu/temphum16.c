#include "temphum16.h"


static void temphum16_i2c_write(uint8_t reg, uint8_t *data_in, uint8_t len){
    uint8_t tx_buf[ 257 ];
    uint8_t cnt;

    tx_buf[ 0 ] = reg;
    if ( len > 1 ) {
        tx_buf[ 0 ] |= I2C_AUTO_INCREMENT_RW_BIT;   
    }

    for ( cnt = 1; cnt <= len; cnt++ ) {
        tx_buf[ cnt ] = data_in[ cnt - 1 ];
    }

    i2c_write_blocking(I2C_PORT, TEMPHUM16_ADDRESS, tx_buf, len + 1, false);
}


static void temphum16_i2c_read(uint8_t reg, uint8_t *data_out, uint8_t len){
    if ( len > 1 ) {
        reg |= I2C_AUTO_INCREMENT_RW_BIT;   
    }
    i2c_write_blocking(I2C_PORT, TEMPHUM16_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, TEMPHUM16_ADDRESS, data_out, len, false);
}


void temphum16_get_device_id(uint8_t *device_id ){
    temphum16_i2c_read(TEMPHUM16_REG_DEVICE_ID, device_id, 1);
}


void temphum16_init(){
    sleep_ms(1000);
    //I2C_TEMPHUM16_PORT = i2c_port;
    temphum16_default_cfg();

    sleep_ms(500);

    temphum16_get_device_id(&device_id);
    if (device_id != TEMPHUM16_DEVICE_ID){
        while(1){
            printf("Error: temphum16 chip id \n" );
            sleep_ms(1000);
        }
    }
}


void temphum16_power_mode(uint8_t pwr_mode){
    uint8_t buf[1];
    
    temphum16_i2c_read(TEMPHUM16_REG_CTRL_1, buf, 1);
    
    if ( pwr_mode == TEMPHUM16_PWR_MODE_POWER_DOWN ) {
        buf[0] &= ~CONTINUOUS_MODE_BIT_MASK;    
    } else if ( pwr_mode == TEMPHUM16_PWR_MODE_CONTINUOUS ) {
        buf[0] |= CONTINUOUS_MODE_BIT_MASK;    
    }
    
    temphum16_i2c_write(TEMPHUM16_REG_CTRL_1, buf, 1);
}


void temphum16_bdu_mode(uint8_t bdu_mode){
    uint8_t buf[1];
    
    temphum16_i2c_read(TEMPHUM16_REG_CTRL_1, buf, 1);
    
    if ( bdu_mode == TEMPHUM16_BDU_CONTINUOUS_UPDATE )
        buf[0] &= BDU_CONTINUOUS_UPDATE_BIT_MASK;    
    else if ( bdu_mode == TEMPHUM16_BDU_NOT_UPDAT_UNTIL_READ )
        buf[0] |= ~BDU_CONTINUOUS_UPDATE_BIT_MASK;    
    
    temphum16_i2c_write(TEMPHUM16_REG_CTRL_1, buf, 1);
}


void temphum16_set_odr(uint8_t odr){
    uint8_t buf[1];
    
    temphum16_i2c_read(TEMPHUM16_REG_CTRL_1, buf, 1);
    
    odr &= ODR_BIT_MASK;
    buf[0] |= odr; 
    
    temphum16_i2c_write(TEMPHUM16_REG_CTRL_1, buf, 1);
}


void temphum16_heater(uint8_t heater_mode){
    uint8_t buf[1];
    
    temphum16_i2c_read(TEMPHUM16_REG_CTRL_2, buf, 1);
    
    if ( heater_mode == TEMPHUM16_HEATER_DISABLE )
        buf[0] &= ~HEATER_BIT_MASK;    
    else if ( heater_mode == TEMPHUM16_HEATER_ENABLE )
        buf[0] |= HEATER_BIT_MASK;    

    temphum16_i2c_write(TEMPHUM16_REG_CTRL_2, buf, 1);
}


void temphum16_enable_data_ready_interrupt(){
    uint8_t tx_buf[1];
    
    tx_buf[0] = DATA_READY_INTERRUPT_BIT_MASK;
    
    temphum16_i2c_write(TEMPHUM16_REG_CTRL_3, tx_buf, 1);
}


void temphum16_get_status (uint8_t *status ){
    temphum16_i2c_read(TEMPHUM16_REG_CTRL_2, status, 1);
}


uint8_t temphum16_check_ready(){
    return gpio_get(0);
}


static uint16_t temphum16_get_h_t_out(){
    uint8_t rx_buf[ 2 ];
    uint16_t h_t_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_H_OUT_H, rx_buf, 1);

    h_t_out = rx_buf[ 0 ];
    h_t_out <<= 8;
    
    temphum16_i2c_read(TEMPHUM16_REG_H_OUT_L, rx_buf, 1);
    h_t_out |= rx_buf[ 0 ];

   return h_t_out;
}

static uint16_t temphum16_get_h0_rh() {
    uint16_t h0_rh = 0;
    uint8_t rx_buf[ 1 ];
    
    temphum16_i2c_read(TEMPHUM16_REG_H0_RH_X2, rx_buf, 1);
    
    h0_rh = ( uint16_t ) rx_buf[ 0 ];
    
    return h0_rh;
}

static uint16_t temphum16_get_h1_rh () {
    uint16_t h1_rh = 0;
    uint8_t rx_buf[ 1 ];

    temphum16_i2c_read(TEMPHUM16_REG_H1_RH_X2, rx_buf, 1);
    
    h1_rh = ( uint16_t ) rx_buf[ 0 ];

    return h1_rh;
}

static uint16_t temphum16_get_h0_t0_out () {
    uint8_t rx_buf[ 2 ];
    uint16_t h0_t0_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_H0_T0_OUT_H, rx_buf, 1);

    h0_t0_out = rx_buf[ 0 ];
    h0_t0_out <<= 8;
    
    temphum16_i2c_read(TEMPHUM16_REG_H0_T0_OUT_L, rx_buf, 1);
    h0_t0_out |= rx_buf[ 0 ];

    return h0_t0_out;
}

static uint16_t temphum16_get_h1_t0_out(){
    uint8_t rx_buf[ 2 ];
    uint16_t h1_t0_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_H1_T0_OUT_H, rx_buf, 1);

    h1_t0_out = rx_buf[ 0 ];
    h1_t0_out <<= 8;
    
    temphum16_i2c_read(TEMPHUM16_REG_H1_T0_OUT_L, rx_buf, 1);
    h1_t0_out |= rx_buf[ 0 ];
    
    return h1_t0_out;
}


static uint16_t temphum16_get_t0_out() {
    uint8_t buffer;
    uint16_t t0_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_T0_OUT_H, &buffer, 1);

    t0_out = ((uint16_t)buffer) << 8;

    temphum16_i2c_read(TEMPHUM16_REG_T0_OUT_L, &buffer, 1);

    t0_out |= buffer;

    return t0_out;
}

static uint16_t temphum16_get_t1_out() {
    uint8_t buffer;
    uint16_t t1_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_T1_OUT_H, &buffer, 1);
    t1_out = ((uint16_t)buffer) << 8;

    temphum16_i2c_read(TEMPHUM16_REG_T1_OUT_L, &buffer, 1);
    t1_out |= buffer;

    return t1_out;
}

static uint16_t temphum16_get_t_out()
{
    uint8_t buffer;
    uint16_t t_out = 0;
    
    temphum16_i2c_read(TEMPHUM16_REG_T_OUT_H, &buffer, 1);

    t_out = ( uint16_t ) buffer << 8;

    temphum16_i2c_read(TEMPHUM16_REG_T_OUT_L, &buffer, 1);

    t_out |= buffer;

    return t_out;
}


void temphum16_default_cfg(){
    temphum16_power_mode(TEMPHUM16_PWR_MODE_CONTINUOUS);
    sleep_ms(100);
      
    temphum16_bdu_mode(TEMPHUM16_BDU_NOT_UPDAT_UNTIL_READ);
    sleep_ms(100);
      
    temphum16_set_odr(TEMPHUM16_ODR_1_Hz);
    sleep_ms(100);

    temphum16_enable_data_ready_interrupt();
    sleep_ms(100);
}


void temphum16_get_temperature(float *temperature){
    int16_t t0_out;
    int16_t t1_out;
    int16_t t_out;
    uint16_t t0_degc_x8_u16;
    uint16_t t1_degc_x8_u16;
    int16_t t0_degc, t1_degc;
    uint8_t rx_buf;
    uint8_t tmp; 
    float t_temp;
    float deg;
    
    t_temp = 0.0;
    deg = 0.0;
    temphum16_i2c_read(TEMPHUM16_REG_T0_T1_DEGC_H2, &tmp, 1);
    temphum16_i2c_read(TEMPHUM16_REG_T0_DEGC_X8, &rx_buf, 1);
    
    t0_degc_x8_u16 = ( ( ( uint16_t ) ( tmp & 0x03 ) ) << 8 ) | ( ( uint16_t ) rx_buf );
    t0_degc = ( int16_t ) ( t0_degc_x8_u16 >> 3 );
    
    temphum16_i2c_read(TEMPHUM16_REG_T1_DEGC_X8, &rx_buf, 1);
    
    t1_degc_x8_u16 = ( ( ( uint16_t ) ( tmp & 0x0C ) ) << 6 ) | ( ( uint16_t ) rx_buf );
    t1_degc = ( int16_t ) ( t1_degc_x8_u16 >> 3 );
    t0_out = ( int16_t ) temphum16_get_t0_out();
    t1_out = ( int16_t ) temphum16_get_t1_out();
    t_out = ( int16_t ) temphum16_get_t_out();
    deg = ( float ) ( t1_degc - t0_degc );
    t_temp = ( float )( ( t_out - t0_out ) * deg) / ( float ) (t1_out - t0_out);
    deg = ( float ) t0_degc ;
    
    *temperature = deg + t_temp;
}


void temphum16_get_humidity (float *humidity ) {
    int16_t h0_t0_out;
    int16_t h1_t0_out;
    int16_t h_t_out;
    uint16_t h0_rh;
    uint16_t h1_rh;
    float h_temp;
    float hum;
   
    h_temp = 0.0;
    hum = 0.0;

    h_t_out = temphum16_get_h_t_out();
    h0_rh = temphum16_get_h0_rh();
    h0_rh >>=  1;
    h1_rh = temphum16_get_h1_rh();
    h1_rh >>= 1;
    
    hum = ( float ) ( h1_rh - h0_rh );
    h0_t0_out = ( int16_t ) temphum16_get_h0_t0_out();
    h1_t0_out = ( int16_t ) temphum16_get_h1_t0_out();
    
    h_temp = ( float ) h_t_out;
    h_temp -= ( float ) h0_t0_out;
    h_temp *= hum;
    h_temp /= ( float ) ( h1_t0_out - h0_t0_out );
    hum = ( float ) h0_rh;
    
    *humidity = hum + h_temp;
}