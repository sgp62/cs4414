Stefen Pegels (sgp62)

CLARIFICATIONS ABOUT HW4

First, occasionally, after recompiling, one or both of the chickens will not move from its initial position, as it should. I know this might incur a deduction, but I could not figure out how to solve this over the course of many hours, so you might have to recompile/rerun until both chickens move after laying their first batch of 3 eggs. It works I swear. This affects the rest of my supply chain, so it's important that they are moving around.

To handle most of my synchronization issues, I implemented reading and writing with a shared mutex whenever an object moves, so a moving object grabs a lock and prevents other objects from reading its position until it has completed the move action, to avoid improper reads that might be undefined.

My draw/redisplay method are synchronized with another shared mutex.

My cupcake/oven/mixer synchronization is implemented with a bounded buffer, where the oven is the producer and the children consumers. The random number of cakes the child gets frees up space in the buffer, while the cakes fill the space. The display section just displays the current number of filled spaces.

I have atomic ints in multiple places to protect variables that are changing and accessed between threads.

I have a special collision avoidance method, that is used as an optional parameter for the moveto method, which is the class method that any object calls when it wants to move to a specific position. The optional parameter is for another DisplayObject, and also has a given (x,y) tolerance. If the current object is too close to this specified object, it will stop. To prevent deadlock, the object that stops switches back and forth.

The trucks and chickens use this method to avoid colliding.

The chickens compete for the middle nest, with the first chicken to reach it laying two eggs and the second chicken only laying one.

Thanks for reading this, and I really enjoyed this assignment even if it took forever. Again, please recompile if the chickens don't want to move at the start.

