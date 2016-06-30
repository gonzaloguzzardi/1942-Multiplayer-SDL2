/*
 * Menu.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: matias
 */

#include "Menu.h"

Menu::Menu() {
	// TODO Auto-generated constructor stub

}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

std::string Menu::menuzazo(bool conectado, std::vector<Mensaje> listaDeMensajes)
{
	std::string option = "0";
	/*if(!conectado)
	{
		while(option !="1")
		{
		mostrarMenu(listaDeMensajes);
		cin >> option;
		cout << "\n";
		}
		return option;
	}
	else
	{*/
		mostrarMenu(listaDeMensajes);
		cin >> option;
		cout << "\n";
		return option;

	//}
}


void Menu::mostrarMenu(std::vector<Mensaje> listaDeMensajes)
{
	cout << "Opciones: \n";
	cout << "1 - Conectar \n";
	cout << "2 - Desconectar \n";
	int i = 0;
	for(;i < listaDeMensajes.size();i++)
	{
		cout << 3+i << " - Enviar el Mensaje "
			 << listaDeMensajes[i].id << "\n";
	}

	cout << 3+i << " - Ciclar \n";
	cout << 4+i << " - Salir \n";
	cout << "\n";
}

int  Menu::cmpOptionMenu (std::string option, bool conectado, int salida)
{
	std::string salidaEnS = std::to_string(salida);
	std::string ciclarEnS = std::to_string(salida-1);
	if(conectado)
	{
		if (option == "1")
		{
			cout << "Ya esta conectado en el servidor \n";
			return 1;
		}
		else if (option == "2")
		{
			cout << "El cliente se ha desconectado del servidor. \n";
			return 2;
		}
		else if(option == salidaEnS)
		{
			cout << "Se ha cerrado el cliente.\n";
			return -1;
		}
		else if(stoi(option) >= 3 and stoi(option) < salida-1 )
		{

			return 3;
		}
		else if (option == ciclarEnS)
		{
			return 4;
		}
		else
		{
			cout << "Ingresó un comando inválido \n";
			return 20;
		}

	}
	else
	{
		if(option == "1")
		{
			cout << "Conectando con el servidor...\n";
			return 1;
		}
		else if (option == "2")
		{
			cout << "No esta conectado a ningún servidor. No se ha podido desconectar.\n";
			return 2;
		}
		else if(option == salidaEnS)
		{
			cout << "Se ha cerrado el cliente.\n";
			return -1;
	     }
		else if(stoi(option) >= 3 and stoi(option) < salida )
		{
			cout << "No se puede enviar un mensaje ni ciclar sin estar conectado \n";
			return 3;
		}
		else
		{
			cout << "Ingresó un comando inválido \n";
			return 20;
		}
	}
}

int Menu::ciclar(std::vector<Mensaje> listaDeMensajes, cliente* client)
{
	int timeLeft;
	cout << "Ingrese el tiempo total del ciclado: \n";
	cin >> timeLeft;
	if(timeLeft < 1000)
		timeLeft = 1000;//Seteo un default de un ciclado de por lo menos 1 segundo, no puedo pasarle a ending menos de 0 seg
	time_t start = time(NULL);
	time_t tiempoEnSegundos = timeLeft/1000;
	time_t ending = start + tiempoEnSegundos;
	int indice = 0;

	while(start < ending )
	{
		usleep(1000);
		int indiceModulo = indice % listaDeMensajes.size();
		Mensaje mensajeAEnviar = listaDeMensajes[indiceModulo];

		cout << "Se enviará el mensaje: "
			 << listaDeMensajes[indiceModulo].id << "\n";
		client->escribir(mensajeAEnviar);
		if (!client->leer())
		{
			Logger::Instance()->LOG("Cliente: Se ha perdido la conexión con el servidor.", LogTypeWarn);
	   	   	printf("Se ha perdido la conexión con el servidor.\n");
			return -1;//Devuelve menos 1 si la conexion falla en algun momento
		}
		start = time(NULL);
		indice ++;
	}
	return 0;//Devuelve 0 si no hubo problemas en la conexion
}

