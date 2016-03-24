/*
 * Parserxml.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#include "ParserCliente.h"


ParserCliente::ParserCliente()
{
}

ParserCliente::~ParserCliente()
{
	m_listaMensajes.clear();
}

bool ParserCliente::parsearDocumento(const std::string& nombreArchivoXML)
{
	bool exito = true;
	if (!parsearDoc(nombreArchivoXML, false))
	{
		//exito = false;
		std::stringstream ss;
		ss << "No se pudo parsear con éxito el archivo XML: " << nombreArchivoXML.c_str() << ".";
		Logger::Instance()->LOG(ss.str(), ERROR);
		ss.clear();
		ss << "Se cargará el archivo default: " << XML_CLIENTE_DEFAULT_PATH << ".";
		Logger::Instance()->LOG(ss.str(), WARN);

		exito = parsearDoc(XML_CLIENTE_DEFAULT_PATH, true);
		if (!exito)
		{
			//El archivo XML default también tiene errores. No se pudo parsear.
			Logger::Instance()->LOG("No se pudieron parsear ninguno de los archivos XML cliente.", ERROR);
		}
	}
	return exito;


}

bool ParserCliente::parsearDoc(const std::string& nombreArchivoXML, bool isDefault)
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
	if (!extraerConexionInfo(&doc, isDefault))
		Logger::Instance()->LOG("No se pudo cargar la información de conexión del cliente.", ERROR);

	if (!extraerMensajes(&doc))
		parseadoExitoso = false;

	return parseadoExitoso;
}


bool ParserCliente::validarRoot(const pugi::xml_document* doc)
{
	bool rootValido = true;
	pugi::xml_node root = doc->root().first_child();
	if (root.empty())
		return false;

	std::string rootName = root.name();
	//limpia el texto del nodo para validar
	quitarCaracteresEspeciales(rootName, true);
	pasarAMinuscula(rootName);
	//checkea que los primeros 5 caracteres del root sean "client". Admite como nodo cualquier string que empiece con client
	if ((rootName.length() < 5) ||
		(((rootName[0] == 'c') && (rootName[1] == 'l') && (rootName[2] == 'i') && (rootName[3] == 'e') && (rootName[4] == 'n') && (rootName[5] == 't')) == false))
	{
		rootValido = false;
	}
	return rootValido;
}

bool ParserCliente::extraerMensajes(const pugi::xml_document* doc)
{
	bool exito = true;
	pugi::xml_node mensajesNode = doc->first_child().child("mensajes");

	for (pugi::xml_node msj = mensajesNode.first_child(); msj; msj = msj.next_sibling())
	{
	   Mensaje mensaje;
	   std::string id = msj.child("id").first_child().value();
	   if (!validarMensajeID(id))
	   {
		   Logger::Instance()->LOG("Error en el parseo de mensajes. ID vacío", DEBUG);
		   exito = false;
		   break;
	   }
	   std::string tipo = msj.child("tipo").first_child().value();
	   if (!validarTipoDeDato(tipo))
	   {
		   std::stringstream ss;
		   ss << "Error en parseo de mensajes, en el mensaje con id " << id << ". Tipo de dato inválido: " << tipo;
		   Logger::Instance()->LOG(ss.str(), DEBUG);
		   exito = false;
		   break;
	   }
	   std::string valor = msj.child("valor").first_child().value();
	   if (!validarValorMensaje(valor))
	   {
		   Logger::Instance()->LOG("Error en el parseo de mensajes. Los valores no pueden ser vacío", DEBUG);
		   exito = false;
		   break;
	   }

	   mensaje.id = id;
	   mensaje.tipo = tipo;
	   mensaje.valor = valor;
	   m_listaMensajes.push_back(mensaje);
	}

	return exito;
}

bool ParserCliente::extraerConexionInfo(const pugi::xml_document* doc, bool isLoadingDefault)
{
	bool exito = true;
	pugi::xml_node conexionNode = doc->first_child().child("conexion");

	std::string ipString = conexionNode.child("IP").first_child().value();
	if (!validarIP(ipString))
	{
		exito = false;
		Logger::Instance()->LOG("Información de IP con errores. Se cargará información de conexión desde archivo default.", WARN);
	}
	std::string puertoString = conexionNode.child("puerto").first_child().value();
	if (!validarPuerto(puertoString))
	{
		exito = false;
		Logger::Instance()->LOG("Información del Puerto con errores. Se cargará información de conexión desde archivo default.", WARN);
	}


	if (!exito)
	{
		//si esta cargando desde el default retorna false, de lo contrario revisaria nuevamente el archivo default.
		if (isLoadingDefault)
			return false;

		if (!extraerConexionInfoDefault())
			return false;
	}
	else
	{
		m_conexionInfo.ip = ipString;
		m_conexionInfo.puerto = std::stoi(puertoString);
	}
	return true;
}

bool ParserCliente::extraerConexionInfoDefault()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(XML_CLIENTE_DEFAULT_PATH);
	if (!result)
		return false;

	pugi::xml_node conexionNode = doc.first_child().child("conexion");

	std::string ipString = conexionNode.child("IP").first_child().value();
	if (!validarIP(ipString))
		return false;

	std::string puerto = conexionNode.child("puerto").first_child().value();
	if (!validarPuerto(puerto))
		return false;

	m_conexionInfo.ip = ipString;
	m_conexionInfo.puerto = std::stoi(puerto);

	return true;
}

bool ParserCliente::validarTipoDeDato(std::string& tipoDatoString)
{
	if (tipoDatoString.empty())
		return false;
	quitarEspacios(tipoDatoString);
	quitarCaracteresEspeciales(tipoDatoString, false);
	pasarAMinuscula(tipoDatoString);
	// Si matchea alguna de las siguientes condiciones devuelve true. Tipo de dato correcto
	if ((strcmp(tipoDatoString.c_str(), "int") == 0) || ((strcmp(tipoDatoString.c_str(), "double") == 0))
		|| (strcmp(tipoDatoString.c_str(), "string") == 0) || (strcmp(tipoDatoString.c_str(), "char") == 0))
		return true;

	return false;
}

bool ParserCliente::validarMensajeID(std::string& tipoDatoString)
{
	return !tipoDatoString.empty();
}
bool ParserCliente::validarValorMensaje(std::string& valorMensajeString)
{
	return !valorMensajeString.empty();
}

bool ParserCliente::validarPuerto(std::string& puertoString)
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


bool ParserCliente::validarIP(std::string& ipString)
{
	if (ipString.empty())
		return false;
	quitarEspacios(ipString);
    int num, puntos = 0;
    char *ptr;
    char *ipStr = new char[ipString.length() + 1];
    std::strcpy(ipStr, ipString.c_str());

    if (ipString.empty())
    {
    	delete[] ipStr;
        return false;
    }

    ptr = strtok(ipStr, ".,");
    if (ptr == NULL)
    {
    	delete[] ipStr;
        return false;
    }

    while (ptr) {
        //Los tokens solo puede contener numeros
        if (!validarDigitos(ptr))
        {
        	delete[] ipStr;
            return false;
        }

        num = atoi(ptr);
        //los numeros tienen que estar en el rango de 1 unsinged byte
        if (num >= 0 && num <= 255) {
            ptr = strtok(NULL, ".,");
            if (ptr != NULL)
                ++puntos;
        } else
        {
        	delete[] ipStr;
            return false;
        }
    }
    //la direccion de ip debe contener 3 puntos
    if (puntos != 3)
    {
    	delete[] ipStr;
        return false;
    }
    delete[] ipStr;
    return true;
}
bool ParserCliente::validarDigitos(char *ipStr)
{
    while (*ipStr) {
        if (*ipStr >= '0' && *ipStr <= '9')
            ++ipStr;
        else
            return false;
    }
    return true;
}
