/*
 * ParserStage.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: juampa_94
 */

#include "ParserStage.h"

ParserStage::ParserStage() {
	// TODO Auto-generated constructor stub

}

ParserStage::~ParserStage() {
    if (m_listaDeEnemigos.size() > 0) {
        m_listaDeEnemigos.clear();
    }
    
    if (m_listaDePowerups.size() > 0) {
        m_listaDePowerups.clear();
    }
    
    if (m_listaDeJefes.size() > 0) {
        m_listaDeJefes.clear();
    }
}

void ParserStage::clean() {
    if (m_listaDeEnemigos.size() > 0) {
        m_listaDeEnemigos.clear();
    }
    
    if (m_listaDePowerups.size() > 0) {
        m_listaDePowerups.clear();
    }
    
    if (m_listaDeJefes.size() > 0) {
        m_listaDeJefes.clear();
    }
}

bool ParserStage::parsearDocumento(const std::string& nombreArchivoXML)
{
	bool exito = true;
	if (!parsearDoc(nombreArchivoXML, false))
	{
		std::stringstream ss;
		ss << "No se pudo parsear con éxito el archivo XML: " << nombreArchivoXML.c_str() << ".";
		Logger::Instance()->LOG(ss.str(), LogTypeError);
		ss.clear();
		ss << "Se cargará el archivo default: " << XML_STAGE_DEFAULT_PATH << ".";
		Logger::Instance()->LOG(ss.str(), LogTypeWarn);

		exito = parsearDoc(XML_STAGE_DEFAULT_PATH, true);
		if (!exito)
		{
			//El archivo XML default también tiene errores. No se pudo parsear.
			Logger::Instance()->LOG("No se pudieron parsear ninguno de los archivos XML nivel.", LogTypeError);
			//cout << "No se pudieron parsear ninguno delos archivos XML nivel";
		}
	}
	//cout << "Si se cargo \n";
	return exito;


}

bool ParserStage::parsearDoc(const std::string& nombreArchivoXML, bool isDefault)
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


	extraerEnemigos(&doc);
	extraerJefes(&doc,0);
	extraerPowerups(&doc,0);
	extraerVentana(&doc,0);
	return parseadoExitoso;

}

bool ParserStage::extraerEnemigos(const pugi::xml_document* doc)
{
	bool exito = true;
	pugi::xml_node spritesNode = doc->child("enemigos");

	for (pugi::xml_node spr = spritesNode.first_child(); spr; spr = spr.next_sibling())
	{
	   Enemigo enemigo;
	   std::string id = spr.child("id").first_child().value();
	   std::string frames = spr.child("frames").first_child().value();
	   std::string cantidad = spr.child("cantidad").first_child().value();
	   std::string ancho = spr.child("ancho").first_child().value();
	   std::string alto = spr.child("alto").first_child().value();
	   if (!validarNumero(cantidad))
			{
		   	   Logger::Instance()->LOG("Se cargo el valor cantidad del enemigo por default", LogTypeWarn);
				cantidad = "2";
			}
	   if (!validarNumero(ancho))
	   			{
		   	   	   Logger::Instance()->LOG("Se cargo el valor ancho del enemigo por default", LogTypeWarn);
	   				ancho = "11";
	   			}
	   if (!validarNumero(alto))
	  	   			{
		   	   	   	   Logger::Instance()->LOG("Se cargo el valor alto del enemigo por default", LogTypeWarn);
	  	   				alto = "11";
	  	   			}
	   enemigo.id = id;
	   enemigo.cantidad = stoi(cantidad);
	   enemigo.ancho = stoi(ancho) ;
	   enemigo.alto = stoi(alto);
	   enemigo.frames = stoi(frames);
	   m_listaDeEnemigos.push_back(enemigo);
	}
	return exito;

}

