#pragma once

#include "player.hpp"


//state of a player when one didn't log into any account. unlogged type doesn't have permission to use many functionalities
// usually an intermediete state before logging in
class Unlogged : public Player{ 

    public:
        Unlogged(){
            id=NULL; bestscore=NULL, password="", username=" ";
        }             

        ~Unlogged(){}

        std::string gettype() const override{
            return "Unlogged";
        }
        void default_settings() override{
            vision=0; points=-10;
        };
};
