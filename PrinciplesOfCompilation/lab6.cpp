#include <iostream>
#include <string>
#include <stack>
#include <set>

using namespace std;

const int MAX = 128;
typedef set<int> IntSet;
typedef set<char> CharSet;

// NFA状态
typedef struct NfaState
{
	int index;		// NFA状态的状态号
	char input;		// NFA状态弧上的值，默认为'#'
	int chTrans;	// NFA状态弧转移到的状态号，默认为-1
	IntSet epTrans;	// 当前状态通过ε转移到的状态号集合
}NfaState;

typedef struct NFA
{
	NfaState* head;	// NFA头指针
	NfaState* tail;	// NFA尾指针
}NFA;

NfaState NfaStates[MAX];
int nfaStateNum = 0;		// NFA状态总数

// 对字符串s进行预处理
void preprocess(string& s)
{
	int i = 0, length = s.size();

	while (i < length)
	{
		if ((s[i] >= 'a' && s[i] <= 'z') || s[i] == '*' || s[i] == ')')
		{
			if ((s[i + 1] >= 'a' && s[i + 1] <= 'z') || s[i + 1] == '(')
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
	switch (ch) {
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
		if (s[i] >= 'a' && s[i] <= 'z')
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

	while (!oper.empty()) {
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
void add(NfaState* n1, NfaState* n2, char ch)
{
	n1->input = ch;
	n1->chTrans = n2->index;
}

// 从状态n1到状态n2添加一条弧，弧上的值为ε
void add(NfaState* n1, NfaState* n2)
{
	n1->epTrans.insert(n2->index);
}

// 后缀表达式转NFA
NFA strToNfa(string s)
{
	stack<NFA> NfaStack;

	for (unsigned i = 0; i < s.size(); i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')		// 遇到操作数
		{
			NFA n = createNFA(nfaStateNum);	// 新建一个NFA
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

			switch(s[i]) 
			{
				case '&': {
					NFA n;

					add(n1.tail, n2.head);
					n.head = n1.head;
					n.tail = n2.tail;

					NfaStack.push(n);
					break;
				}
				case '|': {
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

/*打印NFA函数*/
void printNFA(NFA nfa)
{

	cout<<"***************     NFA     ***************"<<endl<<endl; 
	cout<<"NFA总共有"<<nfaStateNum<<"个状态，"<<endl;
	cout<<"初态为"<<nfa.head->index<<"，终态为" <<nfa.tail->index<<"。"<<endl<<endl<<"转移函数为："<<endl;
	
	for(int i = 0; i < nfaStateNum; i++)		/*遍历NFA状态数组*/
	{
		
		if(NfaStates[i].input != '#')			/*如果弧上的值不是初始时的‘#’则输出*/
		{
			cout<<NfaStates[i].index<<"-->'"<<NfaStates[i].input<<"'-->"<<NfaStates[i].chTrans<<'\t';
		}
		
		IntSet::iterator it;					/*输出该状态经过ε到达的状态*/
		for(it = NfaStates[i].epTrans.begin(); it != NfaStates[i].epTrans.end(); it++)
		{
			cout<<NfaStates[i].index<<"-->'"<<' '<<"'-->"<<*it<<'\t';
		}
		
		cout<<endl;
	}
}

/********************NFA转DFA********************/

// DFA的转换弧
typedef struct Edge
{
	char input; // 弧上的值
	int Trans;	// 弧所指向的状态号
} Edge;

typedef struct DfaState
{
	bool isEnd;
	int index;
	IntSet closure;
	int edgeNum;
	Edge edges[10];
} DfaStete;

typedef struct DFA
{
	int startState;
    IntSet endStates;
    CharSet terminator;
    int trans[MAX][26];
} DFA;




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
}