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


void egg_a() {
  int lasttick = 0, numticks = 5;
  std::string egg_str = "[000]";

  std::shared_lock readpos_lock(bakery.cpos);
  bakery.wait_cpos.wait(readpos_lock, [&](){
    return bakery.current_x != 0;
  });

  while(true){
    if(truck1.current_x == bakery.current_x-13 && truck1.current_y == bakery.current_y+4){
      eggs1.update_contents(egg_str, lasttick);
      eggs2.update_contents(egg_str, lasttick);
      eggs3.update_contents(egg_str, lasttick);
      eggs1.draw(34, 65, lasttick, 1);
      lasttick++;


      int bx = bakery.current_x;

      eggs1.move_to(eggs1.current_y, bx+5, false, lasttick, numticks);
      eggs2.draw(34, 65, lasttick, 1);
      lasttick++;
      int temptick = lasttick;
      std::thread t1( [&]{ eggs1.move_to(eggs1.current_y, bx+14, false, temptick, numticks); });
      t1.detach();
      eggs2.move_to(eggs2.current_y, bx+5, false, lasttick, numticks);
      while(DisplayObject::mc&0x01 && DisplayObject::mc&0x02){
        eggs1.draw(eggs1.current_y, eggs1.current_x, lasttick);
        eggs2.draw(eggs2.current_y, eggs2.current_x, lasttick);
        lasttick++;
      }
      
      eggs1.update_contents("", lasttick);
      eggs1.flag = true;

      eggs3.draw(34, 65, lasttick, 1);
      lasttick++;

      temptick = lasttick;
      std::thread t2( [&]{ eggs2.move_to(eggs2.current_y, bx+14, false, temptick, numticks); });
      t2.detach();
      eggs3.move_to(eggs3.current_y, bx+5, false, lasttick, numticks);
      while(DisplayObject::mc&0x01 && DisplayObject::mc&0x02){
        eggs3.draw(eggs3.current_y, eggs3.current_x, lasttick);
        eggs2.draw(eggs2.current_y, eggs2.current_x, lasttick);
        lasttick++;
      }
      
      eggs2.update_contents("", lasttick);
      eggs2.flag = true;

      eggs3.move_to(eggs3.current_y, bx+14, false, lasttick, numticks);
      while(DisplayObject::mc&0x01 && DisplayObject::mc&0x02){
        eggs3.draw(eggs3.current_y, eggs3.current_x, lasttick);
        lasttick++;
      }
      eggs3.update_contents("", lasttick);
      eggs3.flag = true;
    }
    else{
      eggs1.draw(eggs1.current_y, eggs1.current_x, lasttick);
      eggs2.draw(eggs2.current_y, eggs2.current_x, lasttick);
      eggs3.draw(eggs3.current_y, eggs3.current_x, lasttick);
      lasttick++;
    }
  }
  
}

void flour_a() {
  int lasttick = 0, numticks = 5;
  std::string flour_str = "[flour]";
  std::shared_lock readpos_lock(bakery.cpos);
  bakery.wait_cpos.wait(readpos_lock, [&](){
    return bakery.current_x != 0;
  });
  int bx = bakery.current_x;

  while(true){
    if(truck2.current_x == bakery.current_x-13 && truck2.current_y == bakery.current_y+8){
      flour1.update_contents(flour_str, lasttick);
      flour2.update_contents(flour_str, lasttick);
      flour3.update_contents(flour_str, lasttick);
      flour1.draw(38, 63, lasttick, 1);
      lasttick++;


      flour1.move_to(flour1.current_y, bx+3, false, lasttick, numticks);
      flour2.draw(38, 63, lasttick, 1);
      lasttick++;
      int temptick = lasttick;
      std::thread t1( [&]{ flour1.move_to(flour1.current_y, bx+12, false, temptick, numticks); });
      t1.detach();
      flour2.move_to(flour2.current_y, bx+3, false, lasttick, numticks);
      while(DisplayObject::mc&0x010 && DisplayObject::mc&0x020){
        flour1.draw(flour1.current_y, flour1.current_x, lasttick);
        flour2.draw(flour2.current_y, flour2.current_x, lasttick);
        lasttick++;
      }
      flour1.update_contents("", lasttick);
      flour1.flag = true;

      flour3.draw(38, 63, lasttick, 1);
      lasttick++;

      temptick = lasttick;
      std::thread t2( [&]{ flour2.move_to(flour2.current_y, bx+12, false, temptick, numticks); });
      t2.detach();
      flour3.move_to(flour3.current_y, bx+3, false, lasttick, numticks);
      while(DisplayObject::mc&0x010 && DisplayObject::mc&0x020){
        flour3.draw(flour3.current_y, flour3.current_x, lasttick);
        flour2.draw(flour2.current_y, flour2.current_x, lasttick);
        lasttick++;
      }
      flour2.update_contents("", lasttick);
      flour2.flag = true;
      //lasttick++;
      flour3.move_to(flour3.current_y, bx+12, false, lasttick, numticks);
      while(DisplayObject::mc&0x010 && DisplayObject::mc&0x020){
        flour3.draw(flour3.current_y, flour3.current_x, lasttick);
        lasttick++;
      }
      flour3.update_contents("", lasttick);
      flour3.flag = true;
    //lasttick++;
    }  
    else{
      flour1.draw(flour1.current_y, flour1.current_x, lasttick);
      flour2.draw(flour2.current_y, flour2.current_x, lasttick);
      flour3.draw(flour3.current_y, flour3.current_x, lasttick);
      lasttick++;
    }
  }
}

