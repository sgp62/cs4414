#include <iostream>
#include <shared_mutex>
#include <mutex>
#include "displayobject.hpp"
#include <condition_variable>
#include <atomic>


char DisplayObject::theFarm[NLINES][LINELEN][NLAYERS];
std::shared_mutex DisplayObject::mtx;


std::mutex DisplayObject::child_mtx;
std::condition_variable DisplayObject::child_wait;
std::atomic_int DisplayObject::who;
std::mutex DisplayObject::cake_mtx;
std::condition_variable_any DisplayObject::not_enough;
std::condition_variable_any DisplayObject::not_full;
std::atomic_int DisplayObject::nfree = 6;
std::atomic_int DisplayObject::nfull = 0;

std::condition_variable_any DisplayObject::want_rw;
int active_readers, writers_waiting;
bool active_writer;
int DisplayObject::tick = 0;
DisplayObject DisplayObject::def = DisplayObject("", 0);


DisplayObject::DisplayObject(const std::string& str, const int n)
{
	current_x = 0;
	current_y = 0;
	image = str;
	layer = n;
	width = 0;
	height = 1;
	int i = 0, x = 0;
	while(char c = image[i++])
	{
		if(c == '#')
		{
			++height;
			x = 0;
		}
		else
		{
			if(++x > width)
			{
				width = x;
			}
		}
	}
}

DisplayObject::DisplayObject(const DisplayObject& from)
{
	width = from.width;
	height = from.height;
	layer = from.layer;
	current_x = from.current_x;
	current_y = from.current_y;
	image = from.image;
}

DisplayObject::~DisplayObject()
{
	if(current_x > 0 && current_y > 0)
	{
		erase();
		current_x = 0;
		current_y = 0;
	}
}

void DisplayObject::update_contents(const std::string& str)
{
	int x = current_x;
	int y = current_y;
	if(current_x > 0 && current_y > 0)
	{
		erase();
		current_x = 0;
		current_y = 0;
	}
	image = str;
	if(current_x > 0 && current_y > 0)
	{
		draw(x, y);
	}
}

void DisplayObject::draw(int y, int x, char c)
{
	if(y < 1 || x < 1 || y >= NLINES || x >= LINELEN)
		return;


	theFarm[y][x][layer] = c;
}

void DisplayObject::draw(int y, int x, int lasttick, int numticks)
{
	if(lasttick == -1) lasttick = tick;
	std::shared_lock shared_lock(mtx);
	want_rw.wait(shared_lock, [&]() { return (tick >= lasttick+numticks); });

	if(current_x != 0 && current_y != 0)
	{
		erase();
	}
	current_x = x;
	current_y = y;
	int i = 0;
	while(char c = image[i++])
	{
		if(c == '#')
		{
			++y;
			x = current_x;
		}
		else
		{
			DisplayObject::draw(y, x, c);
			++x;
		}
	}
}


void DisplayObject::erase()
{
	int x = current_x;
	int y = current_y;
	int i = 0;
	while(char c = image[i++])
	{
		if(c == '#')
		{
			++y;
			x = current_x;
		}
		else
		{
			if(y >= 1 && x >= 1 && y < NLINES && x < LINELEN)
				theFarm[y][x][layer] = 0;
			++x;
		}
	}
}

void DisplayObject::redisplay()
{
	
	std::unique_lock unique_lock(mtx);
	active_writer = true;

	std::string toDisplay("\033[1;1H");
	toDisplay += '|';
	for(int c = 0; c < LINELEN; c++)
		toDisplay += '_';
	toDisplay += "|\n";
	for(int line = 0; line < NLINES; line++)
	{
		toDisplay += '|';
		for(int col = 0; col < LINELEN; col++)
		{
			int layer = 4;
			char c = 0;
			while(layer--)
			{
				if((c = theFarm[line][col][layer]))
					break;
			}
			toDisplay += c? c: ' ';
		}
		toDisplay += "|\n";
	}
	toDisplay += '|';
	for(int c = 0; c < LINELEN; c++)
		toDisplay += '_';
	toDisplay += "|\n";
	std::cout << toDisplay << std::endl;
	tick++;
	active_writer = false;
	want_rw.notify_all();

}

void DisplayObject::startwrite(){
	std::unique_lock writelock(cpos);
	++writers_waiting;
	wait_cpos.wait(writelock, [&]() { return readers == 0; });
	--writers_waiting;
	writer = true;
}

void DisplayObject::endwrite(){
	std::unique_lock writelock(cpos);
	writer = false;
	wait_cpos.notify_all();
}

void DisplayObject::startread(){
	std::shared_lock readlock(cpos);
	wait_cpos.wait(readlock, [&](){ return !active_writer && writers_waiting == 0; });
	++readers;
}



void DisplayObject::checkcollision(DisplayObject &opp, int& lt, int nt){
	//opp.startread();
	if((abs(current_x - opp.current_x) <= width+3) && (abs(current_y - opp.current_y) <= height+3)){
		if(!myturn){
			while(true){
				//std::cout << "waiting" << std::endl;
				startwrite();
				draw(current_y, current_x, lt, nt);
				lt+= nt;
				endwrite();
				opp.startread();
				if((abs(current_x - opp.current_x) > (width+3)) || (abs(current_y - opp.current_y) > (height+3))) {
					opp.endread();
					break;
				}
				opp.endread();
			}
			//myturn = 1;
		}
	}
	//opp.endread();
}

void DisplayObject::endread(){
	std::shared_lock readlock(cpos);
	if(--readers == 0) wait_cpos.notify_all();
}

void DisplayObject::move_to(int dy, int dx, bool yfirst, int &lt, int nt, DisplayObject& opp){
  if(yfirst) {
		if(current_y > dy){
			do{
				checkcollision(opp, lt, nt);
				startwrite();
				draw(current_y-1, current_x, lt, nt);
				lt+= nt;
				endwrite();
			}
			while(current_y > dy);
		}
		else if (current_y < dy) {
			do{
				checkcollision(opp, lt, nt);
				startwrite();
				draw(current_y+1, current_x, lt, nt);
				lt+= nt;
				endwrite();
			}
			while(current_y < dy);
		}
	}

	if(current_x > dx){
		do{
			checkcollision(opp, lt, nt);
			startwrite();
			draw(current_y, current_x-1, lt, nt);
			lt+= nt;
			endwrite();
		}
		while(current_x > dx);
	}
	else if (current_x < dx) {
		do{
			checkcollision(opp, lt, nt);
			startwrite();
			draw(current_y, current_x+1, lt, nt);
			lt+= nt;
			endwrite();
		}
		while(current_x < dx);
	}

  if (!yfirst){
    if(current_y > dy){
    	do{
				checkcollision(opp, lt, nt);
				startwrite();
      	draw(current_y-1, current_x, lt, nt);
      	lt+= nt;
				endwrite();
      }
      while(current_y > dy);
    }
    else if (current_y < dy) {
    	do{
				checkcollision(opp, lt, nt);
				startwrite();
      	draw(current_y+1, current_x, lt, nt);
      	lt+= nt;
				endwrite();
    	}
     	while(current_y < dy);
   	}

	}
}