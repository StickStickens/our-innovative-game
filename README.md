# our-innovative-game
simple game 

# description:
We present our fun game where you need to find a way out of a labirynth.
(before you start plaing you need to complete the menu- as it was designed as a mini escape-room)

In the real labirynth tiles colours have the following meaning:
- blue - a puzzle you need to solve
- broown - a pit you need to avoid
- yellow - a path you can wolk on
- pink - walls
- green - your destination

finish the maze while solving as many puzzles as possible to get highest number of points!

If you log in as a moderator you will be also able to create new levels in a interactive map creator

good luck :D


# running
**use a release verision**

or

**compile from source files:**
- download [sfml library](https://www.sfml-dev.org/download/) files and add to project directory 

- compile code using following commands:

```
g++ -c Project_C++.cpp -[directory to project]\SFML\include

g++ Project_C++.o -o Project_C++ -L"[directory to project]\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system
```

