#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

/*
0--就绪
1--等待
2--运行
3--完成
*/

typedef struct PCB
{
    string id;
    string name;
    int status;        //进程状态
    int runTime;       //已运行时间
    int remainingTime; //未运行时间
    int totalTime;     //完成需要运行时间
    struct PCB *next;
} PCB;

PCB *p_run = NULL;   //运行指针
PCB *p_wait0 = NULL; //等待队列队首指针
PCB *p_wait1 = NULL;
PCB *p_wait2 = NULL;
PCB *p_ready = NULL;      //就绪队列队首指针
PCB *p_source = NULL;     //资源池首指针
PCB *p_wait_rear0 = NULL; //等待队列队尾指针
PCB *p_wait_rear1 = NULL;
PCB *p_wait_rear2 = NULL;
PCB *p_ready_rear = NULL;  //就绪队列队尾指针
PCB *p_source_rear = NULL; //资源池尾指针

int num; //记录资源池中 PCB 的个数

void initial();
void insert();
void kill();
void time_control();
void susp(int x);
void wakeup();
void show();

void process_control(); //进程调度程序
void create(PCB* p);
void verify(string ID, PCB* p, int* tag);

int main()
{
    initial();
    int a, x;
    while (1)
    {
        cout << " --------菜单-------" << endl;
        cout << endl;
        cout << " 1 创建" << endl;
        cout << " 2 撤销" << endl;
        cout << " 3 时间片到" << endl;
        cout << " 4 进程挂起" << endl;
        cout << " 5 唤醒进程" << endl;
        cout << " 6 显示" << endl;
        cout << " 0 退出" << endl;
        cout << endl;
        cout << " -------------------" << endl;
        cin >> a;
        switch (a)
        {
        case 1:
            insert();
            break;
        case 2:
            kill();
            break;
        case 3:
            time_control();
            break;
        case 4:
            cout << " 选择阻塞原因" << endl;
            cout << "0 等待 IO" << endl;
            cout << "1 进程 sleep" << endl;
            cout << "2 等待解锁" << endl;
            cin >> x;
            susp(x);
            break;
        case 5:
            wakeup();
            break;
        case 6:
            show();
            break;
        case 0:
            exit(0);
            break;
        default:
            break;
        }
    }
    return 0;
}

void initial()
{
    PCB *p;

    cout << "输入 PCB 的总个数" << endl;
    cin >> num;

    p_ready = new PCB();
    cout << "输入 ID" << endl;
    cin >> p_ready->id;
    cout << "输入进程名称" << endl;
    cin >> p_ready->name;
    cout << "输入运行时间" << endl;
    cin >> p_ready->totalTime;
    p_ready->status = 0;
    p_ready->next = NULL;
    p_ready->remainingTime = p_ready->totalTime;
    p_ready->runTime = 0;
    p_ready_rear = p_ready;
    p_wait0 = p_wait_rear0 = NULL;
    p_wait1 = p_wait_rear1 = NULL;
    p_wait2 = p_wait_rear2 = NULL;
    p_source = p_source_rear = NULL;

    for (int i = 1; i < num; i++)
    {
        p = new PCB();
        cout << "输入 ID" << endl;
        cin >> p->ID;
        create(p);
    }
    process_control();
}

void insert()
{
    int *tag = 0;
    string ID;
    cout << "输入 ID" << endl;
    cin >> ID;

    verify(ID, p_ready, tag);
    verify(ID, p_wait0, tag);
    verify(ID, p_wait1, tag);
    verify(ID, p_wait2, tag);

    if (p_source == NULL)
        *tag = 1;
    if (*tag == 0)
    {
        p = p_source;
        p_source = p_source->next;
        strcpy(p->ID, ID);
        create(p);
    }
    else
    {
        cout << "创建失败" << endl;
    }
}

void process_control()
{
    p_run = p_ready;
    p_ready = p_ready->next;
}

void create(PCB* p)
{
    cout << "输入进程名称" << endl;
    cin >> p->name;
    cout << "输入运行时间" << endl;
    cin >> p->totalTime;
    p->status = 0;
    p->next = NULL;
    p->remainingTime = p->totalTime;
    p->runTime = 0;
    p_ready_rear->next = p;
    p_ready_rear = p_ready_rear->next;
}

void verify(string ID, PCB* p, int* tag)
{
    while (p != NULL)
    {
        if (strcmp(p->id, ID) == 0)
        {
            *tag = 1;
            break;
        }
        p = p->next;
    }
}

void show()
{
    PCB *p;
    cout << "---------------" << endl;
    cout << "正在运行的进程 " << endl;
    cout << "ID:" << p_run->id << " Name:" << p_run->name << " 总时间:" << p_run->totalTime << " 已完成时间 : " << p_run->runTime << " 未完成时间 : " << p_run->remainingTime << endl;
    cout << "---------------" << endl;
    p = p_ready;
    cout << "就绪的进程 " << endl;
    while (p != NULL)
    {
        cout << "ID:" << p->id << " Name:" << p->name << " 总时间:" << p->totalTime << " 已完成时间 : " << p->runTime << " 未完成时间 : " << p->remainingTime << endl;
        p = p->next;
    }
    cout << "---------------" << endl;
    p = p_wait0;
    cout << "等待 IO 的进程 " << endl;
    while (p != NULL)
    {
        cout << "ID:" << p->id << " Name:" << p->name << " 总时间:" << p->totalTime << " 已完成时间 : " << p->runTime << " 未完成时间 : " << p->remainingTime << endl;
        p = p->next;
    }
    cout << "---------------" << endl;
    p = p_wait1;
    cout << "进程 sleep 的进程 " << endl;
    while (p != NULL)
    {
        cout << "ID:" << p->id << " Name:" << p->name << " 总时间:" << p->totalTime << " 已完成时间 : " << p->runTime << " 未完成时间 : " << p->remainingTime << endl;
        p = p->next;
    }
    cout << "---------------" << endl;
    p = p_wait2;
    cout << "等待解锁的进程 " << endl;
    while (p != NULL)
    {
        cout << "ID:" << p->id << " Name:" << p->name << " 总时间:" << p->totalTime << " 已完成时间 : " << p->runTime << " 未完成时间 : " << p->remainingTime << endl;
        p = p->next;
    }
    cout << "---------------" << endl;
}
