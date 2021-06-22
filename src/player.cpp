#include "hwlib.hpp"
#include "player.hpp"
#include "entities.hpp" 
#include "../lib/MPU.hpp"


void Player::get_movement(){
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto chip = MPU6050(i2c_bus,0);
    chip.setup(3);
    auto data = chip.getAccdata(5) ;
    start.y += data.z * 2;
    end.y += data.z  *2;
}

void Player::check_next_pos(const border & top, const border & bottom){
    while( end.y > 58 ){
        start.y -=1;
        end.y -= 1;
    }
    while(start.y < 5){
        start.y +=1 ;
        end.y += 1 ;
    }
}