#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

const int MAX = 20;
const int SPACE = 10;
int n;                        // 作业数量
double aveWholeTime = 0;      // 平均周转时间
double aveValueWholeTime = 0; // 带权平均周转时间

typedef struct Time
{
    int hour;
    int minute;
} Time;

Time bridgingTime;

typedef struct JCB
{
    bool state = false;
    string name;           // 作业名
    Time arriveTime;       // 到达时间
    int runTime;           // 运行时间
    Time startTime;        // 开始时间
    Time endTime;          // 结束时间
    int wholeTime;         // 周转时间
    double valueWholeTime; // 带权周转时间
} JCB;
JCB job[MAX];

void initial();
bool compareArrive(const JCB &j1, const JCB &j2);
Time calculateEndTime(Time startTime, int runTime);
int calculateWholeTime(Time arriveTime, Time endTime);
void process(int num);
void SJF();
void display();

void initial()
{
    cout << "请输入作业的个数：";
    cin >> n;
    if (n > MAX)
    {
        cout << "作业数量过大";
        return;
    }
    for (int i = 0; i < n; i++)
    {
        cout << "请输入第" << i + 1 << "个作业的名字、进入时间、运行时间：" << endl;
        cin >> job[i].name;
        scanf("%d:%d", &job[i].arriveTime.hour, &job[i].arriveTime.minute);
        cin >> job[i].runTime;
    }
}

bool compareArrive(const JCB &j1, const JCB &j2)
{
    if (j1.arriveTime.hour != j2.arriveTime.hour)
        return j1.arriveTime.hour < j2.arriveTime.hour;
    else
        return j1.arriveTime.minute < j2.arriveTime.minute;
}

Time calculateEndTime(Time startTime, int runTime)
{
    Time t;
    t.minute = (startTime.minute + runTime) % 60;
    t.hour = startTime.hour + (startTime.minute + runTime) / 60;
    return t;
}

int calculateWholeTime(Time arriveTime, Time endTime)
{
    int t;
    if (endTime.minute < arriveTime.minute)
    {
        endTime.minute += 60;
        endTime.hour--;
    }
    t = (endTime.hour - arriveTime.hour) * 60 + (endTime.minute - arriveTime.minute);
    return t;
}

void process(int num)
{
    // 计算开始时间
    job[num].startTime = bridgingTime;
    // 计算结束时间
    job[num].endTime = calculateEndTime(job[num].startTime, job[num].runTime);
    // 计算周转时间
    job[num].wholeTime = calculateWholeTime(job[num].arriveTime, job[num].endTime);
    // 计算带权周转时间
    job[num].valueWholeTime = (double)job[num].wholeTime / job[num].runTime;

    aveWholeTime += job[num].wholeTime;
    aveValueWholeTime += job[num].valueWholeTime;

    bridgingTime = job[num].endTime;
    job[num].state = true;
}

void SJF()
{
    int min = 1;
    bridgingTime = job[0].arriveTime;

    process(0);

    for (int i = 1; i < n; i++)
    {
        // 在剩余的作业中按顺序选择min
        for (int j = 1; j < n; j++)
            if (!job[j].state)
            {
                min = j;
                break;
            }
        // 在顺序的作业中寻找运行时间真正的min
        for (int j = 1; j < n; j++)
        {
            if (job[j].state || j == min)
                continue;
            if (job[j].runTime < job[min].runTime)
                min = j;
        }

        process(min);
    }
    // 计算平均周转时间
    aveWholeTime /= n;
    // 计算带权平均周转时间
    aveValueWholeTime /= n;
}

void display()

{
    cout << "******************************************************" << endl;
    cout << setw(SPACE) << "作业名"
         << " ";
    cout << setw(SPACE) << "进入时间"
         << " ";
    cout << setw(SPACE) << "运行时间"
         << " ";
    cout << setw(SPACE) << "开始时间"
         << " ";
    cout << setw(SPACE) << "结束时间"
         << " ";
    cout << setw(SPACE) << "周转时间"
         << " ";
    cout << setw(SPACE) << "带权周转时间" << endl;

    cout << setiosflags(ios::fixed) << setprecision(1);
    for (int i = 0; i < n; i++)
    {
        cout << setw(SPACE) << job[i].name << " ";
        if (job[i].arriveTime.minute < 10)
            cout << setw(SPACE) << job[i].arriveTime.hour << ":0" << job[i].arriveTime.minute << " ";
        else
            cout << setw(SPACE) << job[i].arriveTime.hour << ":" << job[i].arriveTime.minute << " ";
        cout << setw(SPACE) << job[i].runTime << " ";
        if (job[i].startTime.minute < 10)
            cout << setw(SPACE) << job[i].startTime.hour << ":0" << job[i].startTime.minute << " ";
        else
            cout << setw(SPACE) << job[i].startTime.hour << ":" << job[i].startTime.minute << " ";
        if (job[i].endTime.minute < 10)
            cout << setw(SPACE) << job[i].endTime.hour << ":0" << job[i].endTime.minute << " ";
        else
            cout << setw(SPACE) << job[i].endTime.hour << ":" << job[i].endTime.minute << " ";
        cout << setw(SPACE) << job[i].wholeTime << " ";
        cout << setw(SPACE) << job[i].valueWholeTime << " " << endl;
    }
    cout << "作业平均周转时间 T = " << aveWholeTime << "分钟" << endl;
    cout << setiosflags(ios::fixed) << setprecision(3);
    cout << "作业带权平均周转时间 W = " << aveValueWholeTime << endl;
}

int main()
{
    initial();
    sort(job, job + n, compareArrive);
    SJF();
    display();
    return 0;
}