#pragma once
#include <thread>

#include "Features/Feature.hpp"

#include "Modules/Console.hpp"
#include "Modules/Module.hpp"

#include "Cheats.hpp"
#include "Command.hpp"
#include "Game.hpp"
#include "Interface.hpp"
#include "Variable.hpp"

#define SAR_VERSION "1.11"
#define SAR_BUILD __TIME__ " " __DATE__
#define SAR_WEB "https://sar.portal2.sr"

#define SAFE_UNLOAD_TICK_DELAY 33

class SAR {
public:
    Modules* modules;
    Features* features;
    Cheats* cheats;
    Game* game;

public:
    SAR();

    virtual bool Load();
    virtual void Unload();

    inline const char* Version() { return SAR_VERSION; }
    inline const char* Build() { return SAR_BUILD; }
    inline const char* Website() { return SAR_WEB; }
};

extern SAR sar;
