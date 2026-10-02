#include <string>
#include "house.hpp"
#include "raylib.h"
#include "structs.hpp"
#include "tabletop.hpp"
#include "player.hpp"


House Constructor(const string& name, Color color, uint8_t value, uint8_t price, uint8_t tier, HSETYPE type){
    return House{name, color, (uint8_t)type, value, price};
}

string GetName(const House& house){
    return house.name;
}

uint8_t GetPrice(const House& house){
    return house.price;
}

uint8_t GetResidencePrice(const House& house){
    return house.residencePrice;
}

uint8_t GetHouseQnt(const House& house){
    return house.housesBuilt;
}

uint8_t GetValue(const Game& game, const House& house){
        uint32_t value = 0;
    if(house.type == NORMAL) {
        switch(house.housesBuilt) {
        case 0:
            value = house.value;
            break;
        case 1:
            value = house.value * 5;
            break;
        case 2:
            value = house.value * 15;
            break;
        case 3:
            value = house.value * 40;
            break;
        case 4:
            value = house.value * 60;
            break;
        case 5:
            value = house.value * 80;
            break;
        }
    } else if(house.type == RAILROAD) {
        uint8_t count = 0;
        for(int i = 5; i < 36; i += 10) {
            if(game.houses[i].owner == house.owner) count++;
        }
        switch(count) {
        case 1:
            value = 25;
            break;
        case 2:
            value = 50;
            break;
        case 3:
            value = 100;
            break;
        case 4:
            value = 200;
            break;
        }
    }
    return value;
}

bool SetOwner(House& house, int8_t newOwner){
    int8_t lastOwner = house.owner;
    if(house.type == NORMAL || house.type == RAILROAD || house.type == COMPANY){
        house.owner = newOwner;
    }
    return lastOwner != house.owner;
}

void SetHouseColor(House& house, Color color){
    house.color = color;
}

Color GetHouseColor(const House& house){
    return house.color;
}

int8_t GetOwner(const House& house){
    return house.owner;
}

bool Buy(House& house, Player& player){
    if(house.owner != -1){
        return false;
    }
    if(player.money < house.price || !house.price){
        return false;
    }

    RemoveMoney(player, house.price);
    SetOwner(house, GetID(player));
    return true;
}

uint16_t PayRent(Game& game, const House& house, Player& payer){
    TransferMoney(payer, GetPlayer(game, uint8_t(GetOwner(house))), house.value);
    return house.value;
}

bool BuildHouse(Game& game, House& house, Player& player){
    if(house.type != NORMAL) return false;
    if(house.owner != player.ID) return false;
    if(house.mortgaged) return false;
    if(house.housesBuilt >= 5) return false;

    uint8_t groupSize = 0;
    uint8_t ownedInGroup = 0;
    for(int i = 0; i < game.qntHouse; i++) {
        House& current = game.houses[i];
        if(current.type == NORMAL && ColorToInt(current.color) == ColorToInt(house.color)) {
            groupSize++;
            if(current.owner == player.ID) ownedInGroup++;
        }
    }
    if(ownedInGroup < groupSize) return false;

    if(player.money < house.residencePrice) return false;

    RemoveMoney(player, house.residencePrice);
    house.housesBuilt++;

    if(house.housesBuilt == 5) {
        game.hotelsBuilt++;
    } else {
        game.housesBuilt++;
    }

    return true;
}
