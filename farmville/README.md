Stefen Pegels (sgp62)

CLARIFICATIONS ABOUT HW3/ MY DESIGN DECISIONS

I decided to create 3 nests and two chickens, who walk counterclockwise, laying eggs in empty nests, in such a way that they do not collide with each other. Right now the chicken lays 3 eggs at once,
since the nest state depends on synchronization with chicken position, I am leaving 1-2-3 eggs to part 2.

The farmer goes in a circle, collecting eggs from full nests and stopping to milk the cow, and bringing these supplies to the Northernmost barn, the egg/butter barn.

The Southern barn, the flour/sugar barn, has an "infinite" supply, and requires no fieldwork to keep it restocked.

There are two trucks, and one goes to the North barn while one goes to the South barn. The path of the trucks is made so that they have no intersections.
Trucks also are "full" when they are full of 3 of one ingredient, not both. So the truck has to move eggs to the bakery, go back, get butter, and bring that to the bakery next, for example. The truck also sits for 10 seconds as produce is unloaded, in part 2, it will have to wait on the thread instead.

Sometimes, there's a truck at the top of the screen in the top left corner, which sits there for some reason. Not sure why; might be an issue with my window size. The other two trucks function normally.

Right now, 3 items of each ingredient populate along the conveyor belt periodically In part 2, I plan to have that third produce sit at the end of the conveyor belt, and become one of the two for the next batch. This was a synchronization decision that depended on mixer state, so I didn't include it. The produce will also only appear when the truck arrives, with synchronization.

The mixer randomly fills with all 8 ingredients before turning into two sets of batter that are transferred to the oven. In part 2, I'll have to synchronize the addition of ingredients as they hit the end of the conveyor belt.

The cupcake tray populates based on the batter in the oven, then resets after one second.

The children are randomly selected to move into the bakery, but they don't grab cupcakes at this time. They also currently can collide; this is a part 2 synchronization that will need to be implemented. 
I used a shared mutex to handle draw and redisplay synchronization, this was a static class variable, and the mutex is implemented in the class .cpp file. Draw grabs a shared lock, and redisplay grabs a unique lock on the mutex, to allow for multiple readers but only one writer. I also used a system of storing the ticks of redisplay and used that throughout the code, for functions that draw on a frequency that is an integer multiple of the redisplay frequency. This way, when redisplay frequency is changed, the drawing speed should scale as well.

Since the counters depended on the states of multiple threads, and in a larger sense required a complete pipeline of farm to bakery synchronization between threads, I left it to part 2.

I also used a lot of do-while loops for my animations, and when I got to the children, I found that creating a method to perform a general movement pattern was more efficient. In part 2, I may refactor some of the original code to make it less cumbersome.


