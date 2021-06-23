#include "hwlib.hpp"
#include <array>
#include "src/pong/enemy.hpp"
#include "src/pong/entities.hpp"
#include "lib/MPU.hpp"
#include "src/pong/player.hpp"

int main(){
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto button = hwlib::target::pin_in( hwlib::target::pins::d10 );
    auto button2 = hwlib::target::pin_in( hwlib::target::pins::d11 );
    auto oled = hwlib::glcd_oled( i2c_bus, 0x3c ); 
    auto top = border(oled, hwlib::xy(0,0), hwlib::xy(128,0),hwlib::xy(1,-1));
    auto bottom = border(oled, hwlib::xy(0,oled.size.y-1), hwlib::xy(128,oled.size.y-1),hwlib::xy(1,-1));
    auto goal1 = deathwall(oled,hwlib::xy(1,1),hwlib::xy(1,62),1);
    auto goal2 = deathwall(oled,hwlib::xy(127,1),hwlib::xy(127,62),2);
    auto ai = enemy(oled, hwlib::xy(117,18),hwlib::xy(122,36),hwlib::xy(-1,1),1);
    auto me = Player(oled, hwlib::xy(6,18),hwlib::xy(10,36),hwlib::xy(-1,1));
    ball pong(oled,hwlib::xy(60,28), hwlib::xy(64,32), hwlib::xy(4,5));
    std::array< sprite *, 7 > objects = { &pong, &top, &bottom, &goal1, &goal2, &ai, &me };
    auto chip = MPU6050(i2c_bus, 0);
    for(;;){
      while(goal1.clear && goal2.clear){
        if(button.read()){
          oled.clear();
          for(;;){
            auto f1 = hwlib::font_default_8x8();
            auto d1 = hwlib::terminal_from(oled,f1);
            auto all_data = chip.test(button);
            if(button2.read()){
              break;
            }
            d1 <<  '\f' << "acc_x: " << all_data.acc.x  << "\nacc_y: " << all_data.acc.y
                << "\nacc_z: " << all_data.acc.z << "\ntemp: " << all_data.temp << "\ngyro_x: " << all_data.gyr.x << 
                "\ngyro_y: " << all_data.gyr.y << "\ngyro_z: " << all_data.gyr.z <<hwlib::flush;
        }
          }

        oled.clear();
        for( auto & p : objects ){
            p->draw();
        }
        oled.flush();
        for( auto & p : objects ){
            p->update();
      }
      ai.get_target(pong);
      ai.move_toTarget();
      ai.check_next_pos(top,bottom);
      me.get_movement();
      me.check_next_pos(top,bottom);
      pong.check_next_pos(ai);
        for( auto & p : objects ){
          for( auto & other : objects ){
            if(p!=other){
              p->interact( *other );
          }

        }
      } 
    }
    hwlib::wait_ms(2000);
    pong.start = hwlib::xy(60,28);
    pong.end = hwlib::xy(64,32);
    pong.speed = hwlib::xy(4,5);
    goal1.clear = true;
    goal2.clear = true;
  }

}