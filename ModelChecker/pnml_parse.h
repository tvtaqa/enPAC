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

string itos(int n);             //������intת����string 

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
	bool sourceP; //false����Դ�ڵ��Ǳ�Ǩ��true����Դ�ڵ��ǿ���
	int targetNum = 0;
	int weight = 1;
};

//���������
typedef struct Place
{
	string name;
	int num = 0;
	int initialMarking = 0;
	//����Ϊ��������20181208
	bool sig = false;//�Ƿ�����Ҫ������������Ϊtrue
	int solution[max_place_num];//��Ҫ������Ӧ��P������
	bool visited = false;//��Ϊ����Ҫ��������ǰβ�����Ƿ񱻸�ֵΪ1��(�����Է������ʱ����)
};

//��Ǩ�����
typedef struct Trans
{
	string name;
	int num = 0;
};

//�ɴ�ͼ�еı߱�
typedef struct RGEdge {
	int t;                                                //����ߴ���ķ����ı�Ǩ���
	int target;                                         //�������Ŀ��ڵ��״̬���
	struct RGEdge *nextedge = NULL;   //�ñ�ָ�����һ����
}RGEdge, *PRGEdge;

//�ɴ�ͼ�еĶ����
typedef struct RGNode {
	string name;
	int m[max_place_num];                   //��״̬�洢�ı�ʶ��Ϣ
	bool flag = 0;                                  //��0����1
	int isfirable[max_tran_num] = { 0 };  //�Ӹñ�ʶ�������Է����ı�Ǩ����
	int enableNum = 0;                         //�ñ�ʶ���Է����ı�Ǩ��Ŀ
	rgstack inset;
	PRGEdge firstEdge = NULL;
}RGNode, *PRGNode;

int sum(int m[], int n);
class Petri
{
public:
	int m;   //��������ָ��
	int n;    //��Ǩ����ָ��
	int arcnum;  //������ָ��
	Place *place;
	Trans *transition;
	Arc *arc;
	double **A;//��������
	double **A1;//�������
	double **A2;//�������
public:
	Petri();
	void readPNML(char *filename);  //����Petri
	void getA();           //���ɹ�������
	void printA();         //��ӡ��������
	void printA1();       //��ӡ�������
	void printA2();       //��ӡ�������

	void UpperTriangularMatrix();//��������ת���������Ǿ���
	//void printUpperTriA();//��ӡ�����Ǿ���
	void exchange(double a[], double b[]); //����Ԫ�ػ���
	void sub(double a[], double b[], double k);//����:a[]=a[]-k*b[]

};

class RG
{
public:
	int node;
	int edge;
	RGNode *rgnode;
public:
	RG();		
	void ReachabilityTree(Petri ptnet);    //���ɿɴ�ͼ
	void ModifyMarking();
	void PrintGraph(Petri ptnet, ofstream &outfile);
	bool isBoundless(Petri ptnet, RGNode *curs);
};