#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

const int MAX = 20;
const int SPACE = 10;
int jobNum;                   // 作业数量
int trackNum;                 // 批处理道数
int jobAlgorithmOption;       // 作业调度算法编号
int processAlgorithmOption;   // 进程调度算法编号
double aveWholeTime = 0;      // 平均周转时间
double aveValueWholeTime = 0; // 带权平均周转时间
int memoryjobNum = 0;         // 当前内存中的作业数量

typedef struct Time
{
    int hour;
    int minute;
} Time;

Time currentTime;

typedef struct JCB
{
    bool state = false;    // 标记该作业是否执行完成
    bool inMemory = false; // 标记该作业是否在内存
    bool firstRun = true;  // 标记该作业是否是第一次执行
    string name;           // 作业名
    Time arriveTime;       // 到达时间
    int runTime;           // 运行时间
    int remainingTime;     // 剩余时间
    Time startTime;        // 开始时间
    Time endTime;          // 结束时间
    int wholeTime;         // 周转时间
    double valueWholeTime; // 带权周转时间
} JCB;
JCB job[MAX];

void initial()
{
    cout << "请输入批处理系统道数：";
    cin >> trackNum;
    cout << "请输入作业的个数：";
    cin >> jobNum;

    for (int i = 0; i < jobNum; i++)
    {
        // cout << "请输入第" << i + 1 << "个作业的名字、进入时间、运行时间：" << endl;
        cin >> job[i].name;
        scanf("%d:%d", &job[i].arriveTime.hour, &job[i].arriveTime.minute);
        cin >> job[i].runTime;
        job[i].remainingTime = job[i].runTime;
    }

    currentTime = job[0].arriveTime;

    cout << "******************Option******************" << endl;
    cout << "1-----FCFS(First Come First Serve)" << endl;
    cout << "2-----SJF(Shortest Job First)" << endl;
    cout << "3-----HRN(Highest Response Ratio Next)" << endl;
    cout << "请选择作业调度算法编号：";
    cin >> jobAlgorithmOption;
    cout << "请选择进程调度算法编号：";
    cin >> processAlgorithmOption;
}

bool compareArrive(const JCB &j1, const JCB &j2)
{
    if (j1.arriveTime.hour != j2.arriveTime.hour)
        return j1.arriveTime.hour < j2.arriveTime.hour;
    else
        return j1.arriveTime.minute < j2.arriveTime.minute;
}

void jobFCFS()
{
    int times = trackNum - memoryjobNum;

    for (int i = 0; i < jobNum; i++)
    {
        if (times <= 0)
            break;
        if (!job[i].state && !job[i].inMemory && ((job[i].arriveTime.hour < currentTime.hour) || (job[i].arriveTime.hour == currentTime.hour && job[i].arriveTime.minute <= currentTime.minute)))
        {
            job[i].inMemory = true;
            memoryjobNum++;
            times--;
        }
    }
}

void jobSJF()
{
    int times = trackNum - memoryjobNum;

    while (times > 0)
    {
        int pos = -1;
        int min = 100000;

        for (int i = 0; i < jobNum; i++)
        {
            if (!job[i].state && !job[i].inMemory && job[i].runTime < min && ((job[i].arriveTime.hour < currentTime.hour) || (job[i].arriveTime.hour == currentTime.hour && job[i].arriveTime.minute <= currentTime.minute)))
            {
                min = job[i].runTime;
                pos = i;
            }
        }
        if (pos != -1)
        {
            job[pos].inMemory = true;
            memoryjobNum++;
        }
        times--;
    }
}

