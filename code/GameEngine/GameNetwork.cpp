#include "./../headers/GameNetwork.hpp"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using namespace sf;

GameNetwork::GameNetwork () {
    server.setBlocking (false);
}

bool GameNetwork::connect (std::string ip_port) {
    server.setBlocking (true);
    server.disconnect();

    istringstream entrada (ip_port);
    string ip;
    getline (entrada, ip, ':');
    int port = 50986;
    entrada >> port;

    if (server.connect(ip, port, seconds(2)) == sf::Socket::Done) {
        cout << "Connected to server\n";
        server.setBlocking (false);
        connected = true;
        return connected;
    }
    server.setBlocking (false);
    return false;
}

packetID GameNetwork::receive () {
    if (server.receive(packet) != Socket::Done)
        return packetID::None;

    packet >> player_id >> packet_id;

    cout << "Received packet: " << static_cast<int> (packet_id) << "\n";
    return static_cast<packetID> (packet_id);
}

void GameNetwork::receiveConnect () {
    enemyID = player_id;
    cout << "Connection received, estabilished connection with " << enemyID << "...\n";
}

void GameNetwork::receiveChallenge () {
    enemyID = player_id;
    cout << "Challenge received from " << enemyID << "...\n";
}

bool GameNetwork::receiveLoginResponse() {
    bool correct = false;
    packet >> correct;

    if (correct)
        myID = player_id;

    return correct;
}

bool GameNetwork::receiveRegisterResponse() {
    bool correct = false;
    packet >> correct;

    return correct;
}

vector<Ship> GameNetwork::receiveShips () {
    int name, sz, psz, posI, posJ;
    vector<Ship> ships;
    packet >> sz;

    for (int i = 0; i < sz; ++i) {
        packet >> name;
        packet >>  psz;
        Ship ship;
        ship.name = static_cast<ShipID> (name);
        for (int position = 0; position < psz; ++position) {
            packet >> posI >> posJ;
            ship.positions.push_back({posI, posJ});
        }
        ships.push_back(ship);
    }

    return ships;
}

void GameNetwork::receiveFire(int& iPos, int& jPos) {
    packet >> iPos >> jPos;
}

std::string GameNetwork::receiveChat () {
    std::string msg;
    packet >> msg;
    return msg;
}

void GameNetwork::receiveMine(int& iPos, int& jPos) {
    packet >> iPos >> jPos;
}

std::string GameNetwork::receiveName() {
    std::string eName;
    packet >> eName;
    return eName;
}

std::vector<playerInfo> GameNetwork::receivePlayerList() {
    std::vector<playerInfo> playerList;
    int listSize;
    packet >> listSize;
    for (int i = 0; i < listSize; ++i){
        playerInfo info;
        int statusTemp;
        packet >> info.id >> info.name >> statusTemp;
        info.status = static_cast<statusID> (statusTemp);
        int mapSize;
        packet >> mapSize;
        std::string key;
        int value;
        for (int j = 0; j < mapSize; ++j) {
            packet >> key >> value;
            info.battleOpts[key] = value;
        }
        playerList.push_back(info);
    }
    return playerList;
}

void GameNetwork::buildHeader (packetID pid) {
    packet.clear();
    packet << enemyID;
    packet << static_cast<int> (pid);
}

void GameNetwork::sendLogin(std::string name, std::string password) {
    buildHeader (packetID::Login);
    packet << name << password;
    server.send(packet);
}

void GameNetwork::sendRegister(string user, string password, string name, string email) {
    buildHeader (packetID::Register);
    packet << user << password << name << email;
    server.send(packet);
    cout << "Send register, " << user << " " << password << " " << enemyID << "\n";
}

void GameNetwork::sendConnect () {
    buildHeader (packetID::Connect);
    server.send(packet);
    cout << "[" << myID << "] Estabilishing connection to " << enemyID << "...\n";
}

void GameNetwork::sendChallenge () {
    buildHeader (packetID::Challenge);
    server.send(packet);
    cout << "[" << myID << "] Challenging player " << enemyID << "...\n";
}

void GameNetwork::sendShips (vector<Ship>& ships) {
    buildHeader (packetID::Ships);
    packet << static_cast<int> (ships.size());
    for (auto& ship : ships ) {
        packet << static_cast<int> (ship.name);
        packet << static_cast<int> (ship.positions.size());
        for (auto& position : ship.positions) {
            packet << position.i << position.j;
        }
    }
    server.send(packet);
    cout << "[" << myID << "] ships sended\n";
}

void GameNetwork::sendTurn () {
    buildHeader (packetID::Turn);
    server.send(packet);
    cout << "[" << myID << "] Turn sended\n";
}

void GameNetwork::sendName (string myName) {
    buildHeader (packetID::Name);
    packet << myName;
    server.send(packet);
    cout << "[" << myID << "] Name sended\n";
}

void GameNetwork::sendFire (int i, int j) {
    buildHeader (packetID::Fire);
    packet << i << j;
    server.send(packet);
    cout << "[" << myID << "] Fire sended\n";
}

void GameNetwork::sendChat (std::string msg) {
    buildHeader (packetID::Chat);
    packet << msg;
    server.send(packet);
    cout << "[" << myID << "] Chat message sended\n";
}

void GameNetwork::sendMine (int i, int j) {
    buildHeader (packetID::Mine);
    packet << i << j;
    server.send(packet);
    cout << "[" << myID << "] Fire sended\n";
}

void GameNetwork::sendOptions (mapOptions& bo) {
    buildHeader (packetID::Options);
    packet << static_cast<int> (bo.size());
    for (auto& element: bo)
        packet << element.first << element.second;
    server.send(packet);
    cout << "[" << myID << "] Options sended\n";
}

void GameNetwork::sendGameEnd () {
    buildHeader (packetID::GameEnd);
    server.send(packet);
    cout << "[" << myID << "] GameEnd sended\n";
}

void GameNetwork::sendMatchWinner (string time, int shoots1, int shoots2) {
    buildHeader (packetID::MatchWinner);
    packet << time << shoots1 << shoots2;
    server.send(packet);
    cout << "[" << myID << "] MatchWinner sended\n";
}

void GameNetwork::sendDisconnect () {
    buildHeader (packetID::Disconnect);
    server.send(packet);
    sleep(seconds(0.2));
    server.disconnect();
    connected = false;
    cout << "[" << myID << "] Disconnect sended\n";
}

void GameNetwork::update () {
    if (timer.getElapsedTime().asSeconds() >= 6 and connected) {
        buildHeader (packetID::keepAlive);
        server.send(packet);
        //cout << "[" << myID << "] keepAlive sended\n";
        timer.restart();
    }
}