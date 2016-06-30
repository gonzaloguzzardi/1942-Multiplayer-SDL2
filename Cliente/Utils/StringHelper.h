/*
 * StringHelper.h
 *
 *  Created on: Apr 2, 2016
 *      Author: gonzalo
 */

#ifndef SRC_SERVER_STRINGHELPER_H_
#define SRC_SERVER_STRINGHELPER_H_

#include <string>
#include <cstring>
#include <stdexcept>
#include <algorithm>

class StringHelper
{
public:
	static bool validateInt(std::string& stringToValidate)
	{
		quitarEspacios(stringToValidate);
		for (unsigned int i = 0 ; i < stringToValidate.length(); i++)
		{
			if (!std::isdigit(stringToValidate[i]))
				return false;
		}
		try {
		  stoi(stringToValidate);
		}
		catch(std::invalid_argument& e){
		  return false;
		}
		catch(std::out_of_range& e){
			return false;
		}

		return true;
	}

	static bool validateChar(std::string& stringToValidate)
	{
		if (stringToValidate.length() > 1)
		{
			quitarEspacios(stringToValidate);
			if (stringToValidate.length() > 1)
				return false;
		}
		return true;
	}

	static bool validateDouble(std::string& stringToValidate)
	{
		quitarEspacios(stringToValidate);
		bool pointFound = false;
		for (unsigned int i = 0 ; i < stringToValidate.length(); i++)
		{
			if (stringToValidate[i] == '.')
			{
				if (pointFound)
					return false;
				pointFound = true;
			}
			if (!std::isdigit(stringToValidate[i]) && (stringToValidate[i] != '.'))
				return false;
		}
		try {
		  stod(stringToValidate);
		}
		catch(std::invalid_argument& e){
		  return false;
		}
		catch(std::out_of_range& e){
			return false;
		}

		return true;
	}
private:
	static void quitarEspacios(std::string& texto)
	{
		texto.erase(std::remove_if(texto.begin(),
					texto.end(),
					[](char c) { return (std::isspace(c));})
		, texto.end());
	}
};



#endif /* SRC_SERVER_STRINGHELPER_H_ */
