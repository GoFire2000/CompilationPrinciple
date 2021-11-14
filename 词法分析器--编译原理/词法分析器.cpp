#include <windows.h>
#include <bits/stdc++.h>
using namespace std;
#define LL long long
#define Inf 1e9
//���ʷ��Ŷ�Ӧ���ͣ�1��ʶ����2�ؼ��֣�3�޷�������4��ϵ�������5����������ͱ�����
#define idType 1
#define keywordsType 2
#define numType 3
#define relopType 4
#define singleType 5
const int maxn = 1010;
int state; //���ͱ�������ǰ״ָ̬ʾ
int len;//����ʶ��ĵ����ַ�������
char token[100]; //�ַ����飬��ŵ�ǰ����ʶ��ĵ����ַ���
char C; //�ַ���������ŵ�ǰ������ַ�
int iskey; //���ͱ�����ֵΪ-1����ʶʶ����ĵ������û��Զ����ʶ�������򣬱�ʾʶ����ĵ����ǹؼ��֣���ֵΪ�ؼ��ֵļǺ�
int identry; //���ͱ����������ڷ��ű��е�ָ��λ��

FILE *sptr;//Դ���ļ�
FILE *dptr;//����ļ�

char keywords[100][100];//�ؼ����ַ�����
int numofKeywords;//�ؼ�������

int idNum;//��ʶ������
char idNumStr[100];//���ڴ洢��ʶ��idֵ���ַ���

struct MarkTable{//�Ǻţ���־������
    int type;//���ͣ�1��ʶ����2�ؼ��֣�3�޷�������4��ϵ�������5�����������6�����ţ�7��ֵ�ţ�8ע�ͱ�ǣ�9�ָ���
    char mark[100];
    char val[100];
    char str[100];//�洢ԭtoken
}mark[40000];
int numofMark;//�Ǻ�����

int lineNum = 1;//��������
int charNum;//������ĸ��

bool judgeError;//�ж��Ƿ���ִ���
int errorType;//��������
int errorNum;//��������
/*
 * ���̣�ÿ����һ�Σ�������ǰָ��forward��ָʾ�����뻺�����ж�һ���ַ����������������C�У�Ȼ���ƶ�forwar��ʹָ֮����һ���ַ�
 */
void get_char(){
    C = fgetc(sptr);
    if(charNum != EOF) ++charNum;
}
/*
 * ���̣�ÿ�ε���ʱ�����C���ַ��Ƿ�Ϊ�ո�����ǣ��򷴸����ù���get_char��ֱ��C�н���һ���ǿ��ַ�Ϊֹ
 */
bool get_nbc(){
    bool tag = false;
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            lineNum++;
            tag = true;
        }
        C = fgetc(sptr);
    }
    return tag;
}
/*
 * �����������ж�C�е��ַ��Ƿ�����ĸ�������򷵻�true�����򷵻�false
 */
