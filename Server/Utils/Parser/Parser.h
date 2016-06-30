/*
 * Parser.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_PARSER_PARSER_H_
#define UTILS_PARSER_PARSER_H_

#include <string>
#include <cstring>
#include <algorithm>

class Parser
{

public:
    virtual ~Parser(){};
	virtual bool parsearDocumento(const std::string& nombreArchivoXML) = 0;

protected:
	void quitarCaracteresEspeciales(std::string& texto, bool aceptaNumeros)
	{
		if (aceptaNumeros)
		{
			texto.erase(std::remove_if(texto.begin(),
						texto.end(),
						[](char c) { return ((std::isspace(c)) || (!std::isalnum(c)));})
			, texto.end());
		}
		else
		{
			texto.erase(std::remove_if(texto.begin(), texto.end(),[](char c)
			{ return ((std::isspace(c)) || (!std::isalpha(c)));
			} ), texto.end());
		}
	}
	void quitarEspacios(std::string& texto)
	{
		texto.erase(std::remove_if(texto.begin(),
					texto.end(),
					[](char c) { return (std::isspace(c));})
		, texto.end());
	}
	void pasarAMinuscula(std::string& texto)
	{
		std::transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
	}
};

#endif /* UTILS_PARSER_PARSER_H_ */
