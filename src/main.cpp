#include "raylib.h"

#include <iostream>
#include "game_logic.hpp"
#include "constants.hpp"


int main()
{
    cout << "Main" << endl;
    InitWindow(ScreenW, ScreenH, "Capital Alien");

    SetTargetFPS(60);

    cout << "Initializing camera" << endl;
    Camera3D camera = {
        {0,1,0}, 
        {0,0,2}, 
        {0,1,0}, 
        60, 
        CAMERA_PERSPECTIVE
    };
    cout << "Initializing Game" << endl;
    Init();
    cout << "Initialized Game" << endl;
    while (!WindowShouldClose()){
        UpdatePre();
        Update();
        
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
        Render3D();
        EndMode3D();
        
        BeginDrawing();
        Render2D();
        EndDrawing();
        
        UpdatePost();
        Debug();
        WaitTime(0.01);
        system("cls");
    }
    CloseWindow();

    return 0;
}