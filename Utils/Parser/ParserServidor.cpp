/*
 * ParserServidor.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: gonzalo
 */

#include "ParserServidor.h"


ParserServidor::ParserServidor()
{
}

ParserServidor::~ParserServidor()
{
}

bool ParserServidor::parsearDocumento(const std::string& nombreArchivoXML)
{
	bool exito = true;
	if (!parsearDoc(nombreArchivoXML))
	{
		std::stringstream ss;
		ss << "No se pudo parsear el archivo XML: " << nombreArchivoXML.c_str() << ". ";
		Logger::Instance()->LOG(ss.str(), ERROR);
		ss.clear();
		ss << "Se cargará el archivo default: " << XML_SERVIDOR_DEFAULT_PATH << ".";
		Logger::Instance()->LOG(ss.str(), WARN);

		exito = parsearDoc(XML_SERVIDOR_DEFAULT_PATH);
		if (!exito)
		{
			//El archivo XML default también tiene errores. No se pudo parsear.
			Logger::Instance()->LOG("No se pudieron parsear ninguno de los archivos XML servidor.", ERROR);
		}
	}
	return exito;
}

bool ParserServidor::parsearDoc(const std::string& nombreArchivoXML)
{
	bool parseadoExitoso = true;;
	std::stringstream ss;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(nombreArchivoXML.c_str());

	if (!result)
	{
		parseadoExitoso = false;
		//No se pudo abrir el archivo XML
		//LOGUEO DE ERRORES EN CASO DE QUE NO SE PUEDA CARGAR EL ARCHIVO XML
		ss << "Archivo " << nombreArchivoXML.c_str() <<  " dañado. Error Description: " << result.description() << ".";
		Logger::Instance()->LOG(ss.str(), ERROR);

		return parseadoExitoso;
	}

	if (!validarRoot(&doc))
	{
		parseadoExitoso = false;
		Logger::Instance()->LOG("Formato del nodo raiz inválido o vacío.", ERROR);
		//Root Inválido
		return parseadoExitoso;
	}
	if (!extraerServidorInfo(&doc))
	{
		parseadoExitoso = false;
	}

	return parseadoExitoso;
}


bool ParserServidor::validarRoot(const pugi::xml_document* doc)
{
	bool rootValido = true;
	pugi::xml_node root = doc->root().first_child();
	if (root.empty())
		return false;

	std::string rootName = root.name();
	//limpia el texto del nodo para validar
	quitarCaracteresEspeciales(rootName, true);
	pasarAMinuscula(rootName);
	//checkea que los primeros 6 caracteres del root sean "server" o los primeros 8 sean servidor.
	if (((rootName.length() < 6) || (((rootName[0] == 's') && (rootName[1] == 'e') && (rootName[2] == 'r') && (rootName[3] == 'v') && (rootName[4] == 'e') && (rootName[5] == 'r')) == false))
		&& ((rootName.length() < 8) ||
			(((rootName[0] == 's') && (rootName[1] == 'e') && (rootName[2] == 'r') && (rootName[3] == 'v') && (rootName[4] == 'i') && (rootName[5] == 'd')
			 && (rootName[6] == 'o')  && (rootName[7] == 'r'))== false)))
	{
		rootValido = false;
	}
	return rootValido;
}


bool ParserServidor::extraerServidorInfo(const pugi::xml_document* doc)
{
	bool exito = true;
	pugi::xml_node servidorNode = doc->child("servidor");

	std::string cantMaximaClientesString = servidorNode.child("CantidadMaximaClientes").first_child().value();
	if (!validarCantMaximaClientes(cantMaximaClientesString))
	{
		exito = false;
		Logger::Instance()->LOG("Información Cantidad Maxima de Clientes con errores en el archivo xml del servidor.", WARN);
	}
	std::string puertoString = servidorNode.child("puerto").first_child().value();
	if (!validarPuerto(puertoString))
	{
		exito = false;
		Logger::Instance()->LOG("Información del Puerto con errores en el archivo xml del servidor.", WARN);
	}

	if (exito)
	{
		int cantMaxClientes = std::stoi(cantMaximaClientesString);
		if (cantMaxClientes > CANTIDAD_MAXIMA_CLIENTES_ADMISIBLE_XML)
			exito = false;
		m_servidorInfo.cantMaximaClientes = cantMaxClientes;
		m_servidorInfo.puerto = std::stoi(puertoString);
	}

	return exito;
}



bool ParserServidor::validarCantMaximaClientes(std::string& cantMaximaMensajeString)
{
	if (cantMaximaMensajeString.empty())
		return false;
	quitarEspacios(cantMaximaMensajeString);
	if (cantMaximaMensajeString.length() > 5) //un int de mas de 5 digitos?
		return false;
	for (uint i = 0; i < cantMaximaMensajeString.length(); i++)
	{
		if (!std::isdigit(cantMaximaMensajeString[i]))
			return false;
	}
	return true;
}

bool ParserServidor::validarPuerto(std::string& puertoString)
{
	if (puertoString.empty())
		return false;
	quitarEspacios(puertoString);
	if (puertoString.length() > 5) //un int de mas de 5 digitos?
		return false;
	for (uint i = 0 ; i < puertoString.length(); i++)
	{
		if (!std::isdigit(puertoString[i]))
			return false;
	}
	return true;
}
