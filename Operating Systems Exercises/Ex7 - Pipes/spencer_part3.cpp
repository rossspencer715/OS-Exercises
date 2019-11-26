
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

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

int main(int argc, char** argv){
	if(argc < 6){
		std::cout << "ERROR: not enough arguments." << std::endl;
		return 1;
	}
	if(argc > 6){
		std::cout << "ERROR: too many arguments." << std::endl;
		return 2;
	}
	int intParams[5];
	for(int i = 0; i < 5; i++){
		intParams[i] = atoi(argv[i+1]);
	}

	pid_t child1, child2;
	int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
	
	pipe(pipe1);
	pipe(pipe2);
	pipe(pipe3);
	pipe(pipe4);

	//std::cout << " we out here\n\n";
	child1 = fork();
	if(child1 < 0){
		fprintf(stderr, "Fork failed");
		return 1;
	}
	
	if(child1 > 0){
		//in parent
		child2 = fork();
		if(child2 < 0){
			fprintf(stderr, "Fork failed");
			return 1;
		}
		if(child2 > 0){
			//in parent still
            //close read end of pipe
            close(pipe1[0]);
            //start writing to pipe1 for child 1
            write(pipe1[1], intParams, 5*sizeof(int));
            //close write end of pipe
            close(pipe1[1]);
            wait(NULL);

            //get sorted from child1
            close(pipe2[1]);
            int sortedParams[5];
            read(pipe2[0], sortedParams, 5*sizeof(int));
            std::cout << "Sorted parameters are: ";
            for(int i = 0; i < 5; i++){
            	std::cout << sortedParams[i];
            	if(i == 4){
            		std::cout << std::endl;
            	}
            }
            wait(NULL);

            //get median from child 2
            close(pipe4[1]);
            int median;
            read(pipe4[0], &median, sizeof(int));
            std::cout << "Median is: " << median << std::endl;
		}
        else{
        	//in child 2
            wait(NULL);
            close(pipe3[1]);
            close(pipe4[0]);
            int sortedParams[5];
            //read in sorted from child 1
            read(pipe3[0], sortedParams, 5*sizeof(int));
            int median = sortedParams[2];
            //pass median to parent
            write(pipe4[1], &median, sizeof(int));
            close(pipe4[1]);
        }
	}
	else{
		//in child 1
		wait(NULL);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe3[0]);
        int intParams[5];
        //read in params from parent
        read(pipe1[0], intParams, 5*sizeof(int));
        std::sort(intParams, intParams + 5);
        //write back to parent
        write(pipe2[1], intParams, 5*sizeof(int));
        //write to child 2
        write(pipe3[1], intParams, 5*sizeof(int));
        close(pipe2[1]);
        close(pipe3[1]);
	}

}

