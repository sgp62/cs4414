#include <iostream>
#include <unistd.h>
#include "displayobject.hpp"
#include <thread>
#include <chrono>
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

DisplayObject child("\
  *#\
 /()\\#\
  !!", 1);

DisplayObject egg_barn("\
    __ ^#\
   /  /  \\#\
  |  | _  |#\
  |  |[ ] |", 3);

DisplayObject sugar_barn("\
   __ ^#\
  /  /  \\#\
 |  | _  |#\
 |  |[ ] |", 3);

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
 =================  (      )       |#\
|o   o   o   o   o   (____)        |#\
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

DisplayObject flour("\
\\~~~~/#\
(flour)", 2);

DisplayObject sugar("\
\\~~~~/#\
(sugar)", 2);

DisplayObject butter("[butter]", 2);

DisplayObject eggs("[OOO]", 2);

DisplayObject batter("[ccc]", 2);

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
  egg_barn.draw(27,10);
}

void sugar_barn_a() {
  sugar_barn.draw(37,25);
}

void bakery_a() {
  bakery.draw(30, 70);
}

void cow_a() {
  cow.draw(17, 48);
}

void farmer_a() {
  farmer.draw(22, 19);
}

void child_a() {
  child.draw(30, 120);
}

void egg_a() {
  eggs.draw(34, 63);
}

void flour_a() {
  flour.draw(38, 63);
}

void sugar_a() {
  sugar.draw(42, 63);
}

void butter_a() {
  butter.draw(46, 63);
}

void truck1_a() {
  truck1.draw(31, 9);
}

void truck2_a() {
  truck2.draw(41, 23);
}

void nest_1a() {
  for(int n = 0; n < 10000; n++){
    nest1[n % 4].draw(10, 10);
    std::this_thread::sleep_for(1s);
  }
}

void nest_2a() {
  for(int n = 0; n < 10000; n++){
    nest2[n % 4].draw(20, 10);
    std::this_thread::sleep_for(1s);
  }
}


void cupcakes_a() {
  for(int n = 0; n < 10000; n++){
    cupcakes[n % 7].draw(35, 100);
    std::this_thread::sleep_for(1s);

  }
}

void chicken1_a() {
	int y = 10, oldy = 10, x = 10, oldx = 10;
  for(int n = 0; n < 10000; n++){
    y = std::max(1, y + (1+std::rand()) % 10 - 5);
    x = std::max(1, x + (1+std::rand()) % 10 - 5);
    chicken1.draw(oldy = y, oldx = x);
    std::this_thread::sleep_for(500ms);
  }
}

void mixer_a() {
  bool baked = false;
	std::string mixer_string;
	DisplayObject mixer_contents(mixer_string, 3);
  int mc = 0;
	for(int n = 0; n < 10000; n++)
	{
		if(mc == 0xF)
		{	
			// All the batter ingredients are in the mixer!  Mix them, then
			// clear the contents for the next batch (in fact we really should do more: see hw3 assignment).
			mc = 0;
			mixer_string = "";
			mixer_contents.update_contents(mixer_string);
			mixer_contents.draw(46, 92);
			// This draws a picture of batter in the oven... but never erases it.
			batter.draw(39, 94);
			baked = true;
		}
		else
		{
			mc |= std::rand()&0xF;
			mixer_string = (mc & 0x1)? "E": " ";
			mixer_string += "  ";
			mixer_string += (mc & 0x2)? "B": " ";
			mixer_string += "#";
			mixer_string += (mc & 0x4)? " F": "  ";
			mixer_string += (mc & 0x8)? "S": " ";
			mixer_contents.update_contents(mixer_string);
			mixer_contents.draw(46, 92);
			if(baked && (n % 6) == 5)
			{
				baked = false;
			}
    }
    std::this_thread::sleep_for(1s);
  }
}
// Below is a totally fake main method that simply puts some of the icons on the screen to illustrate various options
// It moves the chicken around randomly (if it goes "into" the barn, it vanishes because the barn is in a higher layer).
// It also illustrates how you can generate a string and turn it into a display object that will vanish after a little while
// when that object goes out of scope and the deconstructor executes.

int main(int argc, char** argv)
{	
  std::thread egg_barn_t(egg_barn_a);
  std::thread sugar_barn_t(sugar_barn_a);
  std::thread bakery_t(bakery_a);
  std::thread cow_t(cow_a);
  std::thread farmer_t(farmer_a);
  std::thread child_t(child_a);
  std::thread egg_t(egg_a);
	std::thread flour_t(flour_a);
  std::thread sugar_t(sugar_a);
  std::thread butter_t(butter_a);
	std::thread truck1_t(truck1_a);
  std::thread truck2_t(truck2_a);
  std::thread nest_1t(nest_1a);
  std::thread nest_2t(nest_2a);
  std::thread cupcakes_t(cupcakes_a);
  std::thread chicken1_t(chicken1_a);
  std::thread mixer_t(mixer_a);
	while(true){
    redisplay();
    std::this_thread::sleep_for(500ms);
  }
  return 0;
}