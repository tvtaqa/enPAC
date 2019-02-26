#include"product.h"
#include"xml2ltl.h"
#include<fstream>
#include<time.h>
#include <unistd.h>
#include <signal.h>
using namespace std;

void  handler(int num)
{
    printf("rg time out .\n");
    exit(0);
  
}

int main(int argc, char *argv[])
{
	cout<<"================================================="<<endl;
	cout<<"=====This is our tool-enPAC for the MCC'2019====="<<endl;
	cout<<"================================================="<<endl;
	if(argc!=2)
	{
		cout<<"Wrong number of arguments!"<<endl;
		return 0;
	}
	string str = argv[1];
	signal(SIGALRM, handler);
    alarm(3600);
	
	//XML½âÎö£º
	char Ffile[50] = "LTLFireability.xml";
	char Cfile[50] = "LTLCardinality.xml";
	convertf(Ffile);
	convertc(Cfile);

	//StateSpace
	ofstream outfile("reachablility.txt", ios::out);
	ofstream outprint("ptnet.txt", ios::out);
	char petrifile[20] = "model.pnml";
	Petri ptnet;
	RG graph;
	
	ptnet.readPNML(petrifile);
	ptnet.getA();
	
	ptnet.UpperTriangularMatrix();

	//ptnet.printA(outprint);

	
	graph.ReachabilityTree(ptnet);
	graph.standardOutput(ptnet);
	graph.PrintGraph(ptnet, outfile);
	char ltlfile[25];
	if(str == "StateSpace"){
		return 0;
	}
	else if(str == "LTLFireability"){
		strcpy(ltlfile, "LTLFireability.txt");
	}
	else if(str == "LTLCardinality") {
		strcpy(ltlfile, "LTLCardinality.txt");
	}
				//LTLFireability check£º
	string S, propertyid;	
	char form[10000];
	ifstream read(ltlfile, ios::in);
	if (!read) { cout << "error!"; getchar(); exit(-1); }
	int timeleft=alarm(0)/16;//Ê£ÏÂµÄÊ±¼ä
	cout<<"timeleft:"<<timeleft*16<<endl;
	while (getline(read, propertyid, ':'))
	{
		
		cout << propertyid << ':';
		getline(read, S);
		strcpy(form, S.c_str());
		cout << endl;
		Lexer lex(form, S.length());
		//syntax analysis
		Syntax_Tree ST;
		formula_stack Ustack;
		ST.reverse_polish(lex);
		ST.build_tree();
		ST.simplify_LTL(ST.root->left);
		ST.negconvert(ST.root->left, Ustack);
		TGBA Tgba;
		Tgba.CreatTGBA(Ustack, ST.root->left);
		Tgba.SimplifyStates();
		TBA tba;
		tba.CreatTBA(Tgba, Ustack);
		string filename = propertyid + ".txt";
		tba.PrintBuchi(filename);
		SBA sba;
		sba.CreatSBA(tba);
		sba.Simplify();
		sba.Compress();
		//cout << "begin:ON-THE-FLY" << endl;
		Product_Automata *product=new Product_Automata;
		product->ModelChecker(ptnet, graph, sba,propertyid,timeleft);
		delete product;
	}
	

	return 0;
}
int main0()
{
	int timeleft;
	ifstream read("a.txt", ios::in);
	//ofstream outfile("reachablility.txt", ios::out);
	if (!read) { cout << "error!"; getchar(); exit(-1); }
	char form[100], p;
	int i = 0;
	string filename = "SBA.txt";
	char petrifile[20] = "model.pnml";

	Petri ptnet;
	RG graph;
	Product_Automata product;

	ptnet.readPNML(petrifile);
	ptnet.getA();
	ptnet.UpperTriangularMatrix();
	graph.ReachabilityTree(ptnet);
	graph.standardOutput(ptnet);
	//graph.PrintGraph(ptnet, outfile);

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

	ST.reverse_polish(lex);
	ST.build_tree();
	cout << "The syntax tree of unsimplified formula£º" << endl;
	ST.print_syntax_tree(ST.root, 0);
	ST.simplify_LTL(ST.root->left);
	cout << endl;
	cout << "The syntax tree of simplified formula£º" << endl;
	ST.print_syntax_tree(ST.root, 0);
	ST.negconvert(ST.root->left, Ustack);
	cout << endl;
	cout << "The converted formula£º" << endl;
	cout << ST.root->left->formula << endl;
	cout << endl;
	cout << "The subformulas of LTL whose main operator is \'U\'£º" << endl;
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
	product.ModelChecker(ptnet, graph, sba,"0",timeleft);
}
