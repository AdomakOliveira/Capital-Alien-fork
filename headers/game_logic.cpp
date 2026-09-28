#include "game_logic.hpp"
#include "game_renderer.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include "constants.hpp"
#include <iostream>
#include <ctime>
#include "events.hpp"

Game mainGame;

static bool rolledThisTurn = false; // O jogador da vez já jogou o dado nesta rodada?
static bool gameOver = false;       // A partida já terminou?
static string winnerName = "";      // Nome do vencedor, quando o jogo termina

// Move o jogador 'total' casas, credita os $200 ao passar pelo Início
// e dispara o evento da casa onde ele parou. Também monta a mensagem de
// feedback que será mostrada na tela (quem jogou, o resultado dos dados
// e o que aconteceu).
static string MoveAndTriggerEvent(Player& player, int dado1, int dado2, int total, const string& actorLabel){
    uint8_t maxHouses = GetHouseQnt(mainGame);
    bool passedStart = false;

    // Avança casa por casa, para detectar se em algum momento passou pelo Início
    for(int i = 0; i < total; i++){
        if(NextHouse(player, maxHouses)){
            passedStart = true;
        }
    }

    if(passedStart){
        AddMoney(player, 200);
    }

    // Dispara o evento correspondente ao tipo da casa onde o jogador parou
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

// Configura o estado inicial da partida: define a semente aleatória, cria o
// tabuleiro com 2 jogadores fixos (Zandiano e Kryll). Chamada uma única vez
// pelo main.cpp antes do loop principal.
void Init(){
    SetRandomSeed((unsigned int)time(NULL));

    Init(mainGame, array_size(BOARD_DATA), 2); // Chama o Init(Game&, int, int) de tabletop.cpp

    GetPlayer(mainGame, 0) = Constructor(0, "Zandiano", RED);
    GetPlayer(mainGame, 1) = Constructor(1, "Kryll", BLUE);
}

void UpdatePre(){

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
        RenderMenu(mainGame);
        return;
    }

    RenderName(mainGame);
    RenderRound(mainGame);
    RenderMoney(mainGame);
    RenderBotoesAcao(mainGame);

    if(gameOver){
        string texto = "Fim de jogo! Vencedor: " + winnerName;
        DrawText(texto.c_str(), 20, ScreenH / 2, 24, GOLD);
    }
}

// Imprime no terminal o nome e a posição do jogador da vez, a cada quadro
// (útil para depuração durante o desenvolvimento)
void Debug(){
    std::cout << "Player: " << GetName(GetPlayer(mainGame)) << std::endl;
    std::cout << "House num: " << to_string(GetPos(GetPlayer(mainGame))) << std::endl;
}

// Ação do botão "JOGAR DADO": joga dois dados e move o jogador.
// Trata separadamente o caso do jogador estar preso (sair com dados duplos,
// usando a carta de saída, ou pagando fiança de $50).
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
            // Tirou dados duplos: sai da prisão de graça
            saiu = true;
            motivo = "tirou dados duplos";
        } else if(player.jailCard){
            // Usa a carta de "saída livre da prisão"
            player.jailCard = false;
            saiu = true;
            motivo = "usou a carta de saida da prisao";
        } else if(player.money >= 50){
            // Paga fiança de $50 para sair
            RemoveMoney(player, 50);
            saiu = true;
            motivo = "pagou $50 de fianca";
        }

        if(!saiu){
            // Nenhuma das condições acima: continua preso e perde a jogada
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

// Ação do botão "COMPRAR": só funciona se o jogador acabou de cair em uma
// propriedade sem dono (game.eventDecision.action == BUY)
string ActionBuy(){
    if(mainGame.eventDecision.action != EVENT_ACTION::BUY){
        return "Nao ha nada para comprar nesta casa.";
    }

    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, (uint8_t)mainGame.eventDecision.houseId);

    if(Buy(house, player)){
        // Compra feita: limpa a decisão pendente
        mainGame.eventDecision.action = EVENT_ACTION::NONE;
        mainGame.eventDecision.houseId = -1;
        return GetName(player) + " comprou " + house.name + " por $" + to_string(house.price);
    }

    return "Dinheiro insuficiente para comprar " + house.name;
}

// Ação do botão "CONSTRUIR": tenta construir uma casa/hotel na propriedade
// onde o jogador da vez está parado
string ActionBuild(){
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(BuildHouse(mainGame, player, house)){
        string tipo = (house.housesBuilt >= 5) ? "um hotel" : "uma casa";
        return GetName(player) + " construiu " + tipo + " em " + house.name;
    }

    return "Nao e possivel construir em " + house.name + " agora.";
}

// Ação do botão "HIPOTECAR": tenta hipotecar a propriedade onde o
// jogador da vez está parado
string ActionMortgage(){
    Player& player = GetPlayer(mainGame);
    House& house = GetHouse(mainGame, GetPos(player));

    if(MortgageProperty(mainGame, player, house)){
        return GetName(player) + " hipotecou " + house.name + " e recebeu $" + to_string(house.mortgagePrice);
    }

    return "Nao e possivel hipotecar " + house.name + ".";
}

// Ação do botão "NEGOCIAR": funcionalidade ainda não implementada (ver TODO.md)
string ActionNegotiate(){
    return "Negociacao entre jogadores ainda nao implementada.";
}

// Ação do botão "PASSAR VEZ": encerra o turno do jogador atual e passa
// para o próximo jogador vivo. Também verifica se sobrou só um jogador
// não-falido, encerrando o jogo nesse caso.
string ActionEndTurn(){
    if(gameOver){
        return "O jogo ja acabou! Vencedor: " + winnerName;
    }

    string finishedName = GetName(GetPlayer(mainGame));

    rolledThisTurn = false;
    mainGame.eventDecision.action = EVENT_ACTION::NONE;
    mainGame.eventDecision.houseId = -1;

    // Conta quantos jogadores ainda não faliram
    uint8_t alive = 0;
    int8_t lastAliveId = -1;
    for(int i = 0; i < mainGame.qntPlayers; i++){
        if(!mainGame.players[i].bankrupt){
            alive++;
            lastAliveId = i;
        }
    }

    if(alive <= 1){
        // Só sobrou um jogador (ou nenhum): fim de jogo
        gameOver = true;
        winnerName = (lastAliveId != -1) ? GetName(mainGame.players[lastAliveId]) : "Ninguem";
        return "Fim de jogo! " + winnerName + " venceu!";
    }

    // Avança para o próximo jogador, pulando os que já faliram, e conta
    // uma nova rodada sempre que voltar ao jogador de índice 0
    uint8_t newIndex;
    do {
        newIndex = NextPlayer(mainGame);
        if(newIndex == 0){
            NextRound(mainGame);
        }
    } while(mainGame.players[newIndex].bankrupt);

    return finishedName + " passou a vez. Agora e a vez de " + GetName(GetPlayer(mainGame)) + ".";
}
