#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "hwlib.hpp"
#include "entities.hpp" 
#include "../lib/MPU.hpp"
using namespace hwlib;
class Player :public border{
public:
    Player(window & w,xy  start, xy  end, xy bounce ):
        border(w,start,end,bounce)
        {}
    void get_movement();
    void check_next_pos(const border & top, const border & bottom);
};
#endif