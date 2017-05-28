#include<stdio.h> 
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
char token[10];
char character;
int val;
FILE *fp;//输出
FILE *fw,*errfw; //输入 
int lineNum;
int num;
char reserveList[9][10]={"begin","end","integer"
,"if","then","else","function","read","write"};
//char symbolList[20][10];
//int consList[10];
//int symlistSize=0;
//int conslistSize=0;
void returnToFile(const char string[],int type){
/*	char temp[10];
	int i;
	i=0;
	while(*(type+i)!='\0')
	{
		temp[i]=*(type+i);
		i++;
	}
	printf("(%s,%d)\n",temp,location);*/
	fprintf(fw,"%16s %d\n",string,type);
	//printf("%16s %d\n",string,type);
}
void getNextchar(){
	if((character=fgetc(fp))==EOF) {
		returnToFile("EOF",25);
		exit(0);
	}
}
void getnbc(){                                                 //但在一个文件的结尾，不会有非空白,在getNextchar()上做文章 
	while(character==' '||character=='\n'||character==9){
		if(character=='\n'){
			lineNum++;
			returnToFile("EOLN",24);
		} 
		getNextchar();
	}//三种空白字符 空格，换行，tab 
}
void concat(){
	char * tchar=&character;
	strcat(token,tchar);
}
int letter(){
	return isalpha(character);
}
int digit(){
	return isdigit(character);
}
void retract(){
	ungetc(character,fp);
}
int reserve(){
	for(int i=1;i<=9;i++){
		if(strcmp(token,reserveList[i-1])==0){
			return i;
		}
	}
	return 0;
}
/*int symbol(){
	for(int i=0;i<symlistSize;i++){
		if(strcmp(token,symbolList[i-1])==0){
			return i;
		}
	}
	strcpy(symbolList[symlistSize++],token);
	return symlistSize-1;
}*/ 
/*int constant(){
	consList[conslistSize++]=atoi(token);
	return conslistSize-1;
} */

void error(){
	fprintf(errfw,"***LINE:%d  冒号后面不是=\n",lineNum);
//	printf("冒号后面不是=\n");
}
void defaultError(){
	fprintf(errfw,"***LINE:%d  冒号后面不是=\n",lineNum);
//	printf("非法输入符号\n");
}

void LexAnalyze(){
	strcpy(token,"");
	getNextchar();
	getnbc();
	switch(character){
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			while(letter()||digit())
			{
				concat();
				getNextchar();
			}
			retract();
			num=reserve();
			if(num!=0){
/*				char str[15];
				itoa(num,str,10);*/
				returnToFile(token,num);
			} 
			else{
				//val=symbol();
				returnToFile(token,10);
			}
		break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			while(digit()){
				concat();
				getNextchar();
			}
			retract();
//			val=constant();
			returnToFile(token,11);
			break;
		case '=':
			returnToFile("=",12);
			break;
		case '<':
			getNextchar();
			if(character=='=') returnToFile("<=",14);
			else{
				retract();
				returnToFile("<",15);
			}
			break;
		case '>':
			getNextchar();
			if(character=='=') returnToFile(">=",16);
			else{
				retract();
				returnToFile(">",17);
			}
			break;
		case ':':
			getNextchar();
			if(character=='=')
			{
//				retract();
				returnToFile(":=",20);
			}
			else error();
			break;
		case '-':
			returnToFile("-",18);
			break;
		case '*':
			returnToFile("*",19);
			break;
		case '(':
			returnToFile("(",21);
			break;
		case ')':
			returnToFile(")",22);
			break;
		case ';':
			returnToFile(";",23);
			break;
		default:
			defaultError();
	}
}
int main(){
	if((fp=fopen("G:\\test.txt","r"))==NULL){
		printf("cannot open test.txt\n");
		return 1;
	}
	if((fw=fopen("G:\\test.dyd","w+"))==NULL||errfw=fopen("G:\\test.err","w+"))==NULL){
		printf("cannot create test.dyd or test.err\n");
		return 1;
	}
	while(!feof(fp)){
		LexAnalyze();
	}

//	fprintf(fw,"123123\n");
	fclose(fp);
	fclose(fw);
	fclose(errfw);
	return 0;
}
