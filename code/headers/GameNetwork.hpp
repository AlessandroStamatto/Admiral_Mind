#ifndef GAMENETWORK_HPP
#define GAMENETWORK_HPP

#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "GameDatatypes.hpp"
#include <memory>
#include <vector>
#include <string>

class GameNetwork
{
    public:
        GameNetwork();
        void update ();

        bool connect (std::string ip_port);
        void buildHeader (packetID pid);
        void sendLogin(std::string name, std::string password);
        void sendRegister(std::string user, std::string password, std::string name, std::string email);
        void sendConnect   ();
        void sendChallenge ();
        void sendShips (std::vector<Ship>& ships);
        void sendTurn ();
        void sendName (std::string myName);
        void sendFire (int i, int j);
        void sendMine (int i, int j);
        void sendChat (std::string msg);
        void sendOptions (mapOptions& bo);
        void sendGameEnd ();
        void sendMatchWinner (std::string time, int shoots1, int shoots2); 
        void sendDisconnect ();

        packetID                receive ();
        bool                    receiveLoginResponse();
        bool                    receiveRegisterResponse();
        void                    receiveConnect ();
        void                    receiveChallenge ();
        std::vector<Ship>       receiveShips ();
        void                    receiveFire(int& iPos, int& jPos);
        void                    receiveMine(int& iPos, int& jPos);
        std::string             receiveChat();
        std::string             receiveName(); //will be deprecated
        std::vector<playerInfo> receivePlayerList();

        sf::TcpSocket server;
        sf::Packet    packet;
        int           packet_id, player_id;  
        int           myID = -1, enemyID = -1;
        bool connected = false;

        sf::Clock timer;
};

#endif // GAMENETWORK_HPP