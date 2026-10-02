#pragma once

#include "structs.hpp"
#include <string>

void ReadConfig(Config& config);

void ClearFile(const string& filepath);

void SendFile(const Game& game, const string& filepath);

void RetrieveFile(Game& game, const string& filepath);
