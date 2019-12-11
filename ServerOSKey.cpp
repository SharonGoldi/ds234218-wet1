
#include "ServerOSKey.h"

ServerOSKey::ServerOSKey(int farmID, int num_of_servers) :
        farmID(farmID), num_of_servers(num_of_servers){};

ServerOSKey::ServerOSKey(ServerOSKey &farm){
        this->num_of_servers = farm.num_of_servers;
        this->farmID = farm.farmID;
}

int ServerOSKey::GetFarmID() {
        return this->farmID;
}

ServerOSKey& ServerOSKey::operator++() {
        this->num_of_servers++;
        return *this;
}

ServerOSKey& ServerOSKey::operator--() {
        this->num_of_servers--;
        return *this;
}

ServerOSKey& ServerOSKey::operator=(ServerOSKey const &server) {
   this->farmID = server.farmID;
   this->num_of_servers = server.num_of_servers;
   return *this;
}

bool operator<(const ServerOSKey &a, const ServerOSKey &b) {
        return  (a.num_of_servers > b.num_of_servers ||
                (a.num_of_servers == b.num_of_servers && a.farmID < b.farmID));
}

bool operator==(const ServerOSKey &a, const ServerOSKey &b) {
        return (a.farmID == b.farmID && a.num_of_servers == b.num_of_servers) ;
}

bool operator>(const ServerOSKey &a, const ServerOSKey &b) {
        return  !(a < b || a == b);
}