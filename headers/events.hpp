#pragma once                                                                                                                                                                                                                                                                    

#include <string>
#include "raylib.h"
#include "structs.hpp"


bool MortgageProperty(Game& game, Player& player, House& house);

bool SellHousesOrHotels(Game& game, Player& player, House& house, uint8_t qnt);

void eventSelector(Game& game, House& house, Player& player, uint8_t dice);

void RefreshScreen(Game& game);