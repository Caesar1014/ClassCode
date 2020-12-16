#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <queue>
#include <set>

using namespace std;

const int MAX = 128;
typedef set<int> IntSet;
typedef set<char> CharSet;

// NFA状态
typedef struct NfaState
{
	char input;	 // NFA状态弧上的值，默认为'#'
	int chTrans; // NFA状态弧转移到的状态号，默认为-1

	int index;		// NFA状态的状态号
	IntSet epTrans; // 当前状态通过ε转移到的状态号集合
} NfaState;

NfaState NfaStates[MAX];
int nfaStateNum = 0; // NFA状态总数

typedef struct NFA
{
	NfaState *head; // NFA头指针
	NfaState *tail; // NFA尾指针
} NFA;

bool isLetter(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

// 对字符串s进行预处理
void preprocess(string &s)
{
	int i = 0, length = s.size();

	while (i < length)
	{
		if (isLetter(s[i]) || s[i] == '*' || s[i] == ')')
		{
			if (isLetter(s[i + 1]) || s[i + 1] == '(')
			{
				s.insert(i + 1, 1, '&');
				length++;
			}
		}
		i++;
	}
}

// 中缀转后缀时用到的优先级比较
int priority(char ch)
{
	switch (ch)
	{
	case '*':
		return 3;
	case '&':
		return 2;
	case '|':
		return 1;
	case '(':
		return 0;
	}

	return -1;
}

// 中缀表达式转后缀表达式
string infixToSuffix(string s)
{
	int length = s.size();
	string str = "";
	stack<char> oper;

	for (int i = 0; i < length; i++)
	{
		if (isLetter(s[i]))
			str += s[i];
		else
		{
			if (s[i] == '(')
				oper.push(s[i]);
			else if (s[i] == ')')
			{
				char ch = oper.top();
				while (ch != '(')
				{
					str += ch;
					oper.pop();
					ch = oper.top();
				}
				oper.pop();
			}
			else
			{
				if (!oper.empty())
				{
					char ch = oper.top();
					// 弹出栈中优先级大于等于当前运算符的运算符
					while (priority(ch) >= priority(s[i]))
					{
						str += ch;
						oper.pop();
						if (oper.empty())
							break;
						ch = oper.top();
					}
					oper.push(s[i]);
				}
				else
				{
					oper.push(s[i]);
				}
			}
		}
	}

	while (!oper.empty())
	{
		char ch = oper.top();
		oper.pop();
		str += ch;
	}

	return str;
}

void init()
{
	int i;
	for (i = 0; i < MAX; i++)
	{

		NfaStates[i].index = i;
		NfaStates[i].input = '#';
		NfaStates[i].chTrans = -1;
	}
}

NFA createNFA(int sum)
{
	NFA n;

	n.head = &NfaStates[sum];
	n.tail = &NfaStates[sum + 1];

	nfaStateNum += 2;

	return n;
}

// 从状态n1到状态n2添加一条弧，弧上的值为ch
void add(NfaState *n1, NfaState *n2, char ch)
{
	n1->input = ch;
	n1->chTrans = n2->index;
}

// 从状态n1到状态n2添加一条弧，弧上的值为ε
void add(NfaState *n1, NfaState *n2)
{
	n1->epTrans.insert(n2->index);
}

// 后缀表达式转NFA
NFA strToNfa(string s)
{
	stack<NFA> NfaStack;

	for (unsigned i = 0; i < s.size(); i++)
	{
		if (isLetter(s[i])) // 遇到操作数
		{
			NFA n = createNFA(nfaStateNum); // 新建一个NFA
			add(n.head, n.tail, s[i]);		// NFA的头指向尾，弧上的值为s[i]
			NfaStack.push(n);				// 将该NFA入栈
		}
		else if (s[i] == '*')
		{
			NFA n1, n2;
			n1 = createNFA(nfaStateNum);
			n2 = NfaStack.top();
			NfaStack.pop();

			add(n1.head, n1.tail);
			add(n1.head, n2.head);
			add(n2.tail, n1.tail);
			add(n2.tail, n2.head);

			NfaStack.push(n1);
		}
		else
		{
			NFA n1, n2;
			n2 = NfaStack.top();
			NfaStack.pop();
			n1 = NfaStack.top();
			NfaStack.pop();

			switch (s[i])
			{
			case '&':
			{
				NFA n;

				add(n1.tail, n2.head);
				n.head = n1.head;
				n.tail = n2.tail;

				NfaStack.push(n);
				break;
			}
			case '|':
			{
				NFA n = createNFA(nfaStateNum);

				add(n.head, n1.head);
				add(n.head, n2.head);
				add(n1.tail, n.tail);
				add(n2.tail, n.tail);

				NfaStack.push(n);
				break;
			}
			}
		}
	}
	return NfaStack.top();
}

// 打印NFA函数
void printNFA(NFA nfa)
{

	cout << "***************     NFA     ***************" << endl
		 << endl;
	cout << "NFA总共有" << nfaStateNum << "个状态，" << endl;
	cout << "初态为" << nfa.head->index << "，终态为" << nfa.tail->index << "。" << endl
		 << endl
		 << "转移函数为：" << endl;

	for (int i = 0; i < nfaStateNum; i++) /*遍历NFA状态数组*/
	{

		if (NfaStates[i].input != '#') /*如果弧上的值不是初始时的‘#’则输出*/
		{
			cout << NfaStates[i].index << "-->'" << NfaStates[i].input << "'-->" << NfaStates[i].chTrans << '\t';
		}

		for (auto it : NfaStates[i].epTrans)
		{
			cout << NfaStates[i].index << "-->'" << ' ' << "'-->" << it << '\t';
		}

		cout << endl;
	}
}

/********************NFA转DFA********************/

// DFA的转换弧
typedef struct Edge
{
	char input; // 弧上的值
	int Trans;	// 弧所指向的状态号
} Edge;

// 定义DFA状态
typedef struct DfaState
{
	bool isEnd; // 是否为终态，是为true，不是为false

	int index;		// DFA状态的状态号
	IntSet closure; // NFA的ε-move()闭包

	int edgeNum;	// DFA状态上的射出弧数
	Edge edges[10]; // DFA状态上的射出弧
} DfaStete;

DfaState DfaStates[MAX]; // DFA状态数组
int dfaStateNum = 0;	 // DFA状态总数

// 定义DFA结构
typedef struct DFA
{
	int startState; // DFA初态

	IntSet endStates;	// DFA终态集
	CharSet terminator; // DFA终结符集

	int trans[MAX][26]; // DFA的转移矩阵
} DFA;

// 求一个状态集的ε-cloure
IntSet epcloure(IntSet s)
{
	stack<int> epStack;

	for (auto it : s)
	{
		epStack.push(it);
	}

	while (!epStack.empty())
	{
		int temp = epStack.top();
		epStack.pop();

		// 遍历它通过ε能转换到的状态集
		for (auto it : NfaStates[temp].epTrans)
		{
			if (!s.count(it))
			{
				s.insert(it);
				epStack.push(it); /*同时压入栈中*/
			}
		}
	}

	return s;
}

// 判断一个状态是否为终态
bool IsEnd(NFA n, IntSet s)
{
	for (auto it : s)
	{
		if (it == n.tail->index)
		{
			return true;
		}
	}
	return false;
}

// 求一个状态集s的ε-cloure(move(ch))
IntSet moveEpCloure(IntSet s, char ch)
{
	IntSet temp;

	for (auto it : s)
	{
		if (NfaStates[it].input == ch)
		{
			temp.insert(NfaStates[it].chTrans);
		}
	}
	temp = epcloure(temp);

	return temp;
}

// NFA转DFA主函数
DFA nfaToDfa(NFA n, string str)
{
	// 参数为NFA和后缀表达式

	cout << "***************     DFA     ***************" << endl
		 << endl;
	DFA d;
	set<IntSet> states; // 定义一个存储整数集合的集合，用于判断求出一个状态集s的ε-cloure(move(ch))后是否出现新状态

	memset(d.trans, -1, sizeof(d.trans));

	for (int i = 0; i < str.size(); i++)
	{
		if (isLetter(str[i]))
		{
			d.terminator.insert(str[i]);
		}
	}

	d.startState = 0;

	IntSet tempSet;
	tempSet.insert(n.head->index); // 将NFA的初态加入到集合中

	DfaStates[0].closure = epcloure(tempSet);			 // 求DFA的初态
	DfaStates[0].isEnd = IsEnd(n, DfaStates[0].closure); // 判断初态是否为终态

	dfaStateNum++;

	queue<int> q;
	q.push(d.startState);

	while (!q.empty())
	{
		int num = q.front();
		q.pop();

		for (auto it : d.terminator)
		{
			IntSet temp = moveEpCloure(DfaStates[num].closure, it); // 计算每个终结符的ε-cloure(move(ch))

			cout << endl;
			for (auto it : temp)
			{
				cout << it << ' ';
			}
			cout << endl;

			if (!states.count(temp) && !temp.empty())
			{
				// 如果求出来的状态集不为空且与之前求出来的状态集不同，则新建一个DFA状态

				states.insert(temp);

				DfaStates[dfaStateNum].closure = temp;

				DfaStates[num].edges[DfaStates[num].edgeNum].input = it;		  // 该状态弧的输入即为当前终结符
				DfaStates[num].edges[DfaStates[num].edgeNum].Trans = dfaStateNum; // 弧转移到的状态为最后一个DFA状态
				DfaStates[num].edgeNum++;

				d.trans[num][it - 'a'] = dfaStateNum; // 更新转移矩阵

				DfaStates[dfaStateNum].isEnd = IsEnd(n, DfaStates[dfaStateNum].closure); // 判断是否为终态

				q.push(dfaStateNum); // 将新的状态号加入队列中

				dfaStateNum++; // DFA状态总数加一
			}
			else
			{
				for (int i = 0; i < dfaStateNum; i++)
				{
					if (temp == DfaStates[i].closure)
					{
						// 找到与该集合相同的DFA状态

						DfaStates[num].edges[DfaStates[num].edgeNum].input = it; // 该状态弧的输入即为当前终结符
						DfaStates[num].edges[DfaStates[num].edgeNum].Trans = i;	 // 该弧转移到的状态即为i
						DfaStates[num].edgeNum++;								 // 该状态弧的数目加一

						d.trans[num][it - 'a'] = i; // 更新转移矩阵

						break;
					}
				}
			}
		}
	}

	// 计算DFA的终态集
	for (int i = 0; i < dfaStateNum; i++)
	{
		if (DfaStates[i].isEnd == true)
		{
			d.endStates.insert(i);
		}
	}

	return d;
}

// 打印DFA函数
void printDFA(DFA d)
{

	int i, j;
	cout << "DFA总共有" << dfaStateNum << "个状态，"
		 << "初态为" << d.startState << endl
		 << endl;

	cout << "有穷字母表为 { ";
	for (auto it : d.terminator)
	{
		cout << it << ' ';
	}
	cout << '}' << endl
		 << endl;

	cout << "终态集为 { ";
	for (auto it : d.endStates)
	{
		cout << it << ' ';
	}
	cout << '}' << endl
		 << endl;

	cout << "转移函数为：" << endl;
	for (i = 0; i < dfaStateNum; i++)
	{
		for (j = 0; j < DfaStates[i].edgeNum; j++)
		{

			if (DfaStates[DfaStates[i].edges[j].Trans].isEnd == true)
			{
				cout << DfaStates[i].index << "-->'" << DfaStates[i].edges[j].input;
				cout << "'--><" << DfaStates[i].edges[j].Trans << ">\t";
			}
			else
			{
				cout << DfaStates[i].index << "-->'" << DfaStates[i].edges[j].input;
				cout << "'-->" << DfaStates[i].edges[j].Trans << '\t';
			}
		}
		cout << endl;
	}

	cout << endl
		 << "转移矩阵为：" << endl
		 << "     ";
	for (auto it : d.terminator)
	{
		cout << it << "   ";
	}
	cout << endl;

	for (i = 0; i < dfaStateNum; i++)
	{

		if (d.endStates.count(i))
		{
			cout << '<' << i << ">  ";
		}
		else
		{
			cout << ' ' << i << "   ";
		}

		for (j = 0; j < 26; j++)
		{
			if (d.terminator.count(j + 'a'))
			{
				if (d.trans[i][j] != -1)
				{
					cout << d.trans[i][j] << "   ";
				}
				else
				{
					cout << "    ";
				}
			}
		}

		cout << endl;
	}
}

/******************DFA的最小化******************/

int main()
{
	string str;
	cin >> str;

	preprocess(str);
	str = infixToSuffix(str);
	cout << str << endl;
	init();
	NFA n = strToNfa(str);
	printNFA(n);
	DFA d = nfaToDfa(n, str);
	printDFA(d);
}