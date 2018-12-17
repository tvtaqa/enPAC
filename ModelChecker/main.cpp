#include"product.h"
#include<fstream>
#include<time.h>
using namespace std;

int main()
{
	clock_t start, finish;
	double totaltime;
	start = clock();


	ifstream read("a.txt", ios::in);
	ofstream outfile("reachablility.txt", ios::out);
	if (!read) { cout << "error!"; getchar(); exit(-1); }
	char form[100], p;
	int i = 0;
	string filename = "SBA.txt";//
	char petrifile[20] = "model1234.pnml";//

	Petri ptnet;
	RG graph;
	Product_Automata product;

	ptnet.readPNML(petrifile);
	ptnet.getA();
	ptnet.UpperTriangularMatrix();
	graph.ReachabilityTree(ptnet);
	graph.PrintGraph(ptnet, outfile);

	while (true)
	{
		read.get(p);
		if (p == ' ' || p == '\t' || p == '\n')
			continue;
		if (p == '#')
			break;
		form[i++] = p;
		cout << form[i - 1];
	}
	cout << endl;

	Lexer lex(form, i);
	Syntax_Tree ST;
	formula_stack Ustack;
	CF_Tree CFT;// 
	ST.reverse_polish(lex);
	ST.build_tree();
	cout << "The syntax tree of unsimplified formula：" << endl;
	ST.print_syntax_tree(ST.root, 0);
	ST.simplify_LTL(ST.root->left);
	cout << endl;
	cout << "The syntax tree of simplified formula：" << endl;
	ST.print_syntax_tree(ST.root, 0);
	ST.negconvert(ST.root->left, Ustack);
	cout << endl;
	cout << "The converted formula：" << endl;
	cout << ST.root->left->formula << endl;
	cout << endl;
	cout << "The subformulas of LTL whose main operator is \'U\'：" << endl;
	vector<STNode>::iterator Uiter;
	for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
	{
		cout << (*Uiter)->formula << endl;
	}
	cout << endl;
	TGBA Tgba;
	Tgba.CreatTGBA(Ustack, ST.root->left);
	Tgba.SimplifyStates();
	cout << endl;
	TBA tba;
	tba.CreatTBA(Tgba, Ustack);

	tba.PrintBuchi(filename);
	cout << "Please the check file \"TBA.txt\". In this file you can the Buchi automata related to the LTL formula!";
	cout << endl;
	SBA sba;
	sba.CreatSBA(tba);
	sba.Simplify();
	sba.Compress();

	cout << "begin:product"<<endl;
	product.ModelChecker(ptnet, graph, sba);
	/*string S, propertyid;
	while (getline(read, propertyid,':'))
	{
		cout << propertyid << ':' ;
		getline(read, S);
		strcpy_s(form, S.c_str());
		cout << form << endl;
		Lexer lex(form, S.length());
		Syntax_Tree ST;
		Transition T;
		formula_stack Ustack;

		ST.reverse_polish(lex);
		//ST.print_reverse_polish();
		ST.build_tree();
		ST.print_syntax_tree(ST.root, 0);
		ST.simplify_LTL(ST.root->left);
		ST.print_syntax_tree(ST.root, 0);
		ST.negconvert(ST.root->left,Ustack);
		cout << ST.root->left->formula << endl;
		T.creat_graph(ST.root->left);
		BA buchi(T.nodes_num);
		buchi.CreatBA(T, Ustack);
		//buchi.State_Simplify(T);
		//buchi.Compress();
		propertyid = propertyid + ".txt";
		buchi.PrintBuchi(propertyid);
	}*/

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒" << endl;
	
	return 0;
}