bool letter(){
    if((C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z')) return true;
    return false;
}
/*
 * �����������ж�C�е��ַ��Ƿ�Ϊ���֣������򷵻�true�����򷵻�false
 */
bool digit(){
    if(C >= '0' && C <= '9') return true;
    return false;
}
/*
 *���̣���C�е��ַ�������token�е��ַ�������
 */
void cat(){
    token[strlen(token)] = C;
}
/*
 *���̣���ǰָ��forward����һ���ַ�
 */
void retract(){
    fseek(sptr, -1, SEEK_CUR);
}
/*
 *����������token�е��ʲ�ؼ��ֱ���token�е����ǹؼ��֣��򷵻ظùؼ��ֵļǺţ����򣬷���ֵΪ-1
 */
int reserve(){
    int tag = -1;
    for(int i = 1; i <= numofKeywords; i++){
        if(strcmp(keywords[i], token) == 0){
            tag = i;
            break;
        }
    }
    return tag;
}
void color(short x)	//�Զ��庯���ݲ����ı���ɫ
{
    if(x>=0 && x<=15)//������0-15�ķ�Χ��ɫ
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);	//ֻ��һ���������ı�������ɫ
    else//Ĭ�ϵ���ɫ��ɫ
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
/*
 *���̣��Է��ֵĴ��������Ӧ�Ĵ���
 */
void outputError(){
    color(4);
    judgeError = false;
    errorNum++;
    printf("<error, %s , line: %d>", token, lineNum);
    fprintf(dptr, "<error, %s , line: %d>", token, lineNum);


    if(errorType == 3 || errorType == 5 || errorType == 6){
        printf("errorcode:%d  Decimal suffix error, invalid suffix on floating constant\n", errorType);
        fprintf(dptr, "errorcode:%d  Decimal suffix error, invalid suffix on floating constant\n", errorType);
    }
    else if(errorType == 13){
        printf("errorcode:%d  Lexical error\n", errorType);
        fprintf(dptr, "errorcode:%d  Lexical error\n", errorType);
    }
    else if(errorType == 23){
        printf("errorcode:%d  missing tereminating \" character\n", errorType);
        fprintf(dptr, "errorcode:%d  missing tereminating \" character\n", errorType);
    }
    else if(errorType == 24){
        printf("errorcode:%d  missing tereminating ' character\n", errorType);
        fprintf(dptr, "errorcode:%d  missing tereminating ' character\n", errorType);
    }
}
void error(int type){
    if(type != 23 && type != 24) cat();
    errorType = type;
    judgeError = true;
    outputError();
    //cout << "There is a bug!" << endl;
}
void LoadKeywords(){//�ؼ��ֵĵ���
    ifstream ifs;
    ifs.open("Keywords.txt", ios::in);
    if(!ifs.is_open()){
        color(4);
        cout << "�ؼ����ļ�������!" << endl;
        ifs.close();
        return;
    }
    ifs >> numofKeywords;
    for(int i = 1; i <= numofKeywords; i++)
        ifs >> keywords[i];
    ifs.close();
}
void retMark(MarkTable nowMark){//����Ǻ�

    /*if(nowMark.type == idType) printf("idType");
    else if(nowMark.type == keywordsType) printf("keywordsType");
    else if(nowMark.type == numType) printf("NumType");
    else if(nowMark.type == relopType) printf("relopType");
    else if(nowMark.type == singleType) printf("singleType");*/
    if(nowMark.type == idType) color(11);
    else if(nowMark.type == keywordsType) color(6);
    else if(nowMark.type == numType) color(14);
    else if(nowMark.type == relopType) color(10);
    else if(nowMark.type == singleType) color(3);
    printf("line%d  ",lineNum);
    printf("< %s , %s >\n", nowMark.mark, nowMark.val);
    fprintf(dptr, "line%d  ",lineNum);
    fprintf(dptr, "< %s , %s >\n", nowMark.mark, nowMark.val);

}
void Work();
int main(){
    LoadKeywords();
    Work();
    return 0;
}
void Work(){
    bool isNextLine;
    sptr = fopen("code.txt", "r");
    dptr = fopen("markTable.txt", "w+");
    state = 0;
    do{
        switch(state){
            case 0://��ʼ״̬
                if(judgeError == false) memset(token, 0, sizeof(token));
                get_char();
                isNextLine = get_nbc();
                //printf("%d (%c) %s\n",isNextLine, C,token);
                /*if(isNextLine){
                    state = 24;
                    break;
                }*/
                switch(C){
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
                    case 'Z': state = 1; break; //���ñ�ʶ��״̬

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = 2; break; //���ó�����״̬
                    case '<': state = 8; break; //����'<'��״̬
                    case '>': state = 9; break; //����'>'��״̬
                    case ':': state = 10; break;//����':'��״̬
                    case '/': state = 11; break;//����'/'��״̬
                    case '=': state = 15; break;//����'='��״̬
                    case '+': state = 16; break;//����'+'��״̬
                    case '-': state = 17; break;//����'-'��״̬
                    case '*': state = 18; break;//����'*'��״̬
                    case '%': state = 19; break;//����'%'��״̬
                    case '!': state = 20; break;//����'!'��״̬
                    case '&': state = 21; break;//����'&'��״̬
                    case '|': state = 22; break;//����'|'��״̬
                    case '"': state = 23; break;//����'"'��״̬
                    case '\'': state = 24; break;
                    case '^':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "^");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '.':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, ".");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case ',':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, ",");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '~':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "~");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '?':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "?");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '(':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "(");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case ')':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, ")");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '{':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "{");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '}':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "}");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '[':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "[");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case ']':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "]");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case ';':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, ";");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '#':{
                        cat();
                        state = 0;
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "#");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    default: state = 13; break;
                }
                break;
            case 1://��ʶ��״̬
                cat();
                get_char();
                if(letter() || digit() || C == '_') state = 1;
                else {
                    retract();
                    state = 0;
                    iskey = reserve();//�ؼ��ַ��ؼǺţ����򷵻�-1
                    if(iskey == -1) {//���ǹؼ���
                        idNum++;
                        itoa(idNum, idNumStr, 10);
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];
                        nowMark.type = idType;

                        strcpy(nowMark.mark, "id");
                        strcpy(nowMark.val, idNumStr);
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                    }else {//�ؼ���
                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];
                        nowMark.type = keywordsType;
                        strcpy(nowMark.mark, token);
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                    }
                }
                break;
            case 2://����״̬
                cat();
                get_char();
                switch(C){
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = 2; break;
                    case '.': state = 3; break;
                    case 'E': state = 5; break;
                    default:
                        retract();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = numType;
                        strcpy(nowMark.mark, "num");
                        strcpy(nowMark.val, token);
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                }
                break;
            case 3://С����״̬
                cat();
                get_char();
                if(digit()) state = 4;
                else {
                    error(3);
                    state = 0;
                }
                break;
            case 4://С��״̬
                cat();
                get_char();
                switch(C){
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = 4; break;
                    case 'E': state = 5; break;
                    default://ʶ���С��
                        retract();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = numType;
                        strcpy(nowMark.mark, "num");
                        strcpy(nowMark.val, token);
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                }
                break;
            case 5://ָ��״̬
                cat();
                get_char();
                switch(C){
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = 7; break;
                    case '+':
                    case '-': state = 6; break;
                    default:
                        retract();
                        error(5);
                        state = 0;
                        break;
                }
                break;
            case 6:
                cat();
                get_char();
                if(digit()) state = 7;
                else {
                    retract();
                    error(6);
                    state = 0;
                }
                break;
            case 7:
                cat();
                get_char();
                if(digit()) state = 7;
                else {
                    retract();
                    state = 0;

                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = numType;
                    strcpy(nowMark.mark, "num");
                    strcpy(nowMark.val, token);
                    strcpy(nowMark.str, token);
                    retMark(nowMark);//�����޷�����
                    break;
                }
                break;
            case 8://'<'״̬
                cat();
                get_char();
                switch(C){
                    case '=':{//<=
                        cat();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = relopType;
                        strcpy(nowMark.mark, "relop");
                        strcpy(nowMark.val, "LE");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '>':{//<>
                        cat();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = relopType;
                        strcpy(nowMark.mark, "relop");
                        strcpy(nowMark.val, "NE");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    case '<':{//<<
                        cat();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = singleType;
                        strcpy(nowMark.mark, "<<");
                        strcpy(nowMark.val, "-");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }
                    default:{//<
                        retract();
                        state = 0;

                        numofMark++;
                        MarkTable &nowMark = mark[numofMark];

                        nowMark.type = relopType;
                        strcpy(nowMark.mark, "relop");
                        strcpy(nowMark.val, "LT");
                        strcpy(nowMark.str, token);
                        retMark(nowMark);
                        break;
                    }

                }
                break;
            case 9://'>'״̬
                cat();
                get_char();
                //printf("<<%c>>", C);
                if(C == '='){//>=
                    cat();
                    state = 0;

                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = relopType;
                    strcpy(nowMark.mark, "relop");
                    strcpy(nowMark.val, "GE");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else if(C == '>'){
                    cat();
                    state = 0;

                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, ">>");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//>
                    retract();
                    state = 0;

                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = relopType;
                    strcpy(nowMark.mark, "relop");
                    strcpy(nowMark.val, "GT");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 10:
                cat();
                get_char();
                if(C == '='){//��=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, ":=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }else {//:
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, ":");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 11:// /
                cat();
                get_char();
                if(C == '*') state = 12; ///*
                else if(C == '/') state = 14; ////
                else if(C == '='){
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "/=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "/");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 12://ע�ʹ���/*
                get_char();
                if(C == '\n') lineNum++;
                while(C != '*') {
                    get_char();
                    if(C == '\n') lineNum++;
                }
                get_char();
                if(C == '\n') lineNum++;
                if(C == '/') state = 0;
                else state = 12;
                break;
            case 14://ע�ʹ���//
                get_char();
                while(C != '\n' && C != EOF) {
                    get_char();
                }
                if(C == '\n') lineNum++;
                state = 0;
                break;
            case 15://=
                cat();
                get_char();
                if(C == '='){//==
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = relopType;
                    strcpy(nowMark.mark, "relop");
                    strcpy(nowMark.val, "EQ");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//=
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 16://+״̬
                cat();
                get_char();
                if(C == '='){//+=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "+=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else if(C == '+'){//++
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "++");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//+
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 17://-״̬
                cat();
                get_char();
                if(C == '='){//-=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "-=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else if(C == '-'){//--
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "--");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else if(C == '>'){//->
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "->");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//-
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "-");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 18://*״̬
                cat();
                get_char();
                if(C == '='){//*=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "*=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }

                else {//*
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "*");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 19://%״̬
                cat();
                get_char();
                if(C == '='){//%=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "%=");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }

                else {//%
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "%");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 20://!
                cat();
                get_char();
                if(C == '='){//!=
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = relopType;
                    strcpy(nowMark.mark, "relop");
                    strcpy(nowMark.val, "NE");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }

                else {//!
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "!");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 21://&
                cat();
                get_char();
                if(C == '&'){//&&
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "&&");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//&
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "&");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 22://|
                cat();
                get_char();
                if(C == '|'){//||
                    cat();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "||");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                else {//&
                    retract();
                    state = 0;
                    numofMark++;
                    MarkTable &nowMark = mark[numofMark];

                    nowMark.type = singleType;
                    strcpy(nowMark.mark, "|");
                    strcpy(nowMark.val, "-");
                    strcpy(nowMark.str, token);
                    retMark(nowMark);
                }
                break;
            case 23: {//"
                cat();
                get_char();
                bool tag = false;
                if(C == '\n') {
                    error(23);
                    lineNum++;
                    state = 0;
                    break;
                }
                if(C == '\\') {
                    cat();
                    get_char();
                    if(C == '\n'){
                        tag = true;
                        error(23);
                        lineNum++;
                        state = 0;
                        break;
                    }
                    cat();
                    get_char();
                    if(C == '\n'){
                        tag = true;
                        error(23);
                        lineNum++;
                        state = 0;
                        break;
                    }
                }
                while(C != '"') {
                    cat();
                    get_char();
                    if(C == '\n') {
                        tag = true;
                        error(23);
                        lineNum++;
                        state = 0;
                        break;
                    }
                    if(C == '\\') {
                        cat();
                        get_char();
                        if(C == '\n'){
                            tag = true;
                            error(23);
                            lineNum++;
                            state = 0;
                            break;
                        }
                        cat();
                        get_char();
                        if(C == '\n'){
                            tag = true;
                            error(23);
                            lineNum++;
                            state = 0;
                            break;
                        }
                    }
                }
                if(tag) break;
                cat();
                state = 0;

                idNum++;
                itoa(idNum, idNumStr, 10);
                numofMark++;
                MarkTable &nowMark = mark[numofMark];
                nowMark.type = idType;

                strcpy(nowMark.mark, "id");
                strcpy(nowMark.val, idNumStr);
                strcpy(nowMark.str, token);
                retMark(nowMark);
                break;
            }
            case 24: {//'
                cat();
                get_char();

                bool tag1 = false;
                if(C == '\n') {
                    error(24);
                    lineNum++;
                    state = 0;
                    break;
                }
                if(C == '\\') {
                    cat();
                    get_char();
                    if (C == '\n') {
                        tag1 = true;
                        error(23);
                        lineNum++;
                        state = 0;
                        break;
                    }
                    cat();
                    get_char();
                    if (C == '\n') {
                        tag1 = true;
                        error(23);
                        lineNum++;
                        state = 0;
                        break;
                    }
                }
                while(C != '\'') {
                    cat();
                    get_char();
                    if(C == '\n') {
                        tag1 = true;
                        error(24);
                        lineNum++;
                        state = 0;
                        break;
                    }
                    if(C == '\\') {
                        cat();
                        get_char();
                        if (C == '\n') {
                            tag1 = true;
                            error(23);
                            lineNum++;
                            state = 0;
                            break;
                        }
                        cat();
                        get_char();
                        if (C == '\n') {
                            tag1 = true;
                            error(23);
                            lineNum++;
                            state = 0;
                            break;
                        }
                    }
                }

                if(tag1) break;
                cat();
                state = 0;

                idNum++;
                itoa(idNum, idNumStr, 10);
                numofMark++;
                MarkTable &nowMark = mark[numofMark];
                nowMark.type = idType;

                strcpy(nowMark.mark, "id");
                strcpy(nowMark.val, idNumStr);
                strcpy(nowMark.str, token);
                retMark(nowMark);
                break;
            }
            case 13:
                error(13);
                state = 0;
                break;
        }
    }while(C != EOF);
    color(7);
    printf("line:%d  ",lineNum);
    printf("idNumber:%d  ",idNum);
    printf("characterNumber:%d  ",charNum);
    printf("errorNum:%d\n",errorNum);

    fprintf(dptr, "line:%d  ",lineNum);
    fprintf(dptr, "idNumber:%d  ",idNum);
    fprintf(dptr, "characterNumber:%d  ",charNum);
    fprintf(dptr, "errorNum:%d\n",errorNum);

}
