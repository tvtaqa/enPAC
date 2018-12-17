#include "product.h"
#include <fstream>

/******************************Global_fucntions*********************************/
bool judgeF(string s)//�ж���F����c���� 
{
	int pos = s.find("<=");
	if (pos == string::npos)
	{
		return true;            //��F���� 
	}
	else return false;          //c���� 
}
int convert(RG rg, Petri ptnet, int i, int j)//rgnode[i].m[j]
{
	int x = 0;
	int sum1 = 0;
	int sum2 = 0;

	for (int ii = 0; ii < ptnet.m; ii++)
	{
		sum1 += ptnet.place[j].solution[ii] * rg.rgnode[0].m[ii];
		//cout << "ptnet.place["<<j<<"].solution["<<ii<<"]: " << ptnet.place[j].solution[ii]<<endl;
		cout << "rg.rgnode[0].m[" << ii << "]: " << rg.rgnode[0].m[ii] << endl;
	}
	cout << "sum1:" << sum1 << endl;
	for (int jj = 0; jj < ptnet.m; jj++)
	{
		if (jj == j)
			continue;
		else
			sum2 += ptnet.place[j].solution[jj] * rg.rgnode[i].m[jj];
	}
	cout << "sum2:" << sum2 << endl;
	x = (sum1 - sum2) / ptnet.place[j].solution[j];

	return x;
}


