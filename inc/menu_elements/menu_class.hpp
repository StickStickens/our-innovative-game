#pragma once

#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include<sstream>
#include "../map_elements/Map_manager.hpp"
#include "button.hpp"
#include "Temp_communicate.hpp"
#include "player.hpp"
#include "unlogged.hpp"
#include "student.hpp"
#include "lecturer.hpp"
#include "admin.hpp"
#include "choose_player.hpp"
#include "authors.hpp"
#include "leaderboard.hpp"
#include "create_new.hpp"
#include "authors.hpp"
#include "choose_map.hpp"
#include "my_stats.hpp"
#include "static_window.hpp"
#include "text_manager_menu.hpp"

class Menu{
    private:
        std::string map;
        sf::RenderWindow& window;
        sf::Font MyFont;
        sf::Text title;
        //new button must be implemented here and in custom constructor (in 2 places); 
        Button  play_game, options, choose_existing_player, create_new, end_game, creators, show_leaderboard,
        edit_map, my_stats, log_out, choose_map;
        sf::Color background_color;
        std::vector<Button*> buttons;
        int typer, typeg, typeb; //specifies if background color should increase(1) or decrease (-1)
        Player* player;
        std::vector<Button*> pressed_buttons;
        Temp_communicate* communicate;
        Map_manager* map_manager;
        int is_new, tile_size;


    public:
        ~Menu(){
            buttons.clear();
            pressed_buttons.clear();
    
            if(player != nullptr){
                delete player;
            }
            delete communicate;
            
        }

        Menu(sf::RenderWindow& my_window, int size): window(my_window), tile_size(size) {
            if (!MyFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")){
                std::cout<<"Why you don't have Arial? Are you crazy!? I was once crazy";
            }
            player= new Unlogged();
            map="";
            set_text(title, MyFont, "Our Innovative Labirynth",                20.f,20.f, 70);
            play_game = Button(MyFont, "-play our brilliant game",           20.f,100.f, 600.f, 30.f, 30);
            options = Button(MyFont, "-to be added",  20.f,160.f, 600.f, 30.f, 30);
            choose_existing_player = Button(MyFont, "-log in",               20.f,220.f, 600.f, 30.f, 30);
            create_new = Button(MyFont, "-create account",                   20.f,280.f, 600.f, 30.f, 30);
            creators = Button(MyFont, "-creators",                           20.f,340.f, 600.f, 30.f, 30);
            show_leaderboard = Button(MyFont, "-show leaderboard",           20.f,400.f, 600.f, 30.f, 30);
            edit_map = Button(MyFont, "-edit map",                           20.f,460.f, 600.f, 30.f, 30);
            log_out = Button(MyFont, "-log out",                             20.f,520.f, 600.f, 30.f, 30);
            choose_map = Button(MyFont, "-choose map",                       20.f,580.f, 600.f, 30.f, 30);
            end_game = Button(MyFont, "-leave game",                         20.f,640.f, 600.f, 30.f, 30);
            my_stats = Button(MyFont, player->gettype() + " " + player->getname(), window.getSize().x-515 , 10.f, 300.f, 15.f, 15); //maybe should change placec to always be rigght upper corner
           
            buttons= {&play_game, &options, &choose_existing_player, &create_new, &end_game
            , &creators, &show_leaderboard, &edit_map, &my_stats, &log_out, &choose_map}; //vector of buttons so i can iterato over them

            //setting window parameters
            background_color= sf::Color(100, 100, 100);
            typer=1; typeg=1; typeb=1;
            communicate= new Temp_communicate(MyFont, "game started", window, 3);
            map_manager = new Map_manager(window);
        };

        //function used to set sf::text inside a window. probably should be moved to text_menager
         void set_text(sf::Text& text, sf::Font& font, sf::String message, float x, float y, int font_size){
             text.setFont(font);
             text.setString(message);
             text.setCharacterSize(font_size);
             text.setColor(sf::Color::Magenta);
             text.setPosition(x ,y);
        }

        void set_window_size(int x, int y) {
            window.setSize(sf::Vector2u(x, y));
        }

        void set_window_position(int x, int y) {
            window.setPosition(sf::Vector2i(x, y));
        }

        Player* get_player(){
            return player;
        }

        void change_background(){ //primitive function that changes background color
            
            if(typer==1){
                if(background_color.r<255){
                    background_color.r=(background_color.r+1);
                }
                else{
                    typer=-1;
                }
            }
            else{
                if(background_color.r>0){
                    background_color.r=(background_color.r-1);
                }
                else{
                    typer=1;
                }
            }

            if(typeg==1){
                if(background_color.g<254){
                    background_color.g=(background_color.g+2);
                }
                else{
                    typeg=-1;
                }
            }
            else{
                if(background_color.g>1){
                    background_color.g=(background_color.g-2);
                }
                else{
                    typeg=1;
                }
            }

            if(typeb==1){
                if(background_color.b<254){
                    background_color.b=(background_color.b+2);
                }
                else{
                    typeb=-1;
                }
            }
            else{
                if(background_color.b>0){
                    background_color.b=(background_color.b-1);
                }
                else{
                    typeb=1;
                }
            }
        
        }
        
