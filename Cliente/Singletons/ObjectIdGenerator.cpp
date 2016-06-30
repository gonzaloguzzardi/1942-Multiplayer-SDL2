#include "ObjectIdGenerator.h"

ObjectIdGenerator* ObjectIdGenerator::s_pInstance = 0;

ObjectIdGenerator::ObjectIdGenerator(): m_IdCounterStrike(10)
{
}

int	ObjectIdGenerator::generateId(){

	m_IdCounterStrike++;
	return m_IdCounterStrike;
}

