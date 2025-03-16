#pragma once
#include <vector> 
#include <fstream>
#include <random>
#include <cstring> 
#include <SFML/Graphics.hpp>
#include "../map_elements/Text_manager.hpp"
#include <cstdlib>

// a parent/abstract class storing all the common componets of the a puzzle, each puzzle of every type has 3 possible levels (1,2,3) 
//depending from the type  of puzzle the levels mean diffrent prizes
class Puzzle
{   
    protected:
        int id;
        char level;
        std::string puzzle_text;
        sf::RenderWindow window;
        text_manager* manager;
    public:
    Puzzle(int x, char y): id(x),level(y){}
    ~Puzzle(){
        delete manager;
        window.close();
    }
    //changes the variable puzzle_text
    void set_text(std::string str1)
    {
        puzzle_text=str1;
    }
    //reads appropriate file with puzzles and picks a puzzle with correct level on random
    virtual void read_puzzle() 
    {

    }
    //verifies the user's answear
    virtual bool solver_checker()
    {
        return true;
    }

};