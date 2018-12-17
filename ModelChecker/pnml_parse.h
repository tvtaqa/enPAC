#pragma once
#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <cstring>
#include<string>
#include<vector>
#include <stdlib.h>
#include<fstream>
using namespace std;

#define max_place_num 1000
#define max_tran_num 1000
#define max_node_num 2500
#define max_arc_num 1000
#define max_incoming 100

string itos(int n);             //函数将int转换成string 

class rgstack
{
public:
	int incoming[max_incoming];
	int pointer;
public:
	rgstack();
	bool isEmpty();
	void push(int num);
	void pop(int &num);
};
class rgqueue
{
private:
	int *priset;
	int head;
	int tail;
	int elemnum;
public:
	rgqueue();
	void EnQueue(int num);
	void DeQueue(int &num);
	bool isEmpty();
};

struct Arc {
	string id;
	string source;
	string target;
	int sourceNum = 0;
	bool sourceP; //false代表源节点是变迁，true代表源节点是库所
	int targetNum = 0;
	int weight = 1;
};

//库所顶点表
typedef struct Place
{
	string name;
	int num = 0;
	int initialMarking = 0;
	//以下为新增属性20181208
	bool sig = false;//是否是重要库所，若是则为true
	int solution[max_place_num];//重要库所对应的P不变量
	bool visited = false;//若为不重要库所，当前尾变量是否被赋值为1过(解线性方程组的时候用)
};

//变迁顶点表
typedef struct Trans
{
	string name;
	int num = 0;
};

//可达图中的边表
typedef struct RGEdge {
	int t;                                                //有向边代表的发生的变迁编号
	int target;                                         //该有向边目标节点的状态编号
	struct RGEdge *nextedge = NULL;   //该边指向的下一条边
}RGEdge, *PRGEdge;

//可达图中的顶点表
typedef struct RGNode {
	string name;
	int m[max_place_num];                   //该状态存储的标识信息
	bool flag = 0;                                  //新0，旧1
	int isfirable[max_tran_num] = { 0 };  //从该标识出发可以发生的变迁名字
	int enableNum = 0;                         //该标识可以发生的变迁数目
	rgstack inset;
	PRGEdge firstEdge = NULL;
}RGNode, *PRGNode;

int sum(int m[], int n);
class Petri
{
public:
	int m;   //库所数组指针
	int n;    //变迁数组指针
	int arcnum;  //弧数组指针
	Place *place;
	Trans *transition;
	Arc *arc;
	double **A;//关联矩阵
	double **A1;//输出矩阵
	double **A2;//输入矩阵
public:
	Petri();
	void readPNML(char *filename);  //解析Petri
	void getA();           //生成关联矩阵
	void printA();         //打印关联矩阵
	void printA1();       //打印输出矩阵
	void printA2();       //打印输入矩阵

	void UpperTriangularMatrix();//关联矩阵转化成上三角矩阵
	//void printUpperTriA();//打印上三角矩阵
	void exchange(double a[], double b[]); //两行元素互换
	void sub(double a[], double b[], double k);//减法:a[]=a[]-k*b[]

};

class RG
{
public:
	int node;
	int edge;
	RGNode *rgnode;
public:
	RG();		
	void ReachabilityTree(Petri ptnet);    //生成可达图
	void ModifyMarking();
	void PrintGraph(Petri ptnet, ofstream &outfile);
	bool isBoundless(Petri ptnet, RGNode *curs);
};