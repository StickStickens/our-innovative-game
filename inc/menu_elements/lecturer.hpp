#pragma once

#include "player.hpp"

// our VIP is treated in a special way- is less punished for not solving puzzles
class Lecturer : public Player{ 

    public:
        Lecturer(int myid, std::string myname, int mybestscore, std::string mypassword){id=myid; username=myname; bestscore=mybestscore; password = mypassword;
        id=myid; points=0; vision=8;}
        //given whether puzzle was solved or not, provide all necessary updates to player 
        //similar as in class student but the prizes are bigger and the punishments lower
        void update_player(int type, int weight, bool was_solved) override{
            if(was_solved){
                vision+=2*type;
                points+=2*type*weight;
            }
            else{
                vision-=round(type/2); 
                points-=round(type*weight/2); 
            }
            return;
        };                

        std::string gettype() const override{
            return "Lecturer";
        }
        void default_settings() override{
            points=0; vision=8;
        }
};