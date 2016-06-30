/*
 * ObjectIdGenerator.h
 *
 *  Created on: May 2, 2016
 *      Author: matias
 */

#ifndef OBJECTIDGENERATOR_H_
#define OBJECTIDGENERATOR_H_


#include <iostream>
#include "../Utils/TiposDefinidos.h"


class ObjectIdGenerator
{
public:

    static ObjectIdGenerator* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new ObjectIdGenerator();
            return s_pInstance;
        }
        return s_pInstance;
    }

    int generateId();

private:

    int m_IdCounterStrike;
    static ObjectIdGenerator* s_pInstance;

    ObjectIdGenerator();
    ~ObjectIdGenerator() {}
    ObjectIdGenerator(const ObjectIdGenerator&);
    ObjectIdGenerator& operator=(const ObjectIdGenerator&);
};

#endif /* OBJECTIDGENERATOR_H_ */
