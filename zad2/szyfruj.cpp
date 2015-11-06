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
#include "szyfruj.hpp"
using namespace std;


void szyfr(string nazwapliku)
{
	
  /* A 256 bit key */			 
  unsigned char *key = (unsigned char *)"01234567890123456012345678901234";
  unsigned char * iv=(unsigned char *)"0123456789012345";

 	unsigned char ciphertext[128];
  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];
  int decryptedtext_len, ciphertext_len;

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

ifstream file (nazwapliku.c_str(), ios::in|ios::binary);
ofstream help ("help", ios::out|ios::binary);
//Get file length
	unsigned char * memblock;
	do
	{
	
    		memblock = new unsigned char [128];
    		file.read ((char *)memblock,128);
		unsigned char * plaintext=memblock;
		ciphertext_len = encrypt (plaintext, file.gcount(), key, iv,ciphertext);
		help.write((const char *)ciphertext,ciphertext_len);
		
	}while(!file.eof());
	
	file.close();
	help.close();
	delete memblock;


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
	

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

}

