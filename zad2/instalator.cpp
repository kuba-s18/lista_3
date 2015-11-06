#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "kodowanie.hpp"
#include "szyfruj.hpp"
#include <termios.h>
#include <unistd.h>
using namespace std;
int main(void)
{

	fstream plik("config", ios::out );
	string kstore="",haslo="";
	int kID,PIN;
	cout<<"Podaj ścieżkę do Keystore'a\n"; 			
	cin>>kstore;
	plik<<kstore<<"\n";		
	cout<<"Podaj ID klucza\n";             			
	cin>>kID;
	plik<<kID<<"\n";   		
	cout<<"Podaj hasło do klucza\n"; 
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      	cin>>haslo;
	plik<<haslo<<"\n"; 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	cout<<"Podaj PIN\n"; 
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      	cin>>PIN;
	plik<<PIN<<"\n";
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	plik.close();
	szyfr("config");




return 0;
}
