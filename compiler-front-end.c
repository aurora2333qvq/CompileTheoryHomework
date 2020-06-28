#pragma warning(disable:4996)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BEGIN 1
#define END 2
#define VAR 3
#define INT 4
#define WHILE 5
#define IF 6
#define THEN 7
#define ELSE 8
#define DO 9
#define EQUAL 10//��ֵ
#define PLUS 11
#define STAR 12
#define LP 13
#define RP 14//������
#define LT 15
#define LE 16
#define GT 17
#define GE 18
#define NE 19
#define EQ 20//�Ⱥ�
#define POINT 21//�����
#define COLON 22//ð��
#define SEMICOLON 23//�ֺ�
#define COMMA 24//����
#define ID 25//��ʶ��
#define CONSTANT 26//����
#define PROGRAM 27
#define MINUS 28//����
#define DIV 29//����
#define N 30//��Ԫʽ�����С


char prog[800], token[80];
char data[800][80];
char data1[800][80];
int number[800];//�洢syn��
char number1[800][80];
char var[100][20];
char ch;
int syn, p, err;
int m, sum;
int i = 0;
int j = 0;
int k = 1, kk = 0;
int count = 0;//��¼����
int flag = 0; //�ж��Ƿ��������������� flag=1;
int  lable = 0;//���ע��
int V = -1;//�����������  V=1;
int l;//�洢��������
int NXQ = 0;


//�ؼ��ִ洢
char *key[18] = {
	"BEGIN","END","VAR","INTEGER","WHILE","IF", "THEN","ELSE","DO",
	"begin","end","var","integer","while","if", "then","else","do"
};


 struct {
	char result[12];
	char ag1[12];
	char op[12];
	char ag2[12];
} four[N];

void read();
void scanner();
void program();
void procedure();
void variable();
void table();
void lrparser();
void condition();
void whiled();
void compound();
int  guanxi();
char* factor();
char* expression();
char* term();
void statement();
void yuju();
char* newtemp();
void  emit(char* op, char* ag1, char* ag2, char* result);



int main() {
	read();
	if (flag == -1) {
		printf("\nδ�����ʶ����%s��\n\n", token);
	}
	if (flag == 0) {
		p = 0;
		j = 0;
		scanner();
		program();

		for (i = 0; i < NXQ; i++) {
			printf("\n%d :(%s ,%s ,%s ,%s )\n", i, four[i].op, four[i].ag1, four[i].ag2, four[i].result);
		}

	}
	return 0;
}

