#ifndef CLIENTE_H
#define CLIENTE_H

#include "../Utils/Logger.h"
#include "../Cliente/mensije.h"
#include "../Cliente/multiqueue.h"
#include "AlanTuring.h"
#include "MessageFactory.h"
#include "../Utils/Timer.h"
#include "../Utils/TiposDefinidos.h"

#include <stdio.h>
//#include <X11/Xlib.h>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <iostream>
#include <sys/time.h>
#include "../Game.h"

#define TIMEOUT_SECONDS 10
#define TIMEOUT_MICROSECONDS 0

#define DISABLE_NAGEL_ALGORITHM 1

class cliente
{
    public:
        cliente(int argc, string ip,int port, std::string name );
        ~cliente();
        bool conectar();
        void desconectar();
        void escribir(Mensaje mensaje);
        void sendMsg(Mensaje msg);
        void sendNetworkMsg(NetworkMessage netMsg);
        void sendInputMsg(InputMessage msg);
        void sendConnectionInfoMsg(ConnectionInfo msg);
        bool leer();
        bool isConnected();
        bool isConnecting();
        bool checkServerConnection();

        //timeout
        void *sendTimeOuts(void);
        void createTimeoutThread();
        static void *timeOut_method(void *context);
        bool sendTimeOutMsg();

    private:
        AlanTuring* m_alanTuring;
        int sockfd, portno;
        bool m_connected;
        bool m_connecting;

        Timer* serverTimeOut;
        Timer* sendTimeOutTimer;
        pthread_t timeOutThread;
        pthread_mutex_t  m_readingMutex;
        pthread_mutex_t  m_writingMutex;
        pthread_cond_t   m_condv;

        struct sockaddr_in serv_addr;
        struct hostent *server;
        void error(const char *msg);
        std::vector<Mensaje> listaDeMensajes;

        char buffer[256];

        void procesarDrawPackage(DrawMessagePack drawMsgPackage);
        void cerrarSoket();
        bool disableNagelAlgorithm(int socketID);
        void procesarMensaje(NetworkMessage networkMessage);
        void setTimeOut();
        bool validarMensaje(DataMessage dataMsg);
        bool lecturaExitosa(int bytesLeidos);

};

#endif // CLIENTE_H
