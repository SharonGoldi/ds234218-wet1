#include "Farm.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

// class server code: (node for Queue)

ServersFarm::Server::Server(int key):key(key),os(LINUX),in_use(false){
    this->next = NULL;
    this->prev = NULL;
}
ServersFarm::Server::~Server() {
    this->next = NULL;
    this->prev =NULL;
}

////////////////////////////////////////////////////////////////////////////////

// class ServerQueue code:

ServersFarm::ServerQueue::ServerQueue(int size ,Server* first):
                size(0), first(NULL), last(NULL){}

ServersFarm::ServerQueue::~ServerQueue(){
    this->last = NULL;
    this->last = NULL;
    Server* curr = this->first;
    while (this->size > 0){
        Server* temp = curr;
        curr = curr->next;
        delete temp;
        (this->size)--;
    }
}

ServersFarm::ServerQueue& ServersFarm::ServerQueue::insert(Server& new_node) {
    if (this->isEmpty()){
        this->first = &new_node;
        new_node.prev = NULL;
    } else {
        new_node.prev = this->last;
        new_node.prev->next = &new_node;
    }
    new_node.next = NULL;
    this->last = &new_node;
    this->size++;

    return *this;
}

ServersFarm::ServerQueue& ServersFarm::ServerQueue::remove(Server* node) {
    assert(!this->isEmpty());

    if (this->size == 1) {
        this->first = NULL;
        this->last = NULL;
    }
    else if (this->first == node){
        this->first = this->first->next;
        this->first->prev = NULL;
    }
    else if(this->last == node) {
        this->last = this->last->prev;
        this->last->next = NULL;
    }
    else {
        Server* next = node->next;
        Server* prev = node->prev;
        next->prev = node->prev;
        prev->next = node->next;
    }
    node->prev = NULL;
    node->next = NULL;
    this->size--;

    return *this;
}

bool ServersFarm::ServerQueue::isEmpty() {
    return this->size == 0;
}

ServersFarm::Server *ServersFarm::ServerQueue::getFirst() {
    return this->first;
}

ServersFarm::Server *ServersFarm::ServerQueue::getLast() {
    return this->last;
}

////////////////////////////////////////////////////////////////////////////////

// class ServersFarm code:

ServersFarm::ServersFarm(int id,int m, FarmsServerNum linux, FarmsServerNum windows):
        farm_id(id),num_of_servers(m),linux_in_use(0),win_in_use(0){
    this->linux_node = linux;
    this->windows_node = windows;
    this->linux_servers = new ServerQueue();
    this->windows_servers = new ServerQueue();
    this->servers = new ServersFarm::Server[m];

    for(int i= 0; i < m; i++){
        this->servers[i].key = i;
        this->linux_servers->insert(this->servers[i]);
    }
};

FarmStatus ServersFarm::reqServer(int server_id, int os,int* assigned_id,bool* os_changed) {
    if (server_id > this->num_of_servers){
        *assigned_id = -1;
        return FARM_FAILURE;
    }
    *assigned_id = server_id;
    if (this->servers[server_id].in_use){
        if (os == (int)WINDOWS){
            this->findFreeWinServer(assigned_id);
            if (*assigned_id == -1){
                this->findFreeLinuxServer(assigned_id);
            }
        }
        else{
            this->findFreeLinuxServer(assigned_id);
            if (*assigned_id == -1){
                this->findFreeWinServer(assigned_id);
            }
        }
    }

    this->servers[*assigned_id].in_use = true;
    *os_changed = this->servers[*assigned_id].os != os;

    switch (this->servers[*assigned_id].os){
        case LINUX:
            this->linux_servers->remove(&this->servers[*assigned_id]);
            this->linux_in_use++;
            break;
        case WINDOWS:
            this->windows_servers->remove(&this->servers[*assigned_id]);
            this->win_in_use++;
            break;
    }
    this->servers[*assigned_id].os = (ServerOS) os;
    return FARM_SUCCESS;
}

FarmStatus ServersFarm::freeServer(int server_id) {
    if (server_id > this->num_of_servers || !(this->servers[server_id].in_use) ){
        return FARM_FAILURE;
    }
    this->servers[server_id].in_use = false;
    switch (this->servers[server_id].os){
        case WINDOWS :
            this->win_in_use--;
            this->windows_servers->insert(this->servers[server_id]);
            break;
        case LINUX:
            this->linux_in_use--;
            this->linux_servers->insert(this->servers[server_id]);
            break;
    }
    return FARM_SUCCESS;
}
ServersFarm& ServersFarm::findFreeWinServer(int* result) {
    if (this->windows_servers->isEmpty()) {
        *result = -1;
    } else {
        *result = this->windows_servers->getFirst()->key;
    }

    return *this;
}

ServersFarm& ServersFarm::findFreeLinuxServer(int* result) {
    if (this->linux_servers->isEmpty()) {
        *result = -1;
    } else {
        *result = this->linux_servers->getFirst()->key;
    }

    return *this;
}

ServersFarm::~ServersFarm() {
    delete this->windows_servers;
    delete this->linux_servers;
    delete[] this->servers;
}

int ServersFarm::numOfServers() {
    return this->num_of_servers;
}

FarmsServerNum ServersFarm::GetServerNode(ServerOS os) {
    assert(os == LINUX || os == WINDOWS);
    if (os == LINUX) {
        return this->linux_node;
    }
    return this->windows_node;

}