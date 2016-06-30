#ifndef PLAYER_H_
#define PLAYER_H_


#include "MoveableObject.h"
#include "Vector2D.h"
#include "Score.h"
#include "Statistics.h"
#include "Singletons/InputHandler.h"
#include "Singletons/TextureManager.h"
#include "Singletons/GameTimeHelper.h"
#include "Utils/TiposDefinidos.h"

class Weapon;
class SecondaryShip;

#define EXPLOTION_ANIMATION_TIME 1000
#define FLIP_ANIMATION_TIME 1500
#define STARTING_LIVES 3
#define STARTING_HEALTH 100


class Player : public MoveableObject
{
public:

	//constructor sin argumento: setea m_controllable siempre en true
    Player();
    //constructor que permite determinar si el player reacciona o no a eventos de input. Si se puede o no controlar
    Player(bool canControl);
    virtual ~Player() {}

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void draw();
    virtual void update();
    virtual void handleInput(InputMessage dataMsg);
    virtual void clean();

    void sendDrawMessage(bool isAlive);

    virtual void collision();

    void setConnected(bool connected) { m_connected = connected; }

    void setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID);
    void setShootingSpeed(int speed);
    void setShootingCooldown(int cooldown);
    void setCollisionDamage(int collisionDamage) { m_collisionDamage = collisionDamage; }
    void setWeapon(Weapon* weapon);

    void refreshDirty() { m_movedByPlayer = false; m_dirty = true;}

    void StopFlipAnimation();
    void startFlipAnimation();

    //Getters
    int getCurrentLives() { return m_lives; }
    bool isDead() { return (m_dead && (m_lives <= 0)); }
    bool hasMoreLives() { return (m_lives < 0); }
    bool isDying() { return m_dying; }
    bool isDoingFlip() { return m_doingFlip; }
    bool isInvulnerable() { return m_invulnerable; }
    bool isConnected() { return m_connected; }
    int getCollisionDamage() { return m_collisionDamage; }
    Statistics getStageStatistics() { return m_stageStats; }
    void resetStageStatistics() { return m_stageStats.reset(); }
    int getLives() {return m_lives;}
    
    void setPlayerTeam(GameTeam playerTeam) { playerGameTeam = playerTeam; }
    GameTeam getPlayerTeam() { return playerGameTeam; }

    void moveAutomatic(const Vector2D& destination, int speed);

    void damage(int damageReceived);
    void takeLife();
    void respawn();

    void createSecondaryShips();
    void shoot();

    void shootWithSecondaryWeapons();


    void addPoints(int points);
    const int getScore();
    void reset();

	void incrementEnemiesKilledStats (int amount) { m_stageStats.incrementEnemiesKilled(amount); }
	void incrementShootsStats (int amount) { m_stageStats.incrementShoots(amount); }
	void incrementHitsStats(int amount) { m_stageStats.incrementHits(amount); }
	void incrementPointsStats (int amount) { m_stageStats.incrementPoints(amount); }


private:
    Weapon* m_currentWeapon;
    SecondaryShip* m_leftSecondaryPlane;
    SecondaryShip* m_rightSecondaryPlane;

    Vector2D m_shootOffset;
    // Determina si el usuario puede controlar este player o no. Sirve por si hay varias instancias de Player en juego.
    // Si m_controllable es false, el Player no reaccionará a eventos de Input
    bool m_connected;
    bool m_doingFlip;

    bool m_hasSecondaryWeapons;

    //Tiempo de animacion de giro
    int m_flipAnimationTime;
    int m_flipRemainingTime;

    //tiempo de animación explosion
    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    //UN timer para esperar un par de milisegundos desde que el jguador dejo de moverse para arrastrarlo
    const int m_holdQuietTimer;
    int m_currentHoldQuietTime;

    //Tiempo que se inicia luego de rspawnear. El jugador es invulnerable durante este tiempo
    const int m_respawningTimer;
    int m_currentRespawningTime;

    void updateFlipAnimation();
    void explote();
    void updateExplotionAnimation();
    void updateRespawningTimer();
    void doRespawningAnimation();

    void updateSecondaryWeapons();

    void moveTo(const Vector2D& destination);

    // variables de control de estado
    int m_lives;
    bool m_dead;
    bool m_dying;
    bool m_exploting;
    bool m_automaticMoving;

    bool m_invulnerable;
    bool m_respawning;

    /*******************/
    int m_health;
    int m_collisionDamage;
    bool m_movedByPlayer;
    GameTeam playerGameTeam;

    Score m_score;
    Statistics m_stageStats;

    Vector2D m_destination;
    int m_autoMoveSpeed;



    //Modifica el estado del juego de acuerdo al input del jugador
};


#endif /* PLAYER_H_ */
