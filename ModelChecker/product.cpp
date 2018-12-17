#include "product.h"
#include <fstream>

/******************************Global_fucntions*********************************/
bool judgeF(string s)//判断是F还是c命题 
{
	int pos = s.find("<=");
	if (pos == string::npos)
	{
		return true;            //是F命题 
	}
	else return false;          //c命题 
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


int sumtoken(Petri ptnet, RG rg, string s, int statenum)//计算库所的token的和 传过来的s是  p1,p2,p3,p4,  的形式   
{
	int sum = 0;
	while (1)
	{
		int pos = s.find_first_of(",");
		if (pos == string::npos)
			break;//若没有找到 则说明没有了 
		string subs = s.substr(0, pos);     //取得一个p1 
		//	cout<<"   "<<subs<<" ";
		for (int i = 0; i < ptnet.m; i++)      //去库所的的数组中寻找对应名字的库所 得到下标 
		{
			if (subs == ptnet.place[i].name)
			{
				int idex = ptnet.place[i].num;                 //得到库所的编号
				if (rg.rgnode[statenum].m[idex] != -100)
					sum += rg.rgnode[statenum].m[idex];


				else//若为-100 则需要转化  
				{
					int x;

					x = convert(rg, ptnet, statenum, idex);//statenum就是i   idex就是j
					cout << "转化得到x: " << x << endl << endl;
					sum += x;

				}
				break;
			}
		}
		s = s.substr(pos + 1, s.length() - pos);          //将前面的用过的P1去除 从p2开始作为新的s串 
	}
	return sum;
}
int localBA(SBA ba, string val)//根据data  返回自动机 邻接表的下标 
{
	for (int i = 0; i < ba.svex_num; i++)
	{
		if (itos(ba.svertics[i].data) == val)
			return i;
	}
	return -1;//找不到则返回-1 
}
int localRG(RG rg, string val)//根据data  返回可达图 邻接表的下标 
{
	for (int i = 0; i < rg.node; i++)
	{
		if (val == rg.rgnode[i].name)
			return i;
		//将int数组专成string 再进行比较 
		/*string str="";
		for(int k=0;k<5;k++)//将int数组专成string
		{
		str+=to_String(  rg.rgnode[i].m[k]   );
		}

		if(str==val)
		return i;*/
	}
	return -1;//找不到则返回-1 
}
bool handleFLTLF(Petri ptnet, RG rg, string s, int statenum)//处理F命题  
{
	int i, j;//while循环变量 
	if (s[0] == '!')//在原命题的基础上 再取非
	{

		s = s.substr(2, s.length() - 1);//去除 !, { 
		bool flag = true;             //初始设为true  
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)
				break;
			string subs = s.substr(0, pos);//取出一个t1 去和 变迁数组比较 得到标号 再去isfirable数组找 编号是否全部存在	
			for (i = 0; i < ptnet.n; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].num;//得到了变迁的编号
					//再去isfirabe数组找是否有这个编号
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)//30是isfirable数组的大小 
					{
						if (idex == rg.rgnode[statenum].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = false;//isfirable数组中没找到 则设为true 并跳出循环 
					}
					break; //名字对应的肯定只有一个 不重名  
				}
			}
			if (flag == false)
			{
				//若t1找不到对应的  下一个t2也不用看了 直接跳出while 
				break;
			}

			//cout<<subs<<endl;
			s = s.substr(pos + 1, s.length() - pos);
		}
		return flag;
	}
	else
	{
		s = s.substr(1, s.length() - 1);//去除{ 
		bool flag = false;//初始状态为false  t1,t2,t3, 只要其中一个t找不到 则为true 即成立(跳出循环) 
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)break;
			string subs = s.substr(0, pos);//取出一个t1 去和 变迁数组比较 得到标号 再去isfirable数组找 编号是否全部存在
			//bool flag=true;//标记t1是否能找到 

			for (i = 0; i < ptnet.n; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].num;//得到了变迁的编号
					//再去isfirabe数组找是否有这个编号
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)
					{
						if (idex == rg.rgnode[i].isfirable[j])
						{
							break;
						}
					}
					if (j >= 30) {
						flag = true;//isfirable数组中没找到 
					}
					break; //名字对应的肯定只有一个 不重名  
				}
			}
			if (flag == true)
			{
				//若t1找不到对应的  下一个t2也不用看了 直接跳出while 
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
	if (s[0] == 't')//则前半部分为token-count形式  
	{
		int pos = s.find_first_of("<=");//定位到<=,取其前半部分  
		string s_tokon = s.substr(12, pos - 13);//去除 "token-count(" ")"  只剩p1,p2,    
		//cout<<" "<<s_tokon<<" ";
		front_sum = sumtoken(ptnet, rg, s_tokon, statenum);//计算token的合 

		//计算后半部分  
		s = s.substr(pos + 2, s.length() - pos - 2); //取得后半部分  若是常数 则后面是一个班常数加上一个'}'

		if (s[0] == 't')//若后半部分还是token-count 
		{
			string s_tokon = s.substr(12, pos - 13);//去除 token-count(
		//	cout<<" "<<s_tokon<<" ";
			latter_sum = sumtoken(ptnet, rg, s_tokon, statenum);//相同处理 

		}
		else//后半部分是常数 
		{
			s = s.substr(0, s.length() - 1);//去除}  只剩一个常数 
			latter_sum = atoi(s.c_str());
			//	 cout<<latter_sum;
		}

	}
	else//前半部分是常数 那后面一定是token-count形式 
	{
		//处理前半部分  现有的串 是从常数开始的 
		int pos = s.find_first_of("<=");//定位到<=,取其前半部分
		string num = s.substr(0, pos);//取出常数 
		front_sum = atoi(num.c_str());
		//	 cout<< " "<<front_sum<<" ";

			 //处理后半部分
		s = s.substr(pos + 14, s.length() - pos - 15);
		//	 cout<<" "<<s<<" ";
		latter_sum = sumtoken(ptnet, rg, s, statenum);
	}
}
bool handleLTLC(Petri ptnet, RG rg, string s, int statenum, ofstream &ofe)//处理C命题  
{
	int front_sum, latter_sum;//前半部分和 后半部分的合 
	if (s[0] == '!')//命题为非 再取非
	{
		//	cout<<" 非 ";
		s = s.substr(2, s.length() - 2);//去除 "!{" 
	//	cout<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "前半部分和：" << front_sum << " 后半部分和：" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return true;
		else
			return false;

	}
	else
	{
		//	cout<<" 正 ";
		s = s.substr(1, s.length() - 1);//去除 "{"
	//	cout<<" "<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "前半部分和：" << front_sum << " 后半部分和：" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return false;
		else
			return true;
	}
}
/****************************Global_fucntions End*******************************/