void sugar_a() {
  int lasttick = 0, numticks = 5;
  std::string sugar_str = "[sugar]";
  std::shared_lock readpos_lock(bakery.cpos);
  bakery.wait_cpos.wait(readpos_lock, [&](){
    return bakery.current_x != 0;
  });
  int bx = bakery.current_x;
  while(true){
    if(truck2.current_x == bakery.current_x-13 && truck2.current_y == bakery.current_y+16){
      sugar1.update_contents(sugar_str, lasttick);
      sugar2.update_contents(sugar_str, lasttick);
      sugar3.update_contents(sugar_str, lasttick);
      sugar1.draw(46, 63, lasttick, 1);
      lasttick++;

      sugar1.move_to(sugar1.current_y, bx+3, false, lasttick, numticks);
      sugar2.draw(46, 63, lasttick, 1);
      lasttick++;
      int temptick = lasttick;
      std::thread t1( [&]{ sugar1.move_to(sugar1.current_y, bx+12, false, temptick, numticks); });
      t1.detach();
      sugar2.move_to(sugar2.current_y, bx+3, false, lasttick, numticks);
      while(DisplayObject::mc&0x040 && DisplayObject::mc&0x080){
        sugar1.draw(sugar1.current_y, sugar1.current_x, lasttick);
        sugar2.draw(sugar2.current_y, sugar2.current_x, lasttick);
        lasttick++;
      }
      sugar1.update_contents("", lasttick);
      sugar1.flag = true;

      sugar3.draw(46, 63, lasttick, 1);
      lasttick++;

      temptick = lasttick;
      std::thread t2( [&]{ sugar2.move_to(sugar2.current_y, bx+12, false, temptick, numticks); });
      t2.detach();
      sugar3.move_to(sugar3.current_y, bx+3, false, lasttick, numticks);
      while(DisplayObject::mc&0x040 && DisplayObject::mc&0x080){
        sugar3.draw(sugar3.current_y, sugar3.current_x, lasttick);
        sugar2.draw(sugar2.current_y, sugar2.current_x, lasttick);
        lasttick++;
      }
      sugar2.update_contents("", lasttick);
      sugar2.flag = true;
      sugar3.move_to(sugar3.current_y, bx+12, false, lasttick, numticks);
      while(DisplayObject::mc&0x040 && DisplayObject::mc&0x080){
        sugar3.draw(sugar3.current_y, sugar3.current_x, lasttick);
        lasttick++;
      }
      sugar3.update_contents("", lasttick);
      sugar3.flag = true;
    }  
    else{
      sugar1.draw(sugar1.current_y, sugar1.current_x, lasttick);
      sugar2.draw(sugar2.current_y, sugar2.current_x, lasttick);
      sugar3.draw(sugar3.current_y, sugar3.current_x, lasttick);
      lasttick++;
    }
  }
}

