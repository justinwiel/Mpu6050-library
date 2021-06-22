#ifndef ENEMY_CPP
#define ENEMY_CPP
#include "hwlib.hpp"
#include "entities.hpp"
class enemy: public border  {
private:
   int8_t max_movement;
   int16_t goal = 0;
public:
    bool active =true;
    enemy(window & w,xy  start, xy  end, xy bounce, int8_t max_movement):
    border(w,start,end,bounce),
    max_movement(max_movement)
    {}
    void get_target(const sprite & ball);
    void move_toTarget();
    void check_next_pos(const border & top, const border & bottom);
};
#endif