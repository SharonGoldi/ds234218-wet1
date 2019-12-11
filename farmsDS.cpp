
#include "farmsDS.h"
#include "ServerOSKey.h"

//#define LINUX 0
//#define WINDOWS 1

FarmsDS::FarmsDS() {
    this->farmsDic = new AvlTree<int,ServersFarm*>();
    this->windowsTree = new AvlTree<ServerOSKey, int>();
    this->linuxTree = new AvlTree<ServerOSKey , int>();
}

FarmsDS::FarmsDS(FarmsDS &fds) {
    this->farmsDic = fds.farmsDic;
    this->windowsTree = fds.windowsTree;
    this->linuxTree = fds.linuxTree;
}

FarmsDS::~FarmsDS() {
    // delete the dic:
    //build an array for the tree nodes
    int treeSize;
    this->farmsDic->Size(&treeSize);
    Node<int,ServersFarm*>** NodeArray = new Node<int,ServersFarm*>* [treeSize];
    //fill the array with the nodes
    this->farmsDic->GetTreeInOrder(NodeArray);
    //delete the arrays in each node
    for(int i = 0; i < treeSize; i++) {
        ServersFarm* farm = *NodeArray[i]->value;
//        ServerOSKey* windows_key = farm->GetServerNode(WINDOWS);
//        ServerOSKey* linux_key = farm->GetServerNode(LINUX);
//        delete windows_key;
//        delete linux_key; // TODO: check delete
        delete farm;
    }
    //delete the local array
    delete[] NodeArray;
    //delete the tree
    delete this->farmsDic;

    // delete the servers trees:
    delete this->linuxTree;
    delete this->windowsTree;
}

FarmsDS& FarmsDS::operator=(FarmsDS const &fds) {
    if (this == &fds) {
        return *this;
    }

    //delete the old tree:
    //build an array for the tree nodes
    int treeSize;
    this->farmsDic->Size(&treeSize);
    Node<int,ServersFarm*>** NodeArray = new Node<int,ServersFarm*>* [treeSize];
    //fill the array with the nodes
    this->farmsDic->GetTreeInOrder(NodeArray);
    //delete the arrays in each node
    for(int i = 0; i < treeSize; i++) {
        ServersFarm *farm = *NodeArray[i]->value;
        delete farm;
    }

    //delete the local array
    delete[] NodeArray;
    //delete the tree
    delete this->farmsDic;
    delete this->windowsTree;
    delete this->linuxTree;
    //copy the new tree
    this->farmsDic = fds.farmsDic;
    this->windowsTree = fds.windowsTree;
    this->linuxTree = fds.linuxTree;

    return *this;
}

TreeStatusType FarmsDS::AddDataCenter(int dataCenterID, int numOfServers) {
    assert (dataCenterID > 0 && numOfServers > 0);
    // check if the tree is empty
    ServersFarm* temp;
    if(this->farmsDic->Find(dataCenterID,&temp) == TREE_SUCCESS) {
        return TREE_FAILURE;
    }

    // build the needed structs to the tree nodes
    ServersFarm* serverFarm;
    ServerOSKey* linuxNum;
    ServerOSKey* windowsNum;
    try{
        serverFarm = new ServersFarm(dataCenterID,numOfServers);
    }
    catch (std::bad_alloc &ba) {
        return TREE_ALLOCATION_ERROR;
    }

    // place the node in the farms dic
    Node<int,ServersFarm*>* node;
    if(this->farmsDic->Add(dataCenterID, serverFarm, &node) == TREE_ALLOCATION_ERROR) {
        delete serverFarm;
        return TREE_ALLOCATION_ERROR;
    }

    Node<ServerOSKey,int>* windowsNode;
    Node<ServerOSKey,int>* linuxNode;
    int zero = 0;
    // place the node in the windows tree
    if (this->windowsTree->Add(*serverFarm->GetServerNode(WINDOWS), zero, &windowsNode) == TREE_ALLOCATION_ERROR) {
        this->farmsDic->Delete(dataCenterID); // TODO: check if we need to delete the value
        delete serverFarm;
        return TREE_ALLOCATION_ERROR;
    }
    // place the node in the linux tree
    if (this->linuxTree->Add(*serverFarm->GetServerNode(LINUX), numOfServers, &linuxNode) == TREE_ALLOCATION_ERROR) {
        this->farmsDic->Delete(
                dataCenterID); // TODO: check if we need to delete the value
        this->windowsTree->Delete(*serverFarm->GetServerNode(WINDOWS));
        delete serverFarm;
        return TREE_ALLOCATION_ERROR;
    }
    return TREE_SUCCESS;
}

