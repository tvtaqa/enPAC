
#ifndef PRODUCT_H
#define PRODUCT_H

#define max_to_string 30
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<string>
#include<cstdlib>
#include "pnml_parse.h"
#include"SBA.h"
#define max_array_num 10000
#define hash_table_num 100000
using namespace std;

/*********************************Global_functions***********************************/
bool judgeF(string s);            //判断是F还是c命题
int convert(RG rg, Petri ptnet, int i, int j);
int sumtoken(Petri ptnet, RG rg, string s, int statenum);         //计算库所的token的和 传过来的s是  p1,p2,p3,p4,  的形式                                                //根据data  返回可达图 邻接表的下标 
bool handleFLTLF(Petri ptnet, RG rg, string s, int statenum);  //处理F命题  
void handleLTLCstep(Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum);//传入前半部分和;后半部分和 ;命题  从{的后一个字符开始; handleLTLC的儿子 
bool handleLTLC(Petri ptnet, RG rg, string s, int statenum);   //处理C命题  
/*******************************Global_functions End**********************************/

typedef struct Product
{                                      //交自动机的状态节点 由 <RGname,BAname> 构成 
	
	int RGname_id;
	int BAname_id;
	
	//重载< 自定义排序规则 (set集合需要用到) 
	/*bool operator <(const Product &a)const
	{
		if (a.RGname != RGname)//若RGname不同 则按照RGname排序 
			return a.RGname < RGname;
		else                              //RGname相同的情况下 按照 BAname排序 
			return a.BAname < BAname;
	}*/
};



class hashtable
{
	//out是对ostream对象的引用， 在该对象上将产生输出， ostream为非const， 因为写入到流会改变流的状态
	//friend ostream& operator<<(ostream &out, const hashtable& h);
public:
	//向哈希表中插入元素
	void insert(Product n);
	//计算要插入的位置
	int hashfunction(int  s);

	//搜索hash表中的元素
	int search(Product n);
private:
	vector<Product> table[hash_table_num]; //一共二十个格子
};



class Product_Automata
{
private:
//	vector<Product> status;      //交自动机状态集合<RGname,BAname> 
//	vector<T> transition;           //变迁集合 
	vector<Product> initial_status; //初始状态集合
//	vector<Product> isaccept;       //可接受状态集合集合
//	vector<int> isAccept_id;           //可接受状态集合的id号
	vector<Product>  stack;
	vector<Product>  flag;
	hashtable h;
	Petri ptnet;
	RG rg;
	SBA ba;
	
	bool result;
public:
	Product_Automata();
	void getProduct();//生成交自动机
	void addinitial_status();//增加初始状态 
	
	void ModelChecker(Petri ptnet, RG rg, SBA ba,string propertyid,int timeleft);
	void dfs1(Product q);
	void dfs2(Product q);
	//void initate_array(Petri ptnet,RG graph,SBA sba);
	bool isLabel(int vi, int sj);       //vi是可达图的下标 sj是自动机的下标 
	~Product_Automata();
	//void garbowbfs(int cur, int temper);
	bool judgeF(string s);            //判断是F还是c命题
	int convert( int i, int j);
	int sumtoken(string s, int statenum);         //计算库所的token的和 传过来的s是  p1,p2,p3,p4,  的形式                                                //根据data  返回可达图 邻接表的下标 
	bool handleFLTLF(string s, int statenum);  //处理F命题  
	void handleLTLCstep(int &front_sum, int &latter_sum, string s, int statenum);//传入前半部分和;后半部分和 ;命题  从{的后一个字符开始; handleLTLC的儿子 
	bool handleLTLC( string s, int statenum);   //处理C命题  

};

#endif // !_HASHTABLE_h


