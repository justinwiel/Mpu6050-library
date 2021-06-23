#ifndef SNAKE_HPP
#define SNAKE_HPP
#include "hwlib.hpp"
#include "../../lib/MPU.hpp"

using namespace hwlib;
class block {
protected:
    window & w;
public:
    xy start;
    xy end;
    xy bounce;
    block(window & w,xy  start, xy  end, xy bounce):
        w(w),
        start(start),
        end(end),
        bounce(bounce)
        {}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void interact(const block & other) = 0;

};

class head :public block {
private:
    xy eye;
public:
    head(window & w,xy  start, xy  end, xy bounce, xy eye):
        block(w,start,end ,bounce),
        eye(eye)
        {}
    void  draw();
    void update();
    l void interact(const block & other) ;
};
#endif
