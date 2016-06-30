/*
 * Parserxml.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_PARSERCLIENTE_H_
#define UTILS_PARSERCLIENTE_H_

#define XML_CLIENTE_DEFAULT_PATH "src/Utils/Default/cliente.xml"

#include "Parser.h"
#include "../Logger.h"
#include "../TiposDefinidos.h"
#include "../pugixml/pugixml.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

using namespace pugi;

class ParserCliente: public Parser
{
	//Parsea el xml de un cliente
	//En caso de que el archivo esté dañado, utiliza un archivo xml default para cargar los datos.
	//En caso de que SOLO el campo conexiones esté dañado, lo recupera del xml default, sin rechazar el xml completo.

public:

	//Obtiene los datos del documento pasado por parámetro
	bool parsearDocumento(const std::string& nombreArchivoXML);

	ParserCliente();
	~ParserCliente();

	//GETTERS
	const Conexion getConexionInfo() const {
		return m_conexionInfo;
	}
	std::vector<Mensaje> getListaMensajes() const {
		return m_listaMensajes;
	}

private:

	Conexion m_conexionInfo;
	std::vector<Mensaje> m_listaMensajes;

	bool parsearDoc(const std::string& nombreArchivoXML, bool isDefault);
	bool extraerMensajes(const pugi::xml_document* doc);
	bool extraerConexionInfo(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerConexionInfoDefault();

	//devuelve true si el nodo root del documento comienza con la palabra client, ignorando caracteres especiales y espacios
	bool validarRoot(const pugi::xml_document* doc);
	//Checkea si es un numero. No checkea si es un puerto bien conocido (<1024) u otros asuntos de protocolo
	bool validarPuerto(std::string& puertoString);
	//checkea que el ip tenga el formato correspondiente a ipv4
	bool validarIP(std::string& ip);
	bool validarDigitos(char *ipStr);
	//tipos de datos validos: int, double, string, char
	bool validarTipoDeDato(std::string& tipoDatoString);
	//Mensaje ID y valor de mensaje solo chekea que el campo no esté vacío, ya que un string puede ser cualquier cosa.
	bool validarMensajeID(std::string& tipoDatoString);
	bool validarValorMensaje(std::string& valorMensajeString);
};


#endif /* UTILS_PARSERCLIENTE_H_ */
