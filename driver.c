#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include"count.h"

int countFile(char *name, int lineNum);
int isSourceFile(char *name);
int main(int argc, char *argv[]){
	int lineNum = 0;
	if(argc == 1){
		fprintf(stderr, "ERROR: not enough parameters\n");
	}else{
		while(--argc > 0){
			lineNum = countFile(*++argv, lineNum);
		}
	}
	printf("%d\n", lineNum);
	return 0;
}

int countFile(char *name, int lineNum){
	struct stat stbuf;
	printf("now %s : %d\n", name, lineNum);
	if(stat(name, &stbuf) == -1){
		fprintf(stderr, "ERROR: file %s does not exists.\n", name);
		return lineNum;
	}
	if((stbuf.st_mode & S_IFMT) == S_IFDIR){
		DIR *dir;
		dir = opendir(name);
		struct dirent * entry;
		while((entry = readdir(dir)) != NULL){
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue;
			char childFileName[1024];
			sprintf(childFileName, "%s/%s", name, entry->d_name);
			printf("before %s : %d\n", childFileName, lineNum);
			lineNum = countFile(childFileName, lineNum);
			printf("after %s : %d\n", childFileName, lineNum);
		}
		closedir(dir);
	}else{
		if(isSourceFile(name) == 1){
			FILE *file = fopen(name, "r");
			int num = count(file);
			printf("now %s : add %d to %d = %d\n", name, num, lineNum, num+lineNum);
			lineNum += num;
			fclose(file);
		}
		return lineNum;	
	}
	return lineNum;
}

int isSourceFile(char *name){
	char *p = name;
	int length = 0;
	while(*p++ != '\0'){
		length++;
	}
	if(*(name + length - 2) == '.' && (*(name + length - 1) == 'c' || *(name + length - 1) == 'h'))
		return 1;
	else 
		return 0;
}
