#include "hwlib.hpp"
#include <array>
#include "enemy.hpp"
#include "entities.hpp"
#include "src/dodge/dodge.hpp"


void play_pong(){
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
    auto oled = hwlib::glcd_oled( i2c_bus, 0x3c ); 
    auto top = border(oled, hwlib::xy(0,0), hwlib::xy(128,0),hwlib::xy(1,-1));
    auto bottom = border(oled, hwlib::xy(0,oled.size.y-1), hwlib::xy(128,oled.size.y-1),hwlib::xy(1,-1));
    auto player1 = deathwall(oled,hwlib::xy(1,1),hwlib::xy(1,62),1);
    auto player2 = deathwall(oled,hwlib::xy(127,1),hwlib::xy(127,62),2);
    auto ai = enemy(oled, hwlib::xy(120,18),hwlib::xy(123,36),hwlib::xy(-1,1),2);
    ball pong(oled,hwlib::xy(60,28), hwlib::xy(64,32), hwlib::xy(4,5));
    std::array< sprite *, 6 > objects = { &pong, &top, &bottom, &player1, &player2, &ai };
    while(true){
      while(player1.clear && player2.clear){
        oled.clear();
        for( auto & p : objects ){
            p->draw();
        }
        oled.flush();
        //hwlib::wait_ms( 50 );
        for( auto & p : objects ){
            p->update();
      }
      ai.get_target(pong);
      ai.move_toTarget();
      ai.check_next_pos(top,bottom);
        for( auto & p : objects ){
          for( auto & other : objects ){
            if(p!=other){
              p->interact( *other );
          }

        }
      }   
    }
    hwlib::wait_ms(1000);
    pong.start = hwlib::xy(60,28);
    pong.end = hwlib::xy(64,32);
    pong.speed = hwlib::xy(4,5);
    player1.clear = true;
    player2.clear = true;
  }

}

int main(){
}