void jobHRN()
{
    int times = trackNum - memoryjobNum;

    while (times > 0)
    {
        double responseRatio = -1.0;
        int pos = -1;
        for (int i = 0; i < jobNum; i++)
        {
            int waitTime = (currentTime.hour - job[i].arriveTime.hour) * 60 + currentTime.minute - job[i].arriveTime.minute;
            double tmpRatio = (double)waitTime / (double)job[i].runTime;
            if (!job[i].state && !job[i].inMemory && tmpRatio > responseRatio && ((job[i].arriveTime.hour < currentTime.hour) || (job[i].arriveTime.hour == currentTime.hour && job[i].arriveTime.minute <= currentTime.minute)))
            {
                responseRatio = tmpRatio;
                pos = i;
            }
        }
        if (pos != -1)
        {
            job[pos].inMemory = true;
            memoryjobNum++;
        }
        times--;
    }
}

int processFCFS()
{
    int pos = -1;
    for (int i = 0; i < jobNum; i++)
    {
        if (job[i].inMemory)
        {
            pos = i;
            break;
        }
    }
    return pos;
}

int processSJF()
{
    int min = 100000;
    int pos = -1;

    for (int i = 0; i < jobNum; i++)
    {
        if (job[i].inMemory && job[i].remainingTime < min)
        {
            min = job[i].remainingTime;
            pos = i;
        }
    }
    return pos;
}

int processHRN()
{
    double responseRatio = -1.0;
    int pos = -1;
    for (int i = 0; i < jobNum; i++)
    {
        int waitTime = (currentTime.hour - job[i].arriveTime.hour) * 60 + currentTime.minute - job[i].arriveTime.minute;
        double tmpRatio = (double)waitTime / job[i].runTime;
        if (job[i].inMemory && tmpRatio > responseRatio)
        {
            responseRatio = tmpRatio;
            pos = i;
        }
    }
    return pos;
}

// 计算已经完成的作业数目
int getFinishedNumber()
{
    int num = 0;
    for (int i = 0; i < jobNum; i++)
        if (job[i].state == true)
            num++;
    return num;
}

Time calculateEndTime(Time startTime, int runTime)
{
    Time t;
    t.minute = (startTime.minute + runTime) % 60;
    t.hour = startTime.hour + (startTime.minute + runTime) / 60;
    return t;
}

void change(int pos)
{
    // 设置开始执行的时间
    if (job[pos].firstRun)
    {
        job[pos].firstRun = false;
        job[pos].startTime = currentTime;
    }
    job[pos].state = true;
    job[pos].endTime = calculateEndTime(currentTime, job[pos].remainingTime);
    job[pos].inMemory = false;
    memoryjobNum--;
}

// ************************执行选择的那个作业（开始执行）************************
//（1）如果当前内存已经满了的话，直接执行完成job[pos]即可。
//（2）如果当前内存未满并且所有未完成的作业都已经调入到了内存，仍然是直接执行完成job[pos]即可。
//（3）如果不是上面两种情况，即内存未满，且存在作业没有调入到内存。这种情况下，要考虑该作业执行过程中，
//     是否需要调入新作业到内存。由于上面已经完成了作业调入内存的功能，那么这次调入内存的作业也只会有
//     一个！这一个一定选择此刻尚未完成、未在内存、开始时间最靠前的作业。还需要判断本次作业执行过程中
//     的时间和将要调入作业的开始时间的关系。
//     A:本次作业执行完时，将要调入的作业尚未开始。那就需要执行完本次作业。并且把当前时间修改为将要调
//       入的这个作业的开始时间。
//     B:本次作业执行过程中，将要调入的作业来了，那就需要把当前时间设置为将要调入的作业的开始时间，并
//       且设置本次作业的剩余执行时间。
//     C:本次作业开始执行的时候，将要调入的作业早已经来过了。这种情况直接交给上面的调入内存步骤解决之。

