#include "snake.hpp"


void head::draw() {
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            if(eye != xy(x,y)){
                w.write(xy(x,y));
            }

        }
    }
}