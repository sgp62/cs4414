#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <atomic>

class DisplayObject {
public:
	int  width;
	int  height;
	int  layer;
	volatile int  current_x;
	volatile int  current_y;
	bool  writer;
	int  readers;
	int  writers_waiting;
	bool myturn;
	std::string image;
	std::shared_mutex cpos;
	std::condition_variable_any wait_cpos;

	DisplayObject(const std::string&, const int);
	DisplayObject(const DisplayObject&);
	~DisplayObject();
	void update_contents(const std::string&);
	void draw(int, int, int lasttick = -1, int numticks = 1);
	static void redisplay();
	void move_to(int, int, bool, int&, int, DisplayObject& = def);
	void startwrite();
	void endwrite();
	void startread();
	void endread();
	void checkcollision(DisplayObject&, int&, int);
	static std::mutex child_mtx;
	static std::condition_variable child_wait;
	static std::mutex cake_mtx;
	static std::condition_variable_any not_full;
	static std::condition_variable_any not_enough;
	static std::atomic_int nfree;
	static std::atomic_int nfull;
	static std::atomic_int who;


private:
	static int tick;
	static DisplayObject def;
	static std::condition_variable_any want_rw;
	static std::shared_mutex mtx;

	static const int LINELEN = 140;
	static const int NLINES = 60;
	static const int NLAYERS = 4;

	static char theFarm[NLINES][LINELEN][NLAYERS];

	void erase();
	void draw(int, int, const char);
};
