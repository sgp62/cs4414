#include <iostream>
#include <unistd.h>
#include "displayobject.hpp"
#include <thread>
#include <chrono>
#include <string>
#include <shared_mutex>
using namespace std::chrono_literals;
//
//  "Background" objects are in layer 0: barn, nest, bakery.
//  Layer 1 objects:  farmer, child
//  Layer 2 objects:  chicken, bags of flour, sugar, box of butter, crate of eggs, cupcakes, cake batter
//  Layer 3 objects:  cow, truck, stuff in the mixer
//
DisplayObject nest1[4] = {DisplayObject("\
\\       /#\
 -------", 0),

DisplayObject("\
\\   O   /#\
 -------", 0),

DisplayObject("\
\\  OO   /#\
 -------", 0),

DisplayObject("\
\\  OOO  /#\
 -------", 0),
};

DisplayObject nest2[4] = {DisplayObject("\
\\       /#\
 -------", 0),

DisplayObject("\
\\   O   /#\
 -------", 0),

DisplayObject("\
\\  OO   /#\
 -------", 0),

DisplayObject("\
\\  OOO  /#\
 -------", 0),
};

DisplayObject nest3[4] = {DisplayObject("\
\\       /#\
 -------", 0),

DisplayObject("\
\\   O   /#\
 -------", 0),

DisplayObject("\
\\  OO   /#\
 -------", 0),

DisplayObject("\
\\  OOO  /#\
 -------", 0),
};

DisplayObject chicken1("\
     O>#\
   ^( )#\
    = =", 2);

DisplayObject chicken2("\
     O>#\
   ^( )#\
    = =", 2);

DisplayObject chicken3("\
     O>#\
   ^( )#\
    = =", 2);

DisplayObject cow("\
   ____ \"#\
 /(____)U#\
  !! !!", 3);

DisplayObject farmer("\
   _#\
  O#\
/(~)\\#\
 ! !", 1);

DisplayObject child1("\
  *#\
 /()\\#\
  !!", 1);

  DisplayObject child2("\
  *#\
 /()\\#\
  !!", 1);

  DisplayObject child3("\
  *#\
 /()\\#\
  !!", 1);

  DisplayObject child4("\
  *#\
 /()\\#\
  !!", 1);

  DisplayObject child5("\
  *#\
 /()\\#\
  !!", 1);

DisplayObject egg_barn("\
    __ ^#\
   /  /  \\#\
  |  | _  |#\
  |  |[ ] |", 0);

DisplayObject sugar_barn("\
   __ ^#\
  /  /  \\#\
 |  | _  |#\
 |  |[ ] |", 0);

DisplayObject bakery("\
-----------------------------------|#\
|                                  |#\
                                   |#\
                                   |#\
                                   -----------#\
 =================                           |#\
|o   o   o   o   o                           |#\
                      _________              |#\
                       ^^^^^^^               |#\
 =================                  #\
|o   o   o   o   o    |_______|    -----------#\
                        oven       |#\
                                   |#\
 =================                 |#\
|o   o   o   o   o                 |#\
                                   |#\
                                   |#\
 =================   (     )       |#\
|o   o   o   o   o   (     )       |#\
|                     mixer        |#\
|                                  |#\
|                                  |#\
-----------------------------------|#\
      Anne's Patisserie", 0);
                                        
DisplayObject truck1("\
   _______#\
  |       |__#\
  -OO----OO-O|", 3);

DisplayObject truck2("\
   _______#\
  |       |__#\
  -OO----OO-O|", 3);

DisplayObject butter1("[butter]", 2);
DisplayObject butter2("[butter]", 2);
DisplayObject butter3("[butter]", 2);

DisplayObject flour1("[flour]", 2); //Created new flour/sugar to fit on conveyor belt
DisplayObject flour2("[flour]", 2);
DisplayObject flour3("[flour]", 2);

DisplayObject sugar1("[sugar]", 2);
DisplayObject sugar2("[sugar]", 2);
DisplayObject sugar3("[sugar]", 2);

DisplayObject eggs1("[OOO]", 2);
DisplayObject eggs2("[OOO]", 2);
DisplayObject eggs3("[OOO]", 2);

DisplayObject batter1("[ccc]", 2);
DisplayObject batter2("[ccc]", 2);

