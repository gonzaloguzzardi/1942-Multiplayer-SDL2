/*
 * Logger.h
 *
 *  Created on: Mar 19, 2016
 *      Author: gonzalo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Time.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

enum LogType { DEBUG, WARN, ERROR };

class Logger
{
public:

	//Singleton
    static Logger* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new Logger();
        }

        return s_pInstance;
    }

    //Funciones de Logueo de Mensajes
    void LOG(const std::string& message, LogType logLevel);

    //Termina el logueo de mensajes. Le da un formato mas prolijo al texto
    void Close();

private:
    //archivo en el que escribe
    std::ofstream m_file;

    string generateFilename();

    //Por Singleton
    static Logger* s_pInstance;
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    //Constructor y Destructor privados
    Logger();
	~Logger();
};

#endif /* LOGGER_H_ */
