#include<stdio.h> 
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_COUNT 1024

/*types��֧�����͵ļ���*/
typedef enum{ integer } types;
/*��¼������Ϣ�Ľṹ��*/
typedef struct {
	char vname[17];
	char vproc[17];
	bool vkind; //1(0��������1���β�)
	types vtype;
	int vlev; //������� 
	int vadr; //���λ�� 
} varRecord;
/*��¼������Ϣ�Ľṹ��*/
typedef struct{
	char pname[17];
	types ptype;
	int plev;
	int varNum;
	int fadr; //��һ�������ڱ������е�λ�� 
	int ladr; //���һ�������ڱ������е�λ�� 
	int parameter;  //��ǰ����������λ�� 
	bool parameterIsDefined;
} proRecord;

FILE* inFile;//�����ļ����
FILE* outFile;//����ļ����
FILE* errFile;//�����ļ����
FILE* varFile;//�����ļ����
FILE* proFile;//�����ļ����
char input[MAX_COUNT][17]; //�����ļ�ÿһ�еķ��ţ���ʶ���� 
int kind[MAX_COUNT];//ÿһ�е����֣���������� 
int inputCount;//������ŵ�����
int pToken;//ָ��ǰ�������
int pChar;//ָ��ǰ��������еĵ�ǰ�ַ�
int lineNum;//��ǰ�к�
int varCount;
int proCount;
varRecord var[MAX_COUNT];//��ű�������������
proRecord pro[MAX_COUNT];//��Ź�������������
varRecord currentVar;//��ŵ�ǰ��������Ϣ
proRecord currentPro;//��ŵ�ǰ���̵���Ϣ
//��ʼ��,����token�� 
bool init(){
	char inFilename[MAX_COUNT] = "G:\\test.dyd";
	char outFilename[MAX_COUNT] = "G:\\test.dys";
	char errFilename[MAX_COUNT] = "G:\\test.err";
	char varFilename[MAX_COUNT] = "G:\\test.var";
	char proFilename[MAX_COUNT] = "G:\\test.pro";
	if((inFile=fopen(inFilename,"r"))&&(outFile=fopen(outFilename,"w"))&&(errFile=fopen(errFilename,"w"))
	&&(varFile=fopen(varFilename,"w"))&&(proFile=fopen(proFilename,"w"))){
		inputCount=0;
		proCount=0;
		varCount=0;
		pToken=0;
		pChar=0;
		lineNum=1;
		currentPro.plev=0;
		currentPro.parameter=-1;
		currentPro.varNum=0; 
		strcpy(currentPro.pname,"");
		while(!feof(inFile)){
			char stringLine[MAX_COUNT];
			if(fgets(stringLine,MAX_COUNT,inFile)){
				char line[20]="";
				strncpy(line,stringLine,19);          //input�ļ���ÿһ���������� "   begin 1     " ,����Ҫ�����ǰ���������������������������� 
				char *kindString=strrchr(line,' '); //�ո����һ�γ��ֵ�λ��
				kind[inputCount]=atoi(kindString+1); 

				char string[17]="";
				strncpy(string,stringLine,16);
				char *firstString=strrchr(string,' '); //�ո����һ�γ��ֵ�λ��
				strcpy(input[inputCount],firstString+1); 
				inputCount++;
			}
		}
		return true;
	}else{
		fclose(inFile);
		fclose(outFile);
		fclose(errFile);
		fclose(varFile);
		fclose(proFile);
	}
}
void final(){                          //���ṹ����������д�뵽ָ���ļ��� 
	for(int i=0;i<varCount;i++){
		int kind=var[i].vkind?1:0;
		char vtype[10]=" ";
		if(var[i].vtype == integer){
			strcpy(vtype,"integer");
		}else{
			strcpy(vtype," ");
		}
		fprintf(varFile,"%16s %16s %d %s %d %d\n", var[i].vname, var[i].vproc, kind, vtype, var[i].vlev, var[i].vadr);
	}
	for(int i=0;i<proCount;i++){
		char type[10]=" ";
		if(pro[i].ptype == integer){
			strcpy(type,"integer");
		}else{
			strcpy(type," ");
		}
		fprintf(proFile,"%16s %s %d %d %d\n", pro[i].pname, type, pro[i].plev, pro[i].fadr, pro[i].ladr);
	}
	if (fseek(inFile, 0, 0) == 0)
	{
		while (!feof(inFile)){
			fputc(fgetc(inFile), outFile);
		}
	} 
	
	fclose(inFile);
	fclose(outFile);
	fclose(errFile);
	fclose(varFile);
	fclose(proFile);
} 
//1 ��Ƿ�δ����  2 ��Ƿ��ض���  3  ִ����䲻��ʶ��
//4  ȱ�ٷ���     5  ��������δ���� 
void error(int errNum,const char* symbol){
	switch(errNum){
		case 1:
			fprintf(errFile, "***LINE:%d  %s��Ƿ�δ����\n",lineNum,input[pToken]);
			break;
		case 2:
			fprintf(errFile, "***LINE:%d  %s��Ƿ��ض���\n",lineNum,input[pToken]);
			break;
		case 3:
			fprintf(errFile, "***LINE:%d  %sִ����䲻��ʶ��\n",lineNum,input[pToken]);
			break;
		case 4:
			fprintf(errFile, "***LINE:%d  %sȱ�ٷ���%s\n",lineNum,input[pToken],symbol);
			break;
		case 5:
			fprintf(errFile, "***LINE:%d  ��������%sδ����\n",lineNum,input[pToken]);
			break;
		default:
			break; 
	} 
}
bool nextToken(){
	pToken++;
	if(strcmp(input[pToken],"EOF")==0){
		return true;
	}
	while(strcmp(input[pToken],"EOLN")==0){
		pToken++;
		lineNum++;
	}
	return false;
}
bool isVarExist(char *vnam,char *vpro,bool vkind){
	for(int i=0;i<varCount;i++){
		if((strcmp(var[i].vname,vnam)==0)&&(strcmp(var[i].vproc,vpro)==0)&&(vkind==var[i].vkind)){
			return true;
		}
	}
	for(int i=0;i<proCount;i++){
		if(strcmp(pro[i].pname,vnam)==0){
			return true;
		}
	}
	return false;
}
bool isProExist(char *pnam){
	for(int i=0;i<varCount;i++){
		if(strcmp(var[i].vname,pnam)==0){
			return true;
		}
	}
	for(int i=0;i<proCount;i++){
		if(strcmp(pro[i].pname,pnam)==0){
			return true;
		}
	}
	return false;
}
int getNextToken(){         //���ı�pToken��ֵ����ǰ�� 
	int tnext=pToken+1;
	while(strcmp(input[tnext],"EOLN")==0){
		tnext++; 
	}
	return tnext;
}
void A();
void B();
void C();
void C_();
void D();
void E();
void F();
void G();
void J();
void K();
void L();
void M();
void M_();
void N();
void O();
void P();
void Q();
void R();
void R_();
void S();
void S_();
void T();
void U();
void W();
void X();
void Y();
void Z();
int main(){
	if(init()){
	
		A();
		final();
	}
}
//�жϵ�ǰ���ս���Ƿ���ȷ������Ԥ��һλ,
//�����ֱ��ǣ�Ҫ�жϵ��ս������ǰ�ս��������ս���������ս������ȥ�������ĸ������Ƿ��б��� 
void judgeSt(const char *string,const char array[][20],int stnum,bool hasVar){   
	if(strcmp(input[pToken],string)==0){
		nextToken();
	}else{
		error(4,string);
		int flag=0;
		for(int i=0;i<stnum;i++){
			if(strcmp(input[pToken],array[i])==0){
				flag=1;
			} 
		}
		if(hasVar){
			if(kind[pToken]==10){
				flag=1;
			}
		}
		if(flag==0){
			nextToken();
		}
	}
}
void A()
{
	B();
}
void B()
{
	char tmp[1][20]={"integer"};
	judgeSt("begin",tmp,1,false);
	C();
	char tmp1[3][20]={"if","read","write"};
	judgeSt(";",tmp1,3,true);
	M();
	
	if(strcmp(input[pToken],"end")==0){
		nextToken();
	}
	else{
		error(4,"end");
	}
}
void C()
{
	D();
	C_();
}
void C_()
{
	if(strcmp(input[pToken],";")==0&&strcmp(input[getNextToken()],"integer")==0){
		nextToken();
		D();
		C_();
	}
	else{
		if(strcmp(input[pToken],"integer")==0){
			error(4,";");
			D();
			C_();
		}
	}
}
void D()
{
	if(strcmp(input[pToken+1],"function") == 0)
	{
		J();
	}else{
		E();
	}
}
void E()
{
	if (strcmp(input[pToken], "integer") == 0)
	{
		nextToken();	
	}
	else
	{
		error(4, "integer");
		//if (kind[pToken] != 10)
		//{
			nextToken();
		//}
	}
	strcpy(currentVar.vname,input[pToken]);
	strcpy(currentVar.vproc,currentPro.pname);
	if(strcmp(input[pToken], input[currentPro.parameter]) == 0){
		currentPro.parameterIsDefined=true;
		currentVar.vkind=true;
	}else{
		currentVar.vkind=false;	
	}
	currentVar.vadr=varCount;
	currentVar.vlev=currentPro.plev;
	currentVar.vtype=integer;
	if (isVarExist(input[pToken], currentPro.pname, currentVar.vkind))//������ڱ���
	{
		error(2,NULL);
	}
	else 
	{
			//����������Ϣ����
		if(currentPro.varNum==0){
			currentPro.fadr=currentVar.vadr;
		}
		currentPro.ladr=currentVar.vadr;
		currentPro.varNum++;
		//����������һ��������Ϣ
		var[varCount]=currentVar;
		varCount++;
	}
	F();
}
void F()
{
	G();
}
void G()
{
	if(kind[pToken]==10){
		nextToken();
	}
}

