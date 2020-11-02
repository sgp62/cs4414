#include <iostream>
#include <unistd.h>
#include "displayobject.hpp"
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

DisplayObject chicken("\
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
                                        
DisplayObject truck("\
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

// Below is a totally fake main method that simply puts some of the icons on the screen to illustrate various options
// It moves the chicken around randomly (if it goes "into" the barn, it vanishes because the barn is in a higher layer).
// It also illustrates how you can generate a string and turn it into a display object that will vanish after a little while
// when that object goes out of scope and the deconstructor executes.

int main(int argc, char** argv)
{	
	egg_barn.draw(1, 1);
	bakery.draw(10, 50);
	cow.draw(17, 18);
	farmer.draw(22, 19);
	child.draw(30, 19);
	eggs.draw(14, 43);
	flour.draw(18, 43);
	sugar.draw(22, 43);
	butter.draw(26, 43);
	truck.draw(42, 15);
	int y = 10, oldy = 10, x = 10, oldx = 10, mc = 0;
	bool baked = false;
	std::string mixer_string;
	DisplayObject mixer_contents(mixer_string, 3);
	for(int n = 0; n < 10000; n++)
	{
		nest1[n % 4].draw(10, 10);
		cupcakes[n % 7].draw(15, 80);
		if(mc == 0xF)
		{	
			// All the batter ingredients are in the mixer!  Mix them, then
			// clear the contents for the next batch (in fact we really should do more: see hw3 assignment).
			mc = 0;
			mixer_string = "";
			mixer_contents.update_contents(mixer_string);
			mixer_contents.draw(26, 72);
			// This draws a picture of batter in the oven... but never erases it.
			batter.draw(19, 74);
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
			mixer_contents.draw(26, 72);
			if(baked && (n % 6) == 5)
			{
				baked = false;
			}
		}
		y = std::max(1, y + (1+std::rand()) % 10 - 5);
		x = std::max(1, x + (1+std::rand()) % 10 - 5);
		chicken.draw(oldy = y, oldx = x);
		redisplay();
		usleep(1000000);
	}
        return 0;
}
