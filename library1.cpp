#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include "farmsDS.h"

StatusType ReplaceEnums(TreeStatusType treeStatusType) {
    switch(treeStatusType) {
        case TREE_ALLOCATION_ERROR:
            return ALLOCATION_ERROR;
        case TREE_FAILURE:
            return FAILURE;
        case TREE_INVALID_INPUT:
            return INVALID_INPUT;
        default: //TREE_SUCCESS
            return SUCCESS;
    }
}

void *Init() {
    FarmsDS* fds;
    try {
        fds = new FarmsDS();
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }
    return fds;
//    void* DS = (void*) fds;
//    return DS;
}

StatusType AddDataCenter(void *DS, int dataCenterID, int numOfServers){
    if (DS == NULL || dataCenterID <= 0 || numOfServers <= 0) {
        return INVALID_INPUT;
    }

    FarmsDS* fds = (FarmsDS*) DS;
    return ReplaceEnums(fds->AddDataCenter(dataCenterID, numOfServers));
}

StatusType RemoveDataCenter(void *DS, int dataCenterID){
    if (DS == NULL || dataCenterID <= 0) {
        return INVALID_INPUT;
    }
    FarmsDS* fds = (FarmsDS*) DS;
    return ReplaceEnums(fds->RemoveDataCenter(dataCenterID));
}

StatusType RequestServer(void *DS, int dataCenterID, int serverID, int os, int *assignedID){
    if (DS == NULL || serverID < 0 || os < 0 || os > 1 || assignedID == NULL) {
        return  INVALID_INPUT;
    }
    FarmsDS* fds = (FarmsDS*) DS;
    return ReplaceEnums(fds->RequestServer(dataCenterID, serverID, os, assignedID));
}

StatusType FreeServer(void *DS, int dataCenterID, int serverID){
    if (DS == NULL || serverID < 0 || dataCenterID <= 0) {
        return INVALID_INPUT;
    }

    FarmsDS* fds = (FarmsDS*) DS;
    return ReplaceEnums(fds->FreeServer(dataCenterID, serverID));
}

StatusType GetDataCentersByOS(void *DS, int os, int **dataCenters, int* numOfDataCenters){
    if (DS == NULL || dataCenters == NULL || numOfDataCenters == NULL || os > 1 || os < 0) {
        return  INVALID_INPUT;
    }
    FarmsDS* fds = (FarmsDS*) DS;
    return ReplaceEnums(fds->GetDataCentersByOS(os, dataCenters, numOfDataCenters));
}

void Quit(void** DS){
    if (DS == NULL || *DS == NULL) {
        return;
    }
    FarmsDS* fds = (FarmsDS*) *DS;
    delete fds;
    *DS = NULL;
}