DisplayObject cupcakes[7] = {
DisplayObject(\
"   ________#\
   /       /#\
  /       /#\
  _______", 2),
DisplayObject(\
"   ________#\
   /       /#\
  /     c /#\
  _______", 2),
DisplayObject(\
"   ________#\
   /       /#\
  /   c c /#\
  _______", 2),
DisplayObject(\
"   ________#\
   /       /#\
  / c c c /#\
  _______", 2),
DisplayObject(\
"   ________#\
   /     c /#\
  / c c c /#\
  _______", 2),
DisplayObject(\
"   ________#\
   /   c c /#\
  / c c c /#\
  _______", 2),
DisplayObject(\
"   ________#\
   / c c c /#\
  / c c c /#\
  _______", 2)
};

// Currently these statistics will all be 0.  But your code should increment eggs_laid for each egg a chicken lays,
// and eggs_used for each egg mixed into batter.  Similarly, increment butter_produced when a farmer sells a box of
// bars of butter, and so forth.  Because we know the cake batter recipe (see hw3 handout), we should be able to 
// track that all products are accounted for: They are either visible on the screen, or became "sold cakes"

int eggs_laid, eggs_used;
int butter_produced, butter_used;
int sugar_produced, sugar_used;
int flour_produced, flour_used;
int cakes_produced, cakes_sold;

void redisplay()
{
        DisplayObject::redisplay();
	std::cout << "Eggs: Laid=" << eggs_laid << ", Used=" << eggs_used << 
                     " Butter: Sold=" << butter_produced << ", Used=" << butter_used <<
                     " Sugar: Sold=" << sugar_produced << ", Used=" << sugar_used <<
                     " Flour: Sold=" << flour_produced << ", Used=" << flour_used << 
                     " Cakes: Baked=" << cakes_produced << ", Sold=" << cakes_sold << std::endl;
}

void egg_barn_a() {
  egg_barn.draw(30,30);
}

void sugar_barn_a() {
  sugar_barn.draw(42,30);
}

void bakery_a() {
  bakery.draw(30, 70);
}

void cow_a() {
  cow.draw(26, 63);
}


