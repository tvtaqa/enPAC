#include"pnml_parse.h"


string itos(int n)
{
	string val = to_string(n);
	return val;
}
int sum(int m[], int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += m[i];
	}
	return sum;
}

/**************************Data_Structure*********************************/
rgstack::rgstack()
{
	pointer = 0;
}
bool rgstack::isEmpty()
{
	return ((pointer > 0) ? false : true);
}
void rgstack::push(int num)
{
	if (pointer >= max_incoming)
	{
		cerr << "Stack overflow! Please set \"max_incoming\" bigger!" << endl;
		exit(-1);
	}
	else
		incoming[pointer++] = num;
}
void rgstack::pop(int &num)
{
	if (pointer > 0)
	{
		num = incoming[--pointer];
	}
}

rgqueue::rgqueue()
{
	head = tail = 0;
	elemnum = 0;
	priset = new int[max_node_num];
}
bool rgqueue::isEmpty()
{
	return ((head == tail) ? true : false);
}
void rgqueue::EnQueue(int num)
{
	if ((tail + 1) % max_node_num == head)
	{
		cerr << "Queue overflow! please set \"max_node_num\" bigger!" << endl;
		exit(-1);
	}
	else
	{
		priset[tail] = num;
		tail = (tail + 1) % max_node_num;
	}
}
void rgqueue::DeQueue(int &num)
{
	if (head != tail)
	{
		num = priset[head];
		head = (head + 1) % max_node_num;
	}
}
/*****************************Data_Structure_End********************************/


/***********************************************************/
Petri::Petri()
{
	m = 0;   //��������ָ��
	n = 0;    //��Ǩ����ָ��
	arcnum = 0;  //������ָ��
	place = new Place[max_place_num];
	transition = new Trans[max_tran_num];
	arc = new Arc[max_arc_num];
	A = new double *[max_tran_num];
	A1 = new double *[max_tran_num];
	A2 = new double *[max_tran_num];
	for (int i = 0; i < max_tran_num; i++)
	{
		A[i] = new double[max_place_num];
		A1[i] = new double[max_place_num];
		A2[i] = new double[max_place_num];
	}
	for (int i = 0; i < max_tran_num; i++)            //initial
	{
		for (int j = 0; j < max_place_num; j++)
		{
			A[i][j] = A1[i][j] = A2[i][j] = 0;
		}
	}
}

RG::RG()
{
	node = 0;
	edge = 0;
	rgnode = new RGNode[max_node_num];	
}

void Petri::readPNML(char *filename) {
	TiXmlDocument mydoc(filename);
	bool loadOK = mydoc.LoadFile();
	if (!loadOK) {
		cout << "could not load the test file" << endl;
		exit(1);
	}
	TiXmlElement *PetriElement = mydoc.RootElement();
	TiXmlElement *netElement = PetriElement->FirstChildElement();
	TiXmlElement *pageElement = netElement->FirstChildElement("page");
	for (TiXmlElement *petriElement = pageElement->FirstChildElement();//��һ����Ԫ��
		petriElement;
		petriElement = petriElement->NextSiblingElement())
	{
		if (petriElement->ValueTStr() == "place") {
			place[m].num = m;
			if (petriElement->FirstChildElement("initialMarking")) {
				TiXmlElement *initialMarkingElement = petriElement->FirstChildElement("initialMarking")->FirstChildElement("text");
				place[m].initialMarking = atoi(initialMarkingElement->GetText());
			}
			TiXmlElement *nameElement = petriElement->FirstChildElement("name")->FirstChildElement("text");
			place[m].name = nameElement->GetText();
			m++;
		}
		if (petriElement->ValueTStr() == "transition") {
			transition[n].num = n;
			TiXmlElement *tnameElement = petriElement->FirstChildElement("name")->FirstChildElement("text");
			transition[n].name = tnameElement->GetText();
			n++;
		}
		if (petriElement->ValueTStr() == "arc") {
			TiXmlAttribute *arcAttr = petriElement->FirstAttribute();
			arc[arcnum].id = arcAttr->Value();
			arcAttr = arcAttr->Next();
			arc[arcnum].source = arcAttr->Value();
			arcAttr = arcAttr->Next();
			arc[arcnum].target = arcAttr->Value();
			if (petriElement->FirstChildElement("inscription")) {
				TiXmlElement *weightElement = petriElement->FirstChildElement("inscription")->FirstChildElement("text");
				arc[arcnum].weight = atoi(weightElement->GetText());
			}
			arcnum++;
		}
	}
	for (int i = 0; i < arcnum; i++) {//����ÿһ����
		for (int j = 0; j < n; j++) {//�ͱ�Ǩ��
			if (arc[i].source==transition[j].name) {
				arc[i].sourceP = false;
				arc[i].sourceNum = transition[j].num;
			}
			if (arc[i].target==transition[j].name) {
				arc[i].targetNum = transition[j].num;
			}
		}
		for (int k = 0; k < m; k++) {//�Ϳ�����
			if (arc[i].source==place[k].name) {
				arc[i].sourceP = 1;
				arc[i].sourceNum = place[k].num;
			}
			if (arc[i].target==place[k].name) {
				arc[i].targetNum = place[k].num;
			}
		}
	}
}