int sumtoken(Petri ptnet, RG rg, string s, int statenum)//���������token�ĺ� ��������s��  p1,p2,p3,p4,  ����ʽ   
{
	int sum = 0;
	while (1)
	{
		int pos = s.find_first_of(",");
		if (pos == string::npos)
			break;//��û���ҵ� ��˵��û���� 
		string subs = s.substr(0, pos);     //ȡ��һ��p1 
		//	cout<<"   "<<subs<<" ";
		for (int i = 0; i < ptnet.m; i++)      //ȥ�����ĵ�������Ѱ�Ҷ�Ӧ���ֵĿ��� �õ��±� 
		{
			if (subs == ptnet.place[i].name)
			{
				int idex = ptnet.place[i].num;                 //�õ������ı��
				if (rg.rgnode[statenum].m[idex] != -100)
					sum += rg.rgnode[statenum].m[idex];


				else//��Ϊ-100 ����Ҫת��  
				{
					int x;

					x = convert(rg, ptnet, statenum, idex);//statenum����i   idex����j
					cout << "ת���õ�x: " << x << endl << endl;
					sum += x;

				}
				break;
			}
		}
		s = s.substr(pos + 1, s.length() - pos);          //��ǰ����ù���P1ȥ�� ��p2��ʼ��Ϊ�µ�s�� 
	}
	return sum;
}
int localBA(SBA ba, string val)//����data  �����Զ��� �ڽӱ���±� 
{
	for (int i = 0; i < ba.svex_num; i++)
	{
		if (itos(ba.svertics[i].data) == val)
			return i;
	}
	return -1;//�Ҳ����򷵻�-1 
}
int localRG(RG rg, string val)//����data  ���ؿɴ�ͼ �ڽӱ���±� 
{
	for (int i = 0; i < rg.node; i++)
	{
		if (val == rg.rgnode[i].name)
			return i;
		//��int����ר��string �ٽ��бȽ� 
		/*string str="";
		for(int k=0;k<5;k++)//��int����ר��string
		{
		str+=to_String(  rg.rgnode[i].m[k]   );
		}

		if(str==val)
		return i;*/
	}
	return -1;//�Ҳ����򷵻�-1 
}
bool handleFLTLF(Petri ptnet, RG rg, string s, int statenum)//����F����  
{
	int i, j;//whileѭ������ 
	if (s[0] == '!')//��ԭ����Ļ����� ��ȡ��
	{

		s = s.substr(2, s.length() - 1);//ȥ�� !, { 
		bool flag = true;             //��ʼ��Ϊtrue  
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)
				break;
			string subs = s.substr(0, pos);//ȡ��һ��t1 ȥ�� ��Ǩ����Ƚ� �õ���� ��ȥisfirable������ ����Ƿ�ȫ������	
			for (i = 0; i < ptnet.n; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].num;//�õ��˱�Ǩ�ı��
					//��ȥisfirabe�������Ƿ���������
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)//30��isfirable����Ĵ�С 
					{
						if (idex == rg.rgnode[statenum].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = false;//isfirable������û�ҵ� ����Ϊtrue ������ѭ�� 
					}
					break; //���ֶ�Ӧ�Ŀ϶�ֻ��һ�� ������  
				}
			}
			if (flag == false)
			{
				//��t1�Ҳ�����Ӧ��  ��һ��t2Ҳ���ÿ��� ֱ������while 
				break;
			}

			//cout<<subs<<endl;
			s = s.substr(pos + 1, s.length() - pos);
		}
		return flag;
	}
	else
	{
		s = s.substr(1, s.length() - 1);//ȥ��{ 
		bool flag = false;//��ʼ״̬Ϊfalse  t1,t2,t3, ֻҪ����һ��t�Ҳ��� ��Ϊtrue ������(����ѭ��) 
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)break;
			string subs = s.substr(0, pos);//ȡ��һ��t1 ȥ�� ��Ǩ����Ƚ� �õ���� ��ȥisfirable������ ����Ƿ�ȫ������
			//bool flag=true;//���t1�Ƿ����ҵ� 

			for (i = 0; i < ptnet.n; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].num;//�õ��˱�Ǩ�ı��
					//��ȥisfirabe�������Ƿ���������
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)
					{
						if (idex == rg.rgnode[i].isfirable[j])
						{
							break;
						}
					}
					if (j >= 30) {
						flag = true;//isfirable������û�ҵ� 
					}
					break; //���ֶ�Ӧ�Ŀ϶�ֻ��һ�� ������  
				}
			}
			if (flag == true)
			{
				//��t1�Ҳ�����Ӧ��  ��һ��t2Ҳ���ÿ��� ֱ������while 
				break;
			}
			//cout<<subs<<endl;
			s = s.substr(pos + 1, s.length() - pos);
		}
		return flag;
	}
}
void handleLTLCstep(Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum)
{
	if (s[0] == 't')//��ǰ�벿��Ϊtoken-count��ʽ  
	{
		int pos = s.find_first_of("<=");//��λ��<=,ȡ��ǰ�벿��  
		string s_tokon = s.substr(12, pos - 13);//ȥ�� "token-count(" ")"  ֻʣp1,p2,    
		//cout<<" "<<s_tokon<<" ";
		front_sum = sumtoken(ptnet, rg, s_tokon, statenum);//����token�ĺ� 

		//�����벿��  
		s = s.substr(pos + 2, s.length() - pos - 2); //ȡ�ú�벿��  ���ǳ��� �������һ���ೣ������һ��'}'

		if (s[0] == 't')//����벿�ֻ���token-count 
		{
			string s_tokon = s.substr(12, pos - 13);//ȥ�� token-count(
		//	cout<<" "<<s_tokon<<" ";
			latter_sum = sumtoken(ptnet, rg, s_tokon, statenum);//��ͬ���� 

		}
		else//��벿���ǳ��� 
		{
			s = s.substr(0, s.length() - 1);//ȥ��}  ֻʣһ������ 
			latter_sum = atoi(s.c_str());
			//	 cout<<latter_sum;
		}

	}
	else//ǰ�벿���ǳ��� �Ǻ���һ����token-count��ʽ 
	{
		//����ǰ�벿��  ���еĴ� �Ǵӳ�����ʼ�� 
		int pos = s.find_first_of("<=");//��λ��<=,ȡ��ǰ�벿��
		string num = s.substr(0, pos);//ȡ������ 
		front_sum = atoi(num.c_str());
		//	 cout<< " "<<front_sum<<" ";

			 //�����벿��
		s = s.substr(pos + 14, s.length() - pos - 15);
		//	 cout<<" "<<s<<" ";
		latter_sum = sumtoken(ptnet, rg, s, statenum);
	}
}
bool handleLTLC(Petri ptnet, RG rg, string s, int statenum, ofstream &ofe)//����C����  
{
	int front_sum, latter_sum;//ǰ�벿�ֺ� ��벿�ֵĺ� 
	if (s[0] == '!')//����Ϊ�� ��ȡ��
	{
		//	cout<<" �� ";
		s = s.substr(2, s.length() - 2);//ȥ�� "!{" 
	//	cout<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "ǰ�벿�ֺͣ�" << front_sum << " ��벿�ֺͣ�" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return true;
		else
			return false;

	}
	else
	{
		//	cout<<" �� ";
		s = s.substr(1, s.length() - 1);//ȥ�� "{"
	//	cout<<" "<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "ǰ�벿�ֺͣ�" << front_sum << " ��벿�ֺͣ�" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return false;
		else
			return true;
	}
}
/****************************Global_fucntions End*******************************/

