#ifndef MPU_HPP
#include "hwlib.hpp"
#define MPU_HPP

        #define  SELF_TEST_X_ACCEL   0x0D
        #define  SELF_TEST_Y_ACCEL   0x0E    
        #define  SELF_TEST_Z_ACCEL   0x0F
        #define  SELF_TEST_A         0x10
        #define  XG_OFFSET_H         0x13 
        #define  XG_OFFSET_L         0x14
        #define  YG_OFFSET_H         0x15
        #define  YG_OFFSET_L         0x16
        #define  ZG_OFFSET_H         0x17
        #define  ZG_OFFSET_L         0x18
        #define  SMPLRT_DIV          0x19
        #define  I2C_MST_EN          0x20
        #define  ACCEL_CONFIG2       0x1D
        #define  LP_ACCEL_ODR        0x1E
        #define  MOT_THR             0x1F   
        #define  MOT_DUR             0x20  
        #define  CONFIG              0x1A
        #define  GYRO_CONFIG         0x1B
        #define  ACCEL_CONFIG        0x1C
        #define  ZMOT_THR            0x21  
        #define  ZRMOT_DUR           0x22  
        #define  FIFO_EN             0x23
        #define  I2C_MST_CTRL        0x24
        #define  I2C_SLV0_ADDR       0x25
        #define  I2C_SLV0_REG        0x26
        #define  I2C_SLV0_CTRL       0x27
        #define  I2C_SLV1_ADDR       0x28
        #define  I2C_SLV1_REG        0x29
        #define  I2C_SLV1_CTRL       0x2A
        #define  I2C_SLV2_ADDR       0x2B
        #define  I2C_SLV2_REG        0x2C
        #define  I2C_SLV2_CTRL       0x2D
        #define  I2C_SLV3_ADDR       0x2E
        #define  I2C_SLV3_REG        0x2F
        #define  I2C_SLV3_CTRL       0x30
        #define  I2C_SLV4_ADDR       0x31
        #define  I2C_SLV4_REG        0x32
        #define  I2C_SLV4_DO         0x33
        #define  I2C_SLV4_CTRL       0x34
        #define  I2C_SLV4_DI         0x35
        #define  I2C_MST_STATUS      0x36
        #define  INT_PIN_CFG         0x37
        #define  INT_ENABLE          0x38
        #define  DMP_INT_STATUS      0x39  // Check DMP interrupt
        #define  INT_STATUS          0x3A
        #define  ACCEL_XOUT_H        0x3B
        #define  ACCEL_XOUT_L        0x3C
        #define  ACCEL_YOUT_H        0x3D
        #define  ACCEL_YOUT_L        0x3E
        #define  ACCEL_ZOUT_H        0x3F
        #define  ACCEL_ZOUT_L        0x40
        #define  TEMP_OUT_H          0x41
        #define  TEMP_OUT_L          0x42
        #define  GYRO_XOUT_H         0x43
        #define  GYRO_XOUT_L         0x44
        #define  GYRO_YOUT_H         0x45
        #define  GYRO_YOUT_L         0x46
        #define  GYRO_ZOUT_H         0x47
        #define  GYRO_ZOUT_L         0x48
        #define  EXT_SENS_DATA_00    0x49
        #define  EXT_SENS_DATA_01    0x4A
        #define  EXT_SENS_DATA_02    0x4B
        #define  EXT_SENS_DATA_03    0x4C
        #define  EXT_SENS_DATA_04    0x4D
        #define  EXT_SENS_DATA_05    0x4E
        #define  EXT_SENS_DATA_06    0x4F
        #define  EXT_SENS_DATA_07    0x50
        #define  EXT_SENS_DATA_08    0x51
        #define  EXT_SENS_DATA_09    0x52
        #define  EXT_SENS_DATA_10    0x53
        #define  EXT_SENS_DATA_11    0x54
        #define  EXT_SENS_DATA_12    0x55
        #define  EXT_SENS_DATA_13    0x56
        #define  EXT_SENS_DATA_14    0x57
        #define  EXT_SENS_DATA_15    0x58
        #define  EXT_SENS_DATA_16    0x59
        #define  EXT_SENS_DATA_17    0x5A
        #define  EXT_SENS_DATA_18    0x5B
        #define  EXT_SENS_DATA_19    0x5C
        #define  EXT_SENS_DATA_20    0x5D
        #define  EXT_SENS_DATA_21    0x5E
        #define  EXT_SENS_DATA_22    0x5F
        #define  EXT_SENS_DATA_23    0x60
        #define  MOT_DETECT_STATUS   0x61
        #define  I2C_SLV0_DO         0x63
        #define  I2C_SLV1_DO         0x64
        #define  I2C_SLV2_DO         0x65
        #define  I2C_SLV3_DO         0x66
        #define  I2C_MST_DELAY_CTRL  0x67
        #define  SIGNAL_PATH_RESET   0x68
        #define  MOT_DETECT_CTRL     0x69
        #define  USER_CTRL           0x6A  
        #define  PWR_MGMT_1          0x6B 
        #define  PWR_MGMT_2          0x6C
        #define  DMP_BANK            0x6D 
        #define  DMP_RW_PNT          0x6E
        #define  DMP_REG             0x6F
        #define  DMP_REG_1           0x70
        #define  DMP_REG_2           0x71
        #define  FIFO_COUNTH         0x72
        #define  FIFO_COUNTL         0x73
        #define  FIFO_R_W            0x74
        #define  WHO_AM_I            0x75 
        #define  XA_OFFSET_H         0x77
        #define  XA_OFFSET_L         0x78
        #define  YA_OFFSET_H         0x7A
        #define  YA_OFFSET_L         0x7B
        #define  ZA_OFFSET_H         0x7D
        #define  ZA_OFFSET_L         0x7E
        #define  I2C_SLV0_EN         0x80
        #define  I2C_READ_FLAG       0x80




