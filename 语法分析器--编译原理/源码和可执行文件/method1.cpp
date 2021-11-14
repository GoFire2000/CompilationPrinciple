#include <bits/stdc++.h>
#define LL long long
#define Inf 1e9
using namespace std;
const int maxn = 1010;
char textStr[maxn]; //要翻译的串
int Index;//下标
/*
E->E+T|E-T|T
T->T*F|T/F|F
F->(E)|num
.
 */
/*
消除左递归后
E->TS
S->AS|#
A->+T|-T
T->FW
W->BW|#
B->*F|/F
F->(E)|num
.
*/
/*
实验要求：在对输入的算术表达式进行分析的过程中，依次输出所采用的的产生式
方法1：编写递归调用程序实现自顶向下的分析
*/
//递归调用程序实现自顶向下的分析
int proc_E();
int proc_T();
int proc_S();
int proc_F();
int proc_A();
int proc_B();
int proc_W();

int isError = false;
void Error(){
    cout << "Error Occurred!" << endl;
}

int proc_E(){
    puts("E->TS");
    bool tag1 = proc_T();
    bool tag2 = proc_S();
    return tag1 & tag2;
}
int proc_T(){
    puts("T->FW");
    bool tag1 = proc_F();
    bool tag2 = proc_W();
    return tag1 & tag2;
}
int proc_S(){
    int tag = proc_A();
    if(tag == -1) {
        return false;
    }else if(tag == 1){
        return proc_S();
    }
    else {
        puts("S->#");
        return true;
    }
}
int proc_A(){
    if(textStr[Index] == '+' || textStr[Index] == '-'){
        puts("S->AS");
        printf("A->%cT\n", textStr[Index]);
        Index++;
        bool tag = proc_T();
        if(!tag) return -1;
        return true;
    }
    return false;
}
int proc_W(){
    int tag = proc_B();
    if(tag == -1){
        return false;
    }
    else if(tag == 1) {
        return proc_W();
    }
    else {
        puts("W->#");
        return true;
    }
}
int proc_B(){
    if(textStr[Index] == '*' || textStr[Index] == '/'){
        puts("W->BW");
        printf("B->%cF\n", textStr[Index]);
        Index++;
        bool tag = proc_F();
        if(!tag) return -1;
        return true;
    }
    return false;
}

int proc_F(){
    if(textStr[Index] == '('){
        puts("F->(E)");
        Index++;
        bool tag =  proc_E();
        if(textStr[Index] == ')') {
            Index++;
            return tag;
        }
        else return false;
        // Index++;
    }else if(textStr[Index] >= '0' && textStr[Index] <='9'){
        puts("F->num");
        int val = 0;
        while(Index < strlen(textStr) && textStr[Index] >= '0' && textStr[Index] <='9'){
            val = val * 10 + textStr[Index] - '0';
            Index++;
        }
        return true;
    }
    else return false;
}

void Init();
int main(){
    Init();
    return 0;
}
//(4+3)/2+(6+3)*2+4-3
void Init(){
    while(true){
        memset(textStr, 0, sizeof(textStr));
        cout << "Please enter a text string, if you want to exit, please enter #:" << endl;
        scanf("%s", textStr);
        int len = strlen(textStr);
        if(len == 1 && textStr[0] == '#') break;
        Index = 0;
        if(proc_E() == false || Index < len) Error();
        
        // printf("Index = %d\n", Index);
        puts("\nRecursive calling program to achieve top-down analysis is complete!\n\n");
        system("pause");
    }
    //2+(3*1/2+3123)
}