set<Product>::iterator it_P;       //����ǰ������� 
set<T>::iterator it_T;                  //����ǰ������� 
set<int>::iterator it;                   //����ǰ������� 
int cont;                                  //���Զ��������
node edge[3001];
//int DFN[3001], LOW[3001];//��ʽǰ���Ǵ洢��Ҫ����������;DFNĬ��Ϊ0����δ���ʹ� 
int stk[3001], stk2[3001], belg[3001], low[3001], heads[3001], tot;;
bool result;
//int stack[3001], heads[3001], visit[3001], cnt, tot, index;
int n, m, cn, cm, scc, lay;
//stack[]�洢��ջ�Ľڵ� head[i]����洢i�ڵ�ָ��ĵ�һ���ڵ� 
//visit[]����Ƿ���ջ  	

string to_String(int n)//������intת����string 
{
	int m = n;
	char s[max_to_string];
	char ss[max_to_string];
	int i = 0, j = 0;
	if (n < 0)// ������
	{
		m = 0 - m;
		j = 1;
		ss[0] = '-';
	}
	while (m > 0)
	{
		s[i++] = m % 10 + '0';
		m /= 10;
	}
	s[i] = '\0';
	i = i - 1;
	while (i >= 0)
	{
		ss[j++] = s[i--];
	}
	ss[j] = '\0';
	return ss;
}

void Product_Automata::getProduct(Petri ptnet, RG rg, SBA ba)//����ɴ�ͼ���Զ��� 
{
	ofstream outfile("getproduct.txt", ios::out);
	ofstream ofe("islabel.txt", ios::out);
	outfile << endl;
	outfile << "���BA�ڵ��ϵ�label��\n";
	for (int i = 0; i < ba.svex_num; i++)
	{
		outfile << "�ڵ�" << i << "��label:" << ba.svertics[i].label << endl;
		outfile << "�ڵ�" << i << "��isaccept:" << ba.svertics[i].isAccept << endl;
		outfile << "�ڵ�" << i << "��isinitial:" << ba.svertics[i].isInitial << endl;


	}
	outfile << endl;
	cont = 1;
	for (int i = 0; i < rg.node; i++)//�����ɴ�ͼÿһ���ڵ�
	{
		for (int j = 0; j < ba.svex_num; j++)//�����Զ���ÿһ���ڵ�
		{
			if (isLabel(ptnet, rg, ba, i, j, ofe))                                  //���⽻���ǿ�   
			{

				Product N;                                   //����һ��״̬�ڵ�N 
				N.BAname = itos(ba.svertics[j].data);       //�Զ��� 
				N.RGname = rg.rgnode[i].name;    //�ɴ�ͼ
				N.id = cont++;                              //���Զ��������+1 
				addstatus(N);                               //���ӽ��Զ�����״̬ 
				outfile << "�����ӵ�Ǩ�ƹ�ϵ��\n";
				addtransition(rg, ba, i, j, N, outfile);                  //����Ǩ��״̬ 
				addinitial_status(ba,i, j, N);                  //���ӳ�ʼ״̬ 
				addisaccept(ba, j, N);                         //���ӿɽ���״̬ 
			}
		}
	}
}

void Product_Automata::addstatus(Product n)//���ӽ��Զ�����״̬ 
{
	status.insert(n);//����״̬�ڵ� ����״̬������ 
}