void Petri::getA() {
	for (int i = 0; i < arcnum; i++) {
		if (arc[i].sourceP) {
			A[arc[i].targetNum][arc[i].sourceNum]=-arc[i].weight;
			A2[arc[i].targetNum][arc[i].sourceNum]= arc[i].weight;//�������
		}

		else{
			A[arc[i].sourceNum][arc[i].targetNum]= arc[i].weight;
			A1[arc[i].sourceNum][arc[i].targetNum]= arc[i].weight;//�������
		}
	}
}

void Petri::printA() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << A[i][j] << "  ";
		}
		cout << endl;
	}
}

void Petri::printA1() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << A1[i][j] << "  ";
		}
		cout << endl;
	}
}

void Petri::printA2() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << A2[i][j] << "  ";
		}
		cout << endl;
	}
}
void Petri::exchange(double a[], double b[]) {//����Ԫ�ػ���
	double *temp;
		temp = new double[m];
		for (int i = 0; i < m; i++) {
			temp[i] = a[i];
			a[i] = b[i];
			b[i] = temp[i];
		}
	}
void Petri::sub(double a[], double b[], double k) {//����:a[]=a[]-k*b[]
	for (int i = 0; i < m; i++) {
		a[i] = a[i] - k * b[i];
	}
}
void Petri::UpperTriangularMatrix() {
	double **B;
    B = new double*[n];
	for (int k = 0; k < n; k++)
	{
		B[k] = new double[m];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			B[i][j] = A[i][j];
		}
	}
	double multiple;//�洢������ϵ
	int site = 0;//site������ڱ������к�
	for (int i = 0; i < m; i++) {//�������е��С�i������ڱ������к�
		if (B[site][i] == 0) {//������е�һ��Ԫ��Ϊ0������ѡһ����Ϊ��ķ�����ͬʱ���л���
			for (int j = site + 1; j < n; j++) {
				if (B[j][i] != 0) {
					exchange(B[j], B[site]);//site i
					break;
				}
			}
		}
		if (B[site][i] == 0) {//���в��ò����Ѿ�ȫ��Ϊ��,�򻯼���һ��
			continue;
		}
		else {//��������Ҫ������Ԫ�أ�����������
			for (int j = site + 1; j < n; j++) {
				multiple = B[j][i] / B[site][i];
				if (!((multiple - (int)multiple) < 1e-15 || (multiple - (int)multiple) < -0.999999999999999|| 
					-(multiple - (int)multiple) < 1e-15 || -(multiple - (int)multiple) < -0.999999999999999) )
				{//multiple ��������
					for (int k = 0; k < m; k++) {
						B[j][k] = B[j][k] * B[site][i];
					}
					multiple = B[j][i] / B[site][i];
				}
				sub(B[j], B[site], multiple);
			}
			site++;
		}
	}
	int signum = 0;//�洢��Ҫ�����ĸ���
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (B[i][j] != 0) {
				//Ѱ�ҵ�һ����Ϊ��ķ�������ͷ����������j����Ҫ��
				place[j].sig = true;
				signum++;
				break;
			}
		}
	}
	//��ӡsignificant��������
	cout << "the num of significant places is:"<<signum << endl;
	/*��ӡ�����Ǿ���*/
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << B[i][j] << "  ";
		}
		cout << endl;
	}

	/*
	������Է�����
	*/
	int sum = 0;
	int x = 0;//��ǰ����õĽ�ĸ���

	int story;//�洢��ǰ��������Ӧ�Ŀ���λ��

	for (int equ = 0; equ <m-signum; equ++) {//һ��Ҫ�⼸�η���?β�����м����ͽ⼸��
		int flag = 0;
		int *solution;
		solution = new int[m];
		for (int i = 0; i < m; i++) {//һά�����ʼ��
			solution[i] = -1;
		}
		for (int i = 0; i < m; i++) {//ע��flag=0����λ��
			if (place[i].sig == false) {//����β����
				if (flag == 0 && place[i].visited == false) {//����ǵ�һ��β��������ֵΪ1
					solution[i] = 1;
					story = i;//����һ��β����i��ֵΪ1������β������ֵΪ0���������������P��������Ӧ�ڿ���story
					flag = 1;
					place[i].visited = true;
					continue;
				}
				else {//����β������ֵΪ0
					solution[i] = 0;
				}
			}
		}
		
		//�������1
		/*
		for (int mark = 0; mark < m; mark++) {
			cout << "solution" << solution[mark] << " ";
		}
		cout << endl;*/

		sum = 0;
		for (int j = signum - 1; j >= 0; j--) {//���¶��Ͻⷽ�̣�j��Ƿ�����������
			int ele;
			for (int find = 0; find < m; find++) {//���е�һ������Ԫ�أ���Ҫ���ı���
				if (B[j][find] != 0) {
					ele = find;
					break;
				}
			}
			for (int k = ele + 1; k < m; k++) {
				sum += B[j][k] * solution[k];
			}
			solution[ele] = -sum / B[j][ele];
		}



		/*
		sum = 0;
		for (int i = equ + 1; i < m; i++) {
			sum += B[equ][i] * solution[i];
		}
		if (B[equ][equ] == 0) {
			solution[equ] = 0;
		}
		else {
			solution[equ] = -sum / B[equ][equ];
		}*/

		/*
		for (int row = n > m ? m - 1: n - 1; row >= 0; row--) {//���������¶������
			sum = 0;
			if (solution[(m-1)-((n > m ? m - 1 : n - 1)-row)] != -1) {//solution�ķ����Ѿ���β������ֵ
				x++;
				continue;
			}
			else {
				for (int i = 1; i <= x; i++) {
					sum += solution[m - i] * B[row ][m - i];
				}
				if (B[row][m - 1 - x] == 0) {
					solution[row] = 0;
				}
				else {
					solution[row] = -sum / B[row][m - 1 - x];
				}
				x++;
			}
		}
		*/
		//�������2
		/*
		cout << "("<<equ+1<<")"<<"P-��������";
		for (int i = 0; i < m; i++) {
			cout << solution[i] << " ";
		}
		cout << endl;*/


		for (int i = 0; i < m; i++) {
			place[story].solution[i] = solution[i];
		}
	}
}
void RG::ReachabilityTree(Petri ptnet) {
	int M0[max_place_num] = { 0 };
	int M1[max_place_num] = { 0 };
	int M[max_place_num] = { 0 };
	//�ɴ�ͼ�ĵ�һ���ڵ㣨һ���ڵ㣺��ʼ��ʶ��
	for (int i = 0; i < ptnet.m; i++) {
		M0[i] = ptnet.place[i].initialMarking;
		rgnode[0].m[i] = M0[i];
	}
	rgnode[0].name = "M0";
	node++;
	//�жϽ���б��л�����û�б�ǵĽڵ�
	int newNode;
	bool exist;
	while (1) 
	{
		for (int i = 0; i < node; i++)
		{
			exist = false;
			if (rgnode[i].flag == 0) {//�ҵ���ʶΪ�µĽڵ�
				exist = true;
				newNode = i;
				break;
			}
		}
		if (!exist) {//�����ڱ�ʶΪ�µĽڵ�
			break;
		}
		//��ѡ��ʶΪ�µĽڵ�newNode����ΪM
		for (int i = 0; i < ptnet.m; i++) {
			M[i] = rgnode[newNode].m[i];
		}
		//�������ڵ�
		rgnode[newNode].flag = 1;

		//if ��M�����б�Ǩ�����ܷ���
		bool enable;
		int enableNumber = 0;
		for (int i = 0; i < ptnet.n; i++) 
		{//�������еı�Ǩ
			enable = true;
			for (int j = 0; j < ptnet.m; j++) 
			{
				if (M[j] < ptnet.A2[i][j]) 
				{//��Ǩti���ܷ���
					enable = false;
					break;
				}
			}
			//��Ǩ��t0��ʼ���
			if (enable) 
			{//��Ǩti���Է���
				rgnode[newNode].isfirable[enableNumber] = i;
				rgnode[newNode].enableNum++;
				enableNumber++;
			}
		}
		if (rgnode[newNode].enableNum == 0) 
		{//�����Ǩ���ܷ�������������ѭ��
			continue;
		}
		//����ÿ����M�¿ɷ����ı�Ǩ
		for (int i = 0; i < rgnode[newNode].enableNum; i++) 
		{
			//�ɷ�����Ǩti�ı��iΪhang
			//����M1
			for (int j = 0; j < ptnet.m; j++) {
				int hang = rgnode[newNode].isfirable[i];
				M1[j] = M[j] + ptnet.A[hang][j];
			}
			RGNode G;
			memcpy(G.m, M1, sizeof(int)*ptnet.m);
			G.inset.push(newNode);
			/*if (isBoundless(ptnet, &G))
			{
				cerr << "The petri net is boundless!" << endl;
				exit(-1);
			}*/
			//if����б��г��ֹ�M1
			bool repeated;
			int ii;
			bool repexist = false;
			for (ii = 0; ii < node; ii++) 
			{
				repeated = true;
				for (int jj = 0; jj < ptnet.m; jj++) 
				{
					if (M1[jj] != rgnode[ii].m[jj]) {
						repeated = false;
						break;
					}
				}
				//������б���ֹ�M1
				if (repeated) {
					repexist = true;
					//ii���ظ��Ľڵ�����
					PRGEdge pEdge;
					pEdge = new RGEdge;
					pEdge->nextedge = NULL;
					pEdge->t = rgnode[newNode].isfirable[i];
					pEdge->target = ii;
					rgnode[ii].inset.push(newNode);
					RGNode *p;
					p = &rgnode[newNode];
					pEdge->nextedge = p->firstEdge;
					p->firstEdge = pEdge;
					break;
				}
			}
			//��M1��û�г��ֹ�����״̬
		    if(!repexist) {
				//�������½ڵ�rgnode[node]����M1��
				for (int i = 0; i < ptnet.m; i++) {
					rgnode[node].m[i] = M1[i];
				}
				rgnode[node].flag = 0;
				rgnode[node].name = "M" + itos(node);
				rgnode[node].inset.push(newNode);
				//graph.rgnode[graph.node].t = graph.rgnode[newNode].isfirable[i];//00000000
				//�����½ڵ�ӵ��ڽӱ�߱���
				RGNode *p;
				PRGEdge pEdge;
				pEdge = new RGEdge;
				pEdge->nextedge = NULL;
				pEdge->t = rgnode[newNode].isfirable[i];
				pEdge->target = node;
				p = &rgnode[newNode];
				pEdge->nextedge = p->firstEdge;
				p->firstEdge = pEdge;
				node++;
			}
		}
	}
	for (int i = 1; i < node; i++) {
		for (int j = 0; j < ptnet.m; j++) {
			if (ptnet.place[j].sig == false) {
				rgnode[i].m[j] = -100;
			}
		}
	}
}