void read() {
	int i;
	char* str = " ";
	p = 0;
	FILE *fp = fopen("data.txt", "r");
	if (fp == NULL)
	{
		printf("null file\n");
	}
	while (!feof(fp))
	{
		fscanf(fp, "%s", &data[p]);
		strcat(data[p], "  ");
		p++;
	}
	fclose(fp);

	for (j = 0; j < p; j++)
		strcat(prog, data[j]);
	FILE *fpWrite = fopen("table.txt", "w");
	FILE *Write = fopen("program.txt", "w");
	FILE *Var = fopen("variable.txt", "w");
	j = 0;
	p = 0;
	//��Դ��ȥ��ע�ʹ洢���µ��ļ���
	do {
		scanner();
		switch (syn) {
		case 0:
			break;

		case -1:
			flag = 1;
			printf("(��ֵԽ��)");
			break;

		case -2:
			flag = 1;
			fprintf(Write, "%-5s", token);
			printf("%-8s  --> (%-5d��ʶ������)\n", token, syn);
			break;

		case -3://ע��
			break;

		case -4:
			printf("%-8s  --> (%-5d�Ƿ��ַ�)\n", token, syn);
			break;

		case -5:
			printf("�������š�{���ַ���ƥ��\n");
			break;

		case -6:
			printf("�Ҳ�����š�}���ַ���ƥ��\n");
			break;

		case 25:
			if (V == 0) {
				fprintf(Var, "%-5s ", token);
				strcpy(var[l++], token);
			}
			if (V == 1) {
				flag = -1;
				for (i = 0; i < l; i++)
					if (strcmp(token, var[i]) == 0)
						flag = 0;
				if (flag == -1)
					return;
			}
			fprintf(Write, "%-5s ", token);
			number[j++] = syn;
			fprintf(fpWrite, "%-8s  --> (%-5d%-5d)\n", token, syn, k);
			printf("%-8s  --> (%-5d%-5d)\n", token, syn, k++);
			break;

		case 26:
			fprintf(Write, "%-5d ", sum);
			number[j++] = syn;
			printf("%-8d  --> (%-5d%-5d)\n", sum, syn, sum);
			fprintf(fpWrite, "%-8d  --> (%-5d%-5d)\n", sum, syn, sum);
			break;
		default:
			if (syn == 1)
				V = 1;
			if (syn == 3)
				V = 0;
			fprintf(Write, "%-5s ", token);
			number[j++] = syn;
			printf("%-8s  --> (%-5d%-5d)\n", token, syn, 0);
			fprintf(fpWrite, "%-8s  --> (%-5d%-5d)\n", token, syn, 0);
			break;
		}
	} while (syn != 0);
	fclose(fpWrite);
	fclose(Write);
	fclose(Var);

	p = 0;
	FILE *fp1 = fopen("program.txt", "r");
	if (fp1 == NULL)
	{
		printf("null file\n");
	}
	while (!feof(fp1))
	{
		fscanf(fp1, "%s ", &data1[p]);
		strcat(data1[p], " ");
		p++;
	}
	fclose(fp1);
	*prog = 0;
	for (j = 0; j < p; j++)
		strcat(prog, data1[j]);
}
/*����ı�����ȡ����*/
void scanner() {
	int n;
	int time = 0;
	syn = 0;
	for (n = 0; n < 8; n++)
		token[n] = '\0';
	//���˿ո�
	ch = prog[p++];
	while (ch == ' ' || ch == '\n') {
		ch = prog[p];
		p++;
	}

	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
	{ //�����Ǳ�ʾ�����߱�����
		m = 0;             //���α���int
		while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {
			token[m++] = ch;
			ch = prog[p++];
		}
		//ʶ���һ����ʾ��
		token[m++] = '\0';
		//����һλ
		p--;
		syn = 25;//�����ı�ʾ��
		for (n = 0; n < 18; n++) //��ʶ��������ַ����Ѷ���ı�ʾ�����Ƚϣ�
			if (strcmp(token, key[n]) == 0) {
				//�ַ������
				syn = n % 9 + 1;
				break;
			}
		if ((strcmp(token, "PROGRAM") == 0) || (strcmp(token, "program") == 0)) {
			//�ַ������
			syn = 27;
		}
	}
	else if ((ch >= '0'&&ch <= '9')) { //����
		{
			char a = ch;
			sum = 0;
			while ((ch >= '0'&&ch <= '9')) {
				sum = sum * 10 + ch - '0';
				ch = prog[p++];	//��ȡ��һ���ַ�
			}
			m = 0;
			//			printf("%c",ch);
			token[m++] = a;
			if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
				token[m++] = ch;
				ch = prog[p++];
				while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {
					token[m++] = ch;
					ch = prog[p++];
				}
				syn = -2;

			}
		}
		//����һ��
		p--;
		if (syn != -2)
			syn = 26;//26��ʾ��ֵ����
					 //��������Ĵ�С��Χ
		if (sum > 999999999)
			syn = -1;
	}
	else switch (ch) { //�����ַ�
	case'<':
		m = 0;
		token[m++] = ch;
		//��ȡ��һ���ַ�
		ch = prog[p++];
		if (ch == '>') {
			syn = 19;
			token[m++] = ch;
		}
		else if (ch == '=') {
			syn = 16;
			token[m++] = ch;
		}
		else {
			syn = 15;
			p--;
		}
		break;
	case'>':
		m = 0;
		token[m++] = ch;
		//��ȡ��һ���ַ�
		ch = prog[p++];
		if (ch == '=') {
			syn = 18;
			token[m++] = ch;
		}
		else {
			syn = 17;
			p--;
		}
		break;
	case':':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '=') {
			syn = 10;
			token[m++] = ch;
		}
		else {
			syn = 22;
			p--;
		}
		break;
	case'+':syn = 11; token[0] = ch; break;
	case'-':syn = 28; token[0] = ch; break;
	case'*':syn = 12; token[0] = ch; break;
	case'/':syn = 29; token[0] = ch; break;
	case'(':syn = 13; token[0] = ch; break;
	case')':syn = 14; token[0] = ch; break;
	case'=':syn = 20; token[0] = ch; break;
	case'.':syn = 21; token[0] = ch; break;
	case';':syn = 23; token[0] = ch; break;
	case',':syn = 24; token[0] = ch; break;
	case '\0':syn = 0; break;
	case '{':
		lable = 1;
		do {
			ch = prog[p++];
			time++;
			if (ch == '\0')
			{
				syn = -5;
				break;
			}
			else
				syn = -3;
		} while (ch != '}'&&ch != '\0');
		if (ch == '\0')
			p = p - time;
		break;

	case '}':lable = 0;
		syn = -6; break;
	default:syn = -4; token[0] = ch; break;
	}
}

