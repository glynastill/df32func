/*-------------------------------------------------------------------------
 * df32func.c
 *     df32func extensions for Console Mode DataFlex 3.2
 *
 * Copyright (c) 2007-2009, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *-------------------------------------------------------------------------
 */

#include "df32func.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

SOCKET s, sc; /* Socket handle */

/*
 * The number of cycles used by the processor since the start obtained on x86
 * processors (Intel, AMD), with the assembly command rdtsc.
 */
int rdtsc()
{
    __asm__ __volatile__("rdtsc");
}

/*
 * CLIENTSOCKET – Creates a communication socket and connects to a remote host on the
 *                supplied port and IP
 */
DLLIMPORT int ClientSocket(int PortNo, char* IPAddress){
    /* Start up Winsock */
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    /* Did something happen? */
    if (error){
        return -1;
    }

    /* Did we get the right Winsock version? */
    if (wsadata.wVersion != 0x0202){
        WSACleanup(); /* Clean up Winsock */
        return -1;
    }

    /* Fill out the information needed to initialize a socket */
    SOCKADDR_IN target; /* Socket address information */

    target.sin_family = AF_INET; /*  address family Internet */
    target.sin_port = htons (PortNo); /* Port to connect on */
    target.sin_addr.s_addr = inet_addr (IPAddress); /* Target IP */

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Create socket */
    if (s == INVALID_SOCKET){
        return -1; /* Couldn't create the socket */
    }

    /* Try connecting */
    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR){
        return -1; /* Couldn't connect */
    }
    else{
        return s; /* Success - return our socket number */
    }
}

/*
 * SERVERSOCKET – Creates a communication socket and a tcp server listening on a the
 *                supplied port number
 */
DLLIMPORT int ServerSocket(int PortNo){
    /* Start up Winsock */
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    /* Did something happen? */
    if (error){
        return -1;
    }

    /* Did we get the right Winsock version? */
    if (wsadata.wVersion != 0x0202){
        WSACleanup(); /* Clean up Winsock */
        return -1;
    }

    /* Fill out the information needed to initialize a socket */
    SOCKADDR_IN target; /* Socket address information */

    target.sin_family = AF_INET; /*  address family Internet */
    target.sin_port = htons (PortNo); /* Port to connect on */
    target.sin_addr.s_addr = INADDR_ANY; /* Target IP */

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Create socket */
    if (s == INVALID_SOCKET){
        return -1; /* Couldn't create the socket */
    }

    /* Try to bind to the socket */
    if (bind(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR) {
        return -1; /* Couldn't connect */
    }
    else{
         /* Now we can start listening, We allow SOMNAXCONN connections. This will not return until we get a connection */
         if (listen(s, SOMAXCONN) == SOCKET_ERROR) {
                       return -1;
         }
         else{
              return s;
         }
    }
}

/*
 * CLOSECONNECTION – shuts down a communication socket and closes any connection on it
 */
DLLIMPORT void CloseConnection (int socket){
    /* Close the socket if it exists */
    if (socket){
        closesocket(socket);
    }

    WSACleanup(); /* Clean up Winsock */
}

/*
 * SEND - Send data over the communication socket
 */
DLLIMPORT int Send(int socket, char* data){
         return send(socket, data, strlen(data), 0);
}

/*
 * RECEIVE - Recieve data over the client socket
 */
DLLIMPORT int Receive(int socket, char *pData){
       char data[255] = {0};
       int bytesIn = 0;

       memset(data, 0, sizeof(data));
       bytesIn = recv(socket, data, sizeof(data), 0);
       if (bytesIn == SOCKET_ERROR) {
                  return -1;
       }
       sprintf (pData, "%s", data );
       return bytesIn;
}

/*
 * ACCEPTCLIENT - Accept a client connection
 */
DLLIMPORT int AcceptClient(){
       SOCKADDR_IN client;
       int clientSize = sizeof(client);
       sc = accept(s, (SOCKADDR *)&client, &clientSize);
       if (sc == INVALID_SOCKET){
              return -1;
      }
      return sc;
}

/*
 * Generate a pseudo random integer from an integer
 */
DLLIMPORT unsigned int PseudoRand(unsigned int w){
       unsigned int m_w = w;
       unsigned int m_z = (w / 2);

       m_z = 36969 * (m_z & 65535) + (m_z >> 16);
       m_w = 18000 * (m_w & 65535) + (m_w >> 16);
       return (m_z << 16) + m_w;  /* 32-bit result */
}

/*
 * Generate a random integer from the cpu rdtsc
 */
DLLIMPORT unsigned int RdtscRand(){
     unsigned int n = 0;
     int i;
     /*  First digit must be non-zero: */
     do
     {
            srand(rdtsc());
            n = rand() % 8;
     } while(n == 0);
     for(i = 1; i < 8; i++)
     {
            n *= 8;
            srand(rdtsc());
            n += rand() % 8;
     }
     return n;
}

/*
 * DLL entry point
 */
BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
