#ifndef SYS_HPP
#define SYS_HPP


// Standard library
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>


// 3rd party
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>



// App
namespace app {
    struct Config {
        std::string caption;
        uint32_t width;
        uint32_t height;

        // Callbacks
        std::function<void()> initCB;
        std::function<void(SDL_Event*)> handleEventCB;
        std::function<void(float delta)> updateCB;
        std::function<void()> renderCB;
        std::function<void()> releaseCB;
    };

    void init(Config* config);
    void update();
    void release();

    std::string getCaption();
    uint32_t getWidth();
    uint32_t getHeight();
    float getWidthFloat();
    float getHeightFloat();
    void exit();
    SDL_Window* getWindow();
    SDL_GLContext getContext();
    
}

#endif