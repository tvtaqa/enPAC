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
bool judgeF(string s);            //�ж���F����c����
int convert(RG rg, Petri ptnet, int i, int j);
int sumtoken(Petri ptnet, RG rg, string s, int statenum);         //���������token�ĺ� ��������s��  p1,p2,p3,p4,  ����ʽ 
int localBA(SBA ba, string val);                                                 //����data  �����Զ��� �ڽӱ���±� 
int localRG(RG rg, string val);                                                 //����data  ���ؿɴ�ͼ �ڽӱ���±� 
bool handleFLTLF(Petri ptnet, RG rg, string s, int statenum);  //����F����  
void handleLTLCstep(Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum);//����ǰ�벿�ֺ�;��벿�ֺ� ;����  ��{�ĺ�һ���ַ���ʼ; handleLTLC�Ķ��� 
bool handleLTLC(Petri ptnet, RG rg, string s, int statenum);   //����C����  
/*******************************Global_functions End**********************************/

typedef struct Product
{                                      //���Զ�����״̬�ڵ� �� <RGname,BAname> ���� 
	string RGname;           //�ɴ�ͼ�ڵ����� 
	string BAname;           //�Զ����ڵ����� 
	int id;                          //�ڵ��һ����� 
	//����< �Զ���������� (set������Ҫ�õ�) 
	bool operator <(const Product &a)const
	{
		if (a.RGname != RGname)//��RGname��ͬ ����RGname���� 
			return a.RGname < RGname;
		else                              //RGname��ͬ������� ���� BAname���� 
			return a.BAname < BAname;
	}
};

typedef struct 	T       //��Ǩ��ϵ   ���������Զ�����״̬�ڵ����  
{
	Product s;             //��Ǩ����ʼ�ڵ� 
	Product e;            //��Ǩ��Ŀ��ڵ� 
	//����< �Զ����������
	bool operator <(const T &a)const
	{
		if (a.s.RGname != s.RGname)            //����s.RGname���� 
			return a.s.RGname < s.RGname;

		else if (a.s.BAname != s.BAname)      //s.RGname��ͬ ��s.BAname���� 
			return a.s.BAname < s.BAname;

		else if (a.e.RGname != e.RGname)     //s.RGname s.BAname����ͬ ����e.RGname���� 
			return a.e.RGname < e.RGname;

		else if (a.e.BAname != e.BAname)      //���涼���� 
			return a.e.BAname < e.BAname;

		else if (a.s.id != s.id)
			return a.s.id < s.id;

		else return  a.e.id < e.id;
	}
};

typedef struct node   //�洢���Զ�������ʽǰ����
{
	int to;        //to�Ǹõ�ָ����յ� ;
	int next;     //next�ǵ�i����ͬ������һ���ߵĴ洢λ�� 
};

class Product_Automata
{
private:
	set<Product> status;      //���Զ���״̬����<RGname,BAname> 
	set<T> transition;           //��Ǩ���� 
	set<Product> initial_status; //��ʼ״̬����
	set<Product> isaccept;       //�ɽ���״̬���ϼ���
	set<int> isAccept_id;           //�ɽ���״̬���ϵ�id�� 
public:
	void getProduct(Petri ptnet, RG rg, SBA ba);//���ɽ��Զ���
	void addisaccept(SBA ba, int i, Product n);//���ӿɽ���״̬ 
	void addinitial_status(SBA ba,int i, int j, Product n);//���ӳ�ʼ״̬ 
	void addstatus(Product n);//���ӽ��Զ�����״̬ 
	void addtransition(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile);//���ӽ��Զ�����Ǩ��״̬ 
	//void tarjan(int x);                  //dfs Ѱ��ǿ��ͨ����  ����ڼ������ڴ����ݹ���ǵ㡣
	void ModelChecker(Petri ptnet, RG rg, SBA ba);
	void garbowbfs(int cur, int temper);

};
bool isLabel(Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe);       //vi�ǿɴ�ͼ���±� sj���Զ������±� 
void add_edge(int x, int y);             //ͨ��Ǩ�ƹ�ϵ �洢���Զ�����ͼ 


#endif
