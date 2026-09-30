#include "filehandler.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "utilities.hpp"

using namespace std;

void SendFile(const Game& game, const string& filepath){
    ofstream file((filepath + "\\data.dat").c_str(), ios::binary | ios::app);
    file.clear();
    if(!file.is_open() || file.fail()){
        cout << "Error..." << endl;
        return;
    }

    cout << "Sending..." << endl;

    for(int i = 0; i < MAXHOUSES; i++){
        file.write((const char*)&game.houses[i], sizeof(House));
    }
    for(int i = 0; i < MAXPLAYERS; i++){
        file.write((const char*)&game.players[i], sizeof(Player));
    }
    for(int i = 0; i < QNTCARDS; i++){
        file.write((const char*)&game.chanceCards[i], sizeof(Card));
    }
    for(int i = 0; i < QNTCARDS; i++){
        file.write((const char*)&game.chestCards[i], sizeof(Card));
    }

    file.write((const char*)&game, sizeof(Game) - sizeof(House)*MAXHOUSES - sizeof(Player)*MAXPLAYERS - sizeof(Card)*QNTCARDS*2);
    file.close();

    cout << "Sent." << endl;
}

void RetrieveFile(Game& game, const string& filepath){
    ifstream file((filepath + "\\data.dat").c_str(), ios::binary );
    if(!file.is_open() || file.fail()){
        cout << "Error..." << endl;
        return;
    }

    cout << "Retrieving..." << endl;
    
    for(int i = 0; i < MAXHOUSES; i++){
        file.read((char*)&game.houses[i], sizeof(House));
    }
    for(int i = 0; i < MAXPLAYERS; i++){
        file.read((char*)&game.players[i], sizeof(Player));
    }
    for(int i = 0; i < QNTCARDS; i++){
        file.read((char*)&game.chanceCards[i], sizeof(Card));
    }
    for(int i = 0; i < QNTCARDS; i++){
        file.read((char*)&game.chestCards[i], sizeof(Card));
    }

    file.read((char*)&game, sizeof(Game) - sizeof(House)*MAXHOUSES - sizeof(Player)*MAXPLAYERS - sizeof(Card)*QNTCARDS*2);
    file.close();

    cout << "Retrieved." << endl;
}
