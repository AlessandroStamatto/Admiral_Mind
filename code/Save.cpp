#include "headers/Save.hpp"
#include <fstream>

using namespace std;

GameSave::GameSave() {
    ifstream input {"battleship.sav"};
    if (input.is_open()) {
        readSuccess = true;
        getline(input, name);
        getline(input, password);
        getline(input, server);
    }
}

void GameSave::save() {
    ofstream output {"battleship.sav"};
    output << name << "\n";
    output << password << "\n";
    output << server << "\n";
}