#include "entities.hpp"




bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

bool ball::overlaps( const sprite & other ){
   
   bool x_overlap = within( 
      start.x + speed.x,
      other.start.x,
      other.end.x )
    || within( 
      other.start.x, 
      start.x, 
      end.x
   );
     
   bool y_overlap = within( 
      start.y + speed.y,
      other.start.y, 
      other.end.y
   ) || within( 
      other.start.y, 
      start.y, 
      end.y
   );
   bool x_overlap_end = within( 
      end.x,  
      other.start.x,
      other.end.x )
    || within( 
      other.end.x, 
      start.x, 
      end.x
   );
     
   bool y_overlap_end = within( 
      end.y, 
      other.start.y, 
      other.end.y
   ) || within( 
      other.end.y, 
      start.y, 
      end.y
   );
   return (x_overlap && y_overlap) || (x_overlap_end && y_overlap_end);
}

void ball::draw() {
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            w.write(xy(x,y));
        }
    }
}

void ball::update() {
    start = start + speed;
    end = end + speed;
}

void ball::interact(const sprite & other){
    if(this != & other){
        if(overlaps(other)){
            speed.x = speed.x * other.bounce.x;
            speed.y = speed.y * other.bounce.y;
        }
    }


}

void border::draw(){
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            w.write(xy(x,y));
        }
    }
}

void border::update(){

}

void border::interact(const sprite & other){
    
}


bool border::overlaps( const sprite & other ){
   
   bool x_overlap = within( 
      start.x ,
      other.start.x,
      other.end.x )
    || within( 
      other.start.x, 
      start.x, 
      end.x
   );
     
   bool y_overlap = within( 
      start.y ,
      other.start.y, 
      other.end.y
   ) || within( 
      other.start.y, 
      start.y, 
      end.y
   );
   bool x_overlap_end = within( 
      end.x,  
      other.start.x,
      other.end.x )
    || within( 
      other.end.x, 
      start.x, 
      end.x
   );
     
   bool y_overlap_end = within( 
      end.y, 
      other.start.y, 
      other.end.y
   ) || within( 
      other.end.y, 
      start.y, 
      end.y
   );
   return (x_overlap && y_overlap) || (x_overlap_end && y_overlap_end);
}
void deathwall::gameOver(){
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w,f);
    w.clear();
    terminal << '\f' << "\n\n\n" << "    Game over" << hwlib::flush;
    clear = false;

}

void deathwall::gameWon(){
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w,f);
    w.clear();
    terminal << '\f' << "\n\n\n" << "     You Win!" << hwlib::flush;
    clear = false;   
}
void deathwall::interact(const sprite & other){
    if(this != & other){
        if(overlaps(other)&&player == 1){
            gameOver();

        }else if(overlaps(other)&&player ==2){
            gameWon();
        }
    }


}