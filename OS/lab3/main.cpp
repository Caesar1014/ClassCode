#include <iostream>
#include <string>
#define threshold 2//自定义阈值为2

using namespace std;

struct space//定义内存结构体
{
    int begin;//起始地址
    int length;//长度
    //int end;//末地址
    string name;//程序名字
    space* next;
};
typedef space* Space;

int total=0;//内存区域总长度
int used=0;//内存区域已用长度

void init(Space empty,Space work,int length)//初始化,全部为空闲区，占用区为空
{
    empty->begin=0;//空闲区首地址即为内存首地址
    empty->length=length;//空闲区长度为整个内存空间
    empty->next=NULL;
    work->next=NULL;
}

void firstSuit(Space empty)//首先适应算法
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//若已无空闲区可用
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->begin>temp2->next->begin)//若temp2所指内容的起始地址高于temp2->next,则交换指针所指
            {
                int begin=temp2->begin;
                int length=temp2->length;
                //int end=temp2->end;
                temp2->begin=temp2->next->begin;
                temp2->length=temp2->next->length;
                //temp2->end=temp2->next->end;
                temp2->next->begin=begin;
                temp2->next->length=length;
                //temp2->next->end=end;
            }
        }
    }
}

void bestSuit(Space empty)//最佳适应算法
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//若已无空闲区可用
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->length<temp2->next->length)//若temp2所指内容的长度小于temp2->next,则交换指针所指
            {
                int begin=temp2->begin;
                int length=temp2->length;
                //int end=temp2->end;
                temp2->begin=temp2->next->begin;
                temp2->length=temp2->next->length;
                //temp2->end=temp2->next->end;
                temp2->next->begin=begin;
                temp2->next->length=length;
                //temp2->next->end=end;
            }
        }
    }
}

void worstSuit(Space empty)//最坏适应算法
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//若已无空闲区可用
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->length>temp2->next->length)//若temp2所指内容的长度大于temp2->next,则交换指针所指
            {
                int begin=temp2->begin;
                int length=temp2->length;
                //int end=temp2->end;
                temp2->begin=temp2->next->begin;
                temp2->length=temp2->next->length;
                //temp2->end=temp2->next->end;
                temp2->next->begin=begin;
                temp2->next->length=length;
                //temp2->next->end=end;
            }
        }
    }
}

void sort(Space empty,int choice)//根据所选算法对empty链表进行排序
{
    switch(choice)
    {
    case 1:
        firstSuit(empty);//首先适应算法
        break;
    case 2:
        bestSuit(empty);//最佳适应算法
        break;
    case 3:
        worstSuit(empty);//最坏适应算法
        break;
    default:
        break;
    }
}

void displayEmpty(Space empty)//遍历打印empty表
{
    cout<<endl<<endl<<"empty"<<endl<<endl;
    if(empty==NULL)//无可用空间
    {
        cout<<"无可用空间"<<endl;
        return;
    }
    space* temp=new space;
    temp=empty;
    cout<<"begin"<<"\t"<<"length"<<"\t"<<"状态"<<endl;//打印表头
    while(temp)//未遍历完
    {
        cout<<temp->begin<<"\t"<<temp->length<<"\t"<<"未分配"<<endl;//打印所指空闲区块信息
        temp=temp->next;//指针移向下一个
    }
}

void displayWork(Space work)
{
    cout<<endl<<endl<<"work"<<endl<<endl;
    if(work==NULL)//无可回收内存
    {
        cout<<"无可回收内存"<<endl;
        return;
    }
    space* temp=new space;
    temp=work->next;//为什么？？？是next
    cout<<"begin"<<"\t"<<"length"<<"\t"<<"程序名"<<endl;//打印表头
    while(temp)//未遍历完
    {
        cout<<temp->begin<<"\t"<<temp->length<<"\t"<<temp->name.c_str()<<endl;//打印所指空闲区块信息，c_str()函数返回一个指向正规C字符串的指针常量, 内容不变
        temp=temp->next;//指针移向下一个
    }
}

