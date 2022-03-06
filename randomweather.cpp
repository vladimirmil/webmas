// Ubuntu	gcc main.cpp -lstdc++ -lm

//#include "pch.h"																	// visual studio trazi ovo
#include <iostream>																	// i/o, std::fixed
#include <thread>																	// std::this_thread
#include <chrono> 																	// vreme
#include <fstream>																	// std::ofstream
#include <ctime> 																	// time_t
#include <string>																	// std::string
#include <random>																	// std::random engine, unif...
#include <iomanip>																	// std::setprecision

int main()
{
	std::ofstream f1;																// moguæe samo pisanje u fajl

	char ch[30];																	// koristi se kod formatiranja datuma i vremena

	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();	// da bude nasumièno svaki put
	std::default_random_engine e(seed);												// da bude nasumièno svaki put
	std::uniform_real_distribution<double> temperature(20, 35);						// distribucija
	std::uniform_int_distribution<int> humidity(40, 100);							// distribucija
	std::uniform_int_distribution<int> wind_speed(0, 15);							// distribucija

	std::string date_and_time;														// deklarisanje stringa za datum i vreme
	time_t rawtime;																	// deklarisanje argumenta za time() funkciju
	struct tm* timeinfo;															// deklarisanje promenjive za smeštanje povratne vrednosti localtime() funkcije

	while (1)																		// beskonaèna petlja
	{
		time(&rawtime); 															// pozivanje time() funkcije 
		timeinfo = localtime(&rawtime); 											// korišæenje localtime() funkcije 
		strftime(ch, 30, "%Y-%m-%d %H:%M:%S", timeinfo);							// formatiranje ispisa u dd/mm/yyy hh:mm:ss, %a na kraj za dan(subota...)
		std::string date_and_time(ch);												// char u string, lakše ispisati string

		f1.open("weatherdata.txt");													// Otvori fajl

		f1 << "{\n";																// Upis u fajl
		f1 << "   \"_id\":\"" << date_and_time << "\",\n";											
		f1 << "   \"temperatura\":" << std::fixed << std::setprecision(2) << temperature(e) << ",\n";		
		f1 << "   \"vlaznost\":" << humidity(e) << ",\n";													
		f1 << "   \"brzinavetra\":" << wind_speed(e) << ",\n";												
		//f1 << "}";
		f1.close();																	// zatvori fajl


		std::this_thread::sleep_for(std::chrono::seconds(1));						// delay od 1s
	}


	return 0;																		// sve ok
}















