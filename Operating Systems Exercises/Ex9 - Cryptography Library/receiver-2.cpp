#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

// generates a public and private key pair using the RSA generate key API
// returns a pointer to a RSA structre which contains the two keys
RSA* generateKeys(){
	RSA* r = NULL;

	// Initialize a new RSA structure instance
	// Use the RSA generate key API to create and add public and private keys to the structure
	r = RSA_new();
	BIGNUM* N = BN_new();
	// having issue using 3 as a bignum*, now using https://stackoverflow.com/questions/4038938/big-number-modulo-a-small-integer-in-openssl as a guide
	int mod = 3;
	mod = htonl(mod);
	BN_bin2bn((unsigned char*)&mod, sizeof(mod), N);
	//2048 bits, exponent = 3 odd number, callback null
	RSA_generate_key_ex(r, 2048, N, NULL);	
	EVP_PKEY* pubkey = EVP_PKEY_new();
  	EVP_PKEY_set1_RSA(pubkey, r);
	BN_free(N);
	return r;
}

// converts the public key in a RSA struct to a c-string
char* convertPubKeyToString(RSA *r, int* len){
	BIO *bp_public = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(bp_public, r);
	size_t pub_len = BIO_pending(bp_public);
	char* pub_key = (char *)malloc(pub_len + 1);
	BIO_read(bp_public, pub_key, pub_len);
	pub_key[pub_len] = '\0';
	*len = pub_len;
	BIO_free_all(bp_public);
	return pub_key;
}

// converts the private key in a RSA struct to a c-string
char* convertPriKeyToString(RSA *r, int* len){
	BIO *bp_private = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);
	size_t pri_len = BIO_pending(bp_private);
	char* pri_key = (char *)malloc(pri_len + 1);
	BIO_read(bp_private, pri_key, pri_len);
	pri_key[pri_len] = '\0';
	*len = pri_len;
	BIO_free_all(bp_private);
	return pri_key;
}

int main()
{
	// 1. Generate RSA key pair
	RSA* r = generateKeys();
	// 2. Convert the keys to strings and display
	int pubLen, privLen;
	char* pubString = convertPubKeyToString(r, &pubLen);
	char* privString = convertPriKeyToString(r, &privLen);
	printf("Public key of receiver:\n%s\n", pubString);
	printf("Private key of receiver:\n%s\n", privString);
	// 3. Open the named pipe in write only mode
	// credit to https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/ for teaching me how to open in write/read only
	int fd = open("pipeEx9", O_WRONLY); 
	// 4. Write the public key to the pipe
	write(fd, pubString, strlen(pubString) + 1);
	// 5. Close the pipe
	close(fd);
	//printf("test\n");
	// 6. Open the pipe again in read only mode
	fd = open("pipeEx9", O_RDONLY);
	//printf("here\n");
	// 7. Read thie incoming message length and the incoming message
	unsigned char msg[256];
	read(fd, msg, 256);
	//int sz = strlen(msg);
	// 8. Close the pipe
	close(fd);
	// 9. Display the received encrypted message
	printf("Encrypted Message:\n%s\n", msg);
	// 10. Decrypt the message using RSA private decryption API
	char decryptedMsg[512];
	int test = RSA_private_decrypt(256, (unsigned char*)msg, (unsigned char*)decryptedMsg, r, RSA_PKCS1_OAEP_PADDING);
	decryptedMsg[test] = '\0';
	// 11. Display the decrypted message
	printf("Decrypted Message:\n%s\n", decryptedMsg);
    return 0;
}
