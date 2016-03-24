/*
 * Time.h
 *
 *  Created on: Mar 19, 2016
 *      Author: gonzalo
 */

#ifndef TIME_H_
#define TIME_H_

#include <time.h>
#include <string>
#include <sstream>

class Time
{
public:

	int hour, min, sec, day, month, year;

	Time()
	{
		hour = min = sec = day = month = year = 0;
	}

	//si se setear isCurrentTime en true, inicializa las variables de Time con el tiempo actual correspondiente
	Time(bool isCurrentTime)
	{
		hour = min = sec = day = month = year = 0;
		if (isCurrentTime)
		{
		   time_t rawtime;
		   struct tm * timeinfo;

		   time (&rawtime);
		   timeinfo = localtime (&rawtime);
		   asctime(timeinfo);

		   hour = timeinfo->tm_hour;
		   min = timeinfo->tm_min;
		   sec = timeinfo->tm_sec;
		   day = timeinfo->tm_mday;
		   month = timeinfo->tm_mon + 1;
		   year = timeinfo->tm_year + 1900;
		}

	}

	//Devuelve hora:min:seg en string
	static const std::string getTime()
	{
	   time_t rawtime;
	   struct tm * timeinfo;

	   time (&rawtime);
	   timeinfo = localtime (&rawtime);
	   asctime(timeinfo);

	   std::stringstream ss;
	   ss << intToString(timeinfo->tm_hour) << ":" << intToString(timeinfo->tm_min);
	   std::string timeToString =  ss.str();

	   return timeToString;
	}

	//Devuelve dia/mes/año en string
	static const std::string getDate()
	{
	   time_t rawtime;
	   struct tm * timeinfo;
	   time (&rawtime);
	   timeinfo = localtime (&rawtime);

	   std::stringstream ss;
	   ss << intToString(timeinfo->tm_mday) << "-" << intToString(timeinfo->tm_mon + 1) << "-" << (timeinfo->tm_year + 1900);
	   std::string dateToString = ss.str() ;

	   return dateToString;
	}

	//devuelve el tiempo actual como lo hace la funcion asctime: Wed Feb 13 17:17:11 2013
	static const std::string getAscTime()
	{
	   time_t rawtime;
	   struct tm * timeinfo;
	   time (&rawtime);
	   timeinfo = localtime (&rawtime);

	   return std::string(asctime(timeinfo));
	}
private:
	static std::string intToString(int timeInt)
	{
		std::stringstream ss;
		if (timeInt < 10)
			ss << "0";
		ss << timeInt;
		return ss.str();
	}
};


#endif /* TIME_H_ */