/*�������*/
void program() {
	if (syn == PROGRAM)//program��ͷ
	{
		//ɨ����һ������
		scanner();
		if (syn == ID)//�ڶ��ַ���Ϊ��ʶ��
		{
			scanner();

			if (syn == SEMICOLON)//�ֺ�
			{
				scanner();
				procedure();
			}
			else {//�������ַ����ǣ�
				printf("error:ȱ�ٷֺţ�\nΥ������ʽ��<����>->program<��ʶ��>;<�ֳ���>\n\n");
				flag = 1;
			}
		}
		else {//�ڶ������ʲ��Ǳ�ʶ��
			printf("error:�ڶ������ʲ��Ǳ�ʶ��\n\nΥ������ʽ��<����>->program<��ʶ��>;<�ֳ���>\n\n");
			flag = 1;
		}
	}
	else {//��һ�����ʲ���program
		printf("error:lack program\n\nΥ������ʽ��<����>->program<��ʶ��>;<�ֳ���>\n\n");
		flag = 1;
	}
}

/*�ֳ������*/
void procedure() {

	if (flag == 0) {
		if (syn != BEGIN && syn != VAR)
		{//����begin �Ҳ���VAR
			printf("error:Υ������ʽ��<�ֳ���>-><����˵��>BEGIN<����>END\n\n");
			flag = 1;
		}
		else if (syn == VAR) { //VAR����
			scanner();
			variable();
		}
		else //syn=1��ʾ�ĵ��ʷ�����begin
			lrparser();//������
	}
}

/*����˵����*/
void variable() {
	table();
	if (flag == 0) {
		if (syn == COLON) {//ð��
			scanner();
			if (syn == INT) {//����
				scanner();
				if (syn == SEMICOLON) {
					scanner();
					if (syn == VAR) { //VAR����
						scanner();
						variable();
					}
					else if (syn == BEGIN) {
						lrparser();
					}
					else {
						printf("1error:Υ������ʽ��<����˵����>-><������>��<����>|<������>��<����>;<����˵����>\n\n");
						flag = 1;
					}
				}
				else {
					printf("2error:Υ������ʽ��<����˵����>-><������>��<����>|<������>��<����>;<����˵����>\n\n");
					printf("2error:Υ������ʽ��<����˵��>->VAR<����˵����>;\n");
					flag = 1;
				}
			}
			else {
				printf("3error:Υ������ʽ��<����˵����>-><������>��<����>|<������>��<����>;<����˵����>\n\n");
				flag = 1;
			}
		}
		else {
			printf("4error:Υ������ʽ��<����˵����>-><������>��<����>|<������>��<����>;<����˵����>\n\n");
			flag = 1;
		}
	}
}

