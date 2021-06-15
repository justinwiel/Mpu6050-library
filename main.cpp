#include "hwlib.hpp"
#include "MPU.hpp"
#include <sstream>



void calibrate(){
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl,sda);
    auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
    auto f1 = hwlib::font_default_8x8();
    // uint8_t data[14] ;
    MPU6050 gyro(i2c_bus, 0);
    gyro.writeRegister(PWR_MGMT_1,0x80);
    hwlib::wait_ms(100);
    gyro.writeRegister(PWR_MGMT_1,0b001);
    gyro.writeRegister(PWR_MGMT_2,0x00);
    hwlib::wait_ms(200);
    gyro.writeRegister(INT_ENABLE,0x00);
    gyro.writeRegister(FIFO_EN,0x00);
    gyro.writeRegister(PWR_MGMT_1,0x00);
    gyro.writeRegister(I2C_MST_CTRL,0x00);
    gyro.writeRegister(USER_CTRL,0x00);
    gyro.writeRegister(USER_CTRL,0x0c);
    hwlib::wait_ms(15);
    gyro.writeRegister(CONFIG,0x01);
    gyro.writeRegister(SMPLRT_DIV, 7);
    gyro.writeRegister(GYRO_CONFIG,0b00011000);
    gyro.writeRegister(ACCEL_CONFIG, 0b00011000);
    gyro.writeRegister(USER_CTRL, 0x40);
    // gyro.writeRegister(FIFO_EN, 0x78);
    // hwlib::wait_ms(40);
    // uint8_t data[12];
    // gyro.writeRegister(FIFO_EN, 0x00);
    // gyro.readRegister(FIFO_COUNTH, data,2);
    // uint16_t fifo_count = ((uint16_t)data[0] << 8) | data[1];
    // uint16_t packet_count = fifo_count/12;
    // int16_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

    // for(int i = 0; i< packet_count;i++){
    //     int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    //     gyro.readRegister(FIFO_R_W,data,12);
    //     accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  
    //     accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    //     accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
    //     gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    //     gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    //     gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    //     accel_bias[0] += (int16_t) accel_temp[0]; 
    //     accel_bias[1] += (int16_t) accel_temp[1];
    //     accel_bias[2] += (int16_t) accel_temp[2];
    //     gyro_bias[0]  += (int16_t) gyro_temp[0];
    //     gyro_bias[1]  += (int16_t) gyro_temp[1];
    //     gyro_bias[2]  += (int16_t) gyro_temp[2];
    // }
    // accel_bias[0] /= (int16_t) packet_count; // Normalize sums to get average count biases
    // accel_bias[1] /= (int16_t) packet_count;
    // accel_bias[2] /= (int16_t) packet_count;
    // gyro_bias[0]  /= (int16_t) packet_count;
    // gyro_bias[1]  /= (int16_t) packet_count;
    // gyro_bias[2]  /= (int16_t) packet_count;




}
int main(){
    // calibrate();
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl,sda);
    uint8_t data[14] ;
    auto wbus = hwlib::i2c_write_transaction (i2c_bus, 0x68);
    wbus.write(0x6b) ;
    wbus.write(0);
    wbus.~i2c_write_transaction();
    auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
    auto f1 = hwlib::font_default_8x8();
    auto d1 = hwlib::terminal_from(oled,f1);
    MPU6050 gyro(i2c_bus,false);
    gyro.setup();
    // hwlib::wait_ms(10000);
    gyro.writeRegister(PWR_MGMT_1, 0x00);
    for(;;){
        gyro.writeRegister(PWR_MGMT_1, 0x00);
        gyro.readRegister(0x3b,data,14);
        auto int_pin = hwlib::target::pin_in(hwlib::target::pins::d10);
        auto acc = gyro.getAccdata();
        auto gyr = gyro.getGyrodata();
        auto temp = gyro.getTempdata();
        gyro.interrupt_enable();
        gyro.wait_4_data(int_pin);
        gyro.interrupt_disable();


        d1 <<  '\f' << "acc_x: " << acc.x  << "\nacc_y: " << acc.y
        << "\nacc_z: " << acc.z << "\ntemp: " << temp << "\ngyro_x: " << gyr.x << 
        "\ngyro_y: " << gyr.y << "\ngyro_z: " << gyr.z <<hwlib::flush;
        hwlib::wait_ms(10);
    }

 	
}