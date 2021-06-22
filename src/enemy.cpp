#include "hwlib.hpp"
#include "entities.hpp"
#include "enemy.hpp"



void enemy::get_target(const sprite & ball){
    goal =  ball.end.y - (ball.end.y - ball.start.y);
}

void enemy::move_toTarget(){
    int16_t  diff = goal - (end.y - int16_t(end.y - start.y));
    if(diff > max_movement){
        diff = max_movement;
        start.y += diff +2;
        end.y += diff +2;
        return;
    }else if(diff < (max_movement * -1)){
        diff = (max_movement * -1);
        start.y += diff -2 ;
        end.y += diff -2;
        return;
    }
    start.y += diff;
    end.y += diff;

}

void enemy::check_next_pos(const border & top, const border & bottom){
    while(overlaps(bottom) || end.y > 58 ){
        start.y -=1;
        end.y -= 1;
    }
    while(overlaps(top)|| start.y < 5){
        start.y +=1;
        end.y += 1;
    }
}
