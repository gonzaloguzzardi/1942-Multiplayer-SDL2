#ifndef SERVER_H
#define SERVER_H

#include "ListaInteligente.h"
#include "AlanTuring.h"
#include "multiqueue.h"
#include "../Utils/Timer.h"
#include "MessageFactory.h"
#include "../Utils/StringHelper.h"
#include "../Utils/Logger.h"
#include "../Game.h"
#include "../Utils/Parser/ParserNivel.h"
#include "../Utils/TextureHelper.h"
#include "../Utils/TiposDefinidos.h"

#include <stdio.h>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <vector>
#include <boost/utility.hpp>

#define TIMEOUT_SECONDS 10
#define TIMEOUT_MICROSECONDS 0

class server
{
    public:
        server(int port,int maxC);
        ~server();

        void escuchar();
        void aceptar();
        void escribir(int id);
        bool leer(int id);
        bool leerBloqueando(int id);
        void closeAllsockets();
        const int getMaxClientes();
        const int getNumClientes();


        /******************************Mensajes***********************************/
        void sendNetworkMsg(int socketReceptor, NetworkMessage netMsg);
        void sendDrawMsgToAll(DrawMessage msg);
        void sendPlayerNames(PlayerNameUpdateInfo playerNameInfo);
        void sendPackToAll(DrawMessagePack msg);
        void sendDrawMsg(int socketReceptor, DrawMessage msg);
        void sendConnectedMsg(int socketReceptor, ConnectedMessage msg);
        void sendResetMsgToAll(ResetInfo resetMsg);
        void sendScoreMsgToAll(ScoreMessage scoreMsg);
        void sendBackgroundInfoToAll(BackgroundInfo backgroundInfo);
        void sendStageStatistics(StageStatistics stageStatistics, int clientID);
        void sendStageBeginningInfoToAll(StageBeginning stageBeginningInfo);
        void sendFinishGameInfoToAll(FinishGameInfo finishGameInfo);
        void sendPlayerDataToClient(PlayerDataUpdateInfo playerDataInfo);

        void sendDisconnectionMsg(int socketReceptor, PlayerDisconnection msg);

        void informPlayerDisconnection(PlayerDisconnection, int playerDiscID);
        void informGameBeginning();
        void informPlayerReconnected(int clientID);
        void informGameBegan(int clientID);
        void informTextureInfos(int clientID);
        /**************************************************************************/

        bool isRunning();

        void encolarDrawMessage(DrawMessage);
        //METODOS LOCOS
         void *procesar(void);
         void *postProcesamiento(void);
         static void *mati_method(void *context);//ES LO UNICO QUE PUEDE TENER GUION BAJO POR LO PODEROSO QUE ES
         static void *mati_method2(void *context);//ESTE ES AUN MAS PODEROSO
         static void *mati_method3(void *context);

    private:

        const int MAX_CLIENTES;
        AlanTuring* m_alanTuring;
        multiqueue<ServerMessage> m_mensajesAProcesar;
        std::vector<multiqueue<NetworkMessage>> m_queuePost;
        bool m_svRunning;
        int m_clientNum;
        ListaInteligente<int> m_listaDeClientes;
        ListaInteligente<Timer> m_listaTimeOuts;
        Timer m_resetTimer;

        std::vector<pthread_t> m_clientThreads;
        std::vector<pthread_t> m_clientResponseThreads;

        int m_lastID;
        bool m_successfulPlayerCreation;
        pthread_mutex_t  m_mutex;
        pthread_cond_t   m_condv;

        int sockfd, newsockfd, portno;

        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        pthread_t threadDeProcesar;

        bool crearCliente (int clientSocket);
        void checkTimeOuts();
        bool lecturaExitosa(int bytesLeidos, int clientID);
        void closeSocket(int id);
        void reducirNumeroClientes();
        void aumentarNumeroClientes();
        void startProcesarThread();
        void *newDialog(void);
        bool procesarMensaje(ServerMessage* serverMsg);
        void error(const char *msg);
        void sendMsg(int socketReceptor, Mensaje msg);
       // void setTimeOut(int socketID);

        void agregarTimeOutTimer(int clientPosition);
        void removeTimeOutTimer(int clientPosition);

        pthread_mutex_t  m_serverInitMutex;

};

#endif // SERVER_H
