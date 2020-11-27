/**********************************************************
 * Program: Assignment 2 - Files & Directories
 * Author: Skinneal (Allison Skinner)
 * Date: 10/18/2020
 * Description:
 * (1) Reads directory enteries
 * (2) Finds a file in the current directory based on user
 * specified criteria
 * (3) Reads and processes the data in the chosen file
 * (4) Creates a directory
 * (5) Creates new files in the newly created directory
 * and writes processed data to these files
 * Input: csv file and user input (integers and strings)
 * Output: files, directories, and written data
 *********************************************************/

#include <time.h> //random
#include <stdio.h> //sprintf
#include <stdlib.h> //random & srandom
#include <string.h> //strings
#include <sys/types.h> //open & create file & size
#include <sys/stat.h> //open & create file & size
#include <fcntl.h> //open & create file
#include <dirent.h> //directory
#include <unistd.h> //read & close

/***********************************************************
 * Function: read_directory
 * Description: read in files from current directory and
 * figure out which file is the smallest and largest
 * Parameters: char and int and int and int
 * Pre-Conditions: .csv files are in current directory
 * Post-Conditions: reads in files and determines which
 * file is the largest and what file is the smallest
 **********************************************************/
void read_directory (char **files, int *size, int *biggest_file_index, int *smallest_file_index) {
	struct dirent *de; //pointer for file directory
	DIR *fd = opendir("."); //open directory
	
	int biggest = 0;
	int smallest = 10000000;
	
	int i = 0; 
	while ((de = readdir(fd)) != NULL) {
		if(strstr(de->d_name, ".csv") && strstr(de->d_name, "movies_")) { //file has proper ending and beginning
			files[i]=de->d_name;
			FILE *fp = fopen(files[i], "r");
			fseek(fp, 0L, SEEK_END);
			size[i] = ftell(fp); //get size of file
			fclose(fp);
			//printf("files[%d]: %s\n", i, files[i]);	
			//printf("size[%d]: %d\n", i, size[i]);

			//largest file
			if (size[i] > biggest) {
				biggest = size[i];
				*biggest_file_index = i;
			}
			//smallest file
			if (size[i] < smallest) {
				smallest = size[i];
				*smallest_file_index = i;
			}

			i++;
		}
	}
}

/***********************************************************
 * Function: main
 * Description: gets user choice and decides what to do
 * Parameters: none
 * Pre-Conditions: user inputs 
 * Post-Conditions: directory and files are created with
 * proper permissions and correct text
 **********************************************************/
int main(){
		srand(time(0));
		char *files[100];
		for (int j=0; j<100; j++){
			files[j] = (int*)malloc(100 * sizeof(int));
		}
		int size[100];
		int biggest_file_index;
		int smallest_file_index;
		read_directory(files,size,&biggest_file_index, &smallest_file_index);	
		//printf("biggest file index: %d\n", biggest_file_index);	
		//printf("smallest file index: %d\n", smallest_file_index);
		//printf("biggest file: %s\n", files[biggest_file_index]);

		int current_file_index;
		char current_file [100];

	int op1; //user's choice to first statement
	int op2; //user's choice to second statement

	//loop to take user input continuously
	while (1) {
		printf("1. Select file to process\n");
		printf("2. Exit the program\n");
		printf("Enter a choice 1 or 2:\n");

		//read user's input
		scanf("%d", &op1);

		//decide what to do based on user's choice
		if (op1 == 1) {	
			printf("Which file do you want to process?\n");
			printf("Enter 1 to pick the largest file.\n");
			printf("Enter 2 to pick the smallest file.\n");
			printf("Enter 3 to specify the name of a file.\n");
			printf("Enter a choice from 1 to 3: \n");
			scanf("%d", &op2);
			
			switch (op2) {
				case 1: //largest file
					printf("Now processing the chosen file named %s\n", files[biggest_file_index]);
					strcpy(current_file, files[biggest_file_index]);
					break;
				case 2: //smallest file
					printf("Now processing the chosen file named %s\n", files[smallest_file_index]);
					strcpy(current_file, files[smallest_file_index]);
					break;	
				case 3: //specify name of file	
					printf("Enter the complete file name:\n");
					scanf("%s", current_file);
					while (!fopen(current_file, "r")) {
						//file not found
						printf("The file %s was not found. Try again.\n", current_file);
						scanf("%s", current_file);
					}
					break;
			}
		
			//create new directory
			char name[50];
			strcpy(name, "skinneal.movies."); //onid.movies.random#
			for (int i=16; i<21; i++) {
				name[i] = rand() % 10 + 48;
			}
			name[21] = 0;
			mkdir(name, 0750); //Owner can read, write, execute and group can read, execute
			printf("Created directory with name %s\n", name);
			
			FILE *in;
			in  = fopen(current_file, "r");
			//printf("in: %d\n", in);
			FILE *out;
			char movie_name[100];
			char movie_date [100];
			char language[100];
			float rating;
			char junk[1000];
			fgets(junk, 500, in);
			while (1){
				if (fscanf(in,"%90[^,],", movie_name) == EOF){break;}
				fscanf(in, "%90[^,],", movie_date);
				fscanf(in,"%s,", language);
				fscanf (in, "%f,", &rating);
				char filename[100];
				strcpy(filename, name);
				strcat(filename, "/");
				strcat(filename, movie_date);
				strcat(filename, ".txt");
				out = fopen(filename, "a");
				fprintf(out, "%s\n", movie_name);
				fclose(out);
				chmod(filename, 0640); //Owner can read, write and group can read
				//printf("movie name: %s\n", movie_name);
				//printf("movie date: %s\n", movie_date);
			}
		}

		//user wants to stop the program
		else if (op1 == 2) { 
			return 0;
		}
	}
}












