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
	if (!parsearDoc(nombreArchivoXML, false))
	{
		std::stringstream ss;
		ss << "No se pudo parsear el archivo XML: " << nombreArchivoXML.c_str() << ". ";
		Logger::Instance()->LOG(ss.str(), LogTypeError);
		ss.clear();
		ss << "Se cargará el archivo default: " << XML_SERVIDOR_DEFAULT_PATH << ".";
		Logger::Instance()->LOG(ss.str(), LogTypeWarn);

		exito = parsearDoc(XML_SERVIDOR_DEFAULT_PATH, true);
		if (!exito)
		{
			//El archivo XML default también tiene errores. No se pudo parsear.
			Logger::Instance()->LOG("No se pudieron parsear ninguno de los archivos XML servidor.", LogTypeError);
		}
	}
	return exito;
}

bool ParserServidor::parsearDoc(const std::string& nombreArchivoXML, bool isDefault)
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
		Logger::Instance()->LOG(ss.str(), LogTypeError);

		return parseadoExitoso;
	}

	if (!validarRoot(&doc))
	{
		parseadoExitoso = false;
		Logger::Instance()->LOG("Formato del nodo raiz inválido o vacío.", LogTypeError);
		//Root Inválido
		return parseadoExitoso;
	}
	if (!extraerServidorInfo(&doc))
	{
		parseadoExitoso = false;
	}
	if (!extraerLoggerInfo(&doc, isDefault))
		Logger::Instance()->LOG("No se pudo cargar la información de logger del servidor.", LogTypeError);

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
		Logger::Instance()->LOG("Información Cantidad Maxima de Clientes con errores en el archivo xml del servidor.", LogTypeWarn);
	}
	std::string puertoString = servidorNode.child("puerto").first_child().value();
	if (!validarPuerto(puertoString))
	{
		exito = false;
		Logger::Instance()->LOG("Información del Puerto con errores en el archivo xml del servidor.", LogTypeWarn);
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

bool ParserServidor::extraerLoggerInfo(const pugi::xml_document* doc, bool isLoadingDefault)
{
	bool exito = true;
	pugi::xml_node loggerNode = doc->first_child().child("logger");

	std::string debugString = loggerNode.child("debug").first_child().value();
	if (!validarLoggerInfo(debugString))
	{
		exito = false;
		Logger::Instance()->LOG("Información de Logger con errores. Se cargará información de logger desde archivo default.", LogTypeWarn);
	}
	std::string warningsString = loggerNode.child("warning").first_child().value();
	if (!validarLoggerInfo(warningsString))
	{
		exito = false;
		Logger::Instance()->LOG("Información de Logger con errores. Se cargará el archivo", LogTypeWarn);
	}
	std::string errorString = loggerNode.child("error").first_child().value();
	if (!validarLoggerInfo(errorString))
	{
		exito = false;
		Logger::Instance()->LOG("Información de Logger con errores. Se cargará información de logger desde archivo default.", LogTypeWarn);
	}

	if (!exito)
	{
		//si esta cargando desde el default retorna false, de lo contrario revisaria nuevamente el archivo default.
		if (isLoadingDefault)
			return false;

		if (!extraerLoggerInfoDefault())
			return false;
	}
	else
	{
		if (debugString.compare("true") == 0)
			m_loggerInfo.debugAvailable = true;
		else
			m_loggerInfo.debugAvailable = false;

		if (warningsString.compare("true") == 0)
			m_loggerInfo.warningAvailable = true;
		else
			m_loggerInfo.warningAvailable = false;

		if (errorString.compare("true") == 0)
			m_loggerInfo.errorAvailable = true;
		else
			m_loggerInfo.errorAvailable = false;
	}
	return true;
}

bool ParserServidor::extraerLoggerInfoDefault()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(XML_SERVIDOR_DEFAULT_PATH);
	if (!result)
		return false;

	pugi::xml_node loggerNode = doc.first_child().child("logger");

	std::string debugString = loggerNode.child("debug").first_child().value();
	if (!validarLoggerInfo(debugString))
	{
		return false;
	}
	std::string warningsString = loggerNode.child("warning").first_child().value();
	if (!validarLoggerInfo(warningsString))
	{
		return false;
	}
	std::string errorString = loggerNode.child("error").first_child().value();
	if (!validarLoggerInfo(errorString))
	{
		return false;
	}

	if (debugString.compare("true") == 0)
		m_loggerInfo.debugAvailable = true;
	else
		m_loggerInfo.debugAvailable = false;

	if (warningsString.compare("true") == 0)
		m_loggerInfo.warningAvailable = true;
	else
		m_loggerInfo.warningAvailable = false;

	if (errorString.compare("true") == 0)
		m_loggerInfo.errorAvailable = true;
	else
		m_loggerInfo.errorAvailable = false;

	return true;
}

bool ParserServidor::validarLoggerInfo(std::string& loggerInfoString)
{
	if (loggerInfoString.empty())
		return false;
	quitarEspacios(loggerInfoString);
	quitarCaracteresEspeciales(loggerInfoString, false);
	pasarAMinuscula(loggerInfoString);

	if ((loggerInfoString.compare("true") != 0) && (loggerInfoString.compare("false") != 0))
	{
		return false;
	}
	return true;
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
