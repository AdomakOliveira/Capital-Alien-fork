#pragma once

#include "structs.hpp"

void RenderHouse(Game game);

void RenderName(Game game);

void TurnAlert(Game game, int clientIndex);

void RenderRound(Game game);

void RenderMoney(Game game);

void RenderMenu(Game& game, int clientIndex, string& PATH);

bool BotaoAcao(Rectangle area, const char* texto);

bool RenderButtons(Game& game, int clientIndex);

void RenderHouseInfo(Game game);

bool IsInMenu();