/*���������*/
void table() {
	if (flag == 0) {
		if (syn != COLON && syn != ID) //var���ǽӷֺţ�Ҳ���Ǳ�ʶ��
		{
			printf("error1:Υ������ʽ��<������>-><����>|<����>,<������>\n\n");
			flag = 1;
		}
		if (syn == ID) {//��ʶ��
			scanner();
			while (syn == COMMA) {//��ȡΪ����
				scanner();
				if (syn == ID)//��ʶ��
					scanner();
				else {//���ź��Ǳ�ʶ��
					printf("error2:���ź��Ǳ�ʶ�� \n Υ������ʽ��<������>-><����>|<����>,<������>\n\n");
					flag = 1;
				}
			}
		}
	}
}

/*�������*/
void lrparser()    //<����>����=begin<��䴮>end
{
	if (flag == 0) {
		if (syn == BEGIN) //syn=1��ʾ�ĵ��ʷ�����begin
		{
			printf("\nBEGIN\n");           //�����begin
			scanner();
			yuju();    //������䴮����
			if (flag == 0) {
				if (syn == END)//�����end
				{
					printf("END\n");
					scanner();
					if (syn == POINT) {
						//scanner();
						scanner();
						if (syn == 0 && flag == 0)//�����'\0',��ɹ�
								printf("\nThis is a right program!\n\n");
						else {
							printf("error3: -->Υ������ʽ��<�ֳ���>-><����˵��>BEGIN<����>END.\n\n");
							flag = 1;
						}

					}
					else {
						printf("error2:ȱ�ٷָ���.-->Υ������ʽ��<�ֳ���>-><����˵��>BEGIN<����>END.\n\n");
						flag = 1;
					}
				}
				else {
					printf("error3:lack END -->Υ������ʽ��<�ֳ���>-><����˵��>BEGIN<����>END.\n\n");
					flag = 1;
				}
			}
		}
		else {//���ʲ���begin
			printf("error4:lack BEGIN--> Υ������ʽ��<�ֳ���>-><����˵��>BEGIN<����>END.\n\n");
			flag = 1;
		}
	}
}

/*�������*/
void yuju()   //<��䴮>����=<���>{��<���>}
{
	if (flag == 0) {
		statement();     //�������
		while (syn == SEMICOLON)  //���������';',��ɨ����һ������
		{
			scanner();
			statement();
		}
		if (syn != END && flag == 0) {
			printf("error1:ȱ�ٷֺţ�-->Υ������ʽ��<����>-><���>|<���>;<����>\n\n");
			flag = 1;
		}
	}
}

/*������*/
void statement()
{
	char *t, *place;//t���渳ֵ�������������place�����Ҳ�����

	t = (char*)malloc(12);
	place = (char*)malloc(12);

	if (flag == 0) {
		if (syn == ID)  //��ֵ���
		{
			strcpy(t, token);
			scanner();
			if (syn == EQUAL)  //�����ʶ�������Ǹ�ֵ�ţ���ɨ����һ������
			{
				scanner();
				strcpy(place, expression());//�������ʽ����
				emit(":=", place, "_", t);
			}
			else    //�����ʶ�����治�Ǹ�ֵ��
			{
				printf("��ֵ������-->Υ������ʽ��<��ֵ���>-><����>:=<�������ʽ>\n\n");
				flag = 1;
			}
		}
		else if (syn == IF) {//�������
			printf("IF\n");
			condition();
		}
		else if (syn == WHILE) {//while���
			printf("WHILE\n");
			whiled();
		}
		else if (syn == BEGIN) {//�������
			printf("into compound\n");
			compound();
			scanner();
		}
		else if (syn == END)
		{
			printf("error1:endǰ�����зֺţ�-->Υ������ʽ��<���>-><���>|<���>;<����>\n\n");
			flag = 1;
		}
		//else if (syn == SEMICOLON);//�����
		else {   //������Ǳ�ʶ��
			printf("������-->Υ������ʽ��<���>-><��ֵ���>|<�������>|<WHILE���>|<�������>\n\n");
			flag = 1;
		}
	}
}

