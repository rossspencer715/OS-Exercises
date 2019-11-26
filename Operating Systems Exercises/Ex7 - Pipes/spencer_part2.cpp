#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

/*
Sources:
For parts 1&2, 
https://stackoverflow.com/questions/19524723/pipe-an-input-to-c-cin-from-bash
https://stackoverflow.com/questions/40740914/using-istream-to-read-from-named-pipe

For part 3,
https://www.geeksforgeeks.org/using-fork-produce-1-parent-3-child-processes/
https://stackoverflow.com/questions/6877697/communicating-between-two-child-processes-with-pipes
https://stackoverflow.com/questions/12864265/using-pipe-to-pass-integer-values-between-parent-and-child
https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
*/

int main(int, char** argv){
	std::ifstream file(argv[1]);
	std::vector<std::string> v;	
	std::string s;
	while(std::getline(file,s)){
		v.push_back(s);
	}
	for (int i = 0; i != v.size(); ++i){
		std::cout << v[i] << std::endl;
	}
	std::cout << "Program failed on operation " << v.size() << "\n";
}