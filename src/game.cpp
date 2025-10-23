#include "game.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "sys.hpp"


namespace game {

    int test = 0;
    bool flipped = false;

    void init() {
        render::addTextureFromFile("player", "data/textures/Player.png");

    }

    void handleEvent(SDL_Event* e) {

    }

    void update(float delta) {
        //std::cout << "Delta: " << delta << "\n"

        if(input::isKeyPressedOnce(input::Keyboard::KB_TAB)) {
            flipped = !flipped;
        }
    }

    void render() {

        render::clear(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        render::bind();

        render::setProjection(glm::ortho(0.0f, app::getWidthFloat(), app::getHeightFloat(), 0.0f));
        render::setView(glm::mat4(1.0f));
        render::setModel(
            glm::translate(glm::mat4(1000.0f), glm::vec3(32.0f, 32.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 0.0f))
        );

        render::setRenderMode(render::RenderMode::RENDER_MODE_WITH_TEXTURES);

        render::enableAlphaBlend();

        render::bindTexture(GL_TEXTURE0, "player");
        render::draw((flipped) ? render::TextureFlip::TF_FLIPPED : render::TextureFlip::TF_NORMAL);
        render::unbindTexture(GL_TEXTURE0);

        render::disableAlphaBlend();
        
        render::unbind();
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