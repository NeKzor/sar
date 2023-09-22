#pragma once
#include "Features/Feature.hpp"

#include <string>
#include <vector>

#include "Utils/SDK.hpp"

#include "Game.hpp"

class ChallengeMode : public Feature {
public:
    static std::vector<ChallengeNodeData_t> customNodes;

public:
    ChallengeMode();
    ~ChallengeMode();
    void ReloadNodes();
    void LoadNodes(SourceGameVersion version);

    static void OnCheatsChanged(IConVar* pVar, const char* pOldString, float flOldValue);
};

extern ChallengeMode* cm;