void butter_a() {
  int lasttick = 0, numticks = 5;
  std::string butter_str = "[butter]";
  std::shared_lock readpos_lock(bakery.cpos);
  bakery.wait_cpos.wait(readpos_lock, [&](){
    return bakery.current_x != 0;
  });
  int bx = bakery.current_x;
  while(true){
    if(truck1.current_x == bakery.current_x-13 && truck1.current_y == bakery.current_y+12){
      butter1.update_contents(butter_str, lasttick);
      butter2.update_contents(butter_str, lasttick);
      butter3.update_contents(butter_str, lasttick);
      butter1.draw(42, 62, lasttick, 1);
      lasttick++;

      butter1.move_to(butter1.current_y, bx+2, false, lasttick, numticks);
      butter2.draw(42, 62, lasttick, 1);
      lasttick++;
      int temptick = lasttick;
      std::thread t1( [&]{ butter1.move_to(butter1.current_y, bx+11, false, temptick, numticks); });
      t1.detach();
      butter2.move_to(butter2.current_y, bx+2, false, lasttick, numticks);
      while(DisplayObject::mc&0x04 && DisplayObject::mc&0x08){
        butter1.draw(butter1.current_y, butter1.current_x, lasttick);
        butter2.draw(butter2.current_y, butter2.current_x, lasttick);
        lasttick++;
      }
      butter1.update_contents("", lasttick);
      butter1.flag = true;

      butter3.draw(42, 62, lasttick, 1);
      lasttick++;

      temptick = lasttick;
      std::thread t2( [&]{ butter2.move_to(butter2.current_y, bx+11, false, temptick, numticks); });
      t2.detach();
      butter3.move_to(butter3.current_y, bx+2, false, lasttick, numticks);
      while(DisplayObject::mc&0x04 && DisplayObject::mc&0x08){
        butter2.draw(butter2.current_y, butter2.current_x, lasttick);
        butter3.draw(butter3.current_y, butter3.current_x, lasttick);
        lasttick++;
      }
      butter2.update_contents("", lasttick);
      butter2.flag = true;
      butter3.move_to(butter3.current_y, bx+11, false, lasttick, numticks);
      while(DisplayObject::mc&0x04 && DisplayObject::mc&0x08){
        butter3.draw(butter3.current_y, butter3.current_x, lasttick);
        lasttick++;
      };
      butter3.update_contents("", lasttick);
      butter3.flag = true;
    }  
    else{
      butter1.draw(butter1.current_y, butter1.current_x, lasttick);
      butter2.draw(butter2.current_y, butter2.current_x, lasttick);
      butter3.draw(butter3.current_y, butter3.current_x, lasttick);
      lasttick++;
    }
  }
}

void farmer_a() {
  int numticks = 2, lasttick = 0;
  farmer.draw(30, 24, lasttick, 1);
  lasttick += 1;
  std::shared_lock readpos_lock(egg_barn.cpos);
  egg_barn.wait_cpos.wait(readpos_lock, [&](){
    return egg_barn.current_x != 0;
  });
  while(true){
    if(farmer.current_x == egg_barn.current_x-3){
      farmer.draw(30, 27, lasttick, numticks*10);
      lasttick += numticks*10;

      farmer.move_to(nest1[0].current_y+2, nest1[0].current_x, false, lasttick, numticks);
      farmer.draw(farmer.current_y, farmer.current_x, lasttick, numticks*5);
      lasttick += numticks*5;

      farmer.move_to(farmer.current_y, nest2[0].current_x, false, lasttick, numticks);
      farmer.draw(farmer.current_y, farmer.current_x, lasttick, numticks*5);
      lasttick += numticks*5;

      farmer.move_to(farmer.current_y, nest3[0].current_x+3, false, lasttick, numticks);
      farmer.draw(farmer.current_y, farmer.current_x, lasttick, numticks*5);
      lasttick += numticks*5;

      farmer.move_to(cow.current_y, farmer.current_x, false, lasttick, numticks);
      farmer.draw(farmer.current_y, farmer.current_x, lasttick, numticks*25);
      lasttick += numticks*25;

      farmer.move_to(egg_barn.current_y, egg_barn.current_x-3, false, lasttick, numticks);
    }
  }
}

