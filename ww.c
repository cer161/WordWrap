#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_LENGTH 1000
#define BUF_SIZE 20

static int width;
static int exceed_width;
static int chars;
static char* prev;
static int check = 0;

struct linkedList{
    char* data;
    struct linkedList *next;
};
typedef struct linkedList linkedList;


//Wraps the text from Stdin (Doesnt take into consideration blank lines bc no new lines in stdin?)
int wrapStdin(char* input){
	int chars = 0;
	const char delim[2] = " ";
	char* token;
	token = strtok(input, delim);

	printf("\n");
	while(token != NULL){
		//If the token exceeds the width, print token on an individual line
		if(strlen(token) > width){
			printf("\n%s\n", token);
			chars=0;
			exceed_width = 1;
		}
		else{
			chars+=strlen(token);			
			if(chars > width){
				printf("\n%s ", token);
				chars=strlen(token) + 1;
			}
			//If there is room on the current line, print the toke on the current line
			else{
				printf("%s ", token);
				chars+=1;
			}
		}
		token = strtok(NULL, delim);
	}
	
	return 0;
}

//Method to delete a character from a string
void deleteChar(char *str, char newLine){
	char *src;
	char *dst;
	for(src = dst = str; *src != '\0'; src++){
		*dst = *src;
		if(*dst != newLine){
			dst++;
		}

	}
	*dst = '\0';
}


//Wraps the text from a file and prints it to stdout
//Still need to edit to take into consideration empty lines and words being cut off by the buffer
int wrapFile(char* input, int bytes){
	int split = 0;
	//handle words cut off by the buffer

	//If the first char in input is a space we are good
	if(input[0] == ' '){
		split = 0;
	}
	//else if the previous last char was a space we are good
	
	else{
	//we are not good , the word was cutoff
		//printf("Split here: ");
		split = 1;	
	}
	
	const char delim[2] = " ";
	char* token = strtok(input, delim);
	
	while(token != NULL){
		//if the token contains a new line character, delete the new line character
		if(strchr(token, '\n')){
			deleteChar(token, '\n');
			//printf("\nDeleted a new line character\n");
		}
		//If the token exceeds the width, print token on an individual line
		if(strlen(token) > width){
			printf("\n%s\n", token);
			chars=0;
			exceed_width = 1;
		}
		else{
			chars+=strlen(token);
			//if adding the token to the current line makes the line length exceed width, print token on a new line
			if(chars > width){
				printf("\n%s ", token);
				chars=strlen(token) + 1;
			
			}
			//If there is room on the current line, print the toke on the current line
			else{	
				if(split == 0){
					printf("%s ", token);
				}
				else{
					printf("%s ", token);
				}
				chars+=1;
			}
		}
		prev = token;
		token = strtok(NULL, delim);
	}
	return 0;
}

//Method to check if the char* points to a directory or a file. Return 1 if file is a directory, return 2 if file is a file.
int isDir(char *fileName){
	struct stat data;
	int err = stat(fileName, &data);
	if(err){
		perror(fileName);
		return 0;
	}
	if(S_ISDIR(data.st_mode)){
		//S_ISDIR macro is true if file is a directory
		return 1;//
	}
	if(S_ISREG(data.st_mode)){
		//S_ISREG macro is true if the file is a regular file
		return 2;
	}
	
	return 0;
}

//free the list 
void freeList(linkedList** list){
	linkedList* temp = *list;
	linkedList* next = NULL;
	while(temp != NULL){
		next = temp->next;
		free(temp->data);
		free(temp);
		temp = next;
	}
	*list = NULL;
}


int main (int argc, char** argv) {
  
	if(argc == 1 || argc > 3){
        	return EXIT_FAILURE;
    	}
	
	char* widthString = argv[1];
	width = atoi(widthString);
	exceed_width = 0;
	//If the user enters an invalid input for width, return exit_failure
	if(width <= 0){
        	return EXIT_FAILURE;
    	}

    	//The user did not enter a file name, read from standard input and print to standard output
	if(argc == 2){
        	char input[MAX_LENGTH];
        	printf("Enter your input to be wrapped: ");
        	fgets(input, MAX_LENGTH, stdin); 
        	printf("Wrapped text: ");
        	wrapStdin(input);
    	}

    	//The user entered a file name, read from the file and print to standard output
    	if(argc == 3){
		char* input = (char*) calloc(MAX_LENGTH, sizeof(char*) * BUF_SIZE);
		linkedList* curr = malloc(sizeof(linkedList));
		linkedList* head;
		curr->data = malloc(sizeof(char*) * BUF_SIZE);
		curr->next = NULL;
        	char* fileName = argv[2];
		chars = 0;
		//Check if user inputed a directory or a file 
		int check = isDir(fileName);
		//The user entered a file
		if(check == 2){
			int f;
			f = open(fileName, O_RDONLY);
			if(f < 0){
				perror("There was an error opening the file");
				return EXIT_FAILURE;
			}
			int bytes;
			int buf = BUF_SIZE;
			int loop = 0;
			int i = 0;
			//Read the contents of the file
			while(bytes != 0){
				if(loop != 1){
					bytes = read(f, curr->data, BUF_SIZE);
					curr->data[bytes] = '\0';
					//wrapFile(curr->data, bytes);
					head = curr;
					loop = 1;
				}
				else{
					curr->next = malloc(sizeof(linkedList));
					curr = curr->next;
				 	curr->data = malloc(sizeof(char*) * BUF_SIZE);
					curr->next = NULL;
					bytes = read(f, curr->data, BUF_SIZE);
					curr->data[bytes] = '\0';
					//wrapFile(curr->data, bytes);	
				}
	
			}
			//Need to store this in a char* and input that into wrapFile
			while(head != NULL){
				printf("%s", head->data);
				head = head->next;
			}
			

			//Close the file and return exit failure upon error
			if(close(f) < 0){
				perror("There was an error closing the file");
				return EXIT_FAILURE;
			}
		}
		//The user entered a directory
		if(check == 1){
		
		}

		//There was an error identifying the file
		if(check == 0){
			return EXIT_FAILURE;
		}
		freeList(&head);
		free(input);
    	}


    	if(exceed_width == 1){
		return EXIT_FAILURE;
    	}
   
    	return EXIT_SUCCESS;
}