set<Product>::iterator it_P;       //定义前向迭代器 
set<T>::iterator it_T;                  //定义前向迭代器 
set<int>::iterator it;                   //定义前向迭代器 
int cont;                                  //交自动机的序号
node edge[3001];
//int DFN[3001], LOW[3001];//链式前向星存储需要的两个数组;DFN默认为0，即未访问过 
int stk[3001], stk2[3001], belg[3001], low[3001], heads[3001], tot;;
bool result;
//int stack[3001], heads[3001], visit[3001], cnt, tot, index;
int n, m, cn, cm, scc, lay;
//stack[]存储入栈的节点 head[i]数组存储i节点指向的第一个节点 
//visit[]标记是否入栈  	

string to_String(int n)//函数将int转换成string 
{
	int m = n;
	char s[max_to_string];
	char ss[max_to_string];
	int i = 0, j = 0;
	if (n < 0)// 处理负数
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

void Product_Automata::getProduct(Petri ptnet, RG rg, SBA ba)//传入可达图和自动机 
{
	ofstream outfile("getproduct.txt", ios::out);
	ofstream ofe("islabel.txt", ios::out);
	outfile << endl;
	outfile << "输出BA节点上的label：\n";
	for (int i = 0; i < ba.svex_num; i++)
	{
		outfile << "节点" << i << "的label:" << ba.svertics[i].label << endl;
		outfile << "节点" << i << "的isaccept:" << ba.svertics[i].isAccept << endl;
		outfile << "节点" << i << "的isinitial:" << ba.svertics[i].isInitial << endl;


	}
	outfile << endl;
	cont = 1;
	for (int i = 0; i < rg.node; i++)//遍历可达图每一个节点
	{
		for (int j = 0; j < ba.svex_num; j++)//遍历自动机每一个节点
		{
			if (isLabel(ptnet, rg, ba, i, j, ofe))                                  //命题交集非空   
			{

				Product N;                                   //生成一个状态节点N 
				N.BAname = itos(ba.svertics[j].data);       //自动机 
				N.RGname = rg.rgnode[i].name;    //可达图
				N.id = cont++;                              //交自动机的序号+1 
				addstatus(N);                               //增加交自动机的状态 
				outfile << "输出添加的迁移关系：\n";
				addtransition(rg, ba, i, j, N, outfile);                  //增加迁移状态 
				addinitial_status(ba,i, j, N);                  //增加初始状态 
				addisaccept(ba, j, N);                         //增加可接受状态 
			}
		}
	}
}

void Product_Automata::addstatus(Product n)//增加交自动机的状态 
{
	status.insert(n);//新增状态节点 加入状态集合中 
}

void Product_Automata::addtransition(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile)    //增加交自动机的迁移状态 
{

	SArcNode *pba;        //指向Ba自动机的邻接表的节点 (非头节点) 
	PRGEdge prg;          //指向可达图邻接表的节点
	bool flag_v, flag_s;    //标记是否找到对应的边 

	outfile << "N的RGname:" << n.RGname << " N的BAname:" << n.BAname << endl;
	//在已有的乘积中 遍历寻找符合要求的某个节点 使其与n组合,加入变迁集合 
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		outfile << "遍历得到的RGname:" << (*it_P).RGname << " BAname:" << (*it_P).BAname << endl;
		//根据传入的变量来定位(返回下标)，临时变量idex保存返回的下标
		int idex = localRG(rg, (*it_P).RGname);

		//以idex为起点去尝试 	 
		prg = rg.rgnode[idex].firstEdge;//prg指向idex接下去的第一个节点 

		while (prg != NULL)
		{
			if (prg->target == i) //在可达图中找到 
			{
				flag_v = true;
				break;
			}
			else
				prg = prg->nextedge;
		}
		if (prg == NULL)
			flag_v = false;//未找到 


		//用传过来的j  以及 status集合遍历得到的BAname  判断其是否是自动机的迁移，即在邻接表中判断 
		idex = localBA(ba, (*it_P).BAname);//临时变量idex保存返回的下标

		//以idex为起点  去寻找是否存在t 
		pba = ba.svertics[idex].firstarc;
		//cout << "ba.vertics[idex]:"<< ba.vertics[idex].data  << endl;
		while (pba != NULL)
		{
			if (pba->adjvex == j) //在自动机邻接表中找到 
			{
				flag_s = true;
				break;
			}
			pba = pba->nextarc;
		}
		if (pba == NULL)
			flag_s = false;//未找到 


		if (flag_v && flag_s) //若都符合 则交自动机的迁移关系集合 新增 
		{
			T t;//临时变量 
			t.s = (*it_P);
			t.e = n;
			transition.insert(t);//增加迁移关系 
			outfile << " 上述满足条件,添加到T集合中,起点的id:" << t.s.id << " 终点的id:" << t.e.id << endl;
		}
	}
	outfile << "\n\n\n";
}

