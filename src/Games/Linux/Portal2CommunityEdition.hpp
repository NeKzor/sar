#pragma once
#include "Game.hpp"

class Portal2CommunityEdition : public Game {
public:
    Portal2CommunityEdition();
    void LoadOffsets() override;
    const char* Version() override;
    const float Tickrate() override;

    static const char* ModDir();
    static const char* GameDir();
};