void J()
{
	proRecord backup=currentPro;
	char tmp[1][20]={"function"};
	judgeSt("integer",tmp,1,false);

	char tmp1[1][20]={" "};
	judgeSt("function",tmp1,0,true);

	strcpy(currentPro.pname,input[pToken]);
	currentPro.varNum=0;
	currentPro.ptype=integer;
	currentPro.parameterIsDefined=false;
	currentPro.plev++;
	if(isProExist(currentPro.pname)){
		error(2,NULL);     //�����Ѿ����� 
	}
	G();
	char tmp2[1][20]={" "};
	judgeSt("(",tmp2,0,true);

	currentPro.parameter=pToken;
	K();
	char tmp3[1][20]={" "};
	judgeSt(")",tmp3,0,true);

	char tmp4[1][20]={"begin"};
	judgeSt(";",tmp4,1,false);
	L();
	currentPro=backup;
}
void K()
{
	F();
}
void L()
{
	char tmp[1][20]={"integer"};
	judgeSt("begin",tmp,1,false);
	C();
	if(!currentPro.parameterIsDefined){
		error(5,input[currentPro.parameter]);            //����������Ĳ���û���ں������ж��� 
	}
	pro[proCount]=currentPro;
	proCount++;
	char tmp1[3][20]={"if","read","write"};
	judgeSt(";",tmp1,3,true);

	M();
	char tmp2[2][20]={";","end"};
	judgeSt("end",tmp2,2,false);
}
void M()
{
	N();
	M_();
}
void M_()
{
	if(strcmp(input[pToken],";") == 0){
		nextToken();
		N();
		M_(); 
	}else{
		if(strcmp(input[pToken], "end") != 0&&strcmp(input[pToken], "eof") != 0){
			error(4,";");
			N();
			M_(); 
		}
	}
}
void N()
{
	if(strcmp(input[pToken], "read") == 0){
		O();
	}else if(strcmp(input[pToken], "write") == 0){
		P();
	}else if(kind[pToken]==10){
		Q();
	}else if(strcmp(input[pToken], "if") == 0){
		W();
	}else{
		error(3,NULL);
		nextToken();
	}
}
void O()
{
	char tmp[1][20]={"("};
	judgeSt("read",tmp,1,false);
	char tmp1[1][20]={" "};
	judgeSt("(",tmp1,0,true);

	if(!isVarExist(input[pToken],currentPro.pname,false)&&!isVarExist(input[pToken],currentPro.pname,true)){
		error(1,NULL);
	}
	F();
	char tmp2[2][20]={";","end"};
	judgeSt(")",tmp2,2,false);

}
void P()
{
	char tmp[1][20]={"("};
	judgeSt("write",tmp,1,false);
	
	char tmp1[1][20]={" "};
	judgeSt("(",tmp1,0,true);
	if(!isVarExist(input[pToken],currentPro.pname,false)&&!isVarExist(input[pToken],currentPro.pname,true)){
		error(1,NULL);
	}
	F();
	char tmp2[2][20]={";","end"};
	judgeSt(")",tmp2,2,false);
}
void Q()
{
	if(!isVarExist(input[pToken],currentPro.pname,false)&&!isVarExist(input[pToken],currentPro.pname,true)&& !isProExist(input[pToken])){
		error(1,NULL);
	}
	F();
	if(strcmp(input[pToken], ":=") == 0){
		nextToken();	
	}else{
		error(4,":=");
		if(kind[pToken]!=10&&kind[pToken]!=11){
			nextToken();	
		}
	}
	R();
}
void R()
{
	S();
	R_();
}
void R_()
{
	if(strcmp(input[pToken], "-") == 0){
		nextToken();
		S();
		R_();
	}
	else{
		if(kind[pToken]==10||kind[pToken]==11){
			S();
			R_();
		}
	}
}
void S()
{
	T();
	S_();
}
void S_()
{
	if(strcmp(input[pToken], "*") == 0){
		nextToken();
		T();
		S_();
	}
	else{
		if(kind[pToken]==10||kind[pToken]==11){
			T();
			S_();
		}
	}
}
void T()
{
	if(kind[pToken]==11){
		U();
	}
	else if (strcmp(input[pToken+1], "(") == 0)
	{
		Z();
	}
	else
	{
		if(!isVarExist(input[pToken],currentPro.pname,true)&&!isVarExist(input[pToken],currentPro.pname,false)){
			error(1,NULL);
		}
		F();
	}
}
void U()
{
	if (kind[pToken] == 11){
		nextToken();
	}
}

