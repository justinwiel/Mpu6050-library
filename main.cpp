#include "hwlib.hpp"
#include "MPU.hpp"
#include <string>



int main(){
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl,sda);
    MPU6050 gyro(i2c_bus,false);
    auto button = hwlib::target::pin_in(hwlib::target::pins::d10);
    auto button2 = hwlib::target::pin_in(hwlib::target::pins::d12);

    
    gyro.test(button);
}