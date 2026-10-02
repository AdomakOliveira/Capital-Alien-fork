#pragma once

#include "structs.hpp"
#include <string>

void Init();
void UpdatePre();
void Update();
void UpdatePost();
void Render3D();
void Render2D();
void Debug();

string ActionRollDice();
string ActionBuy();
string ActionBuild();
string ActionMortgage();
string ActionNegotiate();
string ActionEndTurn();