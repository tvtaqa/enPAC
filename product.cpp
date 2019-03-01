
#include "product.h"
#define FILE_BUFFER_LENGTH 30000
#include <fstream>
#include <unistd.h>
#include <signal.h>

/******************************Global_fucntions*********************************/
bool Product_Automata::judgeF(string s)//判断是F还是c命题 
{
	int pos = s.find("<=");
	if (pos == string::npos)
	{
		return true;            //是F命题 
	}
	else return false;          //c命题 
}
int Product_Automata::convert( int i, int j)//rgnode[i].m[j]
{
	int x = 0;
	int sum1 = 0;
	int sum2 = 0;

	for (int ii = 0; ii < ptnet.m; ii++)
	{
		sum1 += ptnet.place[j].solution[ii] * rg.rgnode[0].m[ii];
		//cout << "ptnet.place["<<j<<"].solution["<<ii<<"]: " << ptnet.place[j].solution[ii]<<endl;
		//cout << "rg.rgnode[0].m[" << ii << "]: " << rg.rgnode[0].m[ii] << endl;
	}
	//cout << "sum1:" << sum1 << endl;
	for (int jj = 0; jj < ptnet.m; jj++)
	{
		if (jj == j)
			continue;
		else
			sum2 += ptnet.place[j].solution[jj] * rg.rgnode[i].m[jj];
	}
	//cout << "sum2:" << sum2 << endl;
	x = (sum1 - sum2) / ptnet.place[j].solution[j];

	return x;
}


int Product_Automata::sumtoken( string s, int statenum)//计算库所的token的和 传过来的s是  p1,p2,p3,p4,  的形式   
{
	int sum = 0;
	while (1)
	{
		int pos = s.find_first_of(",");
		if (pos == string::npos)
			break;//若没有找到 则说明没有了 
		string subs = s.substr(0, pos);     //取得一个p1 
		//	cout<<"   "<<subs<<" ";
		int idex = ptnet.placeid.at(subs);                 //得到库所的编号
		if (rg.rgnode[statenum].m[idex] != -100)
			sum += rg.rgnode[statenum].m[idex];
		else//若为-100 则需要转化  
		{
			int x;
			x = convert(statenum, idex);//statenum就是i   idex就是j
			//cout << "转化得到x: " << x << endl << endl;
			sum += x;

		}
			
		s = s.substr(pos + 1, s.length() - pos);          //将前面的用过的P1去除 从p2开始作为新的s串 
	}
	return sum;
}

