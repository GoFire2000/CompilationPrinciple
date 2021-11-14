#include <bits/stdc++.h>
#define LL long long
#define Inf 1e9
using namespace std;
const int maxn = 1010;
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
方法2： 编写LL(1)语法分析程序，要求如下
 （1）编程实现算法4.2，为给定文法自动构造预测分析表
 （2）编程实现算法4.1，构造LL（1）分析程序
 */
char oriG[maxn][maxn];//存放初始文法内容
int numGra;//文法的数量

char proG[maxn][maxn];//消除左递归后的文法内容

vector<int> G[maxn][maxn];//存放文法
int Count[maxn];//Count[x]表示非终结符x能推导出来的式子数量
map<string, int> strToid;//标识符名字与数字之间的映射

char textStr[maxn]; //要翻译的串

struct Element{
    int type;//type=0为非终结符，type=1为终结符,type=2为空,type=3为结束符号
    string Name;//标识
    Element(){};
    Element(int ttp, string NName){
        type = ttp;
        Name = NName;
    }
}ele[maxn];
int numEle;//终结符和非终结符的数量


vector<int> First[maxn];//First集
vector<int> Follow[maxn];//Follow集



void PreProcess(){//消除左递归，oriG->G
    //ETSAFWB
    ele[1] = Element(0, "E");
    ele[2] = Element(0, "T");
    ele[3] = Element(0, "S");
    ele[4] = Element(0, "A");
    ele[5] = Element(0, "F");
    ele[6] = Element(0, "W");
    ele[7] = Element(0, "B");
    ele[8] = Element(1, "+");
    ele[9] = Element(1, "-");
    ele[10] = Element(1, "*");
    ele[11] = Element(1, "/");
    ele[12] = Element(1, "(");
    ele[13] = Element(1, ")");
    ele[14] = Element(1, "num");
    ele[15] = Element(3, "$");
    ele[16] = Element(2, "#");
    numEle = 16;
    for(int i = 1; i <= numEle; i++) strToid[ele[i].Name] = i;
    numGra += 7;
    for(int i = 1; i <= numGra; i++) {
        string now;
        if (i == 1) now = "E->TS";
        else if (i == 2) now = "S->AS|#";
        else if (i == 3) now = "A->+T|-T";
        else if (i == 4) now = "T->FW";
        else if (i == 5) now = "W->BW|#";
        else if (i == 6) now = "B->*F|/F";
        else if (i == 7) now = "F->(E)|num";
        int len = now.length();
        for(int j = 0; j < len; j++){
            proG[i][j] = now[j];
        }
    }
}

void Process(){//将字符串转为标识符和数组
    for(int d = 1; d <= numGra; d++){
        int len = strlen(proG[d]);
        string now = "";
        int i = 0;
        while(i < len && !(proG[d][i] == '-' && proG[d][i + 1] == '>')) {
            now = now + proG[d][i];
            i++;
        }
        int id = strToid[now];
        i += 2;
        while(i < len){
            int j = i;
            while(j < len && proG[d][j] != '|') j++;
            if(proG[d][j] == '|') j--;
            ++Count[id];
            now = "";
            for(int k = i; k <= j; k++){
                now = now + proG[d][k];
                if(strToid[now]) {
                    G[id][Count[id]].push_back(strToid[now]);
                    now = "";
                }
            }
            i = j + 2;
        }
    }
    for(int i = 1; i <= numEle; i++){
        if(Count[i]){
//            printf("%d->", i);
            cout << ele[i].Name << "->";
            for(int j = 1; j <= Count[i]; j++){
                for(int k = 0; k < G[i][j].size(); k++){
//                    printf("%d", G[i][j][k]);
                    cout << ele[G[i][j][k]].Name;
                }
                if(j != Count[i]) printf("|");
            }
            printf("\n");
        }
    }
}

bool FindInFirst(int id, int tar){//在id的First集合中找有没有tar
    for(int i = 0; i < First[id].size(); i++) {
        if(tar == First[id][i]) return true;
    }
    return false;
}

void FindFirst(int id){//找id的First集
    if(First[id].size()) return;
    for(int i = 1; i <= Count[id]; i++){
        int be = G[id][i][0];
        if(ele[be].type == 1){
            First[id].push_back(be);
        }else if(ele[be].type == 2){
            First[id].push_back(be);
        }else if(ele[be].type == 0){
            int num = 0;
            bool tag = false;
            for(int j = 0; j < G[id][i].size(); j++){
                int now = G[id][i][j];
                if(now == id) break;
                FindFirst(now);
                for(int k = 0; k < First[now].size(); k++){
                    if(First[now][k] != strToid["#"]) First[id].push_back(First[now][k]);
                }
                if(!FindInFirst(now, strToid["#"])) break;
                num++;
            }
            if(num == G[id][i].size()) First[id].push_back(strToid["#"]);
        }
    }
    sort(First[id].begin(), First[id].end());
    First[id].erase(unique(First[id].begin(), First[id].end()), First[id].end());
}
void QueryFirst(){//求FIRST集
    for(int i = 1; i <= numEle; i++){
        if(ele[i].type == 1){
            First[i].push_back(i);
        }else if(ele[i].type == 0){
            FindFirst(i);
        }
    }
}
void PrintFirst(){//输出First集
    for(int i = 1; i<= numEle; i++) if(ele[i].type == 0) {
        cout << "First(" << ele[i].Name << ") = {";
        for(int j = 0; j < First[i].size(); j++){
            int now = First[i][j];
            if(j != First[i].size() - 1) cout << ele[now].Name << ", ";
            else cout << ele[now].Name;
        }
        cout << "}" << endl;
    }
}

