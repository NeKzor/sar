#pragma once
#include "Replay.hpp"

#include "Features/Feature.hpp"

#include "Variable.hpp"
#include "Utils.hpp"

class ReplayPlayer : public Feature {
private:
    int viewIndex;
    bool isPlaying;

public:
    static int count;

public:
    ReplayPlayer();
    void Play(Replay* replay, CUserCmd* cmd);
    void StartPlaying(Replay* replay);
    void StopPlaying();
    bool IsPlaying() const;
};

extern ReplayPlayer* replayPlayer1;
extern ReplayPlayer* replayPlayer2;

extern Variable sar_replay_autoloop;
