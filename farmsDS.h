
#ifndef WET1_FARMSDS_H
#define WET1_FARMSDS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "library1.h"
#include "AVLTree.h"
#include "Farm.h"
#include "ServerOSKey.h"

class FarmsDS {
    AvlTree<int, ServersFarm*>* farmsDic;
    AvlTree<ServerOSKey, int>* windowsTree;
    AvlTree<ServerOSKey, int>* linuxTree;

public:
    FarmsDS();
    FarmsDS(FarmsDS& fds);
    ~FarmsDS();
    FarmsDS& operator=(FarmsDS const& fds);

    TreeStatusType AddDataCenter(int dataCenterID, int numOfServers);
    TreeStatusType RemoveDataCenter(int dataCenterID);
    TreeStatusType RequestServer(int dataCenterID, int serverID, int os, int* assignedID);
    TreeStatusType FreeServer(int dataCenterID, int serverID);
    TreeStatusType GetDataCentersByOS(int os, int** dataCenters, int* numOfDataCenters);
};


#endif //WET1_FARMSDS_H
