#pragma once                                                                                                                                                                                                                                                                    

#include <string>
#include "structs.hpp"

bool SetOwner(House& house, int8_t newOwner);

Color GetHouseColor(const House& house);

int8_t GetOwner(const House& house);

bool Buy(House& house, Player& player);

uint16_t PayRent(Game& game, const House& house, Player& payer);

bool BuildHouse(Game& game, House& house, Player& player);

string GetName(const House& house);

uint8_t GetPrice(const House& house);

uint8_t GetValue(const Game& game, const House& house);

uint8_t GetResidencePrice(const House& house);

uint8_t GetHouseQnt(const House& house);

bool MortgageProperty(Game& game, Player& player, House& house);