#pragma once

#include "static_window.hpp"



class ChooseMap: public Static_window{ //class to choose a map //#include <filesystem>
    private:
        std::string map;
        Player* player;
        bool is_new = false;
    public:
        
    
        ChooseMap(sf::RenderWindow& my_window, Player* my_player): Static_window(my_window), player(my_player){
            namespace fs = std::filesystem;
            std::string path="inc/map_elements/Maps/"; //folder where maps are stored
            int i=0; //index to keep track the number of maps and for displaying purposes
            try {
                for (const auto& entry : fs::directory_iterator(path)) {
                    if (entry.is_regular_file()) {  // check if a regular file
                        //buttons for existing maps
                        buttons.push_back( new Button(MyFont, entry.path().filename().string(), 20.f, 100.f + 50.f * i, 600.f, 30.f, 30, entry.path().filename().string())); 
                    }
                    i+=1;
                }
                //button for a new map
                buttons.push_back( new Button(MyFont, "new empty map",           20.f, 100.f + 50.f * i, 600.f, 30.f, 30, "map"+ std::to_string(i+1))); 

            //errors catching while reading names of files
            } catch (const fs::filesystem_error& err) { 
                std::cerr << "Filesystem error: " << err.what() << '\n'; 
            } catch (const std::exception& ex) {
                std::cerr << "General error: " << ex.what() << '\n';
            }
            set_text(title, MyFont, "Choose a map",           20.f,20.f, 70);
        }

        ~ChooseMap(){
            for(Button* button : buttons){
                delete button;
            }
            
        }

    void check_child_actions(sf:: Event event){ 
        for(int j=0; j<buttons.size(); ++j){
            Button* button= buttons[j];
            if(button->isHovered(event.mouseButton.x, event.mouseButton.y)){ 
                if(j != buttons.size()-1){//choosing existing maps
                map = button->hidden_text; 
                communicate->activate("map " + map + " chosen");
                }
                else{//last button - creating new map; allowed only for admins;
                    if(player->gettype() == "Admin"){
                        map = button->hidden_text;
                        is_new = true; //flag to know that new map is being created
                        communicate->activate("new map " + map + " created"); //but function inside map editor should create it and save- zadanie dla igora
                    }
                    else{
                        communicate->activate("must be admin\nto create map");// communicate if player wants to create map and is not admin
                    }
                }
            }
        }
    }

    std::string getmap(){
        return map;
    }

    bool is_new_map(){ return is_new;}

    //draws buttons and communicates;
    void draw_child_content() override{
        if(communicate){
            communicate->draw();
        }
        for(Button* button : buttons){
            button->draw(window);
        }
    }

};