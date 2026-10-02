#include "structs.hpp"
#include "house.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "tabletop.hpp"
#include <random>
#include <algorithm>

void generateCards(Card cards_arr[], int FLAG);

void Init(Game& game, const Config& config){
    game.qntHouse = MAXHOUSES;
    game.qntPlayers = config.playerQnt;

    generateCards(game.chanceCards, 0);
    generateCards(game.chestCards, 1);

    for(int i = 0; i < MAXHOUSES; i++){
        game.houses[i] = BOARD_DATA[i];
    }

    Color defaultColors[MAXPLAYERS] = {RED, BLUE, GREEN, ORANGE, BROWN};

    for(int i = 0; i < game.qntPlayers; i++){
        game.players[i] = Constructor(i, config.playerNames[i], defaultColors[i % MAXPLAYERS], INITMONEY);
    }
}

Player& GetPlayer(Game& game){
    return game.players[game.playerIndex]; 
}

Player& GetPlayer(Game& game, uint8_t index){
    return game.players[index];
}

House& GetHouse(Game& game, uint8_t index){
    return game.houses[index];
}

uint8_t GetPlayerQnt(const Game& game){
    return game.qntPlayers;
}

uint8_t GetHouseQnt(const Game& game){
    return game.qntHouse;
}

uint16_t GetRound(const Game& game){
    return game.round;
}

void NextRound(Game& game){
    game.round++;
}

uint8_t NextPlayer(Game& game){
    do{
        ++game.playerIndex %= GetPlayerQnt(game);
    }
    while(GetPlayer(game).bankrupt);
    return game.playerIndex;
}

void generateCards(Card cards_arr[], int FLAG) {
    std::random_device rd;

    std::mt19937 gen(rd());

    if(FLAG) { 
        for(int i = 0; i < QNTCARDS; i++) {
            cards_arr[i] = CARDS_CHEST_DATA[i];
        }
    } else {
        for(int i = 0; i < QNTCARDS; i++) {
            cards_arr[i] = CARDS_CHANCE_DATA[i];
        }
    }
    std::shuffle(cards_arr, cards_arr + QNTCARDS, gen);
}