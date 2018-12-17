#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"Lexer.h"
using namespace std;
#define max_set_num 100                //Old, New, Next中集合元素最大个数
#define max_incoming 100
#define max_nodes_num 100

class state_stack;
/******************************语法树用到的数据结构***********************************/
enum character_type {
	unary_op,                   //单目运算符
	binary_op,                  //双目运算符
	AP,                             //原子命题，包括其否定形式
};
enum privilege { unary=1, R_and_U=2, And=3, Or=4, Impli=5, Equ=6, root=7 };
typedef struct Syntax_Tree_Node
{
	string character;         //该节点代表的主逻辑运算符或原子命题
	string formula;            //该节点代表的公式
	character_type ctype;         //节点类型
	state_stack *cfnormal;
	bool isGetCF=false;
	Syntax_Tree_Node *left;
	Syntax_Tree_Node *right;
	Syntax_Tree_Node *parent;
}ST_Node, *STNode;

/*****************************逆波兰式用到的堆栈结构*******************************/
typedef struct stack_unit
{
	word w;
	privilege prilevel;
	stack_unit *next;
}LSNode;

class list_stack           //栈数据结构
{
private:
	LSNode *top;
public:
	list_stack();
	void push_ls(LSNode node);
	bool pop_ls(LSNode &node);
	void pop_ls();
	bool topelem(LSNode &node);
	bool istoplpar();
	bool isEmpty();
	int topprilevel();
};
class formula_stack
{
public:
	vector<STNode> loc;
public:
	void insert(STNode T);
	void pop(STNode &T);
	bool isEmpty();
	bool isExist(STNode T);
	bool decideLabel(STNode label);
	bool isHaveLabel(STNode T, STNode label);
	void print(string &str);
	int size()const;
};

/******************************CF_Tree Date_Structures********************************/
typedef struct CFTLeaf        //CF语法树叶子节点
{
	set<string> trscod;         //transition condition beita
	formula_stack xstate;       //next state
	ST_Node *relevant = NULL;
}CFTreeLeaf, *PCFTreeLeaf;

class state_stack
{
public:
	vector<CFTreeLeaf> states;
public:
	void insert(CFTreeLeaf cfl);
	void pop(CFTreeLeaf &cfl);
	bool isEmpty();
	void clear();
	int size();
	//bool isExist(CFTreeLeaf cfl);
};

typedef struct CF_Tree_Node  //CF语法树节点
{
	string character;
	state_stack ss;
	CF_Tree_Node *nleft;
	CF_Tree_Node *nright;
	CFTreeLeaf *lleft;
	CFTreeLeaf *lright;
	ST_Node *relevant = NULL;
}CFTreeNode, *PCFTreeNode;