        //main loop of Menu class 
        std::string display_menu(){
            set_window_size(1200, 900);
            set_window_position(50, 100);
            while (window.isOpen()){
                sf::Event event;
                while (window.pollEvent(event))
                {

                    //handling resizing of window
                    if (event.type == sf::Event::Resized) {
                        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                        window.setView(sf::View(visibleArea));
                    }

                    //if button pressed makes visal effect of button
                    if (event.type == sf::Event::MouseButtonPressed){
                        if(event.mouseButton.button== sf::Mouse::Left){
                            for(Button* button: buttons){
                                if(button->isHovered(event.mouseButton.x, event.mouseButton.y)){
                                    button->press_visual();
                                    pressed_buttons.push_back(button);
                                }
                            }
                        }
                    }

                     
                    if (event.type== sf::Event::MouseButtonReleased){
                        Button* presscheck= nullptr; //variable to avoid trigerring button when pressed outside button
                        if(pressed_buttons.size()>0){//clears visuals for all pressed butons
                            presscheck=pressed_buttons[0];
                        }
                        for(Button* button: pressed_buttons){
                            button->unpress_visual();
                        }
                        pressed_buttons.clear(); //removes all buttons from a vector

                        if(event.mouseButton.button== sf::Mouse::Left){
                            for(Button* button: buttons){
                                if(button->isHovered(event.mouseButton.x, event.mouseButton.y)){
                                    if(presscheck != nullptr && presscheck==button){
                                        //checking if buttons were pressed. if so doing appropriate action
                                        if(*button==play_game){
                                            if(player->gettype() != "Unlogged" && map != ""){
                                                return "Play";
                                            }
                                            else if(player->gettype() == "Unlogged" ){
                                                std::cerr<<"you must choose player"<<std::endl;
                                                communicate->activate("must be logged in");
                                            }
                                            else if(map == ""){
                                                std::cerr<<"you must choose player"<<std::endl;
                                                communicate->activate("must choose map");
                                            }
                                        }
                                        if(*button==options){
                                            communicate->activate("not working yet");
                                            
                                        }
                                        if(*button==choose_existing_player){
                                            auto choose_player= std::make_unique<Choose_player>(window);
                                            choose_player->display_window();
                                            player= choose_player->get_player(); //getting a player to a class
                                            std::string temp= player->gettype() + " " + player->getname();
                                            my_stats.set_text(temp); //updating the button of current player
                                            if(player->gettype() != "Unlogged" ){
                                                communicate->activate("logged in");
                                            }
                                            else{
                                                communicate->activate("not logged in");
                                            }
                                        }
                                        if(*button==create_new){
                                            auto newplayer= std::make_unique<Create_new>(window);
                                            newplayer->display_window();
                                        }
                                        if(*button==creators){
                                            auto creators = std::make_unique<Authors>(window);
                                            creators->display_window();
                                        }
                                        if(*button==show_leaderboard){
                                            auto leaderboard = std::make_unique<Leaderboard>(window);
                                            leaderboard->display_window();
                                        }
                                        if(*button==edit_map){
                                            if(player->gettype()=="Admin"){
                                                if(map != ""){
                                                    Map_editor* editor;
                                                    if(is_new){
                                                        int x, y;
                                                        std::cout<<"enter x and y; specifing map sizes";
                                                        std::cin>>x>>y;
                                                        editor = new Map_editor(x,y,tile_size,window);
                                                    }
                                                    else{ map_manager->read_map_from_file("inc/map_elements/Maps/" + map, editor);}
                                                    editor->edit_map();
                                                    map_manager->write_map_to_file("inc/map_elements/Maps/" + map, editor);
                                                    delete editor;
                                                    window.create(sf::VideoMode(1200, 900), "Menu");
                                                    return "Continue";
                                                }
                                                else{
                                                    communicate->activate("must choose map");
                                                }
                                            
                                            }
                                            else{
                                                communicate->activate("must be admin");
                                            }
                                        }
                                        if(*button==log_out){
                                            if(player->gettype()!="Unlogged"){
                                            player= new Unlogged();
                                            std::string temp= player->gettype() + " " + player->getname();
                                            my_stats.set_text(temp);
                                            }
                                            else{
                                                communicate->activate("already logged out");
                                            }
                                        }
                                        if(*button==choose_map){
                                            if(player->gettype()!="Unlogged" ){
                                                auto choosemap = std::make_unique<ChooseMap>(window, player);
                                                choosemap->display_window();
                                                map = choosemap->getmap();
                                                is_new = choosemap->is_new_map();

                                                communicate->activate("map " + map + " chosen");
                                            }
                                            else{
                                                std::cerr<<"must be logged in"<<std::endl;
                                                communicate->activate("must be logged in");
                                            }
                                        }
                                        if(*button==end_game){
                                            communicate->activate("leaving");
                                            communicate->draw();
                                            window.display();
                                            usleep(1000000);
                                            window.close();
                                            return "Exit";
                                        }
                                        
                                        if(*button==my_stats){ 
                                            auto mystats = std::make_unique<My_stats>(window, player);
                                            mystats->display_window();
                                            
                                        }
                                    }
                                }
                            }
                        }
                    }
                        
                }
                change_background();
                //drawing of content that is displayed
                usleep(10000);
                window.clear(background_color);
                for(Button* button : buttons){
                    button->draw(window);
                }
                window.draw(title);
                communicate->draw();
                
                window.display();
            }
            return "Continue";
        }


        

        //getter of a map path
        std::string choose_mapa(){
            return "inc/map_elements/Maps/" + map;
        };

};