/*
 * ParserStage.h
 *
 *  Created on: Jun 12, 2016
 *      Author: juampa_94
 */

#ifndef PARSERSTAGE_H_
#define PARSERSTAGE_H_

#define XML_STAGE_DEFAULT_PATH "Stage1.xml"
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


class ParserStage : public Parser{

	//Parsea el xml de un stage
	//En caso de que el archivo esté dañado, utiliza un archivo xml default para cargar los datos.
	//En caso de que algun campo esté dañado, lo recupera del xml default, sin rechazar el xml completo.

public:

	//Obtiene los datos del documento pasado por parámetro
	bool parsearDocumento(const std::string& nombreArchivoXML);

	ParserStage();
	virtual ~ParserStage();
	void clean();

	const Ventana getVentana() const {
			return m_ventana;
		}
	std::vector<Enemigo> getListaDeEnemigos(){
		return m_listaDeEnemigos;
	}
	std::vector<Powerup> getListaDePowerups(){
		return m_listaDePowerups;
	}
	std::vector<Jefe> getListaDeJefes(){
		return m_listaDeJefes;
	}


private:
	Ventana m_ventana;
	std::vector<Enemigo> m_listaDeEnemigos;
	std::vector<Powerup> m_listaDePowerups;
	std::vector<Jefe> m_listaDeJefes;


	bool parsearDoc(const std::string& nombreArchivoXML, bool isDefault);
	bool extraerEnemigos(const pugi::xml_document* doc);
	bool extraerVentana(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerPowerups(const pugi::xml_document* doc, bool isLoadingDefault);
	bool extraerJefes(const pugi::xml_document* doc, bool isLoadingDefault);
	bool validarNumero(std::string& intString);


};

#endif /* PARSERSTAGE_H_ */
