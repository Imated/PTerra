#include "Player.h"

#include "renderer/ShaderLibrary.h"

namespace Terra {
    void Player::init() {
        position = glm::vec2(0, 0);
        size = glm::vec2(1, 1);
        atlas = new Texture("resources/playerAtlas.png");
        shader = ShaderLibrary::get("player").get();
    }


}
