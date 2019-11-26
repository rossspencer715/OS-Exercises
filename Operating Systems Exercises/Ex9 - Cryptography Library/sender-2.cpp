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
#include <string>
#include <iostream>

// converts the public key in an standard c++ string format to RSA format
RSA* convertStringToPubKey(char *key, int length){
	RSA* r = RSA_new();
    BIO* bo = BIO_new(BIO_s_mem());
    BIO_write(bo, key, length);
    PEM_read_bio_RSAPublicKey(bo, &r, 0, 0);
    BIO_free(bo);
    return r;
}

int main()
{
	// 1. Open the named pipe in read only mode
	int fd = open("pipeEx9", O_RDONLY); 	
	// 2. Read the public key of the receiver from the pipe
	char key[2048];
	read(fd, &key, 2048);	
	// 3. Close the pipe
	close(fd);
	// 4. Display the received public key
	std::cout << "Public key of receiver:\n";
	printf("%s\n", key);
	// 5. Input a message from the user using standard input
	std::string msg;
	std::cout << "Enter the message:\n";
	std::getline(std::cin, msg);
	int msgSz = msg.length();
	if(msgSz > 214){
		std::cout << "ERROR: Message is too long to be encrypted.";
		return 1;
	}
	char rawMsg[msgSz + 1];
    	strcpy(rawMsg, msg.c_str());
	rawMsg[msgSz] = '\0';
	//printf("msg is %s\n", rawMsg);
	// 6. Convert the public key of the receiver from string in to RSA strucure format
	RSA* r = convertStringToPubKey(key, strlen(key));
	// 6. Encrypt the message using the RSA public encryption API
	unsigned char encryptedMsg[2048];
	int i = RSA_public_encrypt(strlen(rawMsg), (unsigned char *)rawMsg, (unsigned char*) encryptedMsg, r, RSA_PKCS1_OAEP_PADDING);
	//std::cout << i << " is our fing i\n\n";
	encryptedMsg[2048] = '\0';
	// 7. Display the encrypted message
	printf("Encrypted Message:\n%s\n", encryptedMsg);
	//printf("size is %i\n\n", strlen(encryptedMsg));
	//printf("idk\n");
	// 8. Open the named pipe in write only mode
	fd = open("pipeEx9", O_WRONLY);
	// 9. Write the encrypted message to the pipe
	write(fd, encryptedMsg, 256); //strlen(encryptedMsg) + 1);
	// 10. Close the pipe
	close(fd);
    return 0;
}
