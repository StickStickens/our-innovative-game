#pragma once

#include "static_window.hpp"




class Create_new : public Static_window{ //creating new player
    private:
        text_manager_menu* menager;
        int stage;
        Button student_state;
        Button lecturer_state;
        Button admin_state;
        std::string new_username;
        std::string password;
        std::string playertype;
        sf::Text succesfull_creation;
        std::string succesfull_creation_message;
        int proceed;
    public:


        Create_new(sf::RenderWindow& my_window) : Static_window(my_window){
            stage=-1;
            proceed=0;
            menager= new text_manager_menu(window);
            student_state= Button(MyFont, "i am student",           20.f,100.f, 300.f, 30.f, 30, "Student");
            lecturer_state= Button(MyFont, "i am lecturer",          20.f,140.f, 300.f, 30.f, 30, "Lecturer");
            admin_state= Button(MyFont, "i am admin",           20.f,180.f, 300.f, 30.f, 30, "Admin");
            buttons= {&student_state, &lecturer_state, &admin_state};

        }

        ~Create_new(){
            delete menager;
            buttons.clear();
        }
        
        
        //writes a created player to a file- to the leaderboard
        void write_player_to_file(std::string player_type, std::string name, std::string password){
            std::ofstream file("inc/menu_elements/leaderboard/leaderboard.csv", std::ios::app);
            if(!file){
                std::cerr<<"error opening leaderboard"<<std::endl;
                return;
            }

            std::ifstream filelines("inc/menu_elements/leaderboard/leaderboard.csv");
                int line=0;
                std::string temp;
                while(std::getline(filelines, temp)){
                    line+=1;
                }
            filelines.close();
            file<< "\n" << line+1 <<","<<name << ","<< "0" << "," << player_type << "," << password;

            file.close();
        }

        void check_child_actions(sf:: Event event){ //checks if any button was clicked. if so proceeds and saves new player type
            for(Button* button : buttons){
                if(button->isHovered(event.mouseButton.x, event.mouseButton.y)){
                    proceed=1;
                    playertype=button->hidden_text; 
                }
            }
        }
        
        //checks if entered new username is unique(must be)
        bool unique_name(std::string name){
            auto leaderboard = load_leaderboard();
            for(int i=0; i<leaderboard.size(); i++){
                if(leaderboard[i][1]==name){
                    return false;
                }
            }
            return true;
        }

        //checks if name is valid (doesn't contain ',')
        bool valid_name(std::string name){
            for(char letter : name){
                if(letter ==','){
                    return false;
                }
            }
            return true;
        }

        
        void draw_child_content() override{
            if(stage==-1){//draws student, lecturer, admin buttons
                
                for(Button* button : buttons){
                    button->draw(window);
                }
                if(proceed==1){
                    stage=0;
                }
                
            }
            if(stage==0){ //entering username of new user
                window.clear();
                std::string name = menager->get_text(50,50,"enter your username: ", 15, 30);
                if(! valid_name(name)){ //if name not valid goes to screen saying that player was not created
                    menager->temp_communicate->activate("invalid name\n(escape to leave)");
                    return;
                }
                if(name==";;;"){ //if escape was pressed goes to screen saying that player was not created
                    stage=4;
                    return;
                }

                if(unique_name(name)){ //if name is correct, proceeds
                    new_username=name;
                    stage=1;
                }
                else{//if name already exists
                    std::cerr<<"name not unique"<<std::endl;
                    menager->temp_communicate->activate("name already exists\n(escape to leave)");
                    return;
                }
            }
            if(stage==1){//entering password of new player
                window.clear();
                password = menager->get_text(50,50,"enter your password: ", 15, 30);
                if(! valid_name(password)){//if password invalid
                    menager->temp_communicate->activate("invalid password\n(escape to leave)");
                    return;
                }
                if(password==";;;"){//if escape was pressed goes to screen saying that player was not created
                    stage=4;
                    return;
                }

                write_player_to_file(playertype, new_username, password);//creates player
                stage=2137;
            } 
            if(stage==2137){
                window.clear();
                std::string number = menager->get_text(50,50,"enter your phone number.\nautomized system will call you\nto authorize account creation: ", 15, 30);
                if(number ==";;;"){
                    stage=4;
                    return;
                }
                stage=2;
            } 
            if(stage==2){//stage saying that player was successfully created
                std::string starstring;
                for(int i=0; i<password.size()-1;i++){starstring.append("*");}
                succesfull_creation_message = "successfully created player: " + new_username + "\nwith password: "+ password[0]+ starstring; 
                stage=3;
            }
            if(stage==3){//successful creation screen
                set_text(succesfull_creation, MyFont, succesfull_creation_message,50.f,30.f,30);
                window.draw(succesfull_creation);
            }
            if(stage==4){//unsuccesslu creation screen
                sf::Text unsuccessful_creating;
                std::string unsuccessful_creating_message = "not created account";
                set_text(unsuccessful_creating, MyFont, unsuccessful_creating_message,50.f,30.f,30);
                window.draw(unsuccessful_creating);
                return;
            }
            
        }

};