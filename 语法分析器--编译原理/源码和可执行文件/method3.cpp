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
方法3： 编写LR(1)语法分析程序，要求如下
 （1）构造识别该文法的所有活前缀的DFA
 （2）构造该文法的LR分析表
 （3）编程实现算法4.3，构造LR分析程序
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

int numGS;
vector<int> GS[maxn];



void PreProcess(){//划分终结符和非终结符，输入文法
    //ETSAFWB
    ele[1] = Element(0, "S");
    ele[2] = Element(0, "E");
    ele[3] = Element(0, "T");
    ele[4] = Element(0, "F");
    ele[5] = Element(1, "+");
    ele[6] = Element(1, "-");
    ele[7] = Element(1, "*");
    ele[8] = Element(1, "/");
    ele[9] = Element(1, "(");
    ele[10] = Element(1, ")");
    ele[11] = Element(1, "num");
    ele[12] = Element(3, "$");
    ele[13] = Element(2, "#");
    
    numEle = 13;
    for(int i = 1; i <= numEle; i++) strToid[ele[i].Name] = i;
    numGra = 4;
    for(int i = 1; i <= numGra; i++) {
        string now;
        if(i == 1) now = "S->E";
        else if (i == 2) now = "E->E+T|E-T|T";
        else if (i == 3) now = "T->T*F|T/F|F";
        else if (i == 4) now = "F->(E)|num";
        int len = now.length();
        for(int j = 0; j < len; j++){
            proG[i][j] = now[j];
        }
    }
    
    numGS = 9;
    for(int i = 0; i < numGS; i++) {
        string now;
        if(i == 0) now = "S->E";
        else if (i == 1) now = "E->E+T";
        else if (i == 2) now = "E->E-T";
        else if (i == 3) now = "E->T";
        else if (i == 4) now = "T->T*F";
        else if (i == 5) now = "T->T/F";
        else if (i == 6) now = "T->F";
        else if (i == 7) now = "F->(E)";
        else if (i == 8) now = "F->num";
        int len = now.length();
        for(int j = 0; j < len; j++){
            char ch = now[j];
            string snow = "";
            if(ch == 'n') {
                snow = snow + "num";
                j += 2;
            }
            else snow = snow + ch;
            if(j == 0) {
                GS[i].push_back(strToid[snow]);
                j += 2;
            } else {
                GS[i].push_back(strToid[snow]);
            }
        }
    }
}

