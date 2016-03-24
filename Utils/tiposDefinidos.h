/*
 * tiposDefinidos.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_TIPOSDEFINIDOS_H_
#define UTILS_TIPOSDEFINIDOS_H_

#include <string>

struct Servidor{
	int cantMaximaClientes;
	int puerto;
};

struct Conexion{
	std::string ip;
	int puerto;
};

struct Mensaje{
	std::string id;
	std::string tipo;
	std::string valor;
};



#endif /* UTILS_TIPOSDEFINIDOS_H_ */
