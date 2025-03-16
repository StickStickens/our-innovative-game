#pragma once

#include "static_window.hpp"
#include "text_manager_menu.hpp"


class Choose_player : public Static_window{ //class for a window to log in
    private:
        int stage;
        text_manager_menu* menager;
        std::string username;
        std::string password;
        int id; 
        std::vector<std::vector<std::string>>leaderboard;
        Player* player;
        
    public:
        

        Choose_player(sf::RenderWindow& my_window) : Static_window(my_window){
            stage=0;
            menager= new text_manager_menu(window);
            leaderboard=load_leaderboard();
            player= new Unlogged();
        }

        ~Choose_player(){
            delete menager;
            
        }

        void draw_child_content() override{    
            //stage=0 - entering username 
            if(stage==0){
                window.clear();
                username=menager->get_text(50,50,"enter your username: ", 15, 30);
                if(username==";;;"){ // if escape pressed go to screeen saying you did not log
                    stage=4;
                    return;
                }
                //check if entered name already exists
                if(isvalid(username, 1)){
                stage=1;
                }
                else{
                    std::cerr<<"incorrect name"<<std::endl;
                    menager->temp_communicate->activate("wrong name\nescape to leave");
                    return;
                    }
            }
            //stage=1 - entering your password
            if(stage==1){
                window.clear();
                password=menager->get_text(50,50,"enter your password: ", 15, 30);
                if(password==";;;"){// if escape pressed go to screen saying you did not log
                    stage=4;
                    return;
                }

                if(leaderboard[id-1][4]==password){
                stage=2;
                }
                else{
                std::cerr<<"incorrect passwordd"<<std::endl;
                menager->temp_communicate->activate("wrong password\n(escape to leave)");
                return;
                }
            }
            if(stage==2){ //creating instance of a class player, to which user has logged in
                std::vector<std::string> player_row = leaderboard[id-1];
                    player=create_player(player_row);
                
                stage=3;
            }
            if(stage==3){//end screen of succesfull logging
                sf::Text successful_logging;
                std::string successful_logging_message = "successfully logged as:\n" + player->getname();
                set_text(successful_logging, MyFont, successful_logging_message,50.f,30.f,30);
                window.draw(successful_logging);
                
                return;
                
            }
            if(stage==4){//end screen of not successful logging
                sf::Text unsuccessful_logging;
                std::string unsuccessful_logging_message = "not logged in";
                set_text(unsuccessful_logging, MyFont, unsuccessful_logging_message,50.f,30.f,30);
                window.draw(unsuccessful_logging);
                return;
            }
        }

        Player* create_player(std::vector<std::string> player_row){ //depending on a player type creating admin,lecturer or student

            if(player != nullptr){
                delete player;
            }
            Player* myplayer;
            if(player_row[3]=="Student"){
               myplayer= new Student(std::stoi(player_row[0]), player_row[1], std::stoi(player_row[2]), player_row[4]);
            }
            else if(player_row[3]=="Lecturer"){
                myplayer= new Lecturer(std::stoi(player_row[0]), player_row[1], std::stoi(player_row[2]), player_row[4]);
            }
            else if(player_row[3]=="Admin"){
                myplayer=new Admin(std::stoi(player_row[0]), player_row[1], std::stoi(player_row[2]), player_row[4]);
            }
            else{ std::cerr<< "undefined error while logging into system";}
            return myplayer;
        }

        //checks if username is valid- if already exists
        bool isvalid(std::string text, int column){
            int i=0;
            for(auto row : leaderboard){
                if(text==row[column]){
                    id=i+1;
                    return true;
                }
                i+=1;
            }
            return false;

        }
        //getter of player
         Player* get_player(){
             return player;
         }

};