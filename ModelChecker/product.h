#ifndef PRODUCT_H
#define PRODUCT_H
#define max_to_string 30
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<set>
#include<string>
#include<cstdlib>
#include "pnml_parse.h"
#include"SBA.h"
using namespace std;

/*********************************Global_functions***********************************/
bool judgeF(string s);            //判断是F还是c命题
int convert(RG rg, Petri ptnet, int i, int j);
int sumtoken(Petri ptnet, RG rg, string s, int statenum);         //计算库所的token的和 传过来的s是  p1,p2,p3,p4,  的形式 
int localBA(SBA ba, string val);                                                 //根据data  返回自动机 邻接表的下标 
int localRG(RG rg, string val);                                                 //根据data  返回可达图 邻接表的下标 
bool handleFLTLF(Petri ptnet, RG rg, string s, int statenum);  //处理F命题  
void handleLTLCstep(Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum);//传入前半部分和;后半部分和 ;命题  从{的后一个字符开始; handleLTLC的儿子 
bool handleLTLC(Petri ptnet, RG rg, string s, int statenum);   //处理C命题  
/*******************************Global_functions End**********************************/

typedef struct Product
{                                      //交自动机的状态节点 由 <RGname,BAname> 构成 
	string RGname;           //可达图节点名字 
	string BAname;           //自动机节点名字 
	int id;                          //节点的一个编号 
	//重载< 自定义排序规则 (set集合需要用到) 
	bool operator <(const Product &a)const
	{
		if (a.RGname != RGname)//若RGname不同 则按照RGname排序 
			return a.RGname < RGname;
		else                              //RGname相同的情况下 按照 BAname排序 
			return a.BAname < BAname;
	}
};

typedef struct 	T       //变迁关系   由两个交自动机的状态节点组成  
{
	Product s;             //变迁的起始节点 
	Product e;            //变迁的目标节点 
	//重载< 自定义排序规则
	bool operator <(const T &a)const
	{
		if (a.s.RGname != s.RGname)            //按照s.RGname排序 
			return a.s.RGname < s.RGname;

		else if (a.s.BAname != s.BAname)      //s.RGname相同 则按s.BAname排序 
			return a.s.BAname < s.BAname;

		else if (a.e.RGname != e.RGname)     //s.RGname s.BAname都相同 则按照e.RGname排序 
			return a.e.RGname < e.RGname;

		else if (a.e.BAname != e.BAname)      //下面都类似 
			return a.e.BAname < e.BAname;

		else if (a.s.id != s.id)
			return a.s.id < s.id;

		else return  a.e.id < e.id;
	}
};

typedef struct node   //存储交自动机的链式前向星
{
	int to;        //to是该点指向的终点 ;
	int next;     //next是第i条边同起点的下一条边的存储位置 
};

class Product_Automata
{
private:
	set<Product> status;      //交自动机状态集合<RGname,BAname> 
	set<T> transition;           //变迁集合 
	set<Product> initial_status; //初始状态集合
	set<Product> isaccept;       //可接受状态集合集合
	set<int> isAccept_id;           //可接受状态集合的id号 
public:
	void getProduct(Petri ptnet, RG rg, SBA ba);//生成交自动机
	void addisaccept(SBA ba, int i, Product n);//增加可接受状态 
	void addinitial_status(SBA ba,int i, int j, Product n);//增加初始状态 
	void addstatus(Product n);//增加交自动机的状态 
	void addtransition(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile);//增加交自动机的迁移状态 
	//void tarjan(int x);                  //dfs 寻找强连通分量  代表第几个点在处理。递归的是点。
	void ModelChecker(Petri ptnet, RG rg, SBA ba);
	void garbowbfs(int cur, int temper);

};
bool isLabel(Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe);       //vi是可达图的下标 sj是自动机的下标 
void add_edge(int x, int y);             //通过迁移关系 存储交自动机的图 


#endif
