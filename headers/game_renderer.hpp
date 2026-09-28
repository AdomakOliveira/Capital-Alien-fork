#ifndef RENDERS_HPP
#define RENDERS_HPP

#include "structs.hpp"

void RenderHouse(Game game);

void RenderName(Game game);

void RenderRound(Game game);

void RenderMoney(Game game);

// Desenha a tela de menu inicial (JOGAR / SAIR)
void RenderMenu(Game& game);

// Desenha os botões de ação (jogar dado, comprar, construir, etc.) e a
// caixa de mensagem de feedback
void RenderBotoesAcao(Game& game);

// Indica se o jogo ainda está na tela de menu (true) ou já em partida (false)
bool IsInMenu();

#endif