bool Product_Automata::handleFLTLF( string s, int statenum)//处理F命题  
{
	int i, j;//while循环变量 
	if (s[0] == '!')//在原命题的基础上 再取非
	{

		s = s.substr(2, s.length() - 2);//去除 !, { 
		bool flag = true;             //初始设为true  
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)
				break;
			string subs = s.substr(0, pos);//取出一个t1 去和 变迁数组比较 得到标号 再去isfirable数组找 编号是否全部存在	
			
					int idex = ptnet.transid.at(subs);//得到了变迁的编号
					//再去isfirabe数组找是否有这个编号
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)//30是isfirable数组的大小 
					{
						if (idex == rg.rgnode[statenum].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = false;//isfirable数组中没找到 则设为false 并跳出循环 
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
		//全找到为true  反之为false
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

			
					int idex = ptnet.transid.at(subs);//得到了变迁的编号
					//再去isfirabe数组找是否有这个编号
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)
					{
						if (idex == rg.rgnode[statenum].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = true;//isfirable数组中没找到 
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
		//全找不到为false  
	}
}
void Product_Automata::handleLTLCstep(int &front_sum, int &latter_sum, string s, int statenum)
{
	if (s[0] == 't')//则前半部分为token-count形式  
	{
		int pos = s.find_first_of("<=");//定位到<=,取其前半部分  
		string s_tokon = s.substr(12, pos - 13);//去除 "token-count(" ")"  只剩p1,p2,    
		//cout<<" "<<s_tokon<<" ";
		front_sum = sumtoken(s_tokon, statenum);//计算token的合 

		//计算后半部分  
		s = s.substr(pos + 2, s.length() - pos - 2); //取得后半部分  若是常数 则后面是一个班常数加上一个'}'

		if (s[0] == 't')//若后半部分还是token-count 
		{
			string s_tokon = s.substr(12, s.length() - 14);//去除 token-count(
		//	cout<<" "<<s_tokon<<" ";
			latter_sum = sumtoken(s_tokon, statenum);//相同处理 

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
		s = s.substr(pos + 14, s.length() - pos - 16);
		//	 cout<<" "<<s<<" ";
		latter_sum = sumtoken(s, statenum);
	}
}
bool Product_Automata::handleLTLC(string s, int statenum)//处理C命题  
{
	int front_sum, latter_sum;//前半部分和 后半部分的合 
	if (s[0] == '!')//命题为非 再取非
	{
		//	cout<<" 非 ";
		s = s.substr(2, s.length() - 2);//去除 "!{" 
	//	cout<<s;
		handleLTLCstep(front_sum, latter_sum, s, statenum);
		//ofe << "前半部分和：" << front_sum << " 后半部分和：" << latter_sum << endl;
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
		handleLTLCstep( front_sum, latter_sum, s, statenum);
		//ofe << "前半部分和：" << front_sum << " 后半部分和：" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return false;
		else
			return true;
	}
}
/****************************Global_fucntions End*******************************/
bool result;
bool timeflag;//判断是否超时 

void  sig_handler(int num)
{
    //printf("time out .\n");
    timeflag=false;
  
}


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

void hashtable::insert(Product q)
{
	int idex = hashfunction(q.BAname_id+q.RGname_id);

	table[idex].push_back(q);
}

int hashtable::hashfunction(int   s)
{
	return (s) % hash_table_num;
}

int hashtable::search(Product  n)
{
	int idex = hashfunction(n.BAname_id+n.RGname_id);
	for (int i = 0; i < table[idex].size(); ++i)
	{
		if ( table[idex][i].BAname_id == n.BAname_id 
			&& table[idex][i].RGname_id == n.RGname_id)
		{
			//cout<< table[idex][i].first<<endl;
			return 1;
		}
	}
	//cout << "find not" << s << "  in hash" << endl;
	return -1;
}


void Product_Automata::getProduct()//传入可达图和自动机 
{
	
	/*ofstream outfile("getproduct.txt", ios::out);
	ofstream ofe("islabel.txt", ios::out);
	outfile << endl;
	outfile << "输出BA节点上的label：\n";
	for (int i = 0; i < ba.svex_num; i++)
	{
		outfile << "节点" << i << "的label:" << ba.svertics[i].label << endl;
		outfile << "节点" << i << "的isaccept:" << ba.svertics[i].isAccept << endl;
		outfile << "节点" << i << "的isinitial:" << ba.svertics[i].isInitial << endl;


	}
	outfile << endl;*/
	
	addinitial_status();   
	//cout << "初始节点个数：" << initial_status.size()<<endl;
	vector<Product>::iterator it_P;       //定义前向迭代器 
	for (it_P = initial_status.begin(); it_P != initial_status.end(); it_P++)
	{
		//cout << "chushi:" << endl;
		dfs1((*it_P));
		
		if (!result || !timeflag)break;
	}
}



void Product_Automata::dfs1(Product q)
{
	if(!timeflag)return ;
	h.insert(q);//加入哈希表
	stack.push_back(q);//加入dfs1 的 stack中
	SArcNode *pba = new SArcNode;        //指向Ba自动机的邻接表的节点 (非头节点) 
	PRGEdge prg = new RGEdge;          //指向可达图邻接表的节点
	pba = ba.svertics[q.BAname_id].firstarc;
	prg = rg.rgnode[q.RGname_id].firstEdge;//prg指向idex接下去的第一个节点 


	while (pba != NULL)
	{
		prg = rg.rgnode[q.RGname_id].firstEdge;//prg指向idex接下去的第一个节点 
		while (prg != NULL)
		{
			if (isLabel(prg->target, pba->adjvex))//petri网 可达图 自动机 可达图序号 自动机序号
			{
				Product qs;                                   //生成一个状态节点N 
				qs.BAname_id = pba->adjvex;
				qs.RGname_id = prg->target;
				if (h.search(qs) != 1)//
				{
					dfs1(qs);
				}
				if (ba.svertics[q.BAname_id].isAccept)//p可接受
				{
					dfs2(qs);
				}
				if (!result || !timeflag)
					return;
			}
			prg = prg->nextedge;
		}
		pba = pba->nextarc;
	}
	stack.pop_back();//删除最后一个  stack栈顶出站;

}


void Product_Automata::dfs2(Product q)
{
	if(!timeflag)return ;
	flag.push_back(q);//将传入的q进行标记
	SArcNode *pba = new SArcNode;        //指向Ba自动机的邻接表的节点 (非头节点) 
	PRGEdge prg = new RGEdge;          //指向可达图邻接表的节点
	pba = ba.svertics[q.BAname_id].firstarc;
	prg = rg.rgnode[q.RGname_id].firstEdge;//prg指向idex接下去的第一个节点 
	while (pba != NULL )
	{
		prg = rg.rgnode[q.RGname_id].firstEdge;
		while (prg != NULL)
		{
			bool stackfound = false;
			bool flagfound = false;
			if (isLabel(prg->target, pba->adjvex))
			{
				vector<Product>::iterator stackit;
				vector<Product>::iterator flagit;
				Product qs;                                   //生成一个状态节点N 
				
				qs.BAname_id = pba->adjvex;
				qs.RGname_id = prg->target;

				//判断是否在stack栈中
				for (stackit = stack.begin(); stackit != stack.end(); stackit++)
				{
					if (stackit->BAname_id == qs.BAname_id && stackit->RGname_id == qs.RGname_id)
					{
						stackfound = true;
					}
				}
				//判断是否在flag中
				for (flagit = flag.begin(); flagit != flag.end(); flagit++)
				{
					if (flagit->BAname_id == qs.BAname_id && flagit->RGname_id == qs.RGname_id)
					{
						flagfound = true;
					}
				}
				if (stackfound)
				{
					result = false;
					return;
				}
				//qs未被标记
				else if (!flagfound)
				{
					dfs2(qs);
				}

			}
			prg = prg->nextedge;
			
		}
		pba = pba->nextarc;
		
	}


}





void  Product_Automata::addinitial_status()//增加初始状态 
{

	for (int i = 0; i < ba.svex_num; i++)//遍历自动机每一个节点  i是自动机 j是可达图
		if (ba.svertics[i].isInitial)//若i代表的可达图节点  与 j代表的可达图节点  都是根节点（初始状态节点） 
		{
			int j = 0;
			if (isLabel(j, i))
			{
				Product N;                                   //生成一个状态节点N 
				
				N.BAname_id = i;
				N.RGname_id = j;                            //交自动机的序号+1 
				initial_status.push_back(N);
				//status.push_back(N);
			}

		}
}


bool Product_Automata::isLabel( int vi, int sj)//vi是可达图的下标 sj是自动机的下标 
{

	string str = ba.svertics[sj].label;//保存自动机节点上命题 
	if ("" == str)
		return true;//若命题为空  
	bool  mark = false; //mark标记是否有命题成立 若一直是false 那交就是非false
	while (1)
	{
		int pos = str.find_first_of("&&");
		//ofe << "RG序号:" << vi << " BA序号:" << sj << " BA的label:" << str << endl;
		if (pos == string::npos)//处理最后一个 
		{
			//cout<<str;
			if (judgeF(str))//F命题 
			{
				//cout<<"F命题"; 
				mark = handleFLTLF(str, vi);
				if (mark == true)
				{
					break;//跳出while循环  
				}
			}
			else {//c命题 
				//cout<<"c命题";
				mark = handleLTLC( str, vi);
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
			mark = handleFLTLF(subprop, vi);
			if (mark == true)//即找到一个命题 成立 跳出循环   
			{
				break;
			}
		}
		else//c命题 
		{
			//	cout<<"c命题"; 
			mark = handleLTLC(subprop, vi);
			if (mark == true)//即找到一个命题 成立 跳出循环   
			{
				break;
			}
		}
		//cout<<endl;
		str = str.substr(pos + 2, str.length() - pos -2);
	}

	if (mark == true)//有一个命题成立了
	{

		//ofe << "交为false" << endl << endl;
		return false;
	}

	else //所有命题都不成立 mark一直是false， 满足了非false  
	{
		//ofe << "交为非false 生成节点" << endl << endl;
		return true;
	}

}



Product_Automata::Product_Automata()
{
	
}


void Product_Automata::ModelChecker(Petri p, RG g, SBA s, string propertyid,int timeleft)
{
	signal(SIGALRM, sig_handler);
	//cout<<"each_lefttime:"<<timeleft<<endl;
    alarm(timeleft);
	ptnet = p;
	rg = g;
	ba = s;
	timeflag=true; 
	result = true;//搜索结果开始默认为true
	getProduct();
	//cout<<"ok \n";
	
	string re;
	if(timeflag)
	{
		if(result)
		{
		re="TRUE";
		}
		else 
		{
		re="FALSE";
		}
		
	cout<<"FORMULA "+propertyid+" "+re+" TECHNIQUES SEQUENTIAL EXPLICIT TOPOLOGICAL ABSTRACTIONS LTLNFBA"<<endl;
		
	}
	else cout<<"FORMULA "+propertyid+" "+"CANNOT_COMPUTE"<<endl;
	cout<<"======================================================"<<endl;
	alarm(0);
	//initate_array();
	/*delete[] DFN;
	delete[] LOW;
	delete[] stack;
	delete[] visit;
	delete[] edge;
	delete[] heads;*/
	return;
}


Product_Automata::~Product_Automata()
{

}


