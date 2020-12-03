#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

typedef struct PCB
{
    string name;        //进程名
    PCB *next;          //进程队列指针
} PCB, *LinkList;

typedef LinkList QueuePtr;
typedef struct
{
    QueuePtr top;       //队列的头指针
    QueuePtr rear;      //队列的尾指针
} LinkQueue;            //链队列

LinkQueue SourceQueue, ReadyQueue, WaitQueue, RunQueue;

int choose; //主界面的选择
int num;    //记录资源池中 PCB 的个数

void InitQueue(LinkQueue &Q)
{
    //构造一个只含有头结点的空就绪队列Q
    Q.top = Q.rear = new PCB();
    Q.rear->next = NULL;
}

void EnQueue(LinkQueue &Q, PCB *e)
{
    //插入元素e为链队列Q中新的队尾元素
    e->next = NULL;
    Q.rear->next = e;
    Q.rear = e;
}

void ShowQueue(LinkQueue &Q)
{
    //按顺序显示链队列中的元素
    PCB *p = Q.top->next;

    if (p == NULL)
    {
        cout<<"NULL"<<endl;
    }
    else
    {
        while(p->next != NULL)
        {
            cout<< p->name <<" -> ";
            p = p->next;
        }
        cout<<p->name;
    cout<<" -> NULL"<<endl;
    }
}

void initial()
{
    InitQueue(SourceQueue);
    InitQueue(ReadyQueue);
    InitQueue(WaitQueue);
    InitQueue(RunQueue);
    cout << "输入资源池中 PCB 的总个数 ：";
    cin >> num;

    for(int i = 0; i < num; i++) {
        PCB *p = new PCB();
        EnQueue(SourceQueue, p);
    }
}

void create()
{
    //从资源池中申请一个PCB并放入就绪队列中
    if(num > 0) {
        PCB *p = SourceQueue.top->next;
        SourceQueue.top->next = p->next;
        p->next = NULL;
        num--;
        cout << "请输入进程名：" << endl;
        cin >> p->name;
        EnQueue(ReadyQueue, p);
    } else {
        cout << "资源不足" << endl;
    }
}

void HuoBan(LinkQueue &Q1, LinkQueue &Q2)
{
    //伙伴系统的应用，时间片到时运行转就绪，挂起时运行转等待，激活时等待转运行
    PCB *p = Q1.top->next;
    Q1.top->next = p->next;
    p->next = NULL;
    if(Q1.rear == p) {
        Q1.rear = Q1.top;
    }
    EnQueue(Q2, p);
}

void revoke(LinkQueue &Q)
{
    PCB *p = Q.top->next;
    p->name = "";
    Q.top->next = p->next;
    p->next = NULL;
    if(Q.rear == p) {
        Q.rear = Q.top;
    }
    num++;
}

void show()
{
    puts("════════════════════════════════");
    cout<<"Source_queue ：资源池中还剩"<<num<<"个PCB"<<endl;
    cout<<"Ready_queue ：";
	ShowQueue(ReadyQueue);
	cout << "Wait_queue ：";
    ShowQueue(WaitQueue);
    cout<<"Run_queue ：";
    ShowQueue(RunQueue);
    puts("════════════════════════════════");
}

int main()
{
    puts("---------------初始化---------------");
    initial();
    show();
    while(true) {
        cout<<"------------请选择操作------------"<<endl;
        cout<<"1、创建 2、调用 3、时间片到 4、挂起 5、激活 6、撤销 7、显示 8、退出"<<endl;
        cin >> choose;
        switch(choose) {
            case 1: {
                create();
                break;
            }
            case 2: {
                HuoBan(ReadyQueue,RunQueue);
                break;
            }
            case 3: {
                HuoBan(RunQueue, ReadyQueue);
                break;
            }
            case 4: {
                HuoBan(RunQueue, WaitQueue);
                break;
            }
            case 5: {
                HuoBan(WaitQueue, ReadyQueue);
                break;
            }
            case 6: {
                revoke(RunQueue);
                break;
            }
            case 7: {
                show();
                break;
            }
            case 8: {
                exit(0);
                break;
            }
            default:
                break;
        }
    }
    return 0;
}