void truck1_a() {
  truck1.myturn = true;
  int lasttick = 0, numticks = 2;
  truck1.draw(34, 27, lasttick, 1);
  lasttick++;
  std::string from = "start";
  std::shared_lock readpos_lock(egg_barn.cpos);
  egg_barn.wait_cpos.wait(readpos_lock, [&](){
    return egg_barn.current_x != 0;
  });
  while(true){
    if(truck1.current_x == egg_barn.current_x){
      truck1.draw(truck1.current_y, truck1.current_x, lasttick, numticks * 15);
      lasttick+= numticks*15;
      if(from == "start" || from == "butter"){
        from = "eggs";
        truck1.move_to(truck1.current_y, bakery.current_x-13, false, lasttick, numticks);
      }
      else if(from == "eggs"){
        from = "butter";
        truck1.move_to(bakery.current_y+12, bakery.current_x-28, false, lasttick, numticks, truck2, 0, 8);
        truck1.move_to(truck1.current_y, bakery.current_x-13, true, lasttick, numticks);
      }
    }
    else if(truck1.current_x == bakery.current_x-13){
      //truck1.draw(truck1.current_y, truck1.current_x, lasttick, numticks * 50);
      //lasttick += numticks*50;
      if(from == "butter"){
        while(butter3.current_x != 63){
          truck1.draw(truck1.current_y, truck1.current_x, lasttick);
          lasttick++;
        }
        truck1.move_to(egg_barn.current_y+4, bakery.current_x-28, false, lasttick, numticks, truck2, 0, 8);
      }
      else{
        while(eggs3.current_x != 66){
          truck1.draw(truck1.current_y, truck1.current_x, lasttick);
          lasttick++;
        }
      }
      truck1.move_to(truck1.current_y, egg_barn.current_x, false, lasttick, numticks, truck2, 0, 8);
    }
    //truck1.myturn = false;
  }
}


void truck2_a() {
  truck2.myturn = false;
  int lasttick = 0, numticks = 2;
  truck2.draw(46, 27, lasttick, 1);
  lasttick++;
  std::string from = "start";
  std::shared_lock readpos_lock(sugar_barn.cpos);
  sugar_barn.wait_cpos.wait(readpos_lock, [&](){
    return sugar_barn.current_x != 0;
  });
  while(true){
    if(truck2.current_x == sugar_barn.current_x){
      truck2.draw(truck2.current_y, truck2.current_x, lasttick, numticks * 15);
      lasttick+= numticks*15;
      if(from == "start" || from == "flour"){
        from = "sugar";
        truck2.move_to(truck2.current_y, bakery.current_x-13, false, lasttick, numticks);
      }
      else if(from == "sugar"){
        from = "flour";
        truck2.move_to(bakery.current_y+8, bakery.current_x-28, false, lasttick, numticks, truck1, 0, 8);
        truck2.move_to(truck2.current_y, bakery.current_x-13, false, lasttick, numticks);
      }
    }
    else if(truck2.current_x == bakery.current_x-13){
      //truck1.draw(truck1.current_y, truck1.current_x, lasttick, numticks * 50);
      //lasttick += numticks*50;
      if(from == "flour"){
        while(flour3.current_x != 64){
          truck2.draw(truck2.current_y, truck2.current_x, lasttick);
          lasttick++;
        }
        truck2.move_to(sugar_barn.current_y+4, bakery.current_x-28, false, lasttick, numticks, truck1, 0, 8);
      }
      else{
        while(sugar3.current_x != 64){
          truck2.draw(truck2.current_y, truck2.current_x, lasttick);
          lasttick++;
        }
      }
      truck2.move_to(truck2.current_y, sugar_barn.current_x, false, lasttick, numticks, truck1, 0, 8);
    }
    //truck2.myturn = true;
  }
}

void nest_1a() {
  int lasttick = 0, numticks = 10;
  nest1[0].draw(22, 15, lasttick, 1);
  lasttick++;
  int count = 0;
  while(true){
    if((chicken1.current_x == nest1[0].current_x && chicken1.current_y == nest1[0].current_y-3) 
    || (chicken2.current_x == nest1[0].current_x && chicken2.current_y == nest1[0].current_y-3)){
      if(count < 3){
        nest1[++count].draw(22, 15, lasttick, numticks);
        lasttick += numticks;
      }
      if(count >= 3) count = 3;
    }
    else if(count == 3 && farmer.current_x == nest1[0].current_x && farmer.current_y == nest1[0].current_y+2){
      nest1[0].draw(22, 15, lasttick, numticks);
      lasttick += numticks;
      count = 0;
    }
    else{
      nest1[count].draw(22, 15, lasttick, numticks);
      lasttick++;
    }
  }
}

