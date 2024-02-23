#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event e;

    enum Direction {
        DOWN,
        UP,
        LEFT,
        RIGHT
    };    

    //Head
    SDL_Rect head {500, 500, 10, 10};

    //Body Container
    deque<SDL_Rect> rq;
    int size = 1;

    //Apple Container
    vector<SDL_Rect> apples;

    // Crate apples on the map
    for(int i = 0; i<100; i++) {
        apples.emplace_back(rand()%100*10, rand()%100*10, 10, 10);
    }
    

    bool running = true;
    int dir = 0;

    //Main Loop
    while(running) {
        //Check Input
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) { running = false; }
            else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
                else if(e.key.keysym.sym == SDLK_UP) { dir = UP; }
                else if(e.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
                else if(e.key.keysym.sym == SDLK_RIGHT) { dir = RIGHT; }
            }
        }

        //Move snake in direction
        switch(dir) {
            case DOWN: head.y += 10; break;
            case UP: head.y -= 10; break;
            case LEFT: head.x -= 10; break;
            case RIGHT: head.x += 10; break;
        }

        //Collision with apple
        for_each(apples.begin(), apples.end(), [&](auto& apple) {
            if(head.x == apple.x && head.y == apple.y) {
                size += 10;
                apple.x = -10;
                apple.y = -10;
            }
        });

        //Collision with snake
        for_each(rq.begin(), rq.end(), [&] (auto& snake_segment) {
            if(head.x == snake_segment.x && head.y == snake_segment.y) {
                size = 1;
            }
        });

        //Add newest head to the snake
        rq.push_front(head);

        while(rq.size() > size) rq.pop_back();

        //Clear the window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Draw Body
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for_each(rq.begin(), rq.end(), [&](auto& snake_segment) {
            SDL_RenderFillRect(renderer, &snake_segment);
        });

        //Draw Apples
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for_each(apples.begin(), apples.end(), [&](auto& apple) {
            SDL_RenderFillRect(renderer, &apple);
        });

        //Display
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
    }
    return 0;
}
