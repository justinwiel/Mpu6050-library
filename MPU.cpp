#include "MPU.hpp"
#include "hwlib.hpp"



void MPU6050::setup(){
    uint8_t data[12] ;
    writeRegister(PWR_MGMT_1,0x80);
    hwlib::wait_ms(100);
    writeRegister(PWR_MGMT_1,0b001);
    writeRegister(PWR_MGMT_2,0x00);
    hwlib::wait_ms(200);
    writeRegister(INT_ENABLE,0x00);
    writeRegister(FIFO_EN,0x00);
    writeRegister(PWR_MGMT_1,0x00);
    writeRegister(I2C_MST_CTRL,0x00);
    writeRegister(USER_CTRL,0x00);
    writeRegister(USER_CTRL,0x0c);
    hwlib::wait_ms(15);
    writeRegister(CONFIG,0x01);
    writeRegister(SMPLRT_DIV, 7);
    writeRegister(GYRO_CONFIG,0b00010000);
    writeRegister(ACCEL_CONFIG, 0b00011000);
    writeRegister(USER_CTRL, 0x40);
    writeRegister(SIGNAL_PATH_RESET,0b00000111);
    writeRegister(SIGNAL_PATH_RESET,0b00000000);
    writeRegister(FIFO_EN,0x40);
    hwlib::wait_ms(40);
    writeRegister(FIFO_EN,0x00);
    readRegister(FIFO_COUNTH, data, 2);
    uint16_t count =((uint16_t)data[0] << 8) | data[1];
    uint16_t packet_count = count/12;
    int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
    for(int i = 0; i<packet_count; i++){
        int16_t gyro_temp[3]  = {0, 0, 0}, accel_temp[3] = {0, 0, 0};
        readRegister(ACCEL_XOUT_H,data,12);
        accel_temp[0] = (int16_t)(data[0] << 8) | data[1];
        accel_temp[0] = (int16_t)(data[2] << 8) | data[3];
        accel_temp[0] = (int16_t)(data[4] << 8) | data[5];
        gyro_temp[0] = (int16_t)(data[6] << 8) | data[7];
        gyro_temp[1] = (int16_t)(data[8] << 8) | data[9];
        gyro_temp[2] = (int16_t)(data[10] << 8) | data[11];

        accel_bias[0] += accel_temp[0];
        accel_bias[1] += accel_temp[1];
        accel_bias[2] += accel_temp[2];
        gyro_bias[0] += gyro_temp[0];
        gyro_bias[1] += gyro_temp[1];
        gyro_bias[2] += gyro_temp[2];
    }
    accel_bias[0] /= (int32_t)packet_count;
    accel_bias[1] /= (int32_t)packet_count;
    accel_bias[2] /= (int32_t)packet_count;
    gyro_bias[0] /= (int32_t)packet_count;
    gyro_bias[1] /= (int32_t)packet_count;
    gyro_bias[2] /= (int32_t)packet_count;

    if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) MPU6050::accelsensitivity;}  
    else {accel_bias[2] += (int32_t) MPU6050::accelsensitivity;}

    int8_t data_sign[6];
    data_sign[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; //X HIGH BYTE 
    data_sign[1] = (-gyro_bias[0]/4)       & 0xFF; //X LOW BYTE 
    data_sign[2] = (-gyro_bias[1]/4  >> 8) & 0xFF; //Y HIGH BYTE 
    data_sign[3] = (-gyro_bias[1]/4)       & 0xFF; // Y LOW BYTE 
    data_sign[4] = (-gyro_bias[2]/4  >> 8) & 0xFF; // Z HIGH BYTE 
    data_sign[5] = (-gyro_bias[2]/4)       & 0xFF; // Z LOW BYTE

    writeGyroOffset(data_sign);
}


void MPU6050::writeGyroOffset(int8_t data[6]){
    writeRegister(XG_OFFSET_H,data[0]);
    writeRegister(XG_OFFSET_L,data[1]);
    writeRegister(YG_OFFSET_H,data[2]);
    writeRegister(YG_OFFSET_L,data[3]);
    writeRegister(ZG_OFFSET_H,data[4]);
    writeRegister(ZG_OFFSET_L,data[5]);
}
void MPU6050::writeRegister(uint8_t sub_adrr, uint8_t  data){
    auto wbus = hwlib::i2c_write_transaction (MPU6050::I2C_bus, MPU6050::address);
    wbus.write(sub_adrr) ;
    wbus.write(data);
    wbus.~i2c_write_transaction();
}

uint8_t* MPU6050::readRegister(uint8_t sub_addr, uint8_t* data, uint8_t size){
    auto wbus = hwlib::i2c_write_transaction (MPU6050::I2C_bus, MPU6050::address);
    wbus.write(sub_addr) ;
    wbus.~i2c_write_transaction();
    auto rbus = hwlib::i2c_read_transaction (MPU6050::I2C_bus, 0x68);
    rbus.read(data,size);
    rbus.~i2c_read_transaction();
    return data;
}

void MPU6050::interrupt_enable(){
    writeRegister(INT_ENABLE,0b00000001);
    MPU6050::inerrupt_en = true;
}

void MPU6050::interrupt_disable(){
    writeRegister(INT_ENABLE,0x00);
    MPU6050::inerrupt_en = false;
}

void MPU6050::wait_4_data(hwlib::pin_in & int_pin){
    if(MPU6050::inerrupt_en){
        while(!int_pin.read()){

        }
    }

}


xyz MPU6050::getAccdata(){
    uint8_t data[6];
    readRegister(ACCEL_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz(x,y,z);
}
xyz MPU6050::getGyrodata(){
    uint8_t data[6];
    readRegister(GYRO_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz(x,y,z);
}
int16_t MPU6050::getTempdata(){
    uint8_t data[2];
    readRegister(TEMP_OUT_H,data,2);
    int16_t temp = (data[0] << 8) | data[1];
    return temp/340+36.53;
}