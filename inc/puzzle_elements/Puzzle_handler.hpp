#pragma once

#include "Knowledge.hpp"
#include "Maths.hpp"
#include "Sudoku.hpp"
#include "Puzzle.hpp"

class Puzzle_handler
{
    protected:
    public:
    Puzzle_handler(){}
    bool run(int type,int int_lvl)
    {
        char lvl = int_lvl+'0';
        Puzzle* puzzle = nullptr;
        if (type==1)
        {
            puzzle = new knowledge(type, lvl);
        }
        else if (type==2)
        {
            puzzle = new Maths(type, lvl);
        }
        else if (type==3)
        {
            puzzle = new Sudoku(type, lvl);
        }
        bool ans=true;
        ans = puzzle->solver_checker();
        delete puzzle;
        return ans;
    }
    //Puzzle* test=new Puzzle(0,1);
    //delete test;
};