void PrintGS(int i){//输出单个文法
    for(int j = 0; j < GS[i].size(); j++){
        if(j == 0) {
            cout << ele[GS[i][j]].Name << "->";
        }else {
            cout << ele[GS[i][j]].Name;
        }
    }
    cout << endl;
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
    for(int i = 0; i < numGS; i++){//文法单个逐次输出
        PrintGS(i);
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
    Follow[strToid["S"]].push_back(strToid["$"]);
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
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("|         |");
    for(int j = 1; j <= numEle; j++) if(ele[j].type == 0){//终结符或$
        cout << "          " << left << setw(11) << ele[j].Name << "|";
    }
    cout << endl;
    printf("---------------------------------------------------------------------------------------------------\n");
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
    printf("---------------------------------------------------------------------------------------------------\n");
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
    
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("\n");
    
}


void CreateDFAOfLR1(){//构造识别该文法的所有活前缀的DFA
    // See photos
}

struct Node{
    char tp;
    int v;
    Node(){};
    Node(char tpp, int vv){tp = tpp; v = vv;}
    //四种类型
    //tp = 'S', v 为数字,终结符移进
    //tp = 'R', v 为数字,规约
    //tp = 'M', v 为数字,非终结符移进
    //tp = 'A', v = -1, ACC
};

int numState = 30;//状态数，0~29
Node TableOfLR1[maxn][maxn];//分析表

void CreateAnalysisTableOfLR1(){//构造该文法的LR1分析表
    TableOfLR1[0][strToid["("]] = Node('S', 3);
    TableOfLR1[0][strToid["num"]] = Node('S', 5);
    TableOfLR1[0][strToid["E"]] = Node('M', 1);
    TableOfLR1[0][strToid["T"]] = Node('M', 2);
    TableOfLR1[0][strToid["F"]] = Node('M', 4);

    TableOfLR1[1][strToid["+"]] = Node('S', 6);
    TableOfLR1[1][strToid["-"]] = Node('S', 7);
    TableOfLR1[1][strToid["$"]] = Node('A', -1);

    TableOfLR1[2][strToid["+"]] = Node('R', 3);
    TableOfLR1[2][strToid["-"]] = Node('R', 3);
    TableOfLR1[2][strToid["*"]] = Node('S', 8);
    TableOfLR1[2][strToid["/"]] = Node('S', 9);
    TableOfLR1[2][strToid["$"]] = Node('R', 3);

    
    TableOfLR1[3][strToid["("]] = Node('S', 13);
    TableOfLR1[3][strToid["num"]] = Node('S', 14);
    TableOfLR1[3][strToid["E"]] = Node('M', 10);
    TableOfLR1[3][strToid["T"]] = Node('M', 11);
    TableOfLR1[3][strToid["F"]] = Node('M', 12);

    TableOfLR1[4][strToid["+"]] = Node('R', 6);
    TableOfLR1[4][strToid["-"]] = Node('R', 6);
    TableOfLR1[4][strToid["*"]] = Node('R', 6);
    TableOfLR1[4][strToid["/"]] = Node('R', 6);
    TableOfLR1[4][strToid["$"]] = Node('R', 6);

    
    TableOfLR1[5][strToid["+"]] = Node('R', 8);
    TableOfLR1[5][strToid["-"]] = Node('R', 8);
    TableOfLR1[5][strToid["*"]] = Node('R', 8);
    TableOfLR1[5][strToid["/"]] = Node('R', 8);
    TableOfLR1[5][strToid["$"]] = Node('R', 8);

    TableOfLR1[6][strToid["("]] = Node('S', 3);
    TableOfLR1[6][strToid["num"]] = Node('S', 5);
    TableOfLR1[6][strToid["T"]] = Node('M', 15);
    TableOfLR1[6][strToid["F"]] = Node('M', 4);
    
    TableOfLR1[7][strToid["("]] = Node('S', 3);
    TableOfLR1[7][strToid["num"]] = Node('S', 5);
    TableOfLR1[7][strToid["T"]] = Node('M', 16);
    TableOfLR1[7][strToid["F"]] = Node('M', 4);
    
    TableOfLR1[8][strToid["("]] = Node('S', 3);
    TableOfLR1[8][strToid["num"]] = Node('S', 5);
    TableOfLR1[8][strToid["F"]] = Node('M', 17);
    
    TableOfLR1[9][strToid["("]] = Node('S', 3);
    TableOfLR1[9][strToid["num"]] = Node('S', 5);
    TableOfLR1[9][strToid["F"]] = Node('M', 18);

    
    TableOfLR1[10][strToid["+"]] = Node('S', 20);
    TableOfLR1[10][strToid["-"]] = Node('S', 21);
    TableOfLR1[10][strToid[")"]] = Node('S', 19);
    
    TableOfLR1[11][strToid["+"]] = Node('R', 3);
    TableOfLR1[11][strToid["-"]] = Node('R', 3);
    TableOfLR1[11][strToid["*"]] = Node('S', 22);
    TableOfLR1[11][strToid["/"]] = Node('S', 23);
    TableOfLR1[11][strToid[")"]] = Node('R', 3);
    
    TableOfLR1[12][strToid["+"]] = Node('R', 6);
    TableOfLR1[12][strToid["-"]] = Node('R', 6);
    TableOfLR1[12][strToid["*"]] = Node('R', 6);
    TableOfLR1[12][strToid["/"]] = Node('R', 6);
    TableOfLR1[12][strToid[")"]] = Node('R', 6);
    
    TableOfLR1[13][strToid["("]] = Node('S', 13);
    TableOfLR1[13][strToid["num"]] = Node('S', 14);
    TableOfLR1[13][strToid["E"]] = Node('M', 24);
    TableOfLR1[13][strToid["T"]] = Node('M', 11);
    TableOfLR1[13][strToid["F"]] = Node('M', 12);
    
    TableOfLR1[14][strToid["+"]] = Node('R', 8);
    TableOfLR1[14][strToid["-"]] = Node('R', 8);
    TableOfLR1[14][strToid["*"]] = Node('R', 8);
    TableOfLR1[14][strToid["/"]] = Node('R', 8);
    TableOfLR1[14][strToid[")"]] = Node('R', 8);
    
    TableOfLR1[15][strToid["+"]] = Node('R', 1);
    TableOfLR1[15][strToid["-"]] = Node('R', 1);
    TableOfLR1[15][strToid["*"]] = Node('S', 8);
    TableOfLR1[15][strToid["/"]] = Node('S', 9);
    TableOfLR1[15][strToid["$"]] = Node('R', 1);

    TableOfLR1[16][strToid["+"]] = Node('R', 2);
    TableOfLR1[16][strToid["-"]] = Node('R', 2);
    TableOfLR1[16][strToid["*"]] = Node('S', 8);
    TableOfLR1[16][strToid["/"]] = Node('S', 9);
    TableOfLR1[16][strToid["$"]] = Node('R', 2);
    
    TableOfLR1[17][strToid["+"]] = Node('R', 4);
    TableOfLR1[17][strToid["-"]] = Node('R', 4);
    TableOfLR1[17][strToid["*"]] = Node('R', 4);
    TableOfLR1[17][strToid["/"]] = Node('R', 4);
    TableOfLR1[17][strToid["$"]] = Node('R', 4);
    
    TableOfLR1[18][strToid["+"]] = Node('R', 5);
    TableOfLR1[18][strToid["-"]] = Node('R', 5);
    TableOfLR1[18][strToid["*"]] = Node('R', 5);
    TableOfLR1[18][strToid["/"]] = Node('R', 5);
    TableOfLR1[18][strToid["$"]] = Node('R', 5);
    
    TableOfLR1[19][strToid["+"]] = Node('R', 7);
    TableOfLR1[19][strToid["-"]] = Node('R', 7);
    TableOfLR1[19][strToid["*"]] = Node('R', 7);
    TableOfLR1[19][strToid["/"]] = Node('R', 7);
    TableOfLR1[19][strToid["$"]] = Node('R', 7);
    
    TableOfLR1[20][strToid["("]] = Node('S', 13);
    TableOfLR1[20][strToid["num"]] = Node('S', 14);
    TableOfLR1[20][strToid["T"]] = Node('M', 25);
    TableOfLR1[20][strToid["F"]] = Node('M', 12);

    TableOfLR1[21][strToid["("]] = Node('S', 13);
    TableOfLR1[21][strToid["num"]] = Node('S', 14);
    TableOfLR1[21][strToid["T"]] = Node('M', 26);
    TableOfLR1[21][strToid["F"]] = Node('M',12);

    TableOfLR1[22][strToid["("]] = Node('S', 13);
    TableOfLR1[22][strToid["num"]] = Node('S', 14);
    TableOfLR1[22][strToid["F"]] = Node('M', 27);
    
    TableOfLR1[23][strToid["("]] = Node('S', 13);
    TableOfLR1[23][strToid["num"]] = Node('S', 14);
    TableOfLR1[23][strToid["F"]] = Node('M', 28);

    TableOfLR1[24][strToid["+"]] = Node('S', 20);
    TableOfLR1[24][strToid["-"]] = Node('S', 21);
    TableOfLR1[24][strToid[")"]] = Node('S', 29);

    TableOfLR1[25][strToid["+"]] = Node('R', 1);
    TableOfLR1[25][strToid["-"]] = Node('R', 1);
    TableOfLR1[25][strToid["*"]] = Node('S', 22);
    TableOfLR1[25][strToid["/"]] = Node('S', 23);
    TableOfLR1[25][strToid[")"]] = Node('R', 1);
    
    TableOfLR1[26][strToid["+"]] = Node('R', 2);
    TableOfLR1[26][strToid["-"]] = Node('R', 2);
    TableOfLR1[26][strToid["*"]] = Node('S', 22);
    TableOfLR1[26][strToid["/"]] = Node('S', 23);
    TableOfLR1[26][strToid[")"]] = Node('R', 2);
    
    TableOfLR1[27][strToid["+"]] = Node('R', 4);
    TableOfLR1[27][strToid["-"]] = Node('R', 4);
    TableOfLR1[27][strToid["*"]] = Node('R', 4);
    TableOfLR1[27][strToid["/"]] = Node('R', 4);
    TableOfLR1[27][strToid[")"]] = Node('R', 4);
    
    TableOfLR1[28][strToid["+"]] = Node('R', 5);
    TableOfLR1[28][strToid["-"]] = Node('R', 5);
    TableOfLR1[28][strToid["*"]] = Node('R', 5);
    TableOfLR1[28][strToid["/"]] = Node('R', 5);
    TableOfLR1[28][strToid[")"]] = Node('R', 5);
    
    TableOfLR1[29][strToid["+"]] = Node('R', 7);
    TableOfLR1[29][strToid["-"]] = Node('R', 7);
    TableOfLR1[29][strToid["*"]] = Node('R', 7);
    TableOfLR1[29][strToid["/"]] = Node('R', 7);
    TableOfLR1[29][strToid[")"]] = Node('R', 7);
}

string IntToString(int val){//整形转为字符串
    char snow[15];
    memset(snow, 0, sizeof(snow));
    itoa(val, snow, 10);
    string strnow = snow;
    return strnow;
}

void PrintAnalysisTableOfLR1(){//输出该文法的LR1分析表

    printf("-------------------------------------------------------------------------------------------------------------\n");
    cout << "|  " << setw(6) << "state" << "|" ; 
    for(int i = 1; i <= numEle; i++){
        if(ele[i].type == 1 || ele[i].type == 3){
            cout << "  " << setw(6) << ele[i].Name << "|";
        }
    }
    for(int i = 1; i <= numEle; i++){
        if(ele[i].type == 0 && i != strToid["S"]){
            cout << "  " << setw(6) << ele[i].Name << "|";
        }
    }
    cout << endl;
    printf("-------------------------------------------------------------------------------------------------------------\n");
    for(int d = 0; d < numState; d++){
        cout << "| " << setw(6) << IntToString(d) << " |";
        for(int i = 1; i <= numEle; i++){
            if(ele[i].type == 1 || ele[i].type == 3){
                if(TableOfLR1[d][i].v == 0) cout << "        " << "|";
                else {
                    string now = "";
                    if(TableOfLR1[d][i].tp == 'A') now = "ACC";
                    else if(TableOfLR1[d][i].tp == 'M') now = now + IntToString(TableOfLR1[d][i].v);
                    else {
                        now = now + TableOfLR1[d][i].tp;
                        now = now + IntToString(TableOfLR1[d][i].v);
                    }
                    cout << "  " << setw(6) << now << "|";
                }
            }
        }
        for(int i = 1; i <= numEle; i++){
            if(ele[i].type == 0 && i != strToid["S"]){
                // printf("<<%c%d>> ", TableOfLR1[d][i].tp, TableOfLR1[d][i].v);
                if(TableOfLR1[d][i].v == 0) cout << "        " << "|";
                else {
                    string now = "";
                    if(TableOfLR1[d][i].tp == 'A') now = "ACC";
                    else if(TableOfLR1[d][i].tp == 'M') now = now + IntToString(TableOfLR1[d][i].v);
                    else now = now + TableOfLR1[d][i].tp + IntToString(TableOfLR1[d][i].v);
                    cout << "  " << setw(6) << now << "|";
                }
            }
        }
        cout << endl;
        printf("-------------------------------------------------------------------------------------------------------------\n");
    }
}

void Error(){
    cout << "Error Occurred!" << endl;
}
int staState[maxn], topState;
int staLabel[maxn], topLabel;
// stack<int> staState;//状态栈
// stack<int> staLabel;//符号栈

void Print(int pos, int len, string strOut){
    string strState = "";
    string strLabel = "";
    string strIn = "";
    // string strOut = "";
    for(int i = 0; i < topState; i++) strState = strState + IntToString(staState[i]) + " ";
    for(int i = 0; i < topLabel; i++) strLabel = strLabel + ele[staLabel[i]].Name;
    
    for(int i = pos; i < len; i++) strIn = strIn + textStr[i];
    cout << "|    " << setw(23) << strState << "|    " << setw(23) << strLabel <<"|    " << setw(23) << strIn << "|    " << setw(17) << strOut << "|" <<endl;
    printf("-----------------------------------------------------------------------------------------------------------\n");
}

void AnalysisOfLR1(){//LR1文法的分析
    // while(!staState.empty()) staState.pop();
    // while(!staLabel.empty()) staLabel.pop();
    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("|        stackState         |        stackSymble        |            input          |        output       |\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    topState = 0;
    topLabel = 0;
    int len = strlen(textStr);
    textStr[len++] = '$';
    int i = 0;
    staState[topState++] = (0);
    staLabel[topLabel++] = strToid["#"];
    
    do{
        int x = staState[topState - 1];
        string sch = "";
        if(textStr[i] >= '0' && textStr[i] <= '9') sch = "num";
        else sch = sch + textStr[i];
        if(TableOfLR1[x][strToid[sch]].tp == 'S'){
            string now = "";
            now = now + "Shift " + IntToString(TableOfLR1[x][strToid[sch]].v);
            Print(i, len, now);
            staState[topState++] = (TableOfLR1[x][strToid[sch]].v);
            staLabel[topLabel++] = (strToid[sch]);
            if(sch == "num"){
                while(textStr[i] >= '0' && textStr[i] <= '9') i++;
            }
            else i++;
        }
        else {
            if(TableOfLR1[x][strToid[sch]].tp == 'R'){
                int id = TableOfLR1[x][strToid[sch]].v;
                int num = GS[id].size() - 1;
                string now = "";
                now = now + "reduce by ";
                // GetGS(now, id);
                for(int j = 0; j < GS[id].size(); j++){
                    if(j == 0) {
                        now = now + ele[GS[id][j]].Name + "->";
                    }else {
                        now = now + ele[GS[id][j]].Name;
                    }
                }
                Print(i, len, now);
                for(int j = 1; j <= num; j++) topState--, topLabel--;
                x = staState[topState - 1];
                staLabel[topLabel++] = (GS[id][0]);
                staState[topState++] = (TableOfLR1[x][GS[id][0]].v);
                // PrintGS(id);
            }
            else if(TableOfLR1[x][strToid[sch]].tp == 'A') {
                string now = "";
                now = now + "ACC";
                Print(i, len, now);
                break;
            }
            else {
                Error();
                return;
            }
        }
    }while(true);
    /*
    while(!staLabel.empty()) {
        int x = staLabel.top();
        staLabel.pop();
        cout << ele[x].Name;
    }
    cout << endl;
    */
    
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

    CreateDFAOfLR1(); //构造识别该文法的所有活前缀的DFA

    CreateAnalysisTableOfLR1(); //构造该文法的LR1分析表
    PrintAnalysisTableOfLR1(); //输出该文法的LR1分析表

    while(true){
        memset(textStr, 0, sizeof(textStr));
        cout << "Please enter a text string, if you want to exit, please enter #:" << endl;
        scanf("%s", textStr);
        int len = strlen(textStr);
        if(len == 1 && textStr[0] == '#') break;

        AnalysisOfLR1();//LR1文法的分析
        
        printf("\nUsing LR1 grammar analysis for non recursive analysis is complete!\n\n\n");
        
        system("pause");
    }
    
}