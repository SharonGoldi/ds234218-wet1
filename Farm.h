//
// Created by Dell on 28/11/2019.
//

#ifndef UNTITLED_FARM_H
#define UNTITLED_FARM_H
#include <stdio.h>
#include <assert.h>
#include "ServerOSKey.h"

typedef enum {LINUX = 0,WINDOWS =1} ServerOS;
typedef enum {FARM_SUCCESS = 0, FARM_FAILURE = -1} FarmStatus;

class ServersFarm{

public:
    class Server{
    public:
        int key;
        ServerOS os;
        bool in_use;
        Server* prev;
        Server* next;
        Server(int key = 0);
        ~Server();
    };

    class ServerQueue{
        int size;
        Server* first;
        Server* last;
    public:
        ServerQueue(int size = 0, Server* first = NULL);
        ~ServerQueue();
        ServerQueue& insert(Server& new_node);
        ServerQueue& remove(Server* node);
        Server* getFirst();
        Server* getLast();
        bool isEmpty();
    };

private:
    int farm_id;
    int num_of_servers;
    int linux_in_use;
    int win_in_use;
    Server* servers;
    ServerQueue* windows_servers;
    ServerQueue* linux_servers;
    ServerOSKey* windows_node;
    ServerOSKey* linux_node;
    ServersFarm& findFreeLinuxServer(int* result);
    ServersFarm& findFreeWinServer(int* result);
public:
    ServersFarm(int id,int m, ServerOSKey* linux, ServerOSKey* windows);
    ~ServersFarm();
    FarmStatus reqServer(int server_id,int os,int* assigned_id,bool* os_changed);
    FarmStatus freeServer(int server_id);
    int numOfServers();
    ServerOSKey* GetServerNode(ServerOS os);
};


#endif //UNTITLED_FARM_H

