#pragma once

#include "structs.hpp"
#include <string>

void SendFile(const Game& game, const string& filepath);

void RetrieveFile(Game& game, const string& filepath);
