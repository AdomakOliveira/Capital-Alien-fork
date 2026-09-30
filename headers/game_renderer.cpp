#include "game_renderer.hpp"
#include "game_logic.hpp"
#include "player.hpp"
#include "house.hpp"
#include "tabletop.hpp"
#include "constants.hpp"

void RenderHouse(Game game){
    DrawCube({0,0,1}, 1.2f, 0.5, 2, GetHouseColor(GetHouse(game, GetPos(GetPlayer(game)))));
}

void RenderName(Game game){
    DrawText(GetName(GetPlayer(game)).c_str(), 20, 20, 28, GetPlayerColor(GetPlayer(game)));
}

void RenderRound(Game game){
    string rodada = "Rodada: " + to_string(GetRound(game));
    DrawText(rodada.c_str(), ScreenW - 190, 15, 24, RED);

    string casa = "Casa: " + to_string(GetPos(GetPlayer(game)));
    DrawText(casa.c_str(), ScreenW - 190, 45, 24, RED);
}

void RenderMoney(Game game){
    string texto = "$" + to_string(GetMoney(GetPlayer(game)));
    DrawText(texto.c_str(), ScreenW - 190, 75, 24, DARKGREEN);
}


bool emMenu = true;
int opcaoMenu = 0;

bool IsInMenu(){
    return emMenu;
}

void RenderMenu(Game& game, int clientIndex){
    ClearBackground(BLACK);
    DrawRectangleLines(15, 15, ScreenW - 30, ScreenH - 30, DARKGREEN);

    int bannerLargura = 500;
    int bannerAltura = 80;
    int bannerX = (ScreenW - bannerLargura) / 2;
    int bannerY = 45;

    
    
    DrawRectangle(bannerX, bannerY, bannerLargura, bannerAltura, RED);
    DrawRectangleLines(bannerX, bannerY, bannerLargura, bannerAltura, GOLD);
    
    int larguraTexto = MeasureText("MONOPOLY", 40);
    DrawText("MONOPOLY", bannerX + (bannerLargura - larguraTexto) / 2, bannerY + 20, 40, WHITE);
    
    int larguraSub = MeasureText("CAPITAL ALIEN - BANCO IMOBILIARIO", 16);
    DrawText("CAPITAL ALIEN - BANCO IMOBILIARIO", (ScreenW - larguraSub) / 2, bannerY + 95, 16, YELLOW);
    
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S)){
        if (opcaoMenu == 0){
            opcaoMenu = 1;
        } else {
            opcaoMenu = 0;
        }
    }
    
    Rectangle botaoJogar = { (float)(ScreenW / 2 - 120), 245.0f, 240.0f, 45.0f };
    Rectangle botaoSair  = { (float)(ScreenW / 2 - 120), 305.0f, 240.0f, 45.0f };
    
    Vector2 mouse = GetMousePosition();
    bool mouseEmJogar = CheckCollisionPointRec(mouse, botaoJogar);
    bool mouseEmSair  = CheckCollisionPointRec(mouse, botaoSair);
    
    if (mouseEmJogar) opcaoMenu = 0;
    if (mouseEmSair)  opcaoMenu = 1;
    
    if (opcaoMenu == 0){
        DrawRectangleRec(botaoJogar, DARKGREEN);
        DrawRectangleLines(botaoJogar.x, botaoJogar.y, botaoJogar.width, botaoJogar.height, GOLD);
        DrawText("> JOGAR <", botaoJogar.x + 65, botaoJogar.y + 12, 20, YELLOW);
    } else {
        DrawRectangleRec(botaoJogar, DARKGRAY);
        DrawRectangleLines(botaoJogar.x, botaoJogar.y, botaoJogar.width, botaoJogar.height, GRAY);
        DrawText("JOGAR", botaoJogar.x + 80, botaoJogar.y + 12, 20, WHITE);
    }
    
    if (opcaoMenu == 1){
        DrawRectangleRec(botaoSair, MAROON);
        DrawRectangleLines(botaoSair.x, botaoSair.y, botaoSair.width, botaoSair.height, GOLD);
        DrawText("> SAIR <", botaoSair.x + 75, botaoSair.y + 12, 20, YELLOW);
    } else {
        DrawRectangleRec(botaoSair, DARKGRAY);
        DrawRectangleLines(botaoSair.x, botaoSair.y, botaoSair.width, botaoSair.height, GRAY);
        DrawText("SAIR", botaoSair.x + 90, botaoSair.y + 12, 20, WHITE);
    }
    
    int larguraDica = MeasureText("Use as SETAS e ENTER, ou o MOUSE para escolher", 16);
    DrawText("Use as SETAS e ENTER, ou o MOUSE para escolher", (ScreenW - larguraDica) / 2, 385, 16, LIGHTGRAY);
    
    bool clicou = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    if (IsKeyPressed(KEY_ENTER) || clicou){
        if (opcaoMenu == 0){
            emMenu = false;
        } else if (opcaoMenu == 1){
            CloseWindow();
            exit(0);
        }
    }
    string indexText = "" + clientIndex;
    DrawText(indexText.c_str(), ScreenW - 40, 20, 20, WHITE);
}


