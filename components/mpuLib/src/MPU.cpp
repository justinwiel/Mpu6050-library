// Boost Software License - Version 1.0 - August 17th, 2003

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:

// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//-------------------------------
// author: Justin van der Wiel 2023
//--------------------------------

#include "MPU.hpp"
#include "driver/i2c.h"
#include "esp_log.h"

xyz xyz::operator-(xyz &rhs)
{
    return xyz(x - rhs.x, y - rhs.y, z - rhs.z);
}

xyz xyz::operator-=(xyz &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}
xyz xyz::operator+(xyz &rhs)
{
    return xyz(x + rhs.x, y + rhs.y, z + rhs.z);
}
xyz xyz::operator+=(xyz &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}
xyz xyz::operator*(xyz &rhs)
{
    return xyz(x * rhs.x, y * rhs.y, z * rhs.z);
}
xyz xyz::operator*=(xyz &rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}
xyz xyz::operator*(int16_t &rhs)
{
    return xyz(x * rhs, y * rhs, z * rhs);
}
xyz xyz::operator*=(int16_t &rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

void MPU6050::setup(const MPU6050::sensitiviy &range_setting)
{
    uint8_t result;
    readRegister(WHO_AM_I, &result, 1);
    ESP_LOGI("MPU", "WHO_AM_I: %X", result);
    switch (range_setting)
    {
    case MPU6050::sensitiviy::high:
        fs_range = 250;
        gyrosensitivity = 131;    // lsb per second  sensitivity values from: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf part 6.1 and 6.2
        accelsensitivity = 16384; // lsb per second
        break;
    case MPU6050::sensitiviy::medium:
        fs_range = 500;
        gyrosensitivity = 65.5;  // lsb per second
        accelsensitivity = 8192; // lsb per second
        break;
    case MPU6050::sensitiviy::low:
        fs_range = 1000;
        gyrosensitivity = 32.8;  // lsb per second
        accelsensitivity = 4096; // lsb per second
        break;
    case MPU6050::sensitiviy::fullRange:
        fs_range = 2000;
        gyrosensitivity = 16.4;  // lsb per second
        accelsensitivity = 2048; // lsb per second
        break;
    default:
        fs_range = 2000;
        gyrosensitivity = 16.4;
        accelsensitivity = 2048;
    }
    auto to_write = (int(range_setting) << 3); // first three bytes are ignored as such the value needs to be shifted 3 before being written
    writeRegister(PWR_MGMT_1, 0x80);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    writeRegister(PWR_MGMT_1, 0b0001);
    writeRegister(PWR_MGMT_2, 0x00);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    writeRegister(INT_ENABLE, 0x00);
    writeRegister(FIFO_EN, 0x00);
    writeRegister(PWR_MGMT_1, 0x00);
    writeRegister(I2C_MST_CTRL, 0x00);
    writeRegister(USER_CTRL, 0x00);
    writeRegister(USER_CTRL, 0x0c);
    vTaskDelay(15 / portTICK_PERIOD_MS);
    writeRegister(CONFIG, 0b00000001);
    writeRegister(SMPLRT_DIV, 0);
    writeRegister(GYRO_CONFIG, to_write);
    writeRegister(ACCEL_CONFIG, to_write);
    writeRegister(USER_CTRL, 0x40);
    writeRegister(SIGNAL_PATH_RESET, 0b00000111);
    writeRegister(SIGNAL_PATH_RESET, 0b00000000);
    writeRegister(GYRO_CONFIG, 0b00011000);
    writeRegister(ACCEL_CONFIG, 0b00011000);
    writeRegister(INT_PIN_CFG, 0b00100010);
}

void MPU6050::writeRegister(uint8_t sub_addr, uint8_t data)
{
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    ESP_LOGI("MPU", "Writing to register %X", sub_addr);
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(handle, sub_addr, true);
    i2c_master_write_byte(handle, data, true);
    i2c_master_stop(handle);
    i2c_master_cmd_begin(I2C_NUM_0, handle, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(handle);
}

uint8_t *MPU6050::readRegister(uint8_t sub_addr, uint8_t *data, uint8_t size)
{

    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(handle, sub_addr, true);
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (address << 1) | I2C_MASTER_READ, true);
    if (size > 1)
    {
        i2c_master_read(handle, data, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(handle, data + size - 1, I2C_MASTER_NACK);
    i2c_master_stop(handle);
    i2c_master_cmd_begin(I2C_NUM_0, handle, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(handle);
    return data;
}

xyz MPU6050::getAccdata_scale(int desired_range)
{
    uint8_t data[6];
    readRegister(ACCEL_XOUT_H, data, 6);  // read acclerometer data registers
    int16_t x = (data[0] << 8) | data[1]; // create 16 bit signed integers from your read 8 bit unsigned integers
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz((x / (fs_range / desired_range)) % (desired_range + 1), (y / (fs_range / desired_range)) % (desired_range + 1), (z / (fs_range / desired_range)) % (desired_range + 1));
}
xyz MPU6050::getGyrodata_scale(int desired_range)
{
    uint8_t data[6];
    readRegister(GYRO_XOUT_H, data, 6);   // read gyroscope data registers
    int16_t x = (data[0] << 8) | data[1]; // create 16 bit signed integers from your read 8 bit unsigned integers
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz((x / (fs_range / desired_range)) % (desired_range + 1), (y / (fs_range / desired_range)) % (desired_range + 1), (z / (fs_range / desired_range)) % (desired_range + 1));
}

void MPU6050::getAccdata_scale(const int &desired_range, xyz *acc)
{
    uint8_t data[6];
    uint16_t range_local = getfs_range();
    readRegister(ACCEL_XOUT_H, data, 6);  // read acclerometer data registers
    int16_t x = (data[0] << 8) | data[1]; // create 16 bit signed integers from the read 8 bit unsigned integers
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    acc->x = (x/ (range_local / desired_range)) % (desired_range + 1);
    acc->y = (y / (range_local / desired_range)) % (desired_range + 1);
    acc->z = (z / (range_local / desired_range)) % (desired_range + 1);
}
void MPU6050::getGyrodata_scale(const int &desired_range, xyz *gyr)
{
    uint8_t data[6];
    readRegister(GYRO_XOUT_H, data, 6); // read gyroscope data registers
    uint16_t range_local = getfs_range();
    int16_t x = (data[0] << 8) | data[1]; // create 16 bit signed integers from the read 8 bit unsigned integers
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    gyr->x = (x/ (range_local / desired_range)) % (desired_range + 1);
    gyr->y = (y / (range_local / desired_range)) % (desired_range + 1);
    gyr->z = (z / (range_local / desired_range)) % (desired_range + 1);
}

int16_t MPU6050::getTempdata()
{
    uint8_t data[2];
    readRegister(TEMP_OUT_H, data, 2);
    int16_t temp = (data[0] << 8) | data[1];

    return temp / 340 + 36.53;
}

all_values MPU6050::getAlldata_scale(int desired_range)
{
    return all_values(getAccdata_scale(desired_range), getGyrodata_scale(desired_range), getTempdata());
}

xyz MPU6050::getAccdata()
{
    uint8_t data[6];
    readRegister(ACCEL_XOUT_H, data, 6);
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz(x / accelsensitivity, y / accelsensitivity, z / accelsensitivity);
}

uint16_t fromTwosComplemntToInt(uint16_t data)
{
    if (data & 0x8000)
    {
        return (data ^ 0xFFFF) + 1;
    }
    return data;
}

void MPU6050::getAccdata(xyz *acc)
{
    uint8_t data[6];
    readRegister(ACCEL_XOUT_H, data, 6);
    int16_t x = ((data[0] << 8) | data[1]);
    int16_t y = ((data[2] << 8) | data[3]);
    int16_t z = ((data[4] << 8) | data[5]);
    acc->x = fromTwosComplemntToInt(x);
    acc->y = fromTwosComplemntToInt(y);
    acc->z = fromTwosComplemntToInt(z);
}
xyz MPU6050::getGyrodata()
{
    uint8_t data[6];
    readRegister(GYRO_XOUT_H, data, 6);
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz(x / gyrosensitivity, y / gyrosensitivity, z / gyrosensitivity);
}

void MPU6050::getGyrodata(xyz *gyr)
{
    uint8_t data[6];
    readRegister(GYRO_XOUT_H, data, 6);
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    gyr->x = x;
    gyr->y = y;
    gyr->z = z;
}

all_values MPU6050::getAlldata()
{
    return all_values(getAccdata(), getGyrodata(), getTempdata()); // get all values and put them in an all_values
}

xyz MPU6050::getAccdata_raw()
{
    uint8_t data[6];
    readRegister(ACCEL_XOUT_H, data, 6);
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz(x, y, z);
}
xyz MPU6050::getGyrodata_raw()
{
    uint8_t data[6];
    readRegister(GYRO_XOUT_H, data, 6);
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];
    return xyz(x, y, z);
}
int16_t MPU6050::getTempdata_raw()
{
    uint8_t data[2];
    readRegister(TEMP_OUT_H, data, 2);
    int16_t temp = (data[0] << 8) | data[1];

    return temp;
}

all_values MPU6050::getAlldata_raw()
{
    return all_values(getAccdata_raw(), getGyrodata_raw(), getTempdata_raw());
}

void MPU6050::interrupt_enable()
{
    writeRegister(INT_PIN_CFG, 0b00100001);
    writeRegister(INT_ENABLE, 0b00010001); // all interrupts except i2c master enabled
}
void MPU6050::interrupt_disable()
{
    writeRegister(INT_PIN_CFG, 0b00000001);
    writeRegister(INT_ENABLE, 0b0000000); // all interrupts disabled
}

void MPU6050::read_interrupt(uint8_t data[1])
{
    readRegister(INT_STATUS, data, 1);
}

void MPU6050::fifo_enable()
{
    writeRegister(FIFO_EN, 0b11111000); // set accelerometer and gyroscope fifo_en flags to 1
}

void MPU6050::fifo_disable()
{
    writeRegister(FIFO_EN, 0b00000000); // set accelerometer and gyroscope fifo_en flags to 0
}

all_values MPU6050::fifo_read()
{
    fifo_enable(); // open fifo
    vTaskDelay(50 / portTICK_PERIOD_MS);
    uint8_t count[2];
    readRegister(FIFO_COUNTH, count, 2); // read the amount of bytes in fifo buffer
    int16_t packetcount = (count[0] << 8) | count[1];
    int16_t data[7];
    if (packetcount > 0) // don't read if it's empty
    {
        uint8_t temp[14];
        readRegister(FIFO_R_W, temp, 14);
        data[0] = (temp[0] << 8) | temp[1];
        data[1] = (temp[2] << 8) | temp[3];
        data[2] = (temp[4] << 8) | temp[5];
        data[3] = (temp[6] << 8) | temp[7];
        data[4] = (temp[8] << 8) | temp[9];
        data[5] = (temp[10] << 8) | temp[11];
        data[6] = (temp[12] << 8) | temp[13];
    }
    fifo_disable(); // close to prevent overflow
    return all_values(xyz(data[0] / accelsensitivity, data[1] / accelsensitivity, data[2] / accelsensitivity), xyz(data[4] / gyrosensitivity, data[5] / gyrosensitivity, data[6] / gyrosensitivity), data[3] / 340 + 36.53);
}
all_values MPU6050::fifo_read_test()
{
    fifo_enable(); // open
    vTaskDelay(20 / portTICK_PERIOD_MS);
    uint8_t count[2];
    readRegister(FIFO_COUNTH, count, 2); // read the amount of bytes in fifo buffer
    int16_t packetcount = (count[0] << 8) | count[1];
    int16_t data[7];
    if (packetcount > 0) // don't read if it's empty
    {
        uint8_t temp[14];
        readRegister(FIFO_R_W, temp, 14);
        data[0] = (temp[0] << 8) | temp[1];
        data[1] = (temp[2] << 8) | temp[3];
        data[2] = (temp[4] << 8) | temp[5];
        data[3] = (temp[6] << 8) | temp[7];
        data[4] = (temp[8] << 8) | temp[9];
        data[5] = (temp[10] << 8) | temp[11];
        data[6] = (temp[12] << 8) | temp[13];
    }
    // don't close to test oveflow interrupt
    return all_values(xyz(data[0] / accelsensitivity, data[1] / accelsensitivity, data[2] / accelsensitivity), xyz(data[4] / gyrosensitivity, data[5] / gyrosensitivity, data[6] / gyrosensitivity), data[3] / 340 + 36.53);
}

void MPU6050::fifo_reset()
{
    writeRegister(USER_CTRL, 0b00000100);
}

esp_err_t MPU6050::InitI2C()
{

    return ESP_OK;
}

int16_t MPU6050::getfs_range()
{
    return fs_range;
}


void MPU6050::start_gyro(){
    if(accON){
        writeRegister(PWR_MGMT_2,0x3F);
    }else{
        writeRegister(PWR_MGMT_2,0x7);
    }
}


void MPU6050::stop_gyro(){
    if(accON){
        writeRegister(PWR_MGMT_2,0x38);
    }else{
        writeRegister(PWR_MGMT_2,0x00);
    }
}

void MPU6050::start_acc(){
    if(gyrOn){
        writeRegister(PWR_MGMT_2,0x3F);
    }else{
        writeRegister(PWR_MGMT_2,0x38);
    }
}


void MPU6050::stop_acc(){
    if(gyrOn){
        writeRegister(PWR_MGMT_2,0x7);
    }else{
        writeRegister(PWR_MGMT_2,0x00);
    }
}


void MPU6050::setA0(bool A0){
    MPU6050::A0 = A0;
}