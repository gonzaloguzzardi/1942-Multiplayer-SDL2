#ifndef UTILS_PARSERNIVEL_H_
#define UTILS_PARSERNIVEL_H_

//#define XML_NIVEL_DEFAULT_PATH "Utils/Default/nivel.xml"
#define XML_NIVEL_DEFAULT_PATH "nivel.xml"
#include "Parser.h"
#include "../Logger.h"
#include "../TiposDefinidos.h"
#include "../pugixml/pugixml.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace pugi;

class ParserNivel: public Parser
{
	//Parsea el xml de un nivel
	//En caso de que el archivo esté dañado, utiliza un archivo xml default para cargar los datos.
	//En caso de que algun campo esté dañado, lo recupera del xml default, sin rechazar el xml completo.

public:

	//Obtiene los datos del documento pasado por parámetro
	bool parsearDocumento(const std::string& nombreArchivoXML);

	ParserNivel();
	~ParserNivel();
	void clean();

	//GETTERS
	const Ventana getVentana() const {
		return m_ventana;
	}
	std::vector<Sprite> getListaSprites() const {
		return m_listaDeSprites;
	}
	const Fondo getFondo() const {
		return m_fondo;
	}
	const Escenario getEscenario() const {
		return m_escenario;
	}
	const Avion getAvion() const 	{
		return m_avion;
	}

private:

	Ventana m_ventana;
	std::vector<Sprite> m_listaDeSprites;
	Fondo m_fondo;
	Escenario m_escenario;
	Avion m_avion;
	std::vector<Elemento> m_listaDeElementos;


	bool parsearDoc(const std::string& nombreArchivoXML, bool isDefault);
	bool extraerSprites(const pugi::xml_document* doc);
	bool extraerVentana(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerEscenario(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerFondo(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerElementos(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerAvion(const pugi::xml_document* doc, bool isLoadingDefault);
	//bool extraerConexionInfoDefault();

	//devuelve true si el nodo root del documento comienza con la palabra ventana, ignorando caracteres especiales y espacios
	bool validarRoot(const pugi::xml_document* doc);
	//Checkea si  es un sprite bien .
	bool validarSprite(Sprite* sprite);
	//Checkea si es un numero
	bool validarNumero(std::string& intString);
	bool validarPath(std::string& string);
	bool is_file_exist(std::string& sfileName);
	//checkea que la ventana tenga el formato correspondiente
	bool validarVentana(Ventana* ventana);
	bool validarEscenario(Escenario *escenario);
	bool validarFondo(Fondo* fondo);
	//tipos de datos validos: int, double, string, char
	bool validarTipoDeDato(std::string& tipoDatoString);
	//Mensaje ID y valor de mensaje solo chekea que el campo no esté vacío, ya que un string puede ser cualquier cosa.
	//bool validarMensajeID(std::string& tipoDatoString);
	//bool validarValorMensaje(std::string& valorMensajeString);
};


#endif /* UTILS_PARSERNIVEL_H_ */
