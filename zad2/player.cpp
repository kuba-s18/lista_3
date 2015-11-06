#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "deszyfruj.hpp"
#include "kodowanie.hpp"
using namespace std;

#define BITS 8
int stop=0;
void sigint(int a)
{
    printf("\nSTOP\n");
	remove("odszyfrowany.mp3");
	exit(0);
}
int main(int argc, char *argv[])
{
/* Initialise the library */
 	ERR_load_crypto_strings();
  	OpenSSL_add_all_algorithms();
  	OPENSSL_config(NULL);
	 signal(SIGINT, sigint);
// ODSZYFROWANIE CONFIG
	unsigned char decryptedtext[128];
	int decryptedtext_len;
	ifstream file ("config", ios::in|ios::binary);
	ofstream help ("help", ios::out|ios::binary);
	unsigned char * memblock;
	do
	{
	
    		memblock = new unsigned char [128];
    		file.read ((char *)memblock,128);
		unsigned char * ciphertext=memblock;
		decryptedtext_len = decrypt(ciphertext, strlen ((char *)ciphertext),(unsigned char *)"01234567890123456012345678901234", 			(unsigned char *)"0123456789012345", decryptedtext);
		//decryptedtext[decryptedtext_len] = '\0';
		help.write((const char *)decryptedtext,file.gcount());
		
	}while(!file.eof());
	
	string kstore,kID,haslo,PIN;
	file.close();
	help.close();
	fstream plik( "help",ios::in );
	getline( plik, kstore );
	getline( plik, kID );
	getline( plik, haslo );
	getline( plik, PIN );
	plik.close();
	bool pinOK=false;
	string pintest="";
	while(pinOK==false)
	{
	cout<<"Podaj PIN\n";
	cin>>pintest;
	if(PIN==pintest) pinOK=true;
	}



    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;


    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
	string nazwapliku="";
	cout<<"Podaj nazwe pliku który chcesz uruchomić\n";
	cin>>nazwapliku;
	cout<<"Wciśnij crtl+C by zakończyć odtwarzanie\n";
	deszyfr(nazwapliku,kstore,haslo,kID);
    /* open the file and get the decoding format */
    mpg123_open(mh,"odszyfrowany.mp3");
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
       ao_play(dev, (char *)buffer, done);
	
	}
	remove("odszyfrowany.mp3");
    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}