void nest_2a() {
  int lasttick = 0, numticks = 10;
  nest2[0].draw(22, 35, lasttick, 1);
  lasttick++;
  int count = 0;
  while(true){
    chicken1.startread();
    chicken2.startread();
    farmer.startread();
    int c1x = chicken1.current_x, c1y = chicken1.current_y, c2x = chicken2.current_x, c2y = chicken2.current_y, fx = farmer.current_x, fy = farmer.current_y;
    chicken1.endread();
    chicken2.endread();
    farmer.endread();
    if((c1x == nest2[0].current_x && c1y == nest2[0].current_y-3) 
    || (c2x == nest2[0].current_x && c2y == nest2[0].current_y-3)){
      if(count < 3){
        nest2[++count].draw(22, 35, lasttick, numticks);
        lasttick += numticks;
      }
      if(count >= 3) count = 3;
    }
    else if(count == 3 && fx == nest2[0].current_x && fy == nest2[0].current_y+2){
      nest2[0].draw(22, 35, lasttick, numticks);
      count = 0;
      lasttick += numticks;
    }
    else{
      nest2[count].draw(22, 35, lasttick, numticks);
      lasttick++;
    }

  }
}

void nest_3a() {
  int lasttick = 0, numticks = 10;
  nest3[0].draw(22, 55, lasttick, 1);
  lasttick++;
  int count = 0;
  while(true){
    if((chicken1.current_x == nest3[0].current_x && chicken1.current_y == nest3[0].current_y-3) 
    || (chicken2.current_x == nest3[0].current_x && chicken2.current_y == nest3[0].current_y-3)){
      if(count < 3){
        nest3[++count].draw(22, 55, lasttick, numticks);
        lasttick += numticks;
      }
      if(count >= 3) count = 3;
    }
    else if(count == 3 && farmer.current_x == nest3[0].current_x+3 && farmer.current_y == nest3[0].current_y+2){
      nest3[0].draw(22, 55, lasttick, numticks);
      count = 0;
      lasttick += numticks;
    }
    else{
      nest3[count].draw(22, 55, lasttick, numticks);
      lasttick++;
    }
  }
}

void chicken_1a() {
  chicken1.myturn = true;
  int lasttick = 0, numticks = 4, extraticks = 30;
  chicken1.draw(19, 15, lasttick, 1);
  lasttick+=1;
  std::shared_lock readpos_lock(nest1[0].cpos);
  nest1[0].wait_cpos.wait(readpos_lock, [&](){
    return nest2[0].current_x != 0;
  });
  //nest1[0].startread();
  int x1 = nest1[0].current_x;
  //nest1[0].endread();
  int x2 = nest2[0].current_x;
  while(true){
    if(chicken1.current_x == x1){
      chicken1.draw(chicken1.current_y, chicken1.current_x, lasttick, extraticks);
      lasttick += extraticks;
      chicken1.move_to(chicken1.current_y, x2, false, lasttick, numticks, chicken2, 3, 3);
    }
    else if(chicken1.current_x == x2){
      chicken1.draw(chicken1.current_y, chicken1.current_x, lasttick, numticks);
      lasttick += numticks;
      chicken1.move_to(chicken1.current_y, x1, false, lasttick, numticks);
      //chicken1.draw(chicken1.current_y, chicken1.current_x, lasttick, extraticks);
      //lasttick += extraticks;
    }
  }
}

void chicken_2a() {
  chicken2.myturn = false;
  int lasttick = 0, numticks = 4, extraticks = 30;
  chicken2.draw(19, 55, lasttick, 1);
  lasttick+=1;
  std::shared_lock readpos_lock(nest3[0].cpos);
  nest3[0].wait_cpos.wait(readpos_lock, [&](){
    return nest2[0].current_x != 0;
  });
  //nest2[0].startread();
  int x2 = nest2[0].current_x;
  //nest2[0].endread();
  int x3 = nest3[0].current_x; 
  while(true){
    if(chicken2.current_x == x3){
      chicken2.draw(chicken2.current_y, chicken2.current_x, lasttick, extraticks);
      lasttick += extraticks ;
      chicken2.move_to(chicken2.current_y, x2, false, lasttick, numticks, chicken1, 3, 3);
    }
    else if(chicken2.current_x == x2){
      chicken2.draw(chicken2.current_y, chicken2.current_x, lasttick, numticks);
      lasttick += numticks;
      chicken2.move_to(chicken2.current_y, x3, false, lasttick, numticks);
    }
  }
}

