# pragma once

#include <unistd.h>

#include "Map.hpp"
#include "Text_manager.hpp"
#include "View_manager.hpp"



//class that allows contruction of new maps and editing existing maps - available only in the admin mode
class Map_editor : public Map{
    using Map::Map;
    private:
        int tile_type;
        int end_amount = 0; //to see whether there exists exit
        bool is_player=false;
        text_manager* buffer;
        View_manager* manager;
        sf::Font my_font;

        void check_keyboard(){
            bool key_pressed=false;
            for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
                {
                    if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
                        key_pressed=true;
                }
            if(!key_pressed){return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){tile_type = 0; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){tile_type = 1; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){tile_type = 3; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){tile_type = 2; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){tile_type = 4; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){tile_type = 5; return;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){manager->move_view("up");}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){manager->move_view("down");}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){manager->move_view("left");}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){manager->move_view("right");}
            usleep(40000);
        }

        void change_grid_tile(){
            //get mouse position and then x, y - positions on a grid
            sf::Vector2f mousePosition = manager->get_mouse_position();

            int x = int(mousePosition.x/square_size);
            int y = int(mousePosition.y/square_size);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && manager->is_in_view(std::make_pair(x,y))){
                if(map_grid[x][y]==10){return;}
                if(map_grid[x][y]==4 and tile_type!=4){end_amount-=1;}
                if(map_grid[x][y]==2 and tile_type!=2){puzzle_info.erase(100*x+y);}
                if(map_grid[x][y]==3 and tile_type!=3){is_player=false;}
                else if(tile_type==4 && map_grid[x][y]!=4){end_amount+=1;}
                if(tile_type==3){
                    //there can be only 1 started point, and code below makes sure of it
                    if(player_coords.first!=-1 && player_coords != std::make_pair(x,y)){
                        map_grid[player_coords.first][player_coords.second]=0;}
                    player_coords = std::make_pair(x,y);
                    is_player=true;
                }
                else if(tile_type==2){
                    int puzzle_type, puzzle_diff;
                    // enter puzzle_type and puzzle_diff in the window
                    //It makes most screen goes black, but it helps to see where to write info
                    try{
                        puzzle_type = stoi(get_puzzle_text("enter puzzle type: "));
                        window.clear();
                        puzzle_diff = stoi(get_puzzle_text("enter puzzle diff: "));
                        //insert data to map dict which stores info about all puzzles
                        puzzle_info.erase(100*x+y);
                        puzzle_info.insert({100*x+y, std::make_pair(puzzle_type, puzzle_diff)});
                        window.clear();
                    }
                    catch(...){
                        //Exception if you wrote string which is not an integer
                        buffer->put_text_to_window(square_size*(manager->get_window_x()-9), square_size*0.125,
                        "Invalid data type; please enter 2 integers", square_size/2, 1000);
                        return;

                    }
                }
                if(map_grid[x][y]==3 && tile_type!=3){is_player=false;}
                map_grid[x][y] = tile_type;
            }
        }

        void display_menu(sf::Font my_font){
            buffer->draw_menu_marker(square_size);
            buffer->put_text_to_window(square_size*0.5, square_size*0.2, "Current tile type is: "
                + std::to_string(tile_type), square_size/2);
        }
    public:
        //display which tile are you using (useful as I forgot that many times)
        //Main function of this class, combining all others
        //To edit_map, just create class and call function below
        void edit_map(){

            window.create(sf::VideoMode(20*square_size,16*square_size), "My_Game");
            buffer = new text_manager(window);
            manager = new View_manager(window, x_size, y_size, square_size);
            manager->settle_view(std::make_pair(0,0));
            get_map_info();
            if (!my_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")){
                std::cout<<"Why you don't have Arial? Are you crazy!? I was once crazy";
            }
            buffer->put_text_at_center("Welcome to map editor!", square_size, 1000);
            while (true)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        if(!is_player){
                            buffer->put_text_to_window((manager->get_window_x()-9)*square_size, 10,
                            "Invalid map creation. Add player", square_size/2, 1000);
                        }
                        else if(end_amount==0){
                            buffer->put_text_to_window((manager->get_window_x()-9)*square_size, 10,
                            "Invalid map creation. Add ending point", square_size/2, 1000);
                        }
                        else{return window.close();}
                }
                check_keyboard();
                change_grid_tile();
                window.clear();
                draw_grid(x_size+y_size+2);
                display_menu(my_font);
                window.display();
            }
        }
    std::string get_puzzle_text(std::string communicate){
        std::string ans;
        sf::Event event;
        while (true)
        {
            draw_grid(x_size+y_size+2);
            display_menu(my_font);
            while (window.pollEvent(event))
            {
                //here window is also displayed
                if(buffer->get_text((manager->get_window_x()-6)*square_size, 0.2*square_size, communicate,
                    ans, event, 1, square_size*0.45)){ 
                        if(ans[0]-'0'<1 || ans[0]-'0'>3){
                            buffer->put_text_to_window((manager->get_window_x()-9)*square_size, 10,
                            "Invalid puzzle type. Enter 1, 2 or 3", square_size/2, 700);
                            ans = "";
                            continue;
                        }
                        return ans;
                    };
            }
            window.clear();
        } 
    }

    // getting map info including if there exists an exit ( there must) and where the player is
    void get_map_info(){
        for(int i=0; i<x_size; i++){
            for(int j=0; j<y_size; j++){
                if(map_grid[i][j]==4){ end_amount++;}
                if(map_grid[i][j]==3){ player_coords = std::make_pair(i,j); is_player = true;}
            }
        }
        tile_type = 0;
    }
};