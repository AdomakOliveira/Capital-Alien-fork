#include <string>
#include "house.hpp"
#include "raylib.h"
#include "structs.hpp"
#include "tabletop.hpp"
#include "player.hpp"


House Constructor(const string& name, Color color, uint8_t value, uint8_t price, uint8_t tier, HSETYPE type){
    return House{name, color, (uint8_t)type, value, price};
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

// Inicia um leilão para a casa em que o jogador parou (usado quando um
// jogador decide não comprar a propriedade)
void auctionEvent(Game& game, Player& player, House& house) {
    game.eventDecision.action = EVENT_ACTION::AUCTION;
    game.eventDecision.houseId = player.houseIndex;
    game.auction.active = true;
    game.auction.houseId = player.houseIndex;
    game.auction.currentBid = 0;
    game.auction.highestBidder = -1;
    for(int i = 0; i < game.qntPlayers; i++) {
        if(!game.players[i].bankrupt) {
            game.auction.currentPlayer = i;
            break;
        }
    }
}
