#pragma once 

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "Temp_communicate.hpp"
#include "menu_class.hpp"

//virtual parent class for most windows inside menu
class Static_window{ 
    protected:
        sf::RenderWindow& window;
        sf::Font MyFont;
        sf::Text title;
        Button  go_back;
        std::vector<Button*> buttons;
        Temp_communicate* communicate;
    public:    
        Static_window(sf::RenderWindow& mywindow): window(mywindow) {
            if (!MyFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")){
                std::cout<<"Why you don't have Arial? Are you crazy!? I was once crazy";
            }
            go_back = Button(MyFont, "goback",           5.f,5.f, 100.f, 20.f, 20);
            communicate = new Temp_communicate(MyFont, "", window, 3);
        }

        virtual ~Static_window(){
            if(communicate != nullptr){
                delete communicate;       
            }
        }

        //function used to set sf::text inside a window.
        void set_text(sf::Text& text, sf::Font& font, sf::String message, float x, float y, int font_size){
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(font_size);
        text.setColor(sf::Color::Magenta);
        text.setPosition(x ,y);
        }

        //virtual functions allowing children classes to draw/check actions additionaly, unique for them
        virtual void draw_child_content(){}
        virtual void check_child_actions(sf::Event event){}
    
    //function to read leaderboard- all the information about all players from a file
    std::vector<std::vector<std::string>> load_leaderboard(){
        std::vector<std::vector<std::string>>leaderboard;

        std::ifstream file("inc/menu_elements/leaderboard/leaderboard.csv"); 
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
        return {};
        }
        std::string line;
            
        float i=0;
        while (std::getline(file, line)) {
            i+=1;
            std::vector<std::string> words;
            std::stringstream ss(line);
            std::string word;

            while (std::getline(ss, word, ',')) {
                words.push_back(word);
            }
            leaderboard.push_back(words);
        }

        return leaderboard;
    }
    
    //main loop for each static window
    // handles displying the main window
    void display_window(){
        while (window.isOpen()){
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    window.close();
                }

                if (event.type == sf::Event::Resized) {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }

                if (event.type== sf::Event::MouseButtonPressed){
                    if(event.mouseButton.button== sf::Mouse::Left){
                        
                            if(go_back.isHovered(event.mouseButton.x, event.mouseButton.y)){
                                return;
                            }
                        check_child_actions(event); 
                        
                    }
                }
            }
            usleep(10000);
            window.clear();
            
            go_back.draw(window);
            
            draw_child_content();
            window.draw(title);
            window.display();
        }
    }

};