bool isQuery[maxn];


void FindFollow(int id){//找id的Follow集
    if(isQuery[id]) return; 
    isQuery[id] = true;
    for(int i = 1; i <= numEle; i++) if(ele[i].type == 0){
        for(int j = 1;j <= Count[i]; j++){
            for(int k = 0; k < G[i][j].size(); k++){
                if(G[i][j][k] == id){
                    if(k == G[i][j].size() - 1){
                        if(i == id) continue;
                        FindFollow(i);
                        for(int ii = 0; ii < Follow[i].size(); ii++){
                            Follow[id].push_back(Follow[i][ii]);
                        }
                    }else {
                        int x = G[i][j][k + 1];
                        for(int ii = 0; ii < First[x].size(); ii++){
                            if(First[x][ii] != strToid["#"]) Follow[id].push_back(First[x][ii]);
                        }
                        if(FindInFirst(x, strToid["#"]) && i != id){
                            FindFollow(i);
                            for(int ii = 0; ii < Follow[i].size(); ii++){
                                Follow[id].push_back(Follow[i][ii]);
                            }
                        }
                    }
                }
            }
        }
    }
    
    sort(Follow[id].begin(), Follow[id].end());
    Follow[id].erase(unique(Follow[id].begin(), Follow[id].end()), Follow[id].end());
}

void QueryFollow(){//求Follow集
    Follow[strToid["E"]].push_back(strToid["$"]);
    for(int i = 1; i <= numEle; i++){
        if(ele[i].type == 1) 
            Follow[i].push_back(strToid["$"]);
    }
    for(int i = 1; i <= numEle; i++){
        if(ele[i].type == 0 && !isQuery[i]){
            // printf("i = %d\n", i);
            FindFollow(i);
        }
    }
}

void PrintFollow(){//输出Follow集
    for(int i = 1; i<= numEle; i++) if(ele[i].type == 0){
        cout << "Follow(" << ele[i].Name << ") = {";
        for(int j = 0; j < Follow[i].size(); j++){
            int now = Follow[i][j];
            if(j != Follow[i].size() - 1) cout << ele[now].Name << ", ";
            else cout << ele[now].Name;
        }
        cout << "}" << endl;
    }
}

void PrintFirstAndFollowTable(){//输出First和Follow表格
    cout << "\nFIRST set and FOLLOW set of grammer G:" << endl;
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|         |");
    for(int j = 1; j <= numEle; j++) if(ele[j].type == 0){//终结符或$
        cout << "          " << left << setw(11) << ele[j].Name << "|";
    }
    cout << endl;
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|  FIRST  |");
    for(int i = 1; i<= numEle; i++) if(ele[i].type == 0) {
        string now = "";
        for(int j = 0; j < First[i].size(); j++){
            now = now + ele[First[i][j]].Name;
            if(j != First[i].size() - 1) now = now + ", ";
        }
        cout << "   " << setw(16) << now << "  |";
    }
    cout << endl;
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|  FOLLOW |");
    for(int i = 1; i<= numEle; i++) if(ele[i].type == 0) {
        string now = "";
        for(int j = 0; j < Follow[i].size(); j++){
            now = now + ele[Follow[i][j]].Name;
            if(j != Follow[i].size() - 1) now = now + ", ";
        }
        cout << "   " << setw(16) << now << "  |";
    }
    cout << endl;
    
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
    
}
vector<int> TableLL1[maxn][maxn];

void CreatePredicitionTableOfLL1(){//构造LL1文法的预测分析表哦
    for(int i = 1; i <= numEle; i++) if(ele[i].type == 0){
        for(int j = 1; j <= Count[i]; j++){
            int x = G[i][j][0];
            if(x == strToid["#"] || FindInFirst(x, strToid["#"])){
                for(int k = 0; k < Follow[i].size(); k++){
                    int b = Follow[i][k];
                    TableLL1[i][b].push_back(i);
                    for(int ii = 0; ii < G[i][j].size(); ii++){
                        TableLL1[i][b].push_back(G[i][j][ii]);
                    }
                }
            }
            if(x == strToid["#"]) continue;
            for(int k = 0; k < First[x].size(); k++) if(First[x][k] != strToid["#"]){
                int b = First[x][k];
                TableLL1[i][b].push_back(i);
                for(int ii = 0; ii < G[i][j].size(); ii++){
                    TableLL1[i][b].push_back(G[i][j][ii]);
                }
            }
        }
    }
}