void allocate(Space empty, Space work)//分配内存
{
    int choice;//标记算法选择
    int length;//当前申请程序需要的长度
    int m=0;
    char name[30];
    cout<<"选择算法："<<endl;
    cout<<"1.首先适应算法"<<endl;
    cout<<"2.最佳适应算法"<<endl;
    cout<<"3.最坏适应算法"<<endl;
    cin>>choice;
    sort(empty,choice);//根据所选算法对empty链表排序
    displayEmpty(empty);//遍历打印empty表
    cout<<"当前申请程序需要的长度："<<endl;
    cin>>length;
    if(length>total-used)//当前申请需要长度大于内存区域剩余长度
    {
        cout<<"剩余空间不足"<<endl;
        return;
    }
    cout<<"输入当前申请程序名："<<endl;
    cin>>name;


    space* temp=new space;
	space* preTemp=new space;//？？？？？？preTemp的作用
	temp=empty;
	preTemp=empty;
	while(temp)
	{
		if(temp->length>length)//若当前所指空闲区块满足程序需要长度
        {
			if(temp->length-length<=threshold)//若差值小于等于阈值
			{
				if(temp==empty)//判断是否是第一个结点
                {
					space* prog=new space;//prog代表程序
					prog->begin=temp->begin;//由于差值<=阈值，分配给程序的内存起始地址即为当前空闲区块起始地址
					prog->length=temp->length;//由于差值<=阈值，分配给程序的内存长度即为当前空闲区块长度
					//prog->end=temp->end;//由于差值<=阈值，分配给程序的内存末地址即为当前空闲区块末地址
					prog->name=(string)name;//将输入的名字赋给程序
					space* workList=new space;
					workList=work;
					while(workList->next)//找到work表末尾
					{
						workList=workList->next;
					}
					prog->next=workList->next;//将新分配内存的程序占用情况接在work表末尾
					workList->next=prog;
					empty=empty->next;
				}
				else
				{
					space* prog=new space;
					preTemp->next=temp->next;//？？？？？？
					prog->begin=temp->begin;//由于差值<=阈值，分配给程序的内存起始地址即为当前空闲区块起始地址
					prog->length=temp->length;//由于差值<=阈值，分配给程序的内存长度即为当前空闲区块长度
					//prog->end=temp->end;//由于差值<=阈值，分配给程序的内存长度即为当前空闲区块长度
					prog->name=(string)name;//将输入的名字赋给程序
					space* workList=new space;
					workList=work;
					while(workList->next)//找到work表末尾
					{
						workList=workList->next;
					}
					prog->next=workList->next;//将新分配内存的程序占用情况接在work表末尾
					workList->next=prog;
				}
			}
			else//若差值不小于阈值
            {
				space* prog=new space;
				prog->begin=temp->begin;//由于差值>阈值，分配给程序的内存起始地址即为当前空闲区块起始地址
				prog->length=length;//由于差值>阈值，分配给程序的内存长度即为程序本身长度
				temp->begin=temp->begin+length;//由于差值>阈值，当前空闲区块分割后所剩部分的起始地址即为分割前起始地址+程序长度
				temp->length=temp->length-length;//由于差值>阈值，当前空闲区块分割后所剩部分的长度即为分割前长度-程序长度
				prog->name=(string)name;//将输入的名字赋给程序
				space *workList=new space;
				workList=work;
				while(workList->next)//找到work表末尾
				{
					workList=workList->next;
				}
				prog->next=workList->next;//将新分配内存的程序占用情况接在work表末尾
				workList->next=prog;
			}
			break;
		}
		temp=temp->next;//若当前指向空闲区块不满足程序需要长度，则指向下一个空闲区块
		if(m==1)//？？？？？？
			preTemp=preTemp->next;
		m=1;
	}
	displayEmpty(empty);//打印更新后的empty表
	cout<<endl;
	displayWork(work);//打印更新后的work表
}

int getLength(Space empty)
{
    int length=0;
    space* temp=new space;
    temp=empty;
    while(temp)
    {
        length++;
        temp=temp->next;
    }
    return length;
}

