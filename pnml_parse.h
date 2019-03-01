#pragma once
#include <stdio.h>
#include <iostream>
#include<string>
#include<vector>
#include <stdlib.h>
#include<fstream>
#include<map>
#include "tinyxml.h"

using namespace std;

#define max_place_num 5000
#define max_tran_num 5000
#define max_node_num 10000

#define max_incoming 100



class rgstack
{
public:
	vector<int> incoming;
//	int incoming[max_incoming];
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

struct Place
{
	string name;
	int num = 0;
//	map<string, int> placeid;
	int initialMarking = 0;
	bool sig = false;//是否是重要库所，若是则为true
	int solution[max_place_num];//重要库所对应的P不变量
//	vector<int> solution;
	bool visited = false;//若为不重要库所，当前尾变量是否被赋值为1过(解线性方程组的时候用)
};

struct Transition
{
	string name;
	int num = 0;
//	map<string, int> transid;
};

//可达图中的边表
typedef struct RGEdge {
	int t;                                                //有向边代表的发生的变迁编号
	int target;                                         //该有向边目标节点的状态编号
	struct RGEdge *nextedge = NULL;   //该边指向的下一条边
}RGEdge, *PRGEdge;

string itos(int n);             //函数将int转换成string 
int sum(int m[], int n);

class Petri
{
public:
	int m;   //库所数组指针
	int n;    //变迁数组指针
	int arcnum;  //弧数组指针

	vector<Place> place;
	vector<Transition> transition;
	vector<Arc> arc;

	map<string, int> placeid;
	map<string, int> transid;

	vector<vector<double>> A;
	vector<vector<double>> A1;
	vector<vector<double>> A2;
public:
	Petri();
	void addPlace(Place p);
	void addTransition(Transition t);
	void addArc(Arc a);
	void readPNML(char *filename);  //解析Petri
	void getA();           //生成关联矩阵
	void printA();         //打印关联矩阵
	void printA1();
	void UpperTriangularMatrix();//关联矩阵转化成上三角矩阵
	void exchange(double a[], double b[]); //两行元素互换
	void sub(double a[], double b[], double k);//减法:a[]=a[]-k*b[]
	
};
//可达图中的顶点表
typedef struct RGNode {
	string name;
	int m[max_place_num];
//	vector<int> m;                   //该状态存储的标识信息
	bool flag = 0;                                  //新0，旧1
	int isfirable[max_tran_num];
//	vector<int> isfirable;  //从该标识出发可以发生的变迁名字
	int enableNum = 0;                         //该标识可以发生的变迁数目
	int X[max_tran_num] = {-1};//每个标识marking对应一个顽固集X
	int markX = 0;//X数组的大小
	rgstack inset;
	PRGEdge firstEdge = NULL;
}RGNode, *PRGNode;

class RG
{
public:
	int node;
	int edge;
	vector<RGNode> rgnode;
//	Queue unprocessed;
	rgqueue unprocessed;
//	vector<Queue> unprocessed;
//	Queue <int> unprocessed;
//	vector<rgqueue> unprocessed;
//	rgqueue unprocessed[100];
public:
	RG();		
	void addRGNode(RGNode rg);
//	void newReachabilityTree(Petri ptnet);    //生成可达图
	void PrintGraph(Petri ptnet, ofstream &outfile);//可达图生成到文件中
	void standardOutput(Petri ptnet);
//	bool exist(int X[],int num);
//	void addToUnprocessed(Petri ptnet, RGNode marking);
//	void ReducingSuccessorGenerator(Petri ptnet,RGNode &marking);
	void ReachabilityTree(Petri ptnet);
};