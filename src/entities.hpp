#ifndef BAL_HPP
#define BAL_HPP
#include "hwlib.hpp"
using namespace hwlib;



class sprite {
protected:
    window & w;
public:
    xy start;
    xy end;
    xy bounce;
    sprite(window & w,xy  start, xy  end, xy bounce):
        w(w),
        start(start),
        end(end),
        bounce(bounce)
        {}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void interact(const sprite & other);

};

class ball :public sprite{
protected:

public:
    xy speed;
    ball(window & w,xy  start, xy  end, xy  speed, xy bounce= xy(0,0)):
        sprite(w,  start,  end, bounce),
        speed(speed)
        {}
    void draw() ;
    void update() ;
    bool overlaps(const sprite & other);
    void interact(const sprite & other) ;

    
    
};

class border :public sprite{
public:
    border(window & w,xy  start, xy  end, xy bounce):
        sprite(w,  start,  end, bounce)
        {}
    void draw() ;
    void update() ;
    bool overlaps(const sprite & other);
    virtual void interact(const sprite & other) ;

};

class deathwall :public border{
private:
    int player;
public:
    bool clear = true;
    deathwall(window & w, xy start, xy end, int player):
        border(w,start,end,xy(0,0)),
        player(player)
        {}
    void interact(const sprite & other) ;
    void gameOver() ;
    void gameWon();

    
};
#endif