
#include "ServerOSKey.h"

ServerOSKey::ServerOSKey(int farmID, int num_of_servers) :
    num_of_servers(num_of_servers), farmID(farmID){};

ServerOSKey::ServerOSKey(ServerOSKey &farm){
        this->num_of_servers = farm.num_of_servers;
        this->farmID = farm.farmID;
}

int ServerOSKey::GetFarmID() {
        return this->farmID;
}

int ServerOSKey::GetNumOfServer() {
        return this->num_of_servers;
}

ServerOSKey& ServerOSKey::operator++() {
        this->num_of_servers++;
        return *this;
}

ServerOSKey& ServerOSKey::operator--() {
        this->num_of_servers--;
        return *this;
}

bool operator<(const ServerOSKey &a, const ServerOSKey &b) {
        if (a.num_of_servers > b.num_of_servers) {
                return true;
        } else if (a.num_of_servers == b.num_of_servers) {
                if (a.farmID < b.farmID) {
                        return true;
                }
        }
        return false;
}

bool operator==(const ServerOSKey &a, const ServerOSKey &b) {
        return (a.farmID == b.farmID) ;
}

bool operator>(const ServerOSKey &a, const ServerOSKey &b) {
        return  !(a<b || a==b);
}