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

#include "obstacle.hpp"
#include "../../lib/MPU.hpp"

using namespace dodge;

void Player::draw() {
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            if((y < eye_start.y) || (y > eye_end.y) || (x< eye_start.x ) || (x > eye_end.x)){
                w.write(xy(x,y));
            }

        }
    }
}


void obstacle::draw(){
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            w.write(xy(x,y));
        }
    }
}

void Player::change_pos_x(int8_t val ){
    start_prev.x = start.x;
    end_prev.x = end.x;
    start.x += val;
    end.x += val;
    eye_start.x += val;
    eye_end.x += val;
}

void Player::change_pos_y(int8_t val ){
    start_prev.y = start.y;
    end_prev.y = end.y;
    start.y += val;
    end.y += val;
    eye_start.y += val;
    eye_end.y += val;
}


void Player::update() {
    chip.setup(3);
    auto data = chip.getAccdata(10);
    if(data.z > data.x){
        change_pos_y(9);
    }else if(data.z < data.x && data.z < -4){
        change_pos_y(-9);
    }else if(data.y > 4){
        change_pos_x(-9);
    }else if(data.y < -4){
        change_pos_x(9);

    }
    check_next_pos();


}

void Player::check_next_pos(){
    while( end.y > 58 ){
        start.y -=1;
        end.y -= 1;
        eye_start.y -= 1;
        eye_end.y -= 1;
    }
    while(start.y < 5){
        start.y +=1 ;
        end.y += 1 ;
        eye_start.y += 1;
        eye_end.y += 1;
    }
    while( end.x > 126 ){
        start.x -=1;
        end.x -= 1;
        eye_start.x -= 1;
        eye_end.x -= 1;
    }
    while(start.x < 5){
        start.x +=1 ;
        end.x += 1 ;
        eye_start.x += 1;
        eye_end.x += 1;
    }
}

void Player::interact(sprite & other){
    if(this != & other){
        if(overlaps(other)){
            hit = true;
        }
        
    }
}

void obstacle::update() {
    start.x -= speed;
    end.x -= speed;
}

void obstacle::interact(sprite & other){
    
}

bool obstacle::within( int x, int a, int b ){
    return ( x >= a ) && ( x <= b );

}


bool Player::overlaps( sprite & other ){
    bool out_bounds = start.y < 0 || end.y > 64;
    bool x_overlap_start = within( 
        start.x ,
        other.start.x,
        other.end.x )
    || within( 
        other.start.x , 
        start.x, 
        end.x
   );
     
   bool y_overlap_start = within( 
        start.y,
        other.start.y , 
        other.end.y
    ) || within( 
        other.start.y , 
        start.y, 
        end.y
    ); 
    bool x_overlap_end = within( 
        end.x ,
        other.start.x,
        other.end.x )
    || within( 
        other.end.x , 
        start.x, 
        end.x
   );
     
   bool y_overlap_end = within( 
        end.y,
        other.start.y , 
        other.end.y
    ) || within( 
        other.end.y , 
        start.y, 
        end.y
    ); 
    bool x_overlap_start_speed = within( 
        start.x -1,
        other.start.x,
        other.end.x )
    || within( 
        other.start.x +1 , 
        start.x, 
        end.x
   );
     
    bool y_overlap_start_speed = within( 
        start.y -1 ,
        other.start.y , 
        other.end.y
    ) || within( 
      other.start.y +1 , 
      start.y, 
      end.y
   );
   bool x_overlap_end_speed = within( 
      end.x +1,  
      other.start.x,
      other.end.x )
    || within( 
      other.end.x - 1, 
      start.x, 
      end.x
   );
     
   bool y_overlap_end_speed = within( 
      end.y +1 , 
      other.start.y, 
      other.end.y
   ) || within( 
      other.end.y - 1 , 
      start.y, 
      end.y
   );
    return (x_overlap_start_speed && y_overlap_start_speed) || (x_overlap_end_speed && y_overlap_end_speed) || (x_overlap_start && y_overlap_start) || (x_overlap_end && y_overlap_end) || out_bounds;
}
void Player::gameOver(){
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w,f);
    w.clear();
    terminal << '\f' << "\n\n\n" << "    Game over" << hwlib::flush;

}

void Player::reset(){
    start = org_start;
    end = org_end;
    eye_start  = org_start_eye;
    eye_end = org_end_eye;
    hit = false;
}
void obstacle::reset(){
    start = org_start;
    end = org_end;
}