/*����������*/
void compound() {
	if (flag == 0) {
		scanner();
		yuju();
		if (syn != END && flag == 0) {
			printf("����������ȱ��END-->Υ������ʽ��<�������>->BEGIN<����>END\n\n");
			flag = 1;
		}
	}
}

/*�������*/
void condition() {
	char *t;//t��������op1������1��op2������2��tp��ʱ����
	int FC, temp, temp1;
	t = (char*)malloc(12);
	if (flag == 0) {

		FC = guanxi();//�ٳ������

		if (syn == THEN && flag == 0) {
			printf("THEN\n");
			scanner();
			statement();//����������

			emit("goto", "_", "_", "");	//����if��䣬����else
			temp1 = NXQ - 1;

			if (flag == 0) {
				if (syn == ELSE) {
					printf("ELSE\n");
					scanner();
					temp = NXQ + 1;
					itoa(temp, t, 10);//
					strcpy(four[FC].result, t);

					statement();//����������

					temp = NXQ;
					itoa(temp, t, 10);
					strcpy(four[temp1].result, t);
				}
				else {
					printf("error:ȱ�ٹؼ���else --> Υ������ʽ��<�������>->IF<��ϵ���ʽ>THEN<���>ELSE<���>\n\n");
					flag = 1;
				}
			}
		}
		else {
			printf("error:ȱ�ٹؼ���then --> Υ������ʽ��<�������>->IF<��ϵ���ʽ>THEN<���>ELSE<���>\n\n");
			flag = 1;
		}
	}
}

/*while���*/
void whiled() {
	char  *t;
	int FC, temp1, temp2;
	t = (char*)malloc(12);

	if (flag == 0) {

		temp2 = NXQ;//while��ʼλ��

		FC = guanxi();//���ռٳ���

		if (syn == DO) {
			printf("DO\n");
			scanner();
			statement();//����������


			itoa(temp2, t, 10);//��¼��ת��ַ
			emit("goto", "_", "_", t);	//�ٳ���

			temp1 = NXQ;
			itoa(temp1, t, 10);//��¼��תλ��
			strcpy(four[FC].result, t);

		}
		else {
			printf("ȱ�ٹؼ���DO --> Υ������ʽ��<WHILE���>->WHILE<��ϵ���ʽ>DO<���>\n\n");
			flag = 1;
		}
	}
}

/*��ϵ���ʽ*/
int guanxi() {
	char *t, *op1, *op2, *TC;//op1�����ϵ�������������op2�����Ҳ�����
	int temp, FC;
	op1 = (char*)malloc(12);
	op2 = (char*)malloc(12);
	t = (char*)malloc(12);
	TC = (char*)malloc(12);
	strcpy(t, "");
	scanner();
	strcpy(op1, expression());//�������ʽ
	if (flag == 0) {
		if (syn >= LT && syn <= EQ) {
			if (syn == LT)
				strcpy(t, "<");
			else if (syn == LE) {
				strcpy(t, "<");
				strcat(t, "=");
			}
			else if (syn == GT)
				strcpy(t, ">");
			else if (syn == GE)
			{
				strcpy(t, ">");
				strcat(t, "=");
			}
			else if (syn == NE)
			{
				strcpy(t, "<");
				strcat(t, ">");
			}
			else
				strcpy(t, "=");
			scanner();
			strcpy(op2, expression());//�������ʽ

			temp = NXQ + 2;
			itoa(temp, TC, 10);//��¼��ת��ַ
			emit(t, op1, op2, TC);//�����

			FC = NXQ;
			emit("goto", "_", "_", "");	//�ٳ���
		}
		else {
			printf("Υ������ʽ��<��ϵ���ʽ>-><�������ʽ><��ϵ��><�������ʽ>\n\n");
			flag = 1;
		}
	}
	return FC;
}