void egg_a() {
  std::string egg_str = "[000]";

  while(true){
    if(truck1.current_x == bakery.current_x-13 && truck1.current_y == bakery.current_y+4){
      eggs1.update_contents(egg_str);
      eggs2.update_contents(egg_str);
      eggs3.update_contents(egg_str);
      eggs1.draw(34, 65);
      do{
        eggs1.draw(eggs1.current_y, eggs1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(eggs1.current_x != bakery.current_x+4);
      eggs2.draw(34, 65);
      do{
        eggs2.draw(eggs2.current_y, eggs2.current_x+1);
        eggs1.draw(eggs1.current_y, eggs1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(eggs2.current_x != bakery.current_x+4);
      eggs1.update_contents("");
      eggs3.draw(34, 65);
      do{
        eggs2.draw(eggs2.current_y, eggs2.current_x+1);
        eggs3.draw(eggs3.current_y, eggs3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(eggs3.current_x != bakery.current_x+4);
      eggs2.update_contents("");
      do{
        eggs3.draw(eggs3.current_y, eggs3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(eggs3.current_x != bakery.current_x+13);
      eggs3.update_contents("");
    }
    
  }
  
}

void flour_a() {
  std::string flour_str = "[flour]";

  while(true){
    if(truck2.current_x == bakery.current_x-13 && truck2.current_y == bakery.current_y+12){
      flour1.update_contents(flour_str);
      flour2.update_contents(flour_str);
      flour3.update_contents(flour_str);
      flour1.draw(42, 63);
      do{
        flour1.draw(flour1.current_y, flour1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(flour1.current_x != bakery.current_x+2);
      flour2.draw(42, 63);
      do{
        flour2.draw(flour2.current_y, flour2.current_x+1);
        flour1.draw(flour1.current_y, flour1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(flour2.current_x != bakery.current_x+2);
      flour1.update_contents("");
      flour3.draw(42, 63);
      do{
        flour2.draw(flour2.current_y, flour2.current_x+1);
        flour3.draw(flour3.current_y, flour3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(flour3.current_x != bakery.current_x+2);
      flour2.update_contents("");
      do{
        flour3.draw(flour3.current_y, flour3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(flour3.current_x != bakery.current_x+11);
      flour3.update_contents("");
    }
    
  }
}

void sugar_a() {
  std::string sugar_str = "[sugar]";

  while(true){
    if(truck2.current_x == bakery.current_x-13 && truck2.current_y == bakery.current_y+16){
      sugar1.update_contents(sugar_str);
      sugar2.update_contents(sugar_str);
      sugar3.update_contents(sugar_str);
      sugar1.draw(46, 63);
      do{
        sugar1.draw(sugar1.current_y, sugar1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(sugar1.current_x != bakery.current_x+2);
      sugar2.draw(46, 63);
      do{
        sugar2.draw(sugar2.current_y, sugar2.current_x+1);
        sugar1.draw(sugar1.current_y, sugar1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(sugar2.current_x != bakery.current_x+2);
      sugar1.update_contents("");
      sugar3.draw(46, 63);
      do{
        sugar2.draw(sugar2.current_y, sugar2.current_x+1);
        sugar3.draw(sugar3.current_y, sugar3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(sugar3.current_x != bakery.current_x+2);
      sugar2.update_contents("");
      do{
        sugar3.draw(sugar3.current_y, sugar3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(sugar3.current_x != bakery.current_x+11);
      sugar3.update_contents("");
    }
    
  }
}

void butter_a() {
  std::string butter_str = "[butter]";

  while(true){
    if(truck1.current_x == bakery.current_x-13 && truck1.current_y == bakery.current_y+8){
      butter1.update_contents(butter_str);
      butter2.update_contents(butter_str);
      butter3.update_contents(butter_str);
      butter1.draw(38, 62);
      do{
        butter1.draw(butter1.current_y, butter1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(butter1.current_x != bakery.current_x+1);
      butter2.draw(38, 62);
      do{
        butter2.draw(butter2.current_y, butter2.current_x+1);
        butter1.draw(butter1.current_y, butter1.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(butter2.current_x != bakery.current_x+1);
      butter1.update_contents("");
      butter3.draw(38, 62);
      do{
        butter2.draw(butter2.current_y, butter2.current_x+1);
        butter3.draw(butter3.current_y, butter3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(butter3.current_x != bakery.current_x+1);
      butter2.update_contents("");
      do{
        butter3.draw(butter3.current_y, butter3.current_x+1);
        std::this_thread::sleep_for(500ms);
      }
      while(butter3.current_x != bakery.current_x+10);
      butter3.update_contents("");
    }
    
  }
}

void farmer_a() {
  farmer.draw(30, 27);
  //std::string dest = "nests";
  while(true){
    if(farmer.current_x == egg_barn.current_x-3){
      std::this_thread::sleep_for(2s);
      do{
        farmer.draw(farmer.current_y, farmer.current_x-1);
        std::this_thread::sleep_for(200ms);
      } 
      while(farmer.current_x != nest1[0].current_x);
      do{
        farmer.draw(farmer.current_y-1, farmer.current_x);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_y != nest1[0].current_y+2);
      std::this_thread::sleep_for(1s);
      do{
        farmer.draw(farmer.current_y, farmer.current_x+1);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_x != nest2[0].current_x);
      std::this_thread::sleep_for(1s);
      do{
        farmer.draw(farmer.current_y, farmer.current_x+1);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_x != nest3[0].current_x+3);
      std::this_thread::sleep_for(1s);
      do{
        farmer.draw(farmer.current_y+1, farmer.current_x);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_y != cow.current_y);
      std::this_thread::sleep_for(5s);
      do{
        farmer.draw(farmer.current_y, farmer.current_x-1);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_x != egg_barn.current_x-3);
      do{
        farmer.draw(farmer.current_y+1, farmer.current_x);
        std::this_thread::sleep_for(200ms);
      }
      while(farmer.current_y != egg_barn.current_y);
    }
  }
}

void truck1_a() {
  truck1.draw(34, 30);
  std::string from = "start";
  while(true){
    if(truck1.current_x == egg_barn.current_x){
      std::this_thread::sleep_for(3s);
      if(from == "start" || from == "butter"){
        from = "eggs";
        do{
          truck1.draw(truck1.current_y, truck1.current_x+1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck1.current_x != bakery.current_x-13);
      }
      else if(from == "eggs"){
        from = "butter";
        do{
          truck1.draw(truck1.current_y+1, truck1.current_x);
          std::this_thread::sleep_for(200ms);
        }
        while(truck1.current_y != bakery.current_y+8);
        do{
          truck1.draw(truck1.current_y, truck1.current_x+1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck1.current_x != bakery.current_x-13);
      }
    }
    else if(truck1.current_x == bakery.current_x-13){
      std::this_thread::sleep_for(10s);
      do{
        truck1.draw(truck1.current_y, truck1.current_x-1);
        std::this_thread::sleep_for(200ms);
      }
      while(truck1.current_x != egg_barn.current_x);
      if(from == "butter"){
        do{
          truck1.draw(truck1.current_y-1, truck1.current_x);
          std::this_thread::sleep_for(200ms);
        }
        while(truck1.current_y != egg_barn.current_y+4);
      }

    }
  }
}


void truck2_a() {
  truck2.draw(46, 30);
  std::string from = "start";
  while(true){
    if(truck2.current_x == sugar_barn.current_x){
      std::this_thread::sleep_for(3s);
      if(from == "start" || from == "flour"){
        from = "sugar";
        do{
          truck2.draw(truck2.current_y, truck2.current_x+1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_x != bakery.current_x-13);
      }
      else if(from == "sugar"){
        from = "flour";
        do{
          truck2.draw(truck2.current_y, truck2.current_x+1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_x != bakery.current_x-17);
        do{
          truck2.draw(truck2.current_y-1, truck2.current_x);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_y != sugar_barn.current_y);
        do{
          truck2.draw(truck2.current_y, truck2.current_x+1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_x != bakery.current_x-13);
      }
    }
    else if(truck2.current_x == bakery.current_x-13){
      std::this_thread::sleep_for(10s);
      if(from == "flour"){
        do{
          truck2.draw(truck2.current_y, truck2.current_x-1);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_x != bakery.current_x-17);
        do{
          truck2.draw(truck2.current_y+1, truck2.current_x);
          std::this_thread::sleep_for(200ms);
        }
        while(truck2.current_y != sugar_barn.current_y+4);
      }
      do{
        truck2.draw(truck2.current_y, truck2.current_x-1);
        std::this_thread::sleep_for(200ms);
      }
      while(truck2.current_x != sugar_barn.current_x);

    }
  }
}

void nest_1a() {
  nest1[0].draw(22, 15);
  int count = 0;
  while(true){
    if(count == 3 && farmer.current_x == nest1[0].current_x && farmer.current_y == nest1[0].current_y+2){
      nest1[0].draw(22, 15);
      count = 0;
      std::this_thread::sleep_for(1s);
    }

    if((chicken1.current_x == nest1[0].current_x && chicken1.current_y == nest1[0].current_y-3) 
    || (chicken2.current_x == nest1[0].current_x && chicken2.current_y == nest1[0].current_y-3)){
      if(count < 3){
        nest1[++count].draw(22, 15);
        std::this_thread::sleep_for(1s);
      }
    }
  }
}

void nest_2a() {
  nest2[0].draw(22, 35);
  int count = 0;
  while(true){
    if(count == 3 && farmer.current_x == nest2[0].current_x && farmer.current_y == nest2[0].current_y+2){
      nest2[0].draw(22, 35);
      count = 0;
      std::this_thread::sleep_for(1s);
    }
    if((chicken1.current_x == nest2[0].current_x && chicken1.current_y == nest2[0].current_y-3) 
    || (chicken2.current_x == nest2[0].current_x && chicken2.current_y == nest2[0].current_y-3)){
      if(count < 3){
        nest2[++count].draw(22, 35);
        std::this_thread::sleep_for(1s);
      }
    }
  }
}

void nest_3a() {
  nest3[0].draw(22, 55);
  int count = 0;
  while(true){
    if(count == 3 && farmer.current_x == nest3[0].current_x+3 && farmer.current_y == nest3[0].current_y+2){
      nest3[0].draw(22, 55);
      count = 0;
      std::this_thread::sleep_for(1s);
    }
    if((chicken1.current_x == nest3[0].current_x && chicken1.current_y == nest3[0].current_y-3) 
    || (chicken2.current_x == nest3[0].current_x && chicken2.current_y == nest3[0].current_y-3)){
      if(count < 3){
        nest3[++count].draw(22, 55);
        std::this_thread::sleep_for(1s);
      }
    }
  }
}

void chicken_a(DisplayObject &c, int num) {
  int y1 = nest1[0].current_y, x1 = nest1[0].current_x;
  int y2 = nest2[0].current_y, x2 = nest2[0].current_x;
  int y3 = nest3[0].current_y, x3 = nest3[0].current_x;
  if (num == 1) c.draw(y1-3, x1);
  if (num == 2) c.draw(y2-3, x2);
  while(true){
    if(c.current_x == x1){
      std::this_thread::sleep_for(3s);
      do{
        c.draw(c.current_y, c.current_x+1);
        std::this_thread::sleep_for(400ms);
      }
      while(c.current_x != x2);
    }
    else if(c.current_x == x2){
      std::this_thread::sleep_for(3s);
      do{
        c.draw(c.current_y, c.current_x+1);
        std::this_thread::sleep_for(400ms);
      }
      while(c.current_x != x3);
      std::this_thread::sleep_for(3s);
      do{
        c.draw(c.current_y-1, c.current_x);
        std::this_thread::sleep_for(400ms);
      }
      while(c.current_y != y3-6);
      do{
        c.draw(c.current_y, c.current_x-1);
        std::this_thread::sleep_for(400ms);
      }
      while(c.current_x != x1);
      do{
        c.draw(c.current_y+1, c.current_x);
        std::this_thread::sleep_for(400ms);
      }
      while(c.current_y != y1-3);
    }
  }
}

//batter1.draw(39, 94);

void mixer_a() {
	std::string mixer_string;
	DisplayObject mixer_contents(mixer_string, 3);
  int mc = 0;
	while(true)
	{
		if(mc == 0xFF)
		{	
			// All the batter ingredients are in the mixer!  Mix them, then
			// clear the contents for the next batch (in fact we really should do more: see hw3 assignment).
			mc = 0;
      mixer_string = "";
			mixer_contents.update_contents(mixer_string);
			mixer_contents.draw(47, 92);
      batter1.draw(47, 92);
      batter2.draw(46, 92);
      std::this_thread::sleep_for(1s);
      batter1.draw(39, 94);
      std::this_thread::sleep_for(4s);
      batter1.update_contents("");
      batter2.draw(39, 94);
      std::this_thread::sleep_for(4s);
      batter2.update_contents("");

		}
		else
		{
      batter1.update_contents("[ccc]");
      batter2.update_contents("[ccc]");
      mixer_string = "";
      //set the elements
      if(eggs1.current_x == bakery.current_x+13) mc = mc | 0x1;
      if(eggs2.current_x == bakery.current_x+13) mc = mc | 0x2;
      if(butter1.current_x == bakery.current_x+10) mc = mc | 0x4;
      if(butter2.current_x == bakery.current_x+10) mc = mc | 0x8;
      if(flour1.current_x == bakery.current_x+11) mc = mc | 0x10;
      if(flour2.current_x == bakery.current_x+11) mc = mc | 0x20;
      if(sugar1.current_x == bakery.current_x+11) mc = mc | 0x40;
      if(sugar2.current_x == bakery.current_x+11) mc = mc | 0x80;
      //make the string
      mixer_string = (mc & 0x01)? "E": " ";
			mixer_string +=(mc & 0x02)? "E": " ";
			mixer_string += (mc & 0x04)? "B": " ";
			mixer_string += (mc & 0x08)? "B": " ";
			mixer_string += "#";
			mixer_string += (mc & 0x10)? "F": " ";
			mixer_string += (mc & 0x20)? "F": " ";
			mixer_string += (mc & 0x40)? "S": " ";
			mixer_string += (mc & 0x80)? "S": " ";
      
      mixer_contents.update_contents(mixer_string);
      mixer_contents.draw(47, 92);
    }
    std::this_thread::sleep_for(200ms);
  }
}


void cupcakes_a() {
  bool baked1 = false;
  bool baked2 = false;
  cupcakes[0].draw(35, 100);
  while(true){
    if(batter1.current_y == 39 && !baked1){
      cupcakes[1].draw(35, 100);
      std::this_thread::sleep_for(1s);
      cupcakes[2].draw(35, 100);
      std::this_thread::sleep_for(1s);
      cupcakes[3].draw(35, 100);
      std::this_thread::sleep_for(1s);
      baked1 = true;
    }
    if(batter2.current_y == 39 && !baked2){
      cupcakes[4].draw(35, 100);
      std::this_thread::sleep_for(1s);
      cupcakes[5].draw(35, 100);
      std::this_thread::sleep_for(1s);
      cupcakes[6].draw(35, 100);
      std::this_thread::sleep_for(1s);
      baked2 = true;
    }
    std::this_thread::sleep_for(1s);
    cupcakes[0].draw(35, 100);
    baked1 = false;
    baked2 = false;

  }
}

void move_to(DisplayObject &c, int dx, int dy, bool yfirst){
  if(yfirst){
    if(c.current_y > dy){
      do{
        c.draw(c.current_y-1, c.current_x);
        std::this_thread::sleep_for(100ms);
      }
      while(c.current_y > dy);
    }
    else if (c.current_y < dy) {
      do{
        c.draw(c.current_y+1, c.current_x);
        std::this_thread::sleep_for(100ms);
      }
      while(c.current_y < dy);
    }
  }
  if(c.current_x > dx){
    do{
      c.draw(c.current_y, c.current_x-1);
      std::this_thread::sleep_for(100ms);
    }
    while(c.current_x > dx);
  }
  else if (c.current_x < dx) {
    do{
      c.draw(c.current_y, c.current_x+1);
      std::this_thread::sleep_for(100ms);
    }
    while(c.current_x < dx);
  }
  if(!yfirst){
    if(c.current_y > dy){
      do{
        c.draw(c.current_y-1, c.current_x);
        std::this_thread::sleep_for(100ms);
      }
      while(c.current_y > dy);
    }
    else if (c.current_y < dy) {
      do{
        c.draw(c.current_y+1, c.current_x);
        std::this_thread::sleep_for(100ms);
      }
      while(c.current_y < dy);
    }
  }
}

void child_a(DisplayObject &c, int num) {
  if(num == 1) c.draw(28, 120);
  if(num == 2) c.draw(32, 125);
  if(num == 3) c.draw(36, 130);
  if(num == 4) c.draw(40, 125);
  if(num == 5) c.draw(44, 120);
  int xo = c.current_x, yo = c.current_y;
  int who = 0; //this will need to be synchronized so only one
  while(true){
    who |= std::rand()&0x5;
    if(who == num){
      move_to(c, cupcakes[0].current_x+10, cupcakes[0].current_y, true);
      std::this_thread::sleep_for(2s);
      move_to(c, xo, yo, false);
    }
    std::this_thread::sleep_for(3s);
    who = 0;
  }
  
}

int main(int argc, char** argv)
{	
  std::thread egg_barn_t(egg_barn_a);
  std::thread sugar_barn_t(sugar_barn_a);
  std::thread bakery_t(bakery_a);
  std::thread cow_t(cow_a);
  std::thread farmer_t(farmer_a);
  std::thread egg_t(egg_a);
	std::thread flour_t(flour_a);
  std::thread sugar_t(sugar_a);
  std::thread butter_t(butter_a);
  std::thread nest_1t(nest_1a);
  std::thread nest_2t(nest_2a);
  std::thread nest_3t(nest_3a);
  std::thread truck1_t(truck1_a);
  std::thread truck2_t(truck2_a);
  std::thread cupcakes_t(cupcakes_a);
  std::thread chicken1_t(chicken_a, std::ref(chicken1), 1);
  std::thread chicken2_t(chicken_a, std::ref(chicken2), 2);
  std::thread child1_t(child_a, std::ref(child1), 1);
  std::thread child2_t(child_a, std::ref(child2), 2);
  std::thread child3_t(child_a, std::ref(child3), 3);
  std::thread child4_t(child_a, std::ref(child4), 4);
  std::thread child5_t(child_a, std::ref(child5), 5);

  std::thread mixer_t(mixer_a);
	while(true){
    redisplay();
    std::this_thread::sleep_for(100ms);
  }
  return 0;
}
