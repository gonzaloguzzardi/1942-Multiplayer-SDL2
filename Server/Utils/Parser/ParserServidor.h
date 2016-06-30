/*
 * ParserServidor.h
 *
 *  Created on: Mar 24, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_PARSER_PARSERSERVIDOR_H_
#define UTILS_PARSER_PARSERSERVIDOR_H_

#define XML_SERVIDOR_DEFAULT_PATH "src/Utils/Default/servidor.xml"
#define CANTIDAD_MAXIMA_CLIENTES_ADMISIBLE_XML 32767 //es el valor maximo que se permite como int valido en el xml. Puse uno exageradamente alto

#include "Parser.h"
#include "../Logger.h"
#include "../TiposDefinidos.h"
#include "../pugixml/pugixml.hpp"
#include <string>
#include <cstring>
#include <sstream>

using namespace pugi;

class ParserServidor: public Parser
{
	//Parsea el xml del servidor
	//En caso de que el archivo esté dañado, utiliza un archivo xml default para cargar los datos.

public:

	//Obtiene los datos del documento pasado por parámetro
	bool parsearDocumento(const std::string& nombreArchivoXML);

	ParserServidor();
	~ParserServidor();

	//GETTERS
	const Servidor getServidorInfo() const {
		return m_servidorInfo;
	}
	const LoggerInfo getLoggerInfo() const{
		return m_loggerInfo;
	}

private:

	Servidor m_servidorInfo;
	LoggerInfo m_loggerInfo;

	bool parsearDoc(const std::string& nombreArchivoXML, bool isDefault);
	bool extraerServidorInfo(const pugi::xml_document* doc);
	bool extraerLoggerInfo(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerLoggerInfoDefault();


	//devuelve true si el nodo root es server o servidor, luego de ignorar espacios y caracteres especiales
	bool validarRoot(const pugi::xml_document* doc);

	bool validarCantMaximaClientes(std::string& cantMaximaMensajeString);
	//Checkea si es un numero. No checkea si es un puerto bien conocido (<1024) u otros asuntos de protocolo
	bool validarPuerto(std::string& puertoString);
	bool validarLoggerInfo(std::string& loggerInfoString);
};

#endif /* UTILS_PARSER_PARSERSERVIDOR_H_ */
