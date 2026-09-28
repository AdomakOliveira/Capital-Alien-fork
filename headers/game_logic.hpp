#ifndef FRAMEHANDLE_HPP
#define FRAMEHANDLE_HPP

#include "structs.hpp"
#include <string>

// Ciclo de vida chamado a cada quadro pelo main.cpp
void Init();       // Roda uma vez, antes do loop principal, para configurar o jogo
void UpdatePre();  // Roda no início de cada quadro (hoje sem uso)
void Update();     // Roda no meio de cada quadro (hoje sem uso)
void UpdatePost();  // Roda no fim da atualização de cada quadro (hoje sem uso)
void Render3D();   // Desenha os elementos 3D do quadro atual
void Render2D();   // Desenha os elementos 2D (HUD, menus, botões) do quadro atual
void Debug();      // Imprime informações de depuração no terminal

// Ações disparadas pelos botões da interface. Cada uma retorna um texto
// de feedback pronto para ser mostrado na tela.
string ActionRollDice();   // Joga os dados e move o jogador da vez
string ActionBuy();        // Compra a propriedade em que o jogador está, se possível
string ActionBuild();      // Constrói uma casa/hotel na propriedade atual, se possível
string ActionMortgage();   // Hipoteca a propriedade atual, se possível
string ActionNegotiate();  // Negociação entre jogadores (ainda não implementada)
string ActionEndTurn();    // Passa a vez para o próximo jogador

#endif
