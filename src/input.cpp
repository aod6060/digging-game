#include "sys.hpp"
#include <algorithm>
#include <vector>


namespace input {

    static std::vector<InputState> keys;

    void init() {
        keys.resize(Keyboard::KB_MAX_SIZE);

        std::for_each(keys.begin(), keys.end(), [](InputState& state) {
            state = InputState::IS_RELEASED;
        });
    }

    void handleEvent(SDL_Event* e) {
        if(e->type == SDL_KEYDOWN) {
            if(keys[e->key.keysym.scancode] == InputState::IS_RELEASED) {
                keys[e->key.keysym.scancode] = InputState::IS_PRESSED_ONCE;
            }
        } else if(e->type == SDL_KEYUP) {
            if(keys[e->key.keysym.scancode] == InputState::IS_PRESSED) {
                keys[e->key.keysym.scancode] = InputState::IS_RELEASED_ONCE;
            }
        }
    }

    void update() {
        std::for_each(keys.begin(), keys.end(), [](InputState& state) {
            if(state == InputState::IS_PRESSED_ONCE) {
                state = InputState::IS_PRESSED;
            }

            if(state == InputState::IS_RELEASED_ONCE) {
                state = InputState::IS_RELEASED;
            }
        });

    }

    void release() {
        keys.clear();
    }

    bool isKeyReleased(Keyboard key) {
        return keys[key] == InputState::IS_RELEASED || keys[key] == InputState::IS_RELEASED_ONCE;
    }

    bool isKeyPressedOnce(Keyboard key) {
        return keys[key] == InputState::IS_PRESSED_ONCE;
    }

    bool isKeyPressed(Keyboard key) {
        return keys[key] == InputState::IS_PRESSED || keys[key] == InputState::IS_PRESSED_ONCE;
    }

    bool isKeyReleasedOnce(Keyboard key) {
        return keys[key] == InputState::IS_RELEASED_ONCE;
    }

}