void RG::ModifyMarking() {
	//�޸Ŀɴ�ͼ��ʶ
	
}


void RG::PrintGraph(Petri ptnet, ofstream &outfile)
{
	outfile << "�ɴ�ͼ�ڵ����" << node << endl;
	for (int i = 0; i < node; i++) {
		outfile << "M[" << i << "]" << "(";
		for (int j = 0; j < ptnet.m; j++) {
			outfile << rgnode[i].m[j] << " ";
		}
		outfile << ")";
		PRGEdge p;
		p = rgnode[i].firstEdge;
		for (int k = 0; k < rgnode[i].enableNum; k++) {
			outfile << "[t";
			outfile << p->t;
			outfile << "M";
			outfile << p->target;
			outfile << ",";
			p = p->nextedge;
		}
		outfile << endl;
	}
}

bool RG::isBoundless(Petri ptnet, RGNode *curs)//��û�õ��ж��޽�
{
	rgqueue  ancestors;
	int count = curs->inset.pointer;
	for (int i = 0; i < count; i++)
		ancestors.EnQueue(curs->inset.incoming[i]);
	while (!ancestors.isEmpty())
	{
		int pri;
		ancestors.DeQueue(pri);
		//���curs�Ƿ����rgnode[pri]
		if (sum(curs->m, ptnet.m) > sum(rgnode[pri].m, ptnet.m))
		{
			return true;
		}
		for (int j = 0; j < rgnode[pri].inset.pointer; j++)
			ancestors.EnQueue(rgnode[pri].inset.incoming[j]);
	}
	return false;
}