/*�������ʽ����*/
char* expression()//<���ʽ>����=<��>{+<��> | -<��>}
{
	char *tp, *op1, *op2, *t;//t��������op1������1��op2������2��tp��ʱ����
	t = (char*)malloc(12);
	tp = (char*)malloc(12);
	op1 = (char*)malloc(12);
	op2 = (char*)malloc(12);

	if (flag == 0) {
		//term();//�����
		strcpy(op1, term());
		while (syn == PLUS || syn == MINUS)//�����'+'��'-'����ɨ����һ������
		{
			//scanner();
			if (syn == PLUS)
				*t = '+';
			else
				*t = '-';
			*(t + 1) = '\0';
			scanner();
			strcpy(op2, term());
			//term();//�����
			strcpy(tp, newtemp());//����newtemp()������ʱ����tp;
			emit(t, op1, op2, tp);
			strcpy(op1, tp);//������������Ϊ��һ�α��ʽ������ĵ�һ��

		}
	}
	return op1;
}

/*�����*/
char* term()//<��>����=<����>{*<����> | /<����>
{
	char *tp, *op1, *op2, *t;//t��������op1������1��op2������2��tp��ʱ����
	t = (char*)malloc(12);
	tp = (char*)malloc(12);
	op1 = (char*)malloc(12);
	op2 = (char*)malloc(12);

	if (flag == 0) {
		//factor();//���ӷ���
		strcpy(op1, factor());
		while (syn == STAR || syn == DIV)//�����'*'��'/'����ɨ����һ������
		{
			if (syn == STAR)
				*t = '*';
			else
				*t = '/';
			*(t + 1) = '\0';
			scanner();
			strcpy(op2, factor());//����factor������������ĵڶ���ep2;
			strcpy(tp, newtemp());

			emit(t, op1, op2, tp);
			strcpy(op1, tp);//������������Ϊ��һ�α��ʽ������ĵ�һ��
							//	factor();//���ӷ���
		}
	}
	return op1;
}

/*��ʽ����*/
char* factor()//<����>����=ID | NUM | ��<���ʽ>��
{
	char* fplace;
	fplace = (char*)malloc(12);
	strcpy(fplace, " ");

	if (syn == ID || syn == CONSTANT)//����Ǳ�ʶ�������֣���ɨ����һ������
	{

		if (syn == ID)
			strcpy(fplace, token);
		else
			itoa(sum, fplace, 10);
		scanner();
	}
	else if (syn == LP)//�����������
	{
		scanner();
		strcpy(fplace, expression());//����expression�������ر��ʽ��ֵ
		if (syn == RP)//�����������
			scanner();
		else {
			printf("ȱ�١�)��:Υ������ʽ��<��ʽ>-><����>|����|��<�������ʽ>��\n\n");
			flag = 1;
		}
	}
	else {
		printf("Υ������ʽ��<��ʽ>-><����>|����|��<�������ʽ>��\n\n");
		flag = 1;
	}
	return fplace;
}

void  emit(char* op, char* ag1, char* ag2, char* result) {
	//printf("\n%d:(%s ,%s ,%s ,%s )\n", NXQ++,op, ag1, ag2, result);

	strcpy(four[NXQ].ag1, ag1);
	strcpy(four[NXQ].ag2, ag2);
	strcpy(four[NXQ].op, op);
	strcpy(four[NXQ].result, result);
	NXQ++;
}

char* newtemp() {
	char* p;
	char m[8];
	p = (char*)malloc(8);
	kk++;
	itoa(kk, m, 10);
	strcpy(p + 1, m);
	p[0] = 't';
	return p;
}
