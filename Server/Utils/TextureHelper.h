/*
 * TextureHelper.h
 *
 *  Created on: May 10, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_TEXTUREHELPER_H_
#define UTILS_TEXTUREHELPER_H_

#include <map>
#include <string>

class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();
	int stringToInt(const std::string& stringID);

private:
	std::map<std::string, int> m_textureIDMap;

	void initialize();
};



#endif /* UTILS_TEXTUREHELPER_H_ */
