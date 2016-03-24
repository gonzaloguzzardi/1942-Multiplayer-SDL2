/*
 * main.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: gonzalo
 */

#include <iostream>
#include <string>
#include "Utils/Time.h"
#include "Utils/Logger.h"
#include "Utils/Parser/ParserCliente.h"
#include "Utils/Parser/ParserServidor.h"
using namespace std;

int main(int argc, char **argv)
{
	ParserCliente* clienteParser = new ParserCliente();
	ParserServidor* servidorParser = new ParserServidor();
	clienteParser->parsearDocumento("cliente.xml");
	servidorParser->parsearDocumento("servidor.xml");

	cout << "cliente ip: " << clienteParser->getConexionInfo().ip << "\n";
	cout << "cliente puerto: " << clienteParser->getConexionInfo().puerto << "\n";
	for (uint i = 0; i < clienteParser->getListaMensajes().size();i++)
	{
		cout << "cliente mensaje id: " << clienteParser->getListaMensajes()[i].id << "\n";
		cout << "cliente tipo de mensaje: " << clienteParser->getListaMensajes()[i].tipo << "\n";
		cout << "cliente valor de mensaje: " << clienteParser->getListaMensajes()[i].valor << "\n";
	}
	cout << "\nserver cantidad maxima de clientes: " << servidorParser->getServidorInfo().cantMaximaClientes << "\n";
	cout << "server puerto: " << servidorParser->getServidorInfo().puerto << "\n";
	Logger::Instance()->Close();

	return 0;
}

