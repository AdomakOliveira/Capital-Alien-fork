#pragma once                                                                                                                                                                                                                                                                    

#include <string>
#include "raylib.h"
#include "structs.hpp"

void ResolvePayment(Game& game);

bool SellHousesOrHotels(Game& game, Player& player, House& house, uint8_t qnt);

void EventSelector(Game& game, House& house, Player& player, uint8_t dice);