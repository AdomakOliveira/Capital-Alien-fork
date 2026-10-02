#include <iostream>
#include <ctime>
#include "game_logic.hpp"
#include "game_renderer.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include "constants.hpp"
#include "events.hpp"
#include "filehandler.hpp"

Game mainGame;
int clientIndex = 0;

bool rolledThisTurn = false;
bool gameOver = false;
string winnerName = "";

bool pressedButton = false;

string MoveAndTriggerEvent(Player& player, int dado1, int dado2, int total, const string& actorLabel){
    uint8_t maxHouses = GetHouseQnt(mainGame);
    bool passedStart = false;

    for(int i = 0; i < total; i++){
        if(NextHouse(player, maxHouses)){
            passedStart = true;
        }
    }

    if(passedStart){
        AddMoney(player, 200);
    }

    House& house = GetHouse(mainGame, GetPos(player));
    eventSelector(mainGame, house, player, (uint8_t)total);

    string msg = actorLabel + " tirou " + to_string(dado1) + " e " + to_string(dado2)
               + " (" + to_string(total) + ") e caiu em " + house.name;

    if(passedStart){
        msg += ". Passou pelo Inicio e recebeu $200";
    }
    if(player.bankrupt){
        msg += ". " + GetName(player) + " faliu!";
    }

    return msg;
}

void Init(){
    SetRandomSeed((unsigned int)time(NULL));

    Init(mainGame, array_size(BOARD_DATA), 2);

    GetPlayer(mainGame, 0) = Constructor(0, "Zandiano", RED, INITMONEY);
    GetPlayer(mainGame, 1) = Constructor(1, "Kryll", BLUE, INITMONEY);

    ClearFile(PATH);
}

void UpdatePre() {
    if(IsInMenu()){
        switch (GetKeyPressed()) {
            case KEY_ONE:
            clientIndex = 0;
            break;
            
            case KEY_TWO:
            clientIndex = 1;
            break;
            
            case KEY_THREE:
            clientIndex = 2;
            break;
            
            case KEY_FOUR:
            clientIndex = 3;
            break;
        }
    }
    if(pressedButton){
        SendFile(mainGame, PATH);
    }
    RetrieveFile(mainGame, PATH);
}

void Update(){
}

void UpdatePost(){
}

void Render3D(){
    if(IsInMenu()) return;
    RenderHouse(mainGame);
}

void Render2D(){
    if(IsInMenu()){
        RenderMenu(mainGame, clientIndex);
        return;
    }

    RenderName(mainGame);
    RenderRound(mainGame);
    RenderMoney(mainGame);
    pressedButton = RenderButtons(mainGame, clientIndex);
    RenderHouseInfo(mainGame);

    if(gameOver){
        string texto = "Fim de jogo! Vencedor: " + winnerName;
        DrawText(texto.c_str(), 20, ScreenH / 2, 24, GOLD);
    }
}

void Debug(){
    std::cout << "Player: " << GetName(GetPlayer(mainGame)) << std::endl;
    std::cout << "House num: " << to_string(GetPos(GetPlayer(mainGame))) << std::endl;
    std::cout << "Client Index: " << clientIndex << std::endl;
}

string ActionRollDice(){
    if(gameOver){
        return "O jogo ja acabou! Vencedor: " + winnerName;
    }
    if(rolledThisTurn){
        return "Voce ja jogou os dados nesta rodada! Passe a vez.";
    }

    Player& player = GetPlayer(mainGame);
    int dado1 = GetRandomValue(1, 6);
    int dado2 = GetRandomValue(1, 6);
    int total = dado1 + dado2;

    if(player.arrested){
        bool saiu = false;
        string motivo;

        if(dado1 == dado2){
            saiu = true;
            motivo = "tirou dados duplos";
        } else if(player.jailCard){
            player.jailCard = false;
            saiu = true;
            motivo = "usou a carta de saida da prisao";
        } else if(player.money >= 50){
            RemoveMoney(player, 50);
            saiu = true;
            motivo = "pagou $50 de fianca";
        }

        if(!saiu){
            rolledThisTurn = true;
            return GetName(player) + " continua preso (tirou " + to_string(dado1) + " e " + to_string(dado2) + ")";
        }

        player.arrested = false;
        rolledThisTurn = true;
        return MoveAndTriggerEvent(player, dado1, dado2, total, GetName(player) + " " + motivo + " e saiu da prisao,");
    }

    rolledThisTurn = true;
    return MoveAndTriggerEvent(player, dado1, dado2, total, GetName(player));
}

string ActionBuy(){
    if(mainGame.eventDecision.action != BUY){
        return "Nao ha nada para comprar nesta casa.";
    }

    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, (uint8_t)mainGame.eventDecision.houseId);

    if(Buy(house, player)){
        mainGame.eventDecision.action = NONE;
        mainGame.eventDecision.houseId = -1;
        return GetName(player) + " comprou " + house.name + " por $" + to_string(house.price);
    }

    return "Dinheiro insuficiente para comprar " + house.name;
}

string ActionBuild(){
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(BuildHouse(mainGame, house, player)){
        string tipo = (house.housesBuilt >= 5) ? "um hotel" : "uma casa";
        return GetName(player) + " construiu " + tipo + " em " + house.name;
    }

    return "Nao e possivel construir em " + house.name + " agora.";
}

string ActionMortgage(){
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(MortgageProperty(mainGame, player, house)){
        return GetName(player) + " hipotecou " + house.name + " e recebeu $" + to_string(house.mortgagePrice);
    }

    return "Nao e possivel hipotecar " + house.name + ".";
}

string ActionNegotiate(){
    return "Negociacao entre jogadores ainda nao implementada.";
}

string ActionEndTurn(){
    if(gameOver){
        return "O jogo ja acabou! Vencedor: " + winnerName;
    }

    string finishedName = GetName(GetPlayer(mainGame));

    rolledThisTurn = false;
    mainGame.eventDecision.action = EVENT_ACTION::NONE;
    mainGame.eventDecision.houseId = -1;

    uint8_t alive = 0;
    int8_t lastAliveId = -1;
    for(int i = 0; i < mainGame.qntPlayers; i++){
        if(!mainGame.players[i].bankrupt){
            alive++;
            lastAliveId = i;
        }
    }

    if(alive <= 1){
        gameOver = true;
        winnerName = (lastAliveId != -1) ? GetName(mainGame.players[lastAliveId]) : "Ninguem";
        return "Fim de jogo! " + winnerName + " venceu!";
    }

    uint8_t newIndex;
    do {
        newIndex = NextPlayer(mainGame);
        if(newIndex == 0){
            NextRound(mainGame);
        }
    } while(mainGame.players[newIndex].bankrupt);

    return finishedName + " passou a vez. Agora e a vez de " + GetName(GetPlayer(mainGame)) + ".";
}
