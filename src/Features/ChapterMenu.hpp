#pragma once
#include "Features/Feature.hpp"

#include <vector>

#include "Utils/SDK.hpp"

#include "Game.hpp"

class ChapterMenu : public Feature {
private:
    std::vector<ChapterContextData_t> original;

public:
    ChapterMenu();
    ~ChapterMenu();
    void LoadMaps(SourceGameVersion version);
};

extern ChapterMenu* chapterMenu;
