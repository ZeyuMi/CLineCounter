#include<stdio.h>

#define BUFSIZE 100

#define STATE0 1
#define STATE1 2
#define STATE2 3
#define STATE3 4
#define STATE4 5
#define STATE5 6
#define STATE6 7
#define STATE7 8
#define STATE8 9

int state = STATE0;

int getch();
void ungetch(int);
void error();
char buf[BUFSIZE];
int bufp = 0;
FILE *in;
int lineNum = 0;
int count(FILE *fp){
	lineNum = 0;
	state = STATE0;
	bufp = 0;
	in = fp;
	while(1){
		int c = getch();
		while(state == STATE0 && (c == ' ' || c == '\t'))
			c = getch();
		if(c == EOF)
			return lineNum;
		switch(state){
			case STATE0:
				if(c == '/'){
					state = STATE1;
				}else if(c == '\n'){
					state = STATE0;
				}else{
					lineNum++;
					state = STATE7;
				}
				break;
			case STATE1:
				if(c == '*'){
					state = STATE2;
				}else if(c == '/'){
					state = STATE5;
				}else{
					state = STATE7;
				}
				break;
			case STATE2:
				if(c == '*'){
					state = STATE3;
				}else{
					state = STATE2;
				}
				break;
			case STATE3:
				if(c == '*'){
					state = STATE3;
				}else if(c == '/'){
					state = STATE4;
				}else{
					state = STATE2;
				}
				break;
			case STATE4:
				state = STATE0;
				ungetch(c);
				break;
			case STATE5:
				if(c == '\n'){
					state = STATE6;
				}else{
					state = STATE5;
				}
				break;
			case STATE6:
				state = STATE0;
				ungetch(c);
				break;
			case STATE7:
				if(c == '/'){
					state = STATE1;
				}else if(c == '\n'){
					state = STATE0;
				}else if(c == '\'' || c == '\"'){
					state = STATE8;	
				}else{
					state = STATE7;
				}
				break;
			case STATE8:
				if(c == '\'' || c =='\"' || c == '>'){
					state = STATE7;
				}else{
					state = STATE8;
				}
				break;
			default:
				error();
		}
	}
}

void error(){
	printf("%d", lineNum);
	printf("ERROR: entering wrong state\n");	
}

int getch(){
	return (bufp > 0) ? buf[bufp--] : getc(in);
}

void ungetch(int c){
	if(bufp >= BUFSIZE-1)
		printf("ERROR: buffer is full\n");
	else
		buf[++bufp] = c;
}