string mensagemFeedback = "";
float tempoFeedback = 0.0f;

bool BotaoAcao(Rectangle area, const char* texto){
    Vector2 mouse = GetMousePosition();
    bool mouseSobre = CheckCollisionPointRec(mouse, area);

    if (mouseSobre){
        DrawRectangleRec(area, YELLOW);
        DrawRectangleLines(area.x, area.y, area.width, area.height, BLACK);
        DrawText(texto, area.x + 10, area.y + 12, 14, BLACK);
    } else {
        DrawRectangleRec(area, LIGHTGRAY);
        DrawRectangleLines(area.x, area.y, area.width, area.height, DARKGRAY);
        DrawText(texto, area.x + 10, area.y + 12, 14, BLACK);
    }

    if (mouseSobre && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        return true;
    }
    return false;
}

bool RenderButtons(Game& game, int clientIndex){
    int largura = 125;
    int altura = 40;
    int espaco = 8;
    int xInicial = 18;
    int yLinha = ScreenH - 55;

    if (BotaoAcao({(float)(xInicial + 0 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "JOGAR DADO") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionRollDice();
        tempoFeedback = 3.0f;
    }

    if (BotaoAcao({(float)(xInicial + 1 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "COMPRAR") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionBuy();
        tempoFeedback = 3.0f;
    }

    if (BotaoAcao({(float)(xInicial + 2 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "CONSTRUIR") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionBuild();
        tempoFeedback = 3.0f;
    }

    if (BotaoAcao({(float)(xInicial + 3 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "HIPOTECAR") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionMortgage();
        tempoFeedback = 3.0f;
    }

    if (BotaoAcao({(float)(xInicial + 4 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "NEGOCIAR") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionNegotiate();
        tempoFeedback = 3.0f;
    }

    if (BotaoAcao({(float)(xInicial + 5 * (largura + espaco)), (float)yLinha, (float)largura, (float)altura}, "PASSAR VEZ") && clientIndex == GetID(GetPlayer(game))){
        mensagemFeedback = ActionEndTurn();
        tempoFeedback = 3.0f;
    }

    if (tempoFeedback > 0.0f){
        tempoFeedback = tempoFeedback - GetFrameTime();
        
        int caixaLargura = 420;
        int caixaAltura = 32;
        int caixaX = (ScreenW - caixaLargura) / 2;
        int caixaY = yLinha - 40;
        
        DrawRectangle(caixaX, caixaY, caixaLargura, caixaAltura, BLACK);
        DrawRectangleLines(caixaX, caixaY, caixaLargura, caixaAltura, GOLD);
        
        int larguraMsg = MeasureText(mensagemFeedback.c_str(), 16);
        DrawText(mensagemFeedback.c_str(), caixaX + (caixaLargura - larguraMsg) / 2, caixaY + 8, 16, YELLOW);
        return tempoFeedback >= 2.8f;
    }
    return false;
}