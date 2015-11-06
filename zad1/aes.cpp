#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "kodowanie.h"
using namespace std;

int main (int argc, char *argv[])
{
	
  /* A 256 bit key */			 
  //unsigned char *key = (unsigned char *)"01234567890123456012345678901234";
//unsigned char *iv = (unsigned char *)"0123456789012345";
  // HASLO DO KLUCZA
  //0123456789012345
 	unsigned char ciphertext[128];
  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];
  int decryptedtext_len, ciphertext_len;
/* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

	string kstore="";
	int kID;
	int key_len; 
	unsigned char * iv;
	iv= new unsigned char [16];
	cout<<"Podaj ścieżkę do Keystore'a\n"; 			cin>>kstore; 		
	cout<<"Podaj ID klucza\n";             			cin>>kID;    		
	cout<<"Podaj hasło do klucza\n"; 
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

      	cin>>(char *)iv; 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	unsigned char * cryptedkey;
	unsigned char * key;
	cryptedkey = new unsigned char [32];
	key = new unsigned char [32];
	ifstream Kfile (kstore.c_str(), ios::in|ios::binary);
	Kfile.seekg( +256*kID, ios_base::beg );
	Kfile.read ((char *)cryptedkey,32);
	cout<<cryptedkey<<"\n";
	key_len = decrypt(cryptedkey, 32, (unsigned char *)"01234567890123456012345678901234", iv,(unsigned char *)key);
	Kfile.close();
	cout<<key<<"\n";
	string nazwapliku="";
	cout<<"Podaj nazwe pliku\n";
	cin>>nazwapliku;


ifstream file (nazwapliku.c_str(), ios::in|ios::binary);
ofstream help ("help", ios::out|ios::binary|ios::trunc);
//Get file length
	unsigned char * memblock = new unsigned char [128];
	do
	{
	
    		//memblock = new unsigned char [128];
    		file.read ((char *)memblock,128);
		//szyfruj jak było e przy wywołaniu
		if(!strcmp(argv[1], "e"))
		{
			//unsigned char * plaintext=memblock;
			ciphertext_len = encrypt (memblock,file.gcount(), key, iv,ciphertext);
			help.write((const char *)ciphertext,ciphertext_len);
		}
		//deszyfruj jak było przy wywołaniu coś innego niż e
		else
		{
			unsigned char * ciphertext=memblock;
 			
			decryptedtext_len = decrypt(ciphertext,file.gcount(), key, iv, decryptedtext);
			decryptedtext[decryptedtext_len] = '\0';
			help.write((const char *)decryptedtext,file.gcount());
		}
	}while(!file.eof());
	
	file.close();
	help.close();
	delete memblock;

	ifstream file2 ("help", ios::in|ios::binary);
	ofstream zapis (nazwapliku.c_str(), ios::out|ios::binary|ios::trunc);
	memblock = new unsigned char [128];
	do{
    	memblock = new unsigned char [128];
    	file2.read ((char *)memblock, 128);
	zapis.write((const char *)memblock,file2.gcount());
	}while(!file2.eof());
	zapis.close();
	file2.close();
	remove("help");
	

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}