class xyz{
public:
    int16_t x,y,z;
    xyz(int16_t x, int16_t y, int16_t z):
    x(x),
    y(y),
    z(z)
    {}
    xyz operator-(xyz & rhs);
    xyz operator-=(xyz & rhs);
    xyz operator+(xyz & rhs);
    xyz operator+=(xyz & rhs);
    xyz operator*(xyz & rhs);
    xyz operator*=(xyz & rhs);
    xyz operator*(int16_t & rhs);
    xyz operator*=(int16_t & rhs);

};

class all_values  {
public:
    xyz gyr;
    xyz acc;
    int temp;
    all_values(xyz gyr, xyz acc, int temp):
    gyr(gyr),
    acc(acc),
    temp(temp)
    {}

};


class MPU6050 {
protected: 
    int16_t  gyrosensitivity  = 131;   
    int16_t  accelsensitivity = 16384;
    bool interrupt_en =false;
    uint8_t   address;
    bool  A0;
    int16_t fs_range = 0;
    hwlib::i2c_bus_bit_banged_scl_sda & I2C_bus;
public:
    MPU6050(hwlib::i2c_bus_bit_banged_scl_sda & I2C_bus, bool  A0=0):
    A0(A0),
    I2C_bus(I2C_bus)
    { 
        address = 0x68 + (int)A0; //The setting of A0 sets the adrress to 0x68 when low or 0x69 when high
    } 
    void writeRegister(uint8_t sub_adrr, uint8_t  data);
    uint8_t* readRegister(uint8_t sub_addr, uint8_t* data, uint8_t size);
    void setup(uint8_t range_setting);
    xyz getAccdata(int desired_range);
    xyz getGyrodata(int desired_range);
    int16_t getTempdata();
    all_values getAlldata(int desired_range);
    xyz getAccdata_raw();
    xyz getGyrodata_raw();
    int16_t getTempdata_raw();
    all_values getAlldata_raw();
    int16_t calculateData_div();
    void test(hwlib::pin_in & switch_button);




};


#endif