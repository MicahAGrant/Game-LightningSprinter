#include "animated_sprite.h"

#include <iostream>

AnimatedSprite::AnimatedSprite(const std::vector<Sprite>& sprites, double dt_per_frame, int starting_frame)
    : sprites{sprites}, dt_per_frame{dt_per_frame}, current_frame{starting_frame} {}

void AnimatedSprite::flip(bool flip) {
    for (auto& sprite : sprites) {
        sprite.flip = flip;
    }
}

void AnimatedSprite::update(double dt) {
    if (dt_per_frame < 0) return;

    time += dt;
    if (time > dt_per_frame) {
        time -= dt_per_frame;
        current_frame = (current_frame + 1) % sprites.size();
    }
}

void AnimatedSprite::reset() {
    time = 0;
    current_frame = 0;
}

Sprite AnimatedSprite::get_sprite() {
    if (sprites.empty()) {
        throw std::runtime_error("AnimatedSprite has no frames");
    }
    return sprites.at(current_frame);
}

int AnimatedSprite::number_of_frames() {
    return sprites.size();
}