void Product_Automata::addtransition(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile)    //���ӽ��Զ�����Ǩ��״̬ 
{

	SArcNode *pba;        //ָ��Ba�Զ������ڽӱ�Ľڵ� (��ͷ�ڵ�) 
	PRGEdge prg;          //ָ��ɴ�ͼ�ڽӱ�Ľڵ�
	bool flag_v, flag_s;    //����Ƿ��ҵ���Ӧ�ı� 

	outfile << "N��RGname:" << n.RGname << " N��BAname:" << n.BAname << endl;
	//�����еĳ˻��� ����Ѱ�ҷ���Ҫ���ĳ���ڵ� ʹ����n���,�����Ǩ���� 
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		outfile << "�����õ���RGname:" << (*it_P).RGname << " BAname:" << (*it_P).BAname << endl;
		//���ݴ���ı�������λ(�����±�)����ʱ����idex���淵�ص��±�
		int idex = localRG(rg, (*it_P).RGname);

		//��idexΪ���ȥ���� 	 
		prg = rg.rgnode[idex].firstEdge;//prgָ��idex����ȥ�ĵ�һ���ڵ� 

		while (prg != NULL)
		{
			if (prg->target == i) //�ڿɴ�ͼ���ҵ� 
			{
				flag_v = true;
				break;
			}
			else
				prg = prg->nextedge;
		}
		if (prg == NULL)
			flag_v = false;//δ�ҵ� 


		//�ô�������j  �Լ� status���ϱ����õ���BAname  �ж����Ƿ����Զ�����Ǩ�ƣ������ڽӱ����ж� 
		idex = localBA(ba, (*it_P).BAname);//��ʱ����idex���淵�ص��±�

		//��idexΪ���  ȥѰ���Ƿ����t 
		pba = ba.svertics[idex].firstarc;
		//cout << "ba.vertics[idex]:"<< ba.vertics[idex].data  << endl;
		while (pba != NULL)
		{
			if (pba->adjvex == j) //���Զ����ڽӱ����ҵ� 
			{
				flag_s = true;
				break;
			}
			pba = pba->nextarc;
		}
		if (pba == NULL)
			flag_s = false;//δ�ҵ� 


		if (flag_v && flag_s) //�������� ���Զ�����Ǩ�ƹ�ϵ���� ���� 
		{
			T t;//��ʱ���� 
			t.s = (*it_P);
			t.e = n;
			transition.insert(t);//����Ǩ�ƹ�ϵ 
			outfile << " ������������,��ӵ�T������,����id:" << t.s.id << " �յ��id:" << t.e.id << endl;
		}
	}
	outfile << "\n\n\n";
}

void  Product_Automata::addinitial_status(SBA ba,int i, int j, Product n)//���ӳ�ʼ״̬ 
{

	if (ba.svertics[i].isInitial==true && j == 0)//��i����Ŀɴ�ͼ�ڵ�  �� j����Ŀɴ�ͼ�ڵ�  ���Ǹ��ڵ㣨��ʼ״̬�ڵ㣩 
	{
		initial_status.insert(n);
	}
}

void  Product_Automata::addisaccept(SBA ba, int i, Product n)//���ӿɽ���״̬ 
{
	//�ɴ�ͼ��ÿһ���ڵ�Ĭ���ǿɽ���״̬ ������ֻ��Ҫ�����Զ�������  	
	bool flag = ba.svertics[i].isAccept;//�ж����Ƿ��ǿɽ���״̬ 
	if (flag)
	{
		isaccept.insert(n);
	}
}

bool isLabel(Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe)//vi�ǿɴ�ͼ���±� sj���Զ������±� 
{

	string str = ba.svertics[sj].label;//�����Զ����ڵ������� 
	if ("" == str)
		return true;//������Ϊ��  
	bool  mark = false; //mark����Ƿ���������� ��һֱ��false �ǽ����Ƿ�false
	while (1)
	{
		int pos = str.find_first_of("&&");
		ofe << "RG���:" << vi << " BA���:" << sj << " BA��label:" << str << endl;
		if (pos == string::npos)//�������һ�� 
		{
			//cout<<str;
			if (judgeF(str))//F���� 
			{
				//cout<<"F����"; 
				mark = handleFLTLF(ptnet, rg, str, vi);
				if (mark == true)
				{
					break;//����whileѭ��  
				}
			}
			else {//c���� 
				//cout<<"c����";
				mark = handleLTLC(ptnet, rg, str, vi, ofe);
				if (mark == true)
				{
					break;//����whileѭ��  
				}
			}
			break;//��Ϊ�����һ�� ����ѭ�� 
		}

		string subprop = str.substr(0, pos);//ȡ��һ������ 
		//cout<<subprop<<" ";

		if (judgeF(subprop))//F���� 
		{
			//	cout<<"F����"; 
			mark = handleFLTLF(ptnet, rg, subprop, vi);
			if (mark == true)//���ҵ�һ������ ���� ����ѭ��   
			{
				break;
			}
		}
		else//c���� 
		{
			//	cout<<"c����"; 
			mark = handleLTLC(ptnet, rg, subprop, vi, ofe);
			if (mark == true)//���ҵ�һ������ ���� ����ѭ��   
			{
				break;
			}
		}
		//cout<<endl;
		str = str.substr(pos + 2, str.length() - pos);
	}

	if (mark == true)//��һ�����������
	{

		ofe << "��Ϊfalse" << endl << endl;
		return false;
	}

	else //�������ⶼ������ markһֱ��false�� �����˷�false  
	{
		ofe << "��Ϊ��false ���ɽڵ�" << endl << endl;
		return true;
	}

}