void PrintPredicitionTableOfLL1(){//输出LL1文法的预测分析表
    cout << "\nPredicition Analysis Table of grammer G:" << endl;
    printf("----------------------------------------------------------------------------------\n");
    printf("|        |");
    for(int j = 1; j <= numEle; j++) if(ele[j].type == 1 || ele[j].type == 3){//终结符或$
        cout << "    " << left << setw(4) << ele[j].Name << "|";
    }
    cout << endl;
    printf("----------------------------------------------------------------------------------\n");
    
    for(int i = 1; i <= numEle; i++) if(ele[i].type == 0){
        cout << "|    " << left << setw(4) << ele[i].Name << "|";
        for(int j = 1; j <= numEle; j++) if(ele[j].type == 1 || ele[j].type == 3){//终结符或$
            if(TableLL1[i][j].size() == 0) {
                cout << "        |";
            }else {
                string now = "";
                for(int k = 0 ; k < TableLL1[i][j].size(); k++){
                    int x = TableLL1[i][j][k];
                    if(k == 0){
                        now = now + ele[x].Name + "->";
                    }else{
                        now = now + ele[x].Name;
                    }
                }
                cout << " " << setw(7) << now << "|";
            }
        }
        cout << endl;
        printf("----------------------------------------------------------------------------------\n");
    }
} 

void Error(){
    cout << "Error Occurred!" << endl;
}
int sta[maxn];//栈
int top;
void Print(int pos, int len, string strOut){
    string strSta = "";
    string strIn = "";
    // string strOut = "";
    for(int i = 0; i < top; i++) strSta = strSta + ele[sta[i]].Name;
    for(int i = pos; i < len; i++) strIn = strIn + textStr[i];
    cout << "|    " << setw(15) << strSta << "|    " << setw(23) << strIn << "|    " << setw(15) << strOut << "|" <<endl;
    printf("--------------------------------------------------------------------\n");
}
// stack<int> sta;//栈
// stack<int> q;
void NonRecursivePredictionAnalysisOfLL1(){//LL1文法的非递归预测分析
    // while(!sta.empty()) sta.pop();
    printf("--------------------------------------------------------------------\n");
    printf("|     stack         |          input            |     output        |\n");
    printf("--------------------------------------------------------------------\n");
    top = 0;
    int len = strlen(textStr);
    textStr[len++] = '$';
    int i = 0;
    sta[top++] = (strToid["$"]);
    sta[top++] = (strToid["E"]);
    int x;
    Print(0, len, "");
    while(true){
        int x = sta[top - 1];
        if(x == strToid["$"]) break;
        int val = -1;
        char ch = 0;

        string sch = "";
        if(textStr[i] >= '0' && textStr[i] <= '9') sch = "num";
        else sch = sch + textStr[i];

        if(x == strToid["$"] || ele[x].type == 1){//终结符或$
            if(ele[x].Name == sch){
                top--;
                if(sch == "num"){
                    while(textStr[i] >= '0' && textStr[i] <= '9') i++;
                }
                else i++;
                Print(i, len, "");
            }
            else {
                Error();
                return;
            }
        }else {
            int v = strToid[sch];
            if(TableLL1[x][v].size()){
                top--;
                for(int j = TableLL1[x][v].size() - 1; j >= 1; j--){
                    if(TableLL1[x][v][j] != strToid["#"]) sta[top++] = (TableLL1[x][v][j]);
                }
                string strStr = "";
                for(int j = 0; j < TableLL1[x][v].size(); j++){
                    int now = TableLL1[x][v][j];
                    if(j == 0) strStr = strStr + ele[now].Name + "->";//cout << ele[now].Name << "->";
                    else strStr = strStr + ele[now].Name;//cout << ele[now].Name;
                }
                // cout << endl;
                Print(i, len, strStr);

            }else {
                Error();
                return;
            }
        }
    }
    if(i + 1 < len) Error();
    //(123+3213122)/2
}

void Init();
int main(){
    Init();
    return 0;
}
void Init(){
    PreProcess();//初始化
    Process();//由字符串转为标识符和数组

    QueryFirst();//求First集
    PrintFirst();//输出First集

    QueryFollow();//求Follow集
    PrintFollow();//输出Follow集

    PrintFirstAndFollowTable();//输出First和Follow表格

    CreatePredicitionTableOfLL1(); //构造LL1文法的预测分析表
    PrintPredicitionTableOfLL1(); //输出LL1文法的预测分析表

    
    while(true){
        memset(textStr, 0, sizeof(textStr));
        cout << "Please enter a text string, if you want to exit, please enter #:" << endl;
        scanf("%s", textStr);
        int len = strlen(textStr);
        if(len == 1 && textStr[0] == '#') break;

        NonRecursivePredictionAnalysisOfLL1();//LL1文法的非递归预测分析

        printf("\nUsing LL1 grammar analysis for non recursive analysis is complete!\n\n\n");
        system("pause");
    }

}