void  Product_Automata::addinitial_status(SBA ba,int i, int j, Product n)//增加初始状态 
{

	if (ba.svertics[i].isInitial==true && j == 0)//若i代表的可达图节点  与 j代表的可达图节点  都是根节点（初始状态节点） 
	{
		initial_status.insert(n);
	}
}

void  Product_Automata::addisaccept(SBA ba, int i, Product n)//增加可接受状态 
{
	//可达图的每一个节点默认是可接受状态 ，所以只需要考虑自动机部分  	
	bool flag = ba.svertics[i].isAccept;//判断其是否是可接受状态 
	if (flag)
	{
		isaccept.insert(n);
	}
}

bool isLabel(Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe)//vi是可达图的下标 sj是自动机的下标 
{

	string str = ba.svertics[sj].label;//保存自动机节点上命题 
	if ("" == str)
		return true;//若命题为空  
	bool  mark = false; //mark标记是否有命题成立 若一直是false 那交就是非false
	while (1)
	{
		int pos = str.find_first_of("&&");
		ofe << "RG序号:" << vi << " BA序号:" << sj << " BA的label:" << str << endl;
		if (pos == string::npos)//处理最后一个 
		{
			//cout<<str;
			if (judgeF(str))//F命题 
			{
				//cout<<"F命题"; 
				mark = handleFLTLF(ptnet, rg, str, vi);
				if (mark == true)
				{
					break;//跳出while循环  
				}
			}
			else {//c命题 
				//cout<<"c命题";
				mark = handleLTLC(ptnet, rg, str, vi, ofe);
				if (mark == true)
				{
					break;//跳出while循环  
				}
			}
			break;//因为是最后一个 跳出循环 
		}

		string subprop = str.substr(0, pos);//取出一个命题 
		//cout<<subprop<<" ";

		if (judgeF(subprop))//F命题 
		{
			//	cout<<"F命题"; 
			mark = handleFLTLF(ptnet, rg, subprop, vi);
			if (mark == true)//即找到一个命题 成立 跳出循环   
			{
				break;
			}
		}
		else//c命题 
		{
			//	cout<<"c命题"; 
			mark = handleLTLC(ptnet, rg, subprop, vi, ofe);
			if (mark == true)//即找到一个命题 成立 跳出循环   
			{
				break;
			}
		}
		//cout<<endl;
		str = str.substr(pos + 2, str.length() - pos);
	}

	if (mark == true)//有一个命题成立了
	{

		ofe << "交为false" << endl << endl;
		return false;
	}

	else //所有命题都不成立 mark一直是false， 满足了非false  
	{
		ofe << "交为非false 生成节点" << endl << endl;
		return true;
	}

}

