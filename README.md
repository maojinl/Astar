# Astar
This is an A* path finding algorithm c++ implementation. This is a well-known path finding algorithm in computer gaming programming. The algorithm is extended from Dijkstra algorithm. We make a map with a matrix and 1 means this point could be passed through while 0 means not. The cost to the adjacent points is 10 for the vertical and horizontal points and 14 for the diagonally points. It would calculate the distance from the start point to the destination point with estimated cost for the un-investigated section. When we investigate the next point, we always chose which one has the smallest total cost (real cost + estimated cost).

I have optimized the algorithm with memory pool which can create large number of small objects without fetch memory from the system. Also it is optimized with heap for storing objects in the open list which contains all the points we need to investigate. So if we need to access the point which has the smallest total cost, we just need to get the first objects in the heap.

map.txt is pre-configured map file.
