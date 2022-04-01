#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <time.h>

using namespace std;
using namespace sf;

#define scrwidth 300
#define scrheight 300

#define gamespd 0.5
#define sqrsize 30

class int2{
    public:
    int first;
    int second;
    int2(int FIRST, int SECOND){
        first = FIRST, second = SECOND;
    }
};

class float2{
    public:
    float first;
    float second;
    float2(float FIRST, float SECOND){
        first = FIRST, second = SECOND;
    }
};

int randomNumber(int min, int max){
    srand(clock());
    return rand()%(max-min+1)+min;
}

RenderWindow window(VideoMode(scrwidth, scrheight), "snake");
vector<int2> snake = {int2(0, 0)};
int2 apple(5, 0);

float dirx = 1;
float diry = 0;

void update(){
    time_t start = clock();
    while(true){
        if(!window.isOpen()){
            return;
        }
        if(float(clock()-start)/1000000 > gamespd){
            for(int i = snake.size()-1; i > 0; i--){
                snake[i].first = snake[i-1].first;
                snake[i].second = snake[i-1].second;
                if(snake[0].first + dirx == snake[i].first && snake[0].second + diry == snake[i].second){
                    snake = {int2(snake[0].first, snake[0].second)};
                }
            }
            if(snake[0].first < 0 || snake[0].first > int(scrwidth/sqrsize) || snake[0].second < 0 || snake[0].second > int(scrheight/sqrsize)){
                snake = {int2(snake[0].first, snake[0].second)};
                dirx *= -1;
                diry *= -1;
            }
            snake[0].first += dirx, snake[0].second += diry;
            if(snake[0].first == apple.first && snake[0].second == apple.second){
                apple.first = randomNumber(0, int(scrwidth/sqrsize)-1);
                apple.second = randomNumber(0, int(scrheight/sqrsize)-1);
                snake.push_back(int2(snake[snake.size()-1].first, snake[snake.size()-1].second));
            }
            start = clock();
        }
    }
}

int main(){
    Thread core1(&update);
    core1.launch();

    window.setFramerateLimit(30);
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
            else if(event.type == Event::KeyPressed){
                if(event.key.code == Keyboard::Up){
                    dirx = 0;
                    diry = -1;
                }
                if(event.key.code == Keyboard::Down){
                    dirx = 0;
                    diry = 1;
                }
                if(event.key.code == Keyboard::Left){
                    dirx = -1;
                    diry = 0;
                }
                if(event.key.code == Keyboard::Right){
                    dirx = 1;
                    diry = 0;
                }
            }
        }
        window.clear(Color::Black);
        RectangleShape rectangle;
        for(int i = 0; i < snake.size(); i++){
            rectangle.setFillColor(Color::White);
            rectangle.setPosition(Vector2f(snake[i].first*sqrsize, snake[i].second*sqrsize));
            rectangle.setSize(Vector2f(sqrsize, sqrsize));
            window.draw(rectangle);
        }
        rectangle.setFillColor(Color::Red);
        rectangle.setPosition(Vector2f(apple.first*sqrsize, apple.second*sqrsize));
        rectangle.setSize(Vector2f(sqrsize, sqrsize));
        window.draw(rectangle);
        window.display();
    }
}