// 执行选择的那个作业
void exe(int pos)
{
    if (memoryjobNum == trackNum || memoryjobNum == jobNum - getFinishedNumber())
    {
        change(pos);
        // 设置当前时间
        currentTime = job[pos].endTime;

        //再次设置当前时间
        //如果当前内存中的所有作业都执行完之后，还存在内存外尚未执行的作业X，且X的开始时间在当前时间之后，则需要
        //把这样的作业调入内存，那么要把当前时间设置为X的开始时间，为X调入内存做准备。注意这个X就是目前在外存中，
        //且没有完成、且开始时间在目前时间之后，且在所有外存尚未开始执行的作业中开始时间最早。
        if(memoryjobNum == 0 && getFinishedNumber() < jobNum) {
            for(int i = 0; i < jobNum; i++) {
                if(!job[i].inMemory && !job[i].state) {
                    if ((job[i].arriveTime.hour > currentTime.hour) || (job[i].arriveTime.hour == currentTime.hour && job[i].arriveTime.minute>currentTime.minute)){
                        currentTime = job[i].arriveTime;
                    }
                    break;
                }
            }
        }
    }
    else
    {
        // 选中没有完成且不在内存的最先来的作业
        int tmpPos = -1;
        for (int i = 0; i < jobNum; i++)
        {
            if (!job[i].state && !job[i].inMemory)
            {
                tmpPos = i;
                break;
            }
        }

        Time tmpTime = calculateEndTime(currentTime, job[pos].remainingTime);
        if ((tmpTime.hour < job[tmpPos].arriveTime.hour) || ((tmpTime.hour == job[pos].arriveTime.hour) && (tmpTime.minute <= job[pos].arriveTime.minute)))
        {
            // A:本次作业执行完时，将要调入的作业尚未开始
            change(pos);
            // 设置当前时间
            currentTime = job[pos].arriveTime;
        }
        else if ((tmpTime.hour > job[tmpPos].arriveTime.hour) || ((tmpTime.hour == job[pos].arriveTime.hour) && (tmpTime.minute > job[pos].arriveTime.minute)))
        {
            // B:本次作业执行过程中，将要调入的作业来了
            if (job[pos].firstRun)
            {
                job[pos].firstRun = false;
                job[pos].startTime = currentTime;
            }
            int temp = (job[tmpPos].arriveTime.hour - currentTime.hour) * 60 + job[tmpPos].arriveTime.minute - currentTime.minute;
            job[pos].remainingTime = job[pos].remainingTime - temp;
            currentTime = job[tmpPos].arriveTime;
        }
    }
}

void run()
{
    while (getFinishedNumber() < jobNum)
    {
        /*Step1：调度作业到内存*/
        switch (jobAlgorithmOption)
        {
        case 1:
            jobFCFS();
            break;
        case 2:
            jobSJF();
            break;
        case 3:
            jobHRN();
            break;
        default:
            cout << "input error" << endl;
            return;
        }
        /*Step2：从内存中选一个作业执行*/
        int pos = -1;
        switch (processAlgorithmOption)
        {
        case 1:
            pos = processFCFS();
            break;
        case 2:
            pos = processSJF();
            break;
        case 3:
            pos = processHRN();
            break;
        default:
            cout << "input error" << endl;
            return;
        }
        exe(pos);
    }
}

void display()
{
    cout << "=======================================Result=======================================" << endl;
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

    // cout << setiosflags(ios::fixed) << setprecision(1);
    for (int i = 0; i < jobNum; i++)
    {
        job[i].wholeTime = (job[i].endTime.hour - job[i].arriveTime.hour) * 60 + job[i].endTime.minute - job[i].arriveTime.minute;
        job[i].valueWholeTime = (double)job[i].wholeTime / job[i].runTime;
        aveWholeTime += job[i].wholeTime;
        aveValueWholeTime += job[i].valueWholeTime;

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

    aveWholeTime /= jobNum;
    aveValueWholeTime /= jobNum;

    cout << "作业平均周转时间 T = " << aveWholeTime << "分钟" << endl;
    // cout << setiosflags(ios::fixed) << setprecision(3);
    cout << "作业带权平均周转时间 W = " << aveValueWholeTime << endl;
}

int main()
{
    initial();
    sort(job, job + jobNum, compareArrive);
    run();
    display();
    return 0;
}