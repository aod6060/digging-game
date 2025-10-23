#include "SDL.h"
#include "SDL_events.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "sys.hpp"


namespace app {

    static Config* g_config;
    static bool g_running = true;
    static SDL_Window* g_window = nullptr;
    static SDL_GLContext g_context = nullptr;

    void init(Config* config) {
        g_config = config;

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // create window
        g_window = SDL_CreateWindow(
            g_config->caption.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            g_config->width, 
            g_config->height, 
        SDL_WINDOW_OPENGL);

        // create opengl context and init glew
        g_context = SDL_GL_CreateContext(g_window);
        glewInit();

        // Initing input
        input::init();

        // Call game init function
        if(g_config->initCB) {
            g_config->initCB();
        }
    }

    void update() {
        SDL_Event event;

        uint32_t pre_time = SDL_GetTicks();
        uint32_t curr_time = 0;
        float delta = 0.0f;

        while(g_running) {
            // Calculating Frame Time
            curr_time = SDL_GetTicks();
            delta = (curr_time - pre_time) / 1000.0f;
            pre_time = curr_time;

            // Event Loop
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    app::exit();
                }

                input::handleEvent(&event);

                if(g_config->handleEventCB) {
                    g_config->handleEventCB(&event);
                }
            }


            // Handle Callbacks for update and render
            if(g_config->updateCB) {
                g_config->updateCB(delta);
            }

            if(g_config->renderCB) {
                g_config->renderCB();
            }

            input::update();

            // Swaping window
            SDL_GL_SwapWindow(g_window);
        }

    }

    void release() {
        if(g_config->releaseCB) {
            g_config->releaseCB();
        }

        input::release();
        
        SDL_GL_DeleteContext(g_context);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
    }

    std::string getCaption() {
        return g_config->caption;
    }

    uint32_t getWidth() {
        return g_config->width;
    }

    uint32_t getHeight() {
        return g_config->height;
    }

    float getWidthFloat() {
        return (float)getWidth();
    }

    float getHeightFloat() {
        return (float)getHeight();
    }

    void exit() {
        g_running = false;
    }

    SDL_Window* getWindow() {
        return g_window;
    }

    SDL_GLContext getContext() {
        return g_context;
    }

    
}