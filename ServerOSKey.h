
#ifndef WET1_FARMSSERVERNUM_H
#define WET1_FARMSSERVERNUM_H

class ServerOSKey {
    int num_of_servers;
    int farmID;
public:
    ServerOSKey(int farmID, int num_of_servers);
    ServerOSKey(ServerOSKey& farm);
    ~ServerOSKey() = default;

    int GetFarmID();
    int GetNumOfServer();

    ServerOSKey& operator=(ServerOSKey const& server);
    ServerOSKey& operator++();
    ServerOSKey& operator--();

    friend bool operator<(const ServerOSKey& a, const ServerOSKey& b);
    friend bool operator==(const ServerOSKey& a, const ServerOSKey& b);
    friend bool operator>(const ServerOSKey& a, const ServerOSKey& b);
};



#endif //WET1_FARMSSERVERNUM_H
