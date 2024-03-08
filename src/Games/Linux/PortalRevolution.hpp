#pragma once
#include "Portal2.hpp"

class PortalRevolution : public Portal2 {
public:
    PortalRevolution();
    void LoadOffsets() override;
    const char* Version() override;

    static const char* ModDir();
    static const char* GameDir();
};
