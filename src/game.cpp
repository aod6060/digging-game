#include "game.hpp"


namespace game {
    void init() {

    }

    void handleEvent(SDL_Event* e) {

    }

    void update(float delta) {
        std::cout << "Delta: " << delta << "\n";
    }

    void render() {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void release() {

    }

    void setup(app::Config* config) {
        config->caption = "Digging Game";
        config->width = 640;
        config->height = 480;
        // Setup callbacks
        config->initCB = init;
        config->handleEventCB = handleEvent;
        config->updateCB = update;
        config->renderCB = render;
        config->releaseCB = release;
    }


}