TreeStatusType FarmsDS::RemoveDataCenter(int dataCenterID) {
    assert (dataCenterID > 0);
    // check if the data center is in the tree
    ServersFarm* temp;
    if(this->farmsDic->Find(dataCenterID,&temp) != TREE_SUCCESS) {
        return TREE_FAILURE;
    }
    // delete the nodes in the servers trees
    ServerOSKey* windows_key = temp->GetServerNode(WINDOWS);
    ServerOSKey* linux_key = temp->GetServerNode(LINUX);
    this->windowsTree->Delete(*windows_key);
    this->linuxTree->Delete(*linux_key);
    // delete the nodes structs //TODO: check delete
//    delete windows_key;
//    delete linux_key;
    // delete the dic structs
    this->farmsDic->Delete(dataCenterID);
    delete temp;

    return TREE_SUCCESS;
}

TreeStatusType FarmsDS::RequestServer(int dataCenterID, int serverID, int os,
                                      int *assignedID) {
    // check if the data center is in the tree
    ServersFarm* dataCenter;
    if(this->farmsDic->Find(dataCenterID,&dataCenter) != TREE_SUCCESS) {
        return TREE_FAILURE;
    }

    if(dataCenter->numOfServers() <= serverID) {
        return TREE_INVALID_INPUT;
    }

    // request the server
    bool os_changed;
    dataCenter->reqServer(serverID,os,assignedID,&os_changed);

    // check if there is a free server, if not, return
    if (*assignedID == -1) {
        return TREE_FAILURE;
    }

    // check if a servers OS has been changed and if so, update the OS trees
    if (os_changed) {
        ServerOSKey* temp;
        Node<ServerOSKey ,int>* node;
        int val;
        if (os == WINDOWS) {
            // update the windows tree
            temp = dataCenter->GetServerNode(WINDOWS);
            this->windowsTree->Find(*temp,&val);
            this->windowsTree->Delete(*temp);
            ++(*temp);
            ++val;
            this->windowsTree->Add(*temp,val,&node);

            //update the linux tree
            temp = dataCenter->GetServerNode(LINUX);
            this->linuxTree->Find(*temp,&val);
            this->linuxTree->Delete(*temp);
            --(*temp);
            --val;
            this->linuxTree->Add(*temp,val,&node);

        } else {
            // update the windows tree
            temp = dataCenter->GetServerNode(WINDOWS);
            this->windowsTree->Find(*temp,&val);
            this->windowsTree->Delete(*temp);
            --(*temp);
            --val;
            this->windowsTree->Add(*temp,val,&node);

            //update the linux tree
            temp = dataCenter->GetServerNode(LINUX);
            this->linuxTree->Find(*temp,&val);
            this->linuxTree->Delete(*temp);
            ++(*temp);
            ++val;
            this->linuxTree->Add(*temp,val,&node);
        }
    }
    return TREE_SUCCESS;
}

TreeStatusType FarmsDS::FreeServer(int dataCenterID, int serverID) {
    ServersFarm* temp;
    if(this->farmsDic->Find(dataCenterID,&temp) != TREE_SUCCESS) {
        return TREE_FAILURE;
    }
    if(temp->numOfServers() <= serverID) {
        return TREE_INVALID_INPUT;
    }
    if (temp->freeServer(serverID) == FARM_FAILURE) {
        return TREE_FAILURE;
    }
    return TREE_SUCCESS;
}

TreeStatusType FarmsDS::GetDataCentersByOS(int os, int **dataCenters,
                                           int *numOfDataCenters) {

    AvlTree<ServerOSKey, int>* serversTree;
    // choose the right tree by the OS
    if (os == LINUX) {
        serversTree = this->linuxTree;
    } else if (os == WINDOWS){
        serversTree = this->windowsTree;
    } else {
        return TREE_INVALID_INPUT;
    }
    //allocate the array
    int tree_size;
    this->farmsDic->Size(&tree_size);
    if (tree_size == 0) {
        return TREE_FAILURE;
    }
    *numOfDataCenters = tree_size;

    *dataCenters = (int*) malloc(sizeof(int)*tree_size);
    if (*dataCenters == NULL) {
        return TREE_ALLOCATION_ERROR;
    }
    // fill the array
    Node<ServerOSKey,int>** nodesArray = new Node<ServerOSKey,int>*[tree_size]();
    serversTree->GetTreeInOrder(nodesArray);

    for (int i = 0; i < tree_size; ++i) {
        (*dataCenters)[i] = (*nodesArray[i]->key).GetFarmID();
    }

    delete[] nodesArray;
    return TREE_SUCCESS;
}
