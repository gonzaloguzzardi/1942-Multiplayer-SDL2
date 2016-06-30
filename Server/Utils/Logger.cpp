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
	pthread_mutex_init(&m_logMutex, NULL);
    m_debugAvailable = true;
    m_warningAvailable = true;
    m_errorAvailable = true;
	 std::string filename = generateFilename();

	m_file.open( filename.c_str(), std::ios::out|std::ios::in|std::ios::app );
	 if (!m_file.good())
	 {
		 perror("ERROR: El Logger no pudo abrir el archivo ");
		 perror (filename.c_str());
		 perror (".\n");
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
  pthread_mutex_destroy(&m_logMutex);
  m_file.close();
}

 void Logger::LOG(const std::string& message, LogType logLevel)
 {
	 pthread_mutex_lock(&m_logMutex);
	 switch(logLevel)
	 {
	 case LogTypeDebug:
		 if (!m_debugAvailable)
			 return;
		 m_file << "  DEBUG: ";
		 break;
	 case LogTypeWarn:
		 if (!m_warningAvailable)
			 return;
		 m_file << "  WARNING: ";
		 break;
	 case LogTypeError:
		 if (!m_errorAvailable)
			 return;
		 m_file << "  ERROR: ";
		 break;
	 default:
		 break;
	 }

	 m_file << message << "\n";
	 m_file.flush();
	pthread_mutex_unlock(&m_logMutex);

 }

 void Logger::setLoglevel(bool debug, bool warn, bool errors)
 {
	 m_debugAvailable = debug;
	 m_warningAvailable = warn;
	 m_errorAvailable = errors;
 }

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