void add_edge(int u, int v)//ͨ��Ǩ�ƹ�ϵ �洢���Զ�����ͼ 
{
	//��ʽǰ���Ǵ洢 


	edge[tot].to = v;

	edge[tot].next = heads[u];

	heads[u] = tot++;
}

void Product_Automata::garbowbfs(int cur, int temper)//����ڼ������ڴ����ݹ���ǵ㡣
{
	 if (!result)
	 {
		 return;
	 }
	stk[++cn] = cur;

	stk2[++cm] = cur;

	low[cur] = ++lay;

	for (int i = heads[cur]; ~i; i = edge[i].next) {

		int v = edge[i].to;

		if (!low[v]) {

			garbowbfs(v, lay);

		}
		else if (!belg[v]) {

			while (low[stk2[cm]] > low[v]) {

				cm--;

			}

		}

	}

	if (stk2[cm] == cur && result) 
	{

		bool flag = false;//��ǽڵ��Ƿ��ǿɽ���״̬ 
		cm--;

		scc++;
		

		   //scc����� 
		do {

			//printf("%d ",stk[cn]);
			cout << stk[cn] << " ";
			it = isAccept_id.find(stk[cn]);

			if (it != isAccept_id.end())//�ڿɽ���״̬�������ҵ���id
			{
				flag = true;
			}
			belg[stk[cn]] = scc;

		} while (stk[cn--] != cur);
		if (flag )
		{
			cout << "�����ɽ���״̬" << endl;
			result = false;

		}

		cout << endl;
		
	}
	return;

}
void Product_Automata::ModelChecker(Petri ptnet, RG rg, SBA ba)
{
	memset(heads, -1, sizeof(heads));//��ʼ�� ��heads[]�����ʼ��Ϊ-1 
	result = true;//���������ʼĬ��Ϊtrue
	getProduct(ptnet, rg, ba);
	int x, y;//�����x->y 
	//����һ��set���� �����ǿɽ���״̬��id 
	int ct = 0;
	for (it_P = isaccept.begin(); it_P != isaccept.end(); it_P++)
	{
		cout << "id:" << (*it_P).id << " BAname:" << (*it_P).BAname << " RGname:" << (*it_P).RGname << endl;
		ct++;
		isAccept_id.insert((*it_P).id);
	}
	cout << "�ɽ���״̬�ĸ���:" << ct << endl;
	ct = 0;
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		// cout << (*it_P).id<<endl;
		ct++;
	}
	cout << "����״̬�ĸ���:" << ct << endl;
	//cout << "�ߵĹ�ϵ���ϣ�\n";
	for (it_T = transition.begin(); it_T != transition.end(); it_T++)
	{
		x = (*it_T).s.id;
		y = (*it_T).e.id;

		cout << "<" << x << "," << y << ">" << endl;
		add_edge(x, y);//����add()���� ������ʽǰ���Ǵ洢  
	}
	//������ʼ״̬�ļ��� ���ӳ�ʼ״̬���� 

	scc = lay = 0;

	memset(belg, 0, sizeof(belg));

	memset(low, 0, sizeof(low));


	cout << "��ʼ״̬���ϣ�";
	for (it_P = initial_status.begin(); it_P != initial_status.end(); it_P++)
	{
		int t = (*it_P).id;
		cout << t << endl;
		if (!low[t])
			garbowbfs(t, lay);//�������û�з��ʹ����ʹӴ˵㿪ʼ����ֹ���Զ�������ͼû����
	}
	/*for (int i = 1; i <= 24; i++)
	{
		if (!DFN[i])
			tarjan(i);
	}*/
	if (result)
	{
		cout << "\nresult:true\n";
	}
	else
		cout << "\nresult:false\n";

	return;
}
