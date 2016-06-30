/*
 * tiposDefinidos.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

/*
 * tiposDefinidos.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_TIPOSDEFINIDOS_H_
#define UTILS_TIPOSDEFINIDOS_H_

#define MESSAGE_BUFFER_SIZE 256
#define MESSAGE_ID_BYTES_LIMIT 25
#define MESSAGE_LENGTH_BYTES 4
#define MESSAGE_CODE_BYTES 3
#define MESSAGE_DATA_SIZE (MESSAGE_BUFFER_SIZE - MESSAGE_LENGTH_BYTES - MESSAGE_CODE_BYTES)
#define MESSAGE_VALUE_SIZE (MESSAGE_DATA_SIZE - MESSAGE_ID_BYTES_LIMIT - 1)
#define MAX_NAME_LENGTH 24
#define PATH_MAX_LENGTH 36

#define DRAW_MESSAGE_SIZE 32
#define INPUT_MESSAGE_SIZE 20
#define SCORE_MESSAGE_SIZE 8
#define STAGE_STATISTICS_SIZE 8
#define CONNECTED_MESSAGE_SIZE 16
#define CONNECTIONINFO_MESSAGE_SIZE 24
#define PLAYER_DISCONNECTION_MESSAGE_SIZE 32
#define RESET_MESSAGE_SIZE 4
#define BACKGROUNDINFO_MESSAGE_SIZE 4
#define TEXTUREINFO_MESSAGE_SIZE (PATH_MAX_LENGTH + 12)
#define PLAYER_RECONNECTION_MEESAGE 4

#define DRAW_MESSAGE_PACK_SIZE  10

#define BACKGROUND 1
#define MIDDLEGROUND 10
#define FOREGROUND 20

#include <string>
#include <vector>
#include <SDL2/SDL.h>
class Enemy;
class PowerUp;

// TIPOS RELACIONADOS A ARCHIVOS XML

struct LoggerInfo
{
	bool debugAvailable;
	bool warningAvailable;
	bool errorAvailable;
};

struct Servidor{
	int cantMaximaClientes;
	int puerto;
};

struct Conexion{
	std::string ip;
	int puerto;
};

struct PlayerNameUpdateInfo
{
	int playerID;
	char playerName[24];
};

struct PlayerDataUpdateInfo
{
	int playerID;
	int playerLives;
};

struct Mensaje{
	std::string id;
	std::string tipo;
	std::string valor;
};

//TIPOS DE DATOS RELACIONADOS CON MENSAJES DE RED

struct NetworkMessage
{
	int msg_Length;
	char msg_Code[MESSAGE_CODE_BYTES];
	char msg_Data[MESSAGE_DATA_SIZE];
};

struct ConnectedMessage
{
	bool requestData;
	bool connected;
	int objectID;
	int textureID;
	short windowWidth;
	short windowHeight;
	bool teamMode;
    char teamsName[128];
	short cantPlayers;
};

struct ConnectionInfo
{
    char playerName[24];
    int playerTeamID;
};

struct FinishGameInfo
{
	bool isVictory;
	short winnerID;
	short points;

};

struct StageBeginning
{
	int stageNumber;
};

struct ResetInfo
{
	short windowWidth;
	short windowHeight;
};

struct PlayerDisconnection
{
	int objectID;
	int layer;
	char name[24];
};

struct BackgroundInfo
{
	int backgroundOffset;
};
struct StageStatistics
{
	short accuracy;
	short enemiesKilled;
	short points;
	short something;
};

struct DrawMessage
{
	bool unused1;
	bool connectionStatus;
	bool alive;
	bool hasSound;
	int objectID;
	int textureID;
	short soundID;
	short layer;
	short posX;
	short posY;
	short row;
	short column;
	float angle;
	short alpha;
	short vacio;
};

struct ScoreMessage
{
	short playerID;
	short teamID;
	short pointsacquire;
	short somethingElse;
};

struct InputMessage
{
	int objectID;
	short buttonUp;
	short buttonDown;
	short buttonRight;
	short buttonLeft;
	short buttonShoot;
	short buttonRoll;
	short togglePracticeMode;
	short something;
};


struct DataMessage
{
	char msg_status;
	char msg_ID[MESSAGE_ID_BYTES_LIMIT];
	char msg_value[MESSAGE_VALUE_SIZE];
};

struct TextureInfo
{
	bool lastTexture;
	int textureID;
	short width;
	short height;
	short numFrames;
	char path[PATH_MAX_LENGTH];
};

struct PlayerReconnectionInfo
{
	int playerID;
};

struct DrawMessagePack
{
	int totalSize;
	DrawMessage drawMessages[DRAW_MESSAGE_PACK_SIZE];
};

struct FontTexture
{
	const char* text;
	SDL_Texture* texture;
	short width;
	short height;
	short x;
	short y;
};

struct EnemySpawnInfo
{
	Enemy* enemyToSpawn;
	int stagePosition;
	int posX;
	int posY;
};
struct PowerUpSpawnInfo
{
	PowerUp* powerUpToSpawn;
	int stagePosition;
	int posX;
	int posY;
};

struct Ventana
{
	int ancho;
	int alto;

};

struct Enemigo
{
	std::string id;
	int cantidad;
	int frames;
	int ancho;
	int alto;

};

struct Jefe
{
	std::string id;
	int posicion;
	int frames;
	int ancho;
	int alto;

};

struct Color
{
	Uint8 value_R;
	Uint8 value_G;
	Uint8 value_B;
	Uint8 value_A;
};

struct Powerup
{
	std::string id;
	int cantidad;
	int frames;
	int ancho;
	int alto;

};
struct Sprite
{
	std::string id;
	std::string path;
	int cantidad;
	int ancho;
	int alto;

};
struct Fondo
{
	std::string spriteId;
	int ancho;
	int alto;

};

struct Posicion
{
	int x;
	int y;
};

struct Elemento
{
	std::string spriteId;
	struct Posicion posicion;

};

typedef struct {
    int gameTeamID;
    int gameTeamScore;
    std::string gameTeamName;
} GameTeam;

typedef enum
{
    GameModeCooperative = 0,
    GameModeCompetition
} GameMode;

struct Escenario
{
	int velScroll;
	int cantidadJugadores;
	int cantidadStages;
	int ancho;
	int alto;
	struct Fondo fondo;
	std::vector<struct Elemento> listaDeElementos;
    std::vector<GameTeam> teamsList;
    GameMode gameMode = GameModeCooperative;
};

struct Avion
{
	int velDespl;
	int velDisp;
	int cdDisp;
	std::string avionSprId;
	std::string vueltaSprId;
	std::string disparoSprId;

};






/*struct intMessage
{
	char msg_ID[MESSAGE_ID_BYTES_LIMIT];
	int msg_value;
};
struct charMessage
{
	char msg_ID[MESSAGE_ID_BYTES_LIMIT];
	char msg_value;
};
struct doubleMessage
{
	char msg_ID[MESSAGE_ID_BYTES_LIMIT];
	double msg_value;
};
struct stringMessage
{
	char msg_ID[MESSAGE_ID_BYTES_LIMIT];
	std::string msg_value;
};*/

#endif /* UTILS_TIPOSDEFINIDOS_H_ */
