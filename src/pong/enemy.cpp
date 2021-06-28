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

#include "hwlib.hpp"
#include "entities.hpp"
#include "enemy.hpp"
#include <random>



void enemy::get_target(const sprite & ball){
    goal =  (ball.end.y - (ball.end.y - ball.start.y)/2);
}

void enemy::move_toTarget(){
    int16_t  diff = goal - (end.y - (int16_t(end.y - start.y)/2));
    std::srand(hwlib::now_ticks());
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