void W()
{
	if(strcmp(input[pToken], "if") == 0){
		nextToken();
	}else{
		error(4,"if");
		if(kind[pToken]!=11&&kind[pToken]!=10){
			nextToken();
		}
	}
	X();
	char tmp[3][20]={"if","read","write"};
	judgeSt("then",tmp,3,true);

	N();
	char tmp1[3][20]={"if","read","write"};
	judgeSt("else",tmp1,3,true);
	N(); 
}
void X()
{
	R();
	Y();
	R();
}
void Y()
{
	if((strcmp(input[pToken], "<") == 0)||(strcmp(input[pToken], "<=") == 0)||(strcmp(input[pToken], ">") == 0)||(strcmp(input[pToken], ">=") == 0)||(strcmp(input[pToken], "=") == 0)||(strcmp(input[pToken], "<>") == 0)){
		nextToken();
	}
    else{
   		error(4,"��ϵ�����");
		if(kind[pToken]!=11&&kind[pToken]!=10){
			nextToken();
		}
	}
}
void Z()
{
	if(!isProExist(input[pToken])){
		error(1,NULL);
	}
	G();
	if(strcmp(input[pToken], "(") == 0){
		nextToken();	
	}else{
		error(4,"(");
		if(kind[pToken]!=10&&kind[pToken]!=11){
			nextToken();	
		}
	}
	R();
	char tmp[4][20]={"-","*",";","end"};
	judgeSt(")",tmp,4,false);
}