void oven_a(int lasttick){
  int numticks = 4;
  for(int i = 0; i < 2; i++){
    std::unique_lock guard(DisplayObject::cake_mtx);
    DisplayObject::not_full.wait(guard, [&]() { return DisplayObject::nfree != 0;});
    if(i == 0){
      batter1.draw(39, 94, lasttick, numticks);
      lasttick+=numticks;
      batter1.draw(batter1.current_y, batter1.current_x, lasttick, numticks * 5);
      lasttick+=numticks*5;
      batter1.update_contents("", lasttick);
    }
    if(i == 1){
      batter2.draw(39, 94, lasttick, numticks);
      lasttick+=numticks;
      batter2.draw(batter1.current_y, batter1.current_x, lasttick, numticks * 5);
      lasttick+=numticks*5;
      batter2.update_contents("", lasttick);
    }
    DisplayObject::nfree-=3;
    if(DisplayObject::nfree < 0) DisplayObject::nfree = 0;
    DisplayObject::nfull += 3;
    if(DisplayObject::nfull > 6) DisplayObject::nfull = 6;
    DisplayObject::not_enough.notify_one();
  }
}

void mixer_a() {
  int lasttick = 0, numticks = 4;
	std::string mixer_string = "";
	DisplayObject mixer_contents(mixer_string, 3);
  mixer_contents.draw(47, 92, lasttick, 1);
  lasttick++;
	while(true) {
		if(DisplayObject::mc == 0xFF) {	
      mixer_string = "";
			mixer_contents.update_contents(mixer_string, lasttick);
      lasttick++;
			mixer_contents.draw(47, 92, lasttick, 1);
      lasttick++;
      batter1.update_contents("[ccc]", lasttick);
      batter2.update_contents("[ccc]", lasttick);
      lasttick++;
      batter1.draw(47, 92, lasttick, numticks);
      batter2.draw(46, 92, lasttick, numticks);
      lasttick += numticks;
      //std::thread oven_t([&]{ oven_a(lasttick); });
      //oven_t.detach();
      oven_a(lasttick);
      DisplayObject::mc = 0;
      
		} else {
      //set the elements

      if(!(DisplayObject::mc & 0x1) && (eggs1.flag || eggs2.flag || eggs3.flag)){ 
        DisplayObject::mc = DisplayObject::mc | 0x1;
        eggs1.flag = false; 
        eggs2.flag = false;
        eggs3.flag = false;
      }
      else if(eggs1.flag || eggs2.flag || eggs3.flag) {
        DisplayObject::mc = DisplayObject::mc | 0x2;
        eggs1.flag = false; 
        eggs2.flag = false;
        eggs3.flag = false;
      }

      if(!(DisplayObject::mc & 0x4) && (butter1.flag || butter2.flag || butter3.flag)){ 
        DisplayObject::mc = DisplayObject::mc | 0x4;
        butter1.flag = false; 
        butter2.flag = false;
        butter3.flag = false;
      }
      else if(butter1.flag || butter2.flag || butter3.flag) {
        DisplayObject::mc = DisplayObject::mc | 0x8;
        butter1.flag = false; 
        butter2.flag = false;
        butter3.flag = false;
      }

      if(!(DisplayObject::mc & 0x10) && (flour1.flag || flour2.flag || flour3.flag)){ 
        DisplayObject::mc = DisplayObject::mc | 0x10;
        flour1.flag = false; 
        flour2.flag = false;
        flour3.flag = false;
      }
      else if(flour1.flag || flour2.flag || flour3.flag) {
        DisplayObject::mc = DisplayObject::mc | 0x20;
        flour1.flag = false; 
        flour2.flag = false;
        flour3.flag = false;
      }

      if(!(DisplayObject::mc & 0x40) && (sugar1.flag || sugar2.flag || sugar3.flag)){ 
        DisplayObject::mc = DisplayObject::mc | 0x40;
        sugar1.flag = false; 
        sugar2.flag = false;
        sugar3.flag = false;
      }
      else if(sugar1.flag || sugar2.flag || sugar3.flag) {
        DisplayObject::mc = DisplayObject::mc | 0x80;
        sugar1.flag = false; 
        sugar2.flag = false;
        sugar3.flag = false;
      }

      mixer_string = (DisplayObject::mc & 0x01)? "E": " ";
			mixer_string +=(DisplayObject::mc & 0x02)? "E": " ";
			mixer_string += (DisplayObject::mc & 0x04)? "B": " ";
			mixer_string += (DisplayObject::mc & 0x08)? "B": " ";
			mixer_string += "#";
			mixer_string += (DisplayObject::mc & 0x10)? "F": " ";
			mixer_string += (DisplayObject::mc & 0x20)? "F": " ";
			mixer_string += (DisplayObject::mc & 0x40)? "S": " ";
			mixer_string += (DisplayObject::mc & 0x80)? "S": " ";

      mixer_contents.update_contents(mixer_string, lasttick);
      //lasttick++;
      mixer_contents.draw(47, 92, lasttick, 1);
      lasttick++;
    }
    mixer_contents.draw(47, 92, lasttick, numticks);
    lasttick+= numticks;
  }
}


