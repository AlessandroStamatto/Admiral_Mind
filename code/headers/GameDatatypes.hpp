#ifndef GAMEDATATYPES_HPP
#define GAMEDATATYPES_HPP

#include <vector>
#include <string>
#include <array>
#include <map>
                    // 0    1       2     3    4       5       6        7            8            9       10        11          12       13       14         15          16         17       18
enum class packetID {None, Name, Ships, Fire, Turn, Connect, Login, Playerlist, LoginResponse, Options, GameEnd, Disconnect, keepAlive, Mine, Register, RegisterResponse, Chat, MatchWinner, Challenge};
static std::array<std::string, 19> packetIDnames = {"None", "Name", "Ships", "Fire", "Turn", "Connect", "Login", "Playerlist", 
                                                    "LoginResponse", "Options", "GameEnd", "Disconnect", "keepAlive", "Mine",
                                                    "Register", "RegisterResponse", "Chat", "MatchWinner", "Challenge"};

enum class statusID {unlogged, offline, online, playing};
static std::array<std::string, 4> statusIDnames = {"unlogged", "offline", "online", "playing"};

enum class SquareState
{ Covered, Sea, Fire, Island };

enum class ShipID
{ Carrier = 0, Battleship, Submarine, Cruiser, Destroyer, Mine, Island};

struct sPos 
{int i, j;};

using mapOptions = std::map<std::string, int>;

struct playerInfo {
    int id;
    std::string name;
    statusID status;
    mapOptions battleOpts;
    playerInfo (int _id, std::string _name, statusID _status) {id = _id; name = _name; status = _status;}
    playerInfo () {}
};

struct Ship
{
    ShipID name; std::vector<sPos> positions;
    bool vertical () {return (positions.size() > 1 and positions[0].j == positions[1].j);}
};

#endif // GAMEDATATYPES_HPP