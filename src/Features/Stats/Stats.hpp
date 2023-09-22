#pragma once
#include "Features/Feature.hpp"
#include "Features/Hud/Hud.hpp"
#include "JumpStats.hpp"
#include "StepStats.hpp"
#include "Variable.hpp"
#include "VelocityStats.hpp"

#include <vector>

struct PlayerStats {
	JumpStats *jumps;
	StepStats *steps;
	VelocityStats *velocity;
    int totalJumps = 0;
    int totalSteps = 0;
    int totalGives = 0;
    bool wOnly = true;
    bool mouseless = true;

	PlayerStats()
		: jumps(new JumpStats())
		, steps(new StepStats())
		, velocity(new VelocityStats()) {
	}
	void Reset() {
		this->jumps->Reset();
		this->steps->Reset();
		this->velocity->Reset();
        this->totalJumps = 0;
        this->totalSteps = 0;
        this->totalGives = 0;
	}
	~PlayerStats() {
		SAFE_DELETE(this->jumps);
		SAFE_DELETE(this->steps);
		SAFE_DELETE(this->velocity);
	}
};

class Stats : public Feature {
private:
	std::vector<PlayerStats *> playerStats;

public:
	Stats();
	~Stats();

	PlayerStats *Get(int nSlot);
	void ResetAll();
};

extern Stats *stats;

extern Variable sar_stats_jumps_xy;
extern Variable sar_stats_velocity_peak_xy;
extern Variable sar_stats_auto_reset;
