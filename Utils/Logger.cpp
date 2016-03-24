/*
 * Logger.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: gonzalo
 */

#include "Logger.h"

Logger* Logger::s_pInstance = 0;

Logger::Logger()
{
	 std::string filename = generateFilename();

	m_file.open( filename.c_str(), std::ios::out|std::ios::in|std::ios::app );
	 if (!m_file.good())
	 {
		 perror("ERROR: El Logger no pudo abrir el archivo. \n");
		 return;
	 }
	 m_file << "  ===============================================\n"
	          << "    Comienzo del Log ( "
	          << Time::getDate()
	          << " a las "
	          << Time::getTime()
	          << " ) \n  ===============================================\n\n";
	 m_file.flush();
}

Logger::~Logger()
{
  if (!m_file.good()) return;
  m_file << "\n  ===============================================\n"
        << "    Finaliza el Log ( "
        << Time::getDate()
        << " a las "
        << Time::getTime()
        << " ) \n  ===============================================\n\n";
  m_file.flush();
  m_file.close();
}

 void Logger::LOG(const std::string& message, LogType logLevel)
 {
	 switch(logLevel)
	 {
	 case DEBUG:
		 m_file << "  DEBUG: ";
		 break;
	 case WARN:
		 m_file << "  WARNING: ";
		 break;
	 case ERROR:
		 m_file << "  ERROR: ";
		 break;
	 default:
		 break;
	 }

	 m_file << message << "\n";
	 m_file.flush();

 }

 /*void Logger::LOG(const std::string& message)
 {
	 m_file << "  DEBUG: " << message << "\n";
	 m_file.flush();

 }*/

 void Logger::Close()
 {
	 delete s_pInstance;
	 s_pInstance = 0;
 }

string Logger::generateFilename()
{
	 std::stringstream ss;
	 ss << "Logfiles/logfile " << Time::getDate() << ".log";
	 return ss.str();
}