void cupcakes_a() {
  int lasttick = 0;//, numticks = 10;
  DisplayObject::nfull = 0;
  while(true){
    cupcakes[DisplayObject::nfull].draw(35, 100, lasttick);
    lasttick ++;
  }
}

void child_a(DisplayObject &c, int num) {
  int lasttick = 0, numticks = 10;
  if(num == 1) c.draw(28, 120);
  if(num == 2) c.draw(32, 125);
  if(num == 3) c.draw(36, 130);
  if(num == 4) c.draw(40, 125);
  if(num == 5) c.draw(44, 120);
  //lasttick++;
  int xo = c.current_x, yo = c.current_y;
  int cakes = 1;
  while(true){
    if(DisplayObject::who == num){
      std::unique_lock guard(DisplayObject::child_mtx);
      c.move_to(35, 110, true, lasttick, 1);
      std::unique_lock cake_guard(DisplayObject::cake_mtx);
      cakes = std::rand() % 5 + 1;
      DisplayObject::not_enough.wait(cake_guard, [&](){ return DisplayObject::nfull >= cakes; });
      lasttick = DisplayObject::gettick();
      c.draw(c.current_y, c.current_x, lasttick, numticks);
      lasttick += numticks;
      DisplayObject::nfree += cakes;
      DisplayObject::nfull -= cakes;
      DisplayObject::not_full.notify_all();
      c.move_to(yo, xo, false, lasttick, 1);
      DisplayObject::who = std::rand() % 5 + 1;
    }
    c.draw(c.current_y, c.current_x, lasttick, numticks);
    lasttick += numticks;
  }
}

int main(int argc, char** argv)
{	
  srand(std::time(0));
  DisplayObject::who = std::rand() % 5 + 1;
  std::thread consts_t( [&]() {
    int lasttick = 0;
    std::unique_lock(egg_barn.cpos);
    egg_barn.draw(30,27, lasttick);
    egg_barn.wait_cpos.notify_all();

    std::unique_lock(sugar_barn.cpos);
    sugar_barn.draw(42,27, lasttick);
    sugar_barn.wait_cpos.notify_all();

    std::unique_lock(bakery.cpos);
    bakery.draw(30, 70, lasttick);
    bakery.wait_cpos.notify_all();

    std::unique_lock(cow.cpos);
    cow.draw(26, 63, lasttick);
    cow.wait_cpos.notify_all();
    lasttick++;
  });
  std::thread farmer_t(farmer_a);
  std::thread truck1_t(truck1_a);
  std::thread truck2_t(truck2_a);
  std::thread nest_1t(nest_1a);
  std::thread nest_2t(nest_2a);
  std::thread nest_3t(nest_3a);
  std::thread egg_t(egg_a);
	std::thread flour_t(flour_a);
  std::thread sugar_t(sugar_a);
  std::thread butter_t(butter_a);
  std::thread cupcakes_t(cupcakes_a);
  std::thread chicken1_t(chicken_1a);
  std::thread chicken2_t(chicken_2a);
  std::thread mixer_t(mixer_a);
  //std::thread oven_t(oven_a);
  std::thread child1_t(child_a, std::ref(child1), 1);
  std::thread child2_t(child_a, std::ref(child2), 2);
  std::thread child3_t(child_a, std::ref(child3), 3);
  std::thread child4_t(child_a, std::ref(child4), 4);
  std::thread child5_t(child_a, std::ref(child5), 5);
  
	while(true){
    redisplay();
    std::this_thread::sleep_for(100ms);
  }
  return 0;
}
