/*
 * Menu.h
 *
 *  Created on: Apr 2, 2016
 *      Author: matias
 */

#ifndef MENU_H_
#define MENU_H_
#include "../Utils/Logger.h"
#include "cliente.h"
#include "../Utils/TiposDefinidos.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
class Menu {
public:
	Menu();
	virtual ~Menu();
	std::string menuzazo(bool conectado, std::vector<Mensaje> listaDeMensajes);
	void mostrarMenu(std::vector<Mensaje> listaDeMensajes);
	int  cmpOptionMenu (std::string option, bool conectado, int salida);
	int ciclar(std::vector<Mensaje> listaDeMensajes, cliente* client);

};

#endif /* MENU_H_ */
