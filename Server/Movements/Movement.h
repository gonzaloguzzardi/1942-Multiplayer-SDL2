#ifndef MOVEMENT_H_
#define MOVEMENT_H_

class Movement {
public:
	Movement();
	virtual ~Movement();
	void update(){}
	float getX(){return x;}
	float getY(){return y;}

protected:
	float x,y,m_delay,m_angle,m_amplitud;
};


#endif /* MOVEMENT_H_ */
