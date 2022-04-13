#include "hydro.h"


void hydroprobe_init(/*i2c_inst_t *i2c_port*/void){
    sleep_ms(1000);
    //I2C_HYDRO_PORT = i2c_port;
    uint8_t reg = 0x00;
    uint8_t chipID[1];
    // true, no stop signal is sent
    i2c_write_blocking(I2C_PORT, HYDRO_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, HYDRO_ADDRESS, chipID, 1, false);

    if(chipID[0] != 0xD){
        while(1){
            printf("Error: Hydroprobe chip id");
            sleep_ms(1000);
        }
    }
}


uint16_t hydroprobe_read_raw(){
    uint8_t rx_buf[2];
    uint16_t result;

    i2c_read_blocking(I2C_PORT, HYDRO_ADDRESS, rx_buf, 2, false);

    result = rx_buf[0];
    result <<= 8;
    result |= rx_buf[1];
    result &= 0x0FFF;

    return result;
}


uint16_t hydroprobe_avg_val(){
    uint32_t avg = 0;
    uint8_t cnt;

    for (cnt = 0; cnt < 100; cnt++){
        avg += hydroprobe_read_raw();
        sleep_ms(1);
    }
    
    avg /= 100;

    return (uint16_t) avg;
}


uint16_t hydroprobe_max_val(){
    uint16_t tmp = 0;
    uint16_t max = 0;
    uint8_t cnt;
    
    max = hydroprobe_read_raw();

    for (cnt = 0; cnt < 100; cnt++){
        tmp = hydroprobe_read_raw();
        if ( tmp > max )
            max = tmp;
        
        sleep_ms(1);
    }

    return max;
}


uint16_t hydroprobe_min_val(){
    uint16_t tmp = 0;
    uint16_t min = 65500;
    uint8_t cnt;
    
    min = hydroprobe_read_raw();

    for (cnt = 0; cnt < 100; cnt++){
        tmp = hydroprobe_read_raw();
        if ( tmp < min )
            min = tmp;

        sleep_ms(1);
    }

    return min;
}


uint8_t hydroprobe_rel_env_hum(uint16_t dry_val, uint16_t wet_val){
    uint16_t raw_val;
    float result = 0.0;

    raw_val = hydroprobe_avg_val();

    if (raw_val > dry_val)
        raw_val = dry_val;

    if (raw_val < wet_val)
        raw_val = wet_val;

    result = 100.0 - ((((float)raw_val - (float)wet_val) * 100.0) / ((float)dry_val - (float)wet_val));

    return (uint8_t) result;
}



void hydroprobe_calib(uint16_t* dry_val, uint16_t* wet_val){
    uint8_t cnt;

    //printf(" Keep the Probe dry \r\n");
    
    *dry_val = hydroprobe_max_val();
    sleep_ms(5000);

    //printf(" Submerge the Probe in liquid \r\n");

    for ( cnt = 5; cnt > 0; cnt-- ){
        //printf(" %d \r\n ", ( uint16_t ) cnt);
        sleep_ms( 1000 );
    }

    //printf(" Keep the Probe submerged \r\n");

    sleep_ms(100);
    *wet_val = hydroprobe_min_val();

    //printf("--------------------\r\n");
    sleep_ms(5000);
}
