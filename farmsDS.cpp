
#include "farmsDS.h"
#include "ServerOSKey.h"

//#define LINUX 0
//#define WINDOWS 1

FarmsDS::FarmsDS() {
    this->farmsDic = new AvlTree<int,ServersFarm*>();
    this->windowsTree = new AvlTree<FarmsServerNum, int>();
    this->linuxTree = new AvlTree<FarmsServerNum , int>();
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
        FarmsServerNum windows_key = farm->GetServerNode(WINDOWS);
        FarmsServerNum linux_key = farm->GetServerNode(LINUX);
        delete windows_key;
        delete linux_key;
        delete farm;
    }
    //delete the local array
    delete[] NodeArray;
    //delete the tree
    delete this->farmsDic;

    // delete the servers trees:
    delete this->linuxTree;
    delete this->farmsDic;
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
    FarmsServerNum linuxNum, windowsNum;
    try{
        linuxNum = new ServerOSKey(dataCenterID, numOfServers);
        windowsNum = new ServerOSKey(dataCenterID, 0);
        serverFarm = new ServersFarm(dataCenterID,numOfServers,linuxNum,windowsNum);
    }
    catch (std::bad_alloc &ba) {
        return TREE_ALLOCATION_ERROR;
    }

    // place the node in the farms dic
    Node<int,ServersFarm*>* node;
    if(this->farmsDic->Add(dataCenterID, serverFarm, &node) == TREE_ALLOCATION_ERROR) {
        delete serverFarm;
        delete linuxNum;
        delete windowsNum;
        // TODO: delete serverFarm?
        return TREE_ALLOCATION_ERROR;
    }

    Node<FarmsServerNum,int>* windowsNode;
    Node<FarmsServerNum,int>* linuxNode;
    int zero = 0;
    // place the node in the windows tree
    if (this->windowsTree->Add(windowsNum, zero, &windowsNode) == TREE_ALLOCATION_ERROR) {
        this->farmsDic->Delete(dataCenterID); // TODO: check if we need to delete the value
        delete serverFarm;
        delete linuxNum;
        delete windowsNum;
        return TREE_ALLOCATION_ERROR;
    }
    // place the node in the linux tree
    if (this->linuxTree->Add(linuxNum, numOfServers, &linuxNode) == TREE_ALLOCATION_ERROR) {
        this->farmsDic->Delete(
                dataCenterID); // TODO: check if we need to delete the value
        this->windowsTree->Delete(windowsNum);
        delete serverFarm;
        delete linuxNum;
        delete windowsNum;
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
    FarmsServerNum windows_key = temp->GetServerNode(WINDOWS);
    FarmsServerNum linux_key = temp->GetServerNode(LINUX);
    this->windowsTree->Delete(windows_key);
    this->linuxTree->Delete(linux_key);
    // delete the nodes structs
    delete windows_key;
    delete linux_key;
    // delete the dic structs
    delete temp;
    this->farmsDic->Delete(dataCenterID);

    return TREE_SUCCESS;
}

TreeStatusType FarmsDS::RequestServer(int dataCenterID, int serverID, int os,
                                      int *assignedID) {
    // check if the data center is in the tree
    ServersFarm* temp;
    if(this->farmsDic->Find(dataCenterID,&temp) != TREE_SUCCESS) {
        return TREE_FAILURE;
    }

    if(temp->numOfServers() <= serverID) {
        return TREE_INVALID_INPUT;
    }
    bool* os_changed;
    temp->reqServer(serverID,os,assignedID,os_changed);
    if (os_changed) {
        FarmsServerNum temp = new ServerOSKey(dataCenterID,0);
        Node<FarmsServerNum ,int>* node;
        int* val;
        if (os == WINDOWS) {
            // update the windows tree
            this->windowsTree->Find(temp,val);
            this->windowsTree->Delete(temp);
            temp++;
            *val++;
            this->windowsTree->Add(temp,*val,&node);

            //update the linux tree
            this->linuxTree->Find(temp,val);
            this->linuxTree->Delete(temp);
            temp--;
            *val--;
            this->linuxTree->Add(temp,*val,&node);

        } else {
            // update the windows tree
            this->windowsTree->Find(temp,val);
            this->windowsTree->Delete(temp);
            temp--;
            *val--;
            this->windowsTree->Add(temp,*val,&node);

            //update the linux tree
            this->linuxTree->Find(temp,val);
            this->linuxTree->Delete(temp);
            temp++;
            *val++;
            this->linuxTree->Add(temp,*val,&node);
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

    AvlTree<ServerOSKey*, int>* serversTree;
    if (os == LINUX) {
        serversTree = this->linuxTree;
    } else if (os == WINDOWS){
        serversTree = this->windowsTree;
    } else {
        return TREE_INVALID_INPUT;
    }

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

    Node<FarmsServerNum,int>** nodesArray = new Node<FarmsServerNum,int>*[tree_size]();
    serversTree->GetTreeInOrder(nodesArray);

    for (int i = 0; i < tree_size; ++i) {
        (*dataCenters)[i] = (*nodesArray[i]->key)->GetFarmID();
    }

//    delete[] nodesArray;
    return TREE_SUCCESS;
}
