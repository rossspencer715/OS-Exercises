#include <thread>
#include <iostream>
#include <cstdlib>
#include <vector>

// Used https://www.programiz.com/cpp-programming/library-function/cstdio/sprintf 
// to make sure the flag for an int in sprintf is the same as printf

void foo(int id, int argu){
	char str[50];
	int num = -1;
	while(num % 9999 != argu)
		num = rand();
	sprintf(str, "Thread %i completed.\n", id);
	std::cout << str;
}

int main(int argc, char *argv[]){
	if(argc < 1){
		return 1;
	}
	int arg = atoi(argv[1]);
	std::vector<std::thread> threads;
	for(int i = 0; i < 10; ++i){
		threads.emplace_back(foo, i, arg);
	}
	for(std::thread& th : threads){
		th.join();
	}

	std::cout << "All threads have finished finding numbers.\n";

	return 0;	
	
}

