#pragma once

#include "player.hpp"


 //class student doesnt have speacial privileges but can acess most functionalities
class Student : public Player{

    public:
        Student(int myid, std::string myname, int mybestscore, std::string mypassword){id=myid; username=myname;bestscore=mybestscore; id=myid; password = mypassword;
        points=0; vision=4;
        }

        ~Student(){}

        //given whether puzzle was solved or not, provide all necessary updates to player
        //the vision expands or reduces depending if the puzzle was solved correctly,
        // the change is depended from the level of the puzzle and type - all puzzles have type 1-3 but sudokus are the most difficult and have the biggest prizes
        void update_player(int type, int weight, bool was_solved) override{
            if(was_solved){
                vision+=type;
                points+=type*weight;
            }
            else{
                vision-=type;
                points-=type*weight;
            }
            return;
        };        

        std::string gettype() const override{
            return "Student";
        }
        void default_settings() override{
            points=0; vision=4;
        }
};
