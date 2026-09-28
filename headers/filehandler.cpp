#include "filehandler.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

void SendFile(const Game& game, const string& filepath){
    ofstream file((filepath + "\\data.dat").c_str());
    if(!file.is_open()){
        cout << "Waiting..." << endl;
        return;
    }

    cout << "Sending..." << endl;

    file.write((const char*)&game, sizeof(Game));

    cout << "Sent." << endl;

    if(file.is_open()){
        file.close();
    }
}

void RetrieveFile(Game& game, const string& filepath){
    ifstream file((filepath + "\\data.dat").c_str());
    if(!file.is_open()){
        cout << "Waiting..." << endl;
        return;
    }

    cout << "Retrieving..." << endl;

    file.read((char*)&game, sizeof(Game));

    cout << "Retrieved." << endl;
    if(file.is_open()){
        file.close();
    }    
}