bool ParserStage::extraerVentana(const pugi::xml_document* doc, bool isLoadingDefault)
{
	bool exito = true;
	pugi::xml_node ventanaNode = doc->child("tamaño");
	std::string anchoString = ventanaNode.child("ancho").first_child().value();
	std::string altoString = ventanaNode.child("alto").first_child().value();

	if (!validarNumero(anchoString))
		{
			Logger::Instance()->LOG("Se cargo el ancho de la ventana por default", LogTypeWarn);
			anchoString = "810";
		}
	if (!validarNumero(altoString))
		{
			Logger::Instance()->LOG("Se cargo el alto de la ventana por default", LogTypeWarn);
			altoString = "610";
		}

	m_ventana.alto = stoi(altoString);
	m_ventana.ancho = stoi(anchoString);
	return true;


}

bool ParserStage::extraerPowerups(const pugi::xml_document* doc, bool isLoadingDefault)
{
	bool exito = true;
	pugi::xml_node spritesNode = doc->child("powerups");

	for (pugi::xml_node spr = spritesNode.first_child(); spr; spr = spr.next_sibling())
	{
	   Powerup powerup;
	   std::string id = spr.child("id").first_child().value();
	   std::string frames = spr.child("frames").first_child().value();
	   std::string cantidad = spr.child("cantidad").first_child().value();
	   std::string ancho = spr.child("ancho").first_child().value();
	   std::string alto = spr.child("alto").first_child().value();
	   if (!validarNumero(cantidad))
			{
			   Logger::Instance()->LOG("Se cargo el valor cantidad del  por default", LogTypeWarn);
				cantidad = "2";
			}
	   if (!validarNumero(ancho))
				{
				   Logger::Instance()->LOG("Se cargo el valor ancho del  por default", LogTypeWarn);
					ancho = "11";
				}
	   if (!validarNumero(alto))
					{
					   Logger::Instance()->LOG("Se cargo el valor alto del por default", LogTypeWarn);
						alto = "11";
					}
		if (!validarNumero(frames))
			{
				Logger::Instance()->LOG("Se cargo el valor de frames del  por default", LogTypeWarn);
				frames = "30";
				//id = "default";

			}

	   powerup.id = id;
	   powerup.frames = stoi(frames);
	   powerup.cantidad = stoi(cantidad);
	   powerup.ancho = stoi(ancho) ;
	   powerup.alto = stoi(alto);
	   m_listaDePowerups.push_back(powerup);
	}
	return exito;


}

bool ParserStage::extraerJefes(const pugi::xml_document* doc, bool isLoadingDefault)
{
	bool exito = true;
	pugi::xml_node spritesNode = doc->child("bosses");

	for (pugi::xml_node spr = spritesNode.first_child(); spr; spr = spr.next_sibling())
	{
	   Jefe jefe;
	   std::string id = spr.child("id").first_child().value();
	   std::string frames = spr.child("frames").first_child().value();
	   std::string posicion = spr.child("posicion").first_child().value();
	   std::string ancho = spr.child("ancho").first_child().value();
	   std::string alto = spr.child("alto").first_child().value();
	   if (!validarNumero(posicion))
			{
			   Logger::Instance()->LOG("Se cargo el valor posicion del  por default", LogTypeWarn);
				posicion = "2";
			}
	   if (!validarNumero(ancho))
				{
				   Logger::Instance()->LOG("Se cargo el valor ancho del  por default", LogTypeWarn);
					ancho = "11";
				}
	   if (!validarNumero(alto))
					{
					   Logger::Instance()->LOG("Se cargo el valor alto del por default", LogTypeWarn);
						alto = "11";
					}
		if (!validarNumero(frames))
			{
				Logger::Instance()->LOG("Se cargo el valor de frames del  por default", LogTypeWarn);
				frames = "30";
				//id = "default";

			}

	   jefe.id = id;
	   jefe.frames = stoi(frames);
	   jefe.posicion = stoi(posicion);
	   jefe.ancho = stoi(ancho) ;
	   jefe.alto = stoi(alto);
	   m_listaDeJefes.push_back(jefe);
	}
	return exito;




}
bool ParserStage::validarNumero(std::string& intString)
{
	if (intString.empty())
			return false;
		quitarEspacios(intString);
		for (uint i = 0 ; i < intString.length(); i++)
		{
			if (!std::isdigit(intString[i]))
				return false;
		}
	return true;
}


