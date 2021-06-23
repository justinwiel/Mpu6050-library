#include "hwlib.hpp"
#include "entities.hpp"
#include "enemy.hpp"
#include <random>



void enemy::get_target(const sprite & ball){
    goal =  (ball.end.y - (ball.end.y - ball.start.y)/2);
}

void enemy::move_toTarget(){
    int16_t  diff = goal - (end.y - (int16_t(end.y - start.y)/2));
    std::srand(start.x*start.y);
    int8_t random = std::rand() % 2;
    if(diff > max_movement + random){
        diff = max_movement + random;
        start.y += diff ;
        end.y += diff ;
        return;
    }else if(diff < ((max_movement +random) * -1)){
        diff = ((max_movement +random) * -1);
        start.y += diff  ;
        end.y += diff ;
        return;
    }
    start.y += diff + random;
    end.y += diff + random;

}

void enemy::check_next_pos(const border & top, const border & bottom){
    while( end.y > 58 ){
        start.y -=1;
        end.y -= 1;
    }
    while(start.y < 5){
        start.y +=1;
        end.y += 1;
    }
}