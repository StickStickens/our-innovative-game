#pragma once

#include "player.hpp"

class Admin : public Player{ //can creaate new maps and edit them, ussually doesnt play the game itself

    public:
        Admin(int myid, std::string myname, int mybestscore, std::string mypassword){id=myid; username=myname;bestscore=mybestscore; password = mypassword;
        id=myid; points=0; vision=1000;}

        ~Admin(){}

        void update_player(int type, int weight, bool was_solved) override{return;};//admins do not get points        

        std::string gettype() const override{
            return "Admin";
        }
        void default_settings() override{
            points=0; vision=1000;
        }
};