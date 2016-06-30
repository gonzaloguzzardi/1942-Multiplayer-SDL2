/*
 * ListaID.h
 *
 *  Created on: Mar 29, 2016
 *      Author: gonzalo
 */

#ifndef SRC_SERVER_LISTAINTELIGENTE_H_
#define SRC_SERVER_LISTAINTELIGENTE_H_

#include <vector>
#include <stdio.h>

template <typename T>
class ListaInteligente
{
public:
	ListaInteligente():m_maxSize(8){}
	ListaInteligente(int maxSize):m_maxSize(maxSize)
	{
		resize(maxSize);
	}
	~ListaInteligente()
	{
		clear();
	}

	void resize(int maxSize)
	{
		m_listaInterna.resize(maxSize);
		m_bitset.resize(maxSize);
		resetBitset();
	}

	T getElemAt(int indice)
	{
		return m_listaInterna[indice];
	}

	void removeAt(int indice)
	{

		unsetBitset(indice);

	}

	//Devuelve el indice en donde fue cargado el elemento
	//Devuelve -1 si no pudo cargar el elemento;
	int add (T elem)
	{
		int posicionDisponible = -1;
		for (int i = 0; i < m_bitset.size(); i++)
		{
			if (m_bitset[i] == false)
			{
				posicionDisponible = i;
				break;
			}
		}
		if (posicionDisponible == -1)
			return -1;
		//Existe lugar disponible
		m_listaInterna[posicionDisponible] = elem;
		setBitset(posicionDisponible);
		return posicionDisponible;
	}

	bool addAt(int indice, T elem)
	{
		if (indice >= m_listaInterna.size())
			return false;
		m_listaInterna[indice] = elem;
		setBitset(indice);
		return true;
	}

	bool isAvailable(int indice)
	{
		return m_bitset[indice];
	}

	void clear()
	{
		m_listaInterna.clear();
		m_bitset.clear();
	}
	int size()
	{
		return m_listaInterna.size();
	}

private:
	std::vector<bool> m_bitset;
	std::vector<T> m_listaInterna;
	int m_maxSize;

	void setBitset(int index)
	{
		m_bitset[index] = true;
	}

	void unsetBitset(int index)
	{
		m_bitset[index] = false;
	}

	void resetBitset()
	{
		for (int i = 0; i < m_bitset.size(); i++)
		{
			m_bitset[i] = false;

		}
	}
};



#endif /* SRC_SERVER_LISTAINTELIGENTE_H_ */
