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
#include "kodowanie.hpp"
#include "deszyfruj.hpp"
using namespace std;


void deszyfr(string nazwapliku, string kstore, string haslo,string kID)
{
 	/* Initialise the library */
 	ERR_load_crypto_strings();
  	OpenSSL_add_all_algorithms();
  	OPENSSL_config(NULL);
	
 	unsigned char ciphertext[128];
  	/* Buffer for the decrypted text */
  	unsigned char decryptedtext[128];
  	int decryptedtext_len, ciphertext_len;
	int kID_int = atoi(kID.c_str());
	int key_len; 
	unsigned char * cryptedkey;
	unsigned char * key;
	cryptedkey = new unsigned char [32];
	key = new unsigned char [32];
	ifstream Kfile (kstore.c_str(), ios::in|ios::binary);
	Kfile.seekg( +256*kID_int, ios_base::beg );
	Kfile.read ((char *)cryptedkey,32);
key_len = decrypt(cryptedkey, strlen ((char *)cryptedkey), (unsigned char *)"01234567890123456012345678901234",(unsigned char *)haslo.c_str(),(unsigned char *)key);
	Kfile.close();



ifstream file (nazwapliku.c_str(), ios::in|ios::binary);
ofstream help ("odszyfrowany.mp3", ios::out|ios::binary);
//Get file length
	unsigned char * memblock;
	do
	{
	
    		memblock = new unsigned char [128];
    		file.read ((char *)memblock,128);
		unsigned char * ciphertext=memblock;
		decryptedtext_len = decrypt(ciphertext,file.gcount(), key,(unsigned char *)haslo.c_str(), decryptedtext);
		//decryptedtext[decryptedtext_len] = '\0';
		help.write((const char *)decryptedtext,file.gcount());
		
	}while(!file.eof());
	
	file.close();
	help.close();
	delete memblock;

/*
	ifstream file2 ("help", ios::in|ios::binary);
	ofstream zapis (nazwapliku.c_str(), ios::out|ios::binary|ios::trunc);
	do{
    	memblock = new unsigned char [128];
    	file2.read ((char *)memblock, 128);
	zapis.write((const char *)memblock,file2.gcount());
	}while(!file2.eof());
	zapis.close();
	file2.close();
	remove("help");
	
*/
  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

}