void add_edge(int u, int v)//通过迁移关系 存储交自动机的图 
{
	//链式前向星存储 


	edge[tot].to = v;

	edge[tot].next = heads[u];

	heads[u] = tot++;
}

void Product_Automata::garbowbfs(int cur, int temper)//代表第几个点在处理。递归的是点。
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

		bool flag = false;//标记节点是否是可接受状态 
		cm--;

		scc++;
		

		   //scc是序号 
		do {

			//printf("%d ",stk[cn]);
			cout << stk[cn] << " ";
			it = isAccept_id.find(stk[cn]);

			if (it != isAccept_id.end())//在可接受状态集合中找到此id
			{
				flag = true;
			}
			belg[stk[cn]] = scc;

		} while (stk[cn--] != cur);
		if (flag )
		{
			cout << "包含可接受状态" << endl;
			result = false;

		}

		cout << endl;
		
	}
	return;

}
void Product_Automata::ModelChecker(Petri ptnet, RG rg, SBA ba)
{
	memset(heads, -1, sizeof(heads));//初始化 将heads[]数组初始化为-1 
	result = true;//搜索结果开始默认为true
	getProduct(ptnet, rg, ba);
	int x, y;//读入边x->y 
	//生成一个set集合 里面是可接受状态的id 
	int ct = 0;
	for (it_P = isaccept.begin(); it_P != isaccept.end(); it_P++)
	{
		cout << "id:" << (*it_P).id << " BAname:" << (*it_P).BAname << " RGname:" << (*it_P).RGname << endl;
		ct++;
		isAccept_id.insert((*it_P).id);
	}
	cout << "可接受状态的个数:" << ct << endl;
	ct = 0;
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		// cout << (*it_P).id<<endl;
		ct++;
	}
	cout << "所有状态的个数:" << ct << endl;
	//cout << "边的关系集合：\n";
	for (it_T = transition.begin(); it_T != transition.end(); it_T++)
	{
		x = (*it_T).s.id;
		y = (*it_T).e.id;

		cout << "<" << x << "," << y << ">" << endl;
		add_edge(x, y);//调用add()函数 生成链式前向星存储  
	}
	//遍历初始状态的集合 ，从初始状态出发 

	scc = lay = 0;

	memset(belg, 0, sizeof(belg));

	memset(low, 0, sizeof(low));


	cout << "初始状态集合：";
	for (it_P = initial_status.begin(); it_P != initial_status.end(); it_P++)
	{
		int t = (*it_P).id;
		cout << t << endl;
		if (!low[t])
			garbowbfs(t, lay);//当这个点没有访问过，就从此点开始。防止交自动机有向图没走完
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
