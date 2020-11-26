#include <iostream>
#include <shared_mutex>
#include <mutex>

class DisplayObject {
public:
	int  width;
	int  height;
	int  layer;
	int  current_x;
	int  current_y;
	std::string image;
	std::shared_mutex cpos;
	std::condition_variable_any wait_cpos;

	void moveto(int, int);

	DisplayObject(const std::string&, const int);
	DisplayObject(const DisplayObject&);
	~DisplayObject();
	void update_contents(const std::string&);
	void draw(int, int, int lasttick = -1, int numticks = 1);
	static void redisplay();
	void move_to(int, int, bool, int, int);


private:
	static int tick;
	static std::condition_variable_any want_rw;
	static std::shared_mutex mtx;
	static const int LINELEN = 140;
	static const int NLINES = 60;
	static const int NLAYERS = 4;

	static char theFarm[NLINES][LINELEN][NLAYERS];

	void erase();
	void draw(int, int, const char);
};