void check(Space empty)
{
    space* temp=new space;
    space* preTemp=new space;
    temp=empty;
    preTemp=empty;
    while(temp)
    {
        if(temp->next)//检查所有非末尾空闲区块
        {
            if(temp->begin+temp->length==temp->next->begin)//若当前空闲区块的末地址等于下个空闲区块的起始地址，说明相邻则合并
            {
                temp->length=temp->length+temp->next->length;
                temp->next=temp->next->next;
            }
        }
        temp=temp->next;
    }
    temp=preTemp->next;
    while(temp)
    {
        if(temp->next==NULL)
        {
            if(temp->begin==preTemp->begin+preTemp->length)
            {
                preTemp->length=preTemp->length+temp->length;
                preTemp->next=NULL;
                break;
            }
        }
        temp=temp->next;
        preTemp=preTemp->next;
    }
}

void recycle(Space empty,Space work)
{
    int begin;
    int length;
    string name;
    int len;
    int flag=0;
    displayWork(work);
    cout<<"输入回收程序名字"<<endl;
    cin>>name;
    space* tempWork=new space;
    space* preTempWork=new space;
    space* tempEmpty=new space;
    tempWork=work->next;//？？？？？为什么对work表的遍历都是从work->next开始
    preTempWork=work;
    tempEmpty=empty;
    while(tempWork)
    {
        if(!tempWork->name.compare(name))
        {
            flag=1;//找到目标程序
            begin=tempWork->begin;
            length=tempWork->length;
            len=getLength(empty);
            preTempWork->next=tempWork->next;//从work表删除该节点
            while(tempEmpty)//由于回收内存，增加新的空闲区块，更新empty表状态
            {
                space* newEmpty=new space;
                newEmpty->begin=begin;
                newEmpty->length=length;
                newEmpty->name="未分配";
                if(len>=2)//若当前empty表中是否多于一个空闲区块
                {
                    if(tempEmpty->begin>begin)//保证tempEmpty总是指向empty表的首个空闲区块
                    {
                        newEmpty->begin=tempEmpty->begin;
                        newEmpty->length=tempEmpty->length;
                        tempEmpty->begin=begin;
                        tempEmpty->length=length;
                        newEmpty->next=tempEmpty->next;
                        tempEmpty->next=newEmpty;
                        break;
                    }
                }
                else//若当前empty表中只有一个空闲区块
                {
                    if(tempEmpty->begin>begin)//保证tempEmpty总是指向empty表的首个空闲区块
                    {
                        newEmpty->begin=tempEmpty->begin;
                        newEmpty->length=tempEmpty->length;
                        tempEmpty->begin=begin;
                        tempEmpty->length=length;
                        newEmpty->next=tempEmpty->next;
                        tempEmpty->next=newEmpty;
                    }
                    else
                    {
                        newEmpty->next=tempEmpty->next;
                        tempEmpty->next=newEmpty;
                    }
                    break;
                }
                tempEmpty=tempEmpty->next;//程序可能是被分段存储的
            }
        }
        tempWork=tempWork->next;
        preTempWork=preTempWork->next;
    }
    if(!flag)//未找到对应程序
        cout<<"未找到对应程序"<<endl;
    else
    {
        firstSuit(empty);//对empty表排序
        check(empty);//检查空闲区块能否合并
    }
    displayEmpty(empty);
    displayWork(work);
}

int main()
{
    int length;
    int choice;
    Space empty;
    Space work;
    empty=new space;
    work=new space;
    cout<<"最大内存容量："<<endl;
    cin>>length;
    total=length;
    init(empty,work,length);

    while(1)
    {
        cout<<"Menu"<<endl;
        cout<<"1.添加程序"<<endl;
        cout<<"2.回收内存"<<endl;
        cout<<"3.退出"<<endl;
        cin>>choice;
        switch(choice)
        {
        case 1:
            allocate(empty,work);
            break;
        case 2:
            recycle(empty,work);
            break;
        case 3:
            exit(0);
        }
    }
    return 0;
}


