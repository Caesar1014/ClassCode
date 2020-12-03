#include <iostream>
#include <string>
#define threshold 2//�Զ�����ֵΪ2

using namespace std;

struct space//�����ڴ�ṹ��
{
    int begin;//��ʼ��ַ
    int length;//����
    //int end;//ĩ��ַ
    string name;//��������
    space* next;
};
typedef space* Space;

int total=0;//�ڴ������ܳ���
int used=0;//�ڴ��������ó���

void init(Space empty,Space work,int length)//��ʼ��,ȫ��Ϊ��������ռ����Ϊ��
{
    empty->begin=0;//�������׵�ַ��Ϊ�ڴ��׵�ַ
    empty->length=length;//����������Ϊ�����ڴ�ռ�
    empty->next=NULL;
    work->next=NULL;
}

void firstSuit(Space empty)//������Ӧ�㷨
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//�����޿���������
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->begin>temp2->next->begin)//��temp2��ָ���ݵ���ʼ��ַ����temp2->next,�򽻻�ָ����ָ
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

void bestSuit(Space empty)//�����Ӧ�㷨
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//�����޿���������
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->length<temp2->next->length)//��temp2��ָ���ݵĳ���С��temp2->next,�򽻻�ָ����ָ
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

void worstSuit(Space empty)//���Ӧ�㷨
{
    space* temp1=empty;
    space* temp2=empty;
    if(empty==NULL)//�����޿���������
        return;
    for(temp1=empty;temp1->next!=NULL;temp1=temp1->next)
    {
        for(temp2=empty;temp2->next!=NULL;temp2=temp2->next)
        {
            if(temp2->length>temp2->next->length)//��temp2��ָ���ݵĳ��ȴ���temp2->next,�򽻻�ָ����ָ
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

void sort(Space empty,int choice)//������ѡ�㷨��empty�����������
{
    switch(choice)
    {
    case 1:
        firstSuit(empty);//������Ӧ�㷨
        break;
    case 2:
        bestSuit(empty);//�����Ӧ�㷨
        break;
    case 3:
        worstSuit(empty);//���Ӧ�㷨
        break;
    default:
        break;
    }
}

void displayEmpty(Space empty)//������ӡempty��
{
    cout<<endl<<endl<<"empty"<<endl<<endl;
    if(empty==NULL)//�޿��ÿռ�
    {
        cout<<"�޿��ÿռ�"<<endl;
        return;
    }
    space* temp=new space;
    temp=empty;
    cout<<"begin"<<"\t"<<"length"<<"\t"<<"״̬"<<endl;//��ӡ��ͷ
    while(temp)//δ������
    {
        cout<<temp->begin<<"\t"<<temp->length<<"\t"<<"δ����"<<endl;//��ӡ��ָ����������Ϣ
        temp=temp->next;//ָ��������һ��
    }
}

void displayWork(Space work)
{
    cout<<endl<<endl<<"work"<<endl<<endl;
    if(work==NULL)//�޿ɻ����ڴ�
    {
        cout<<"�޿ɻ����ڴ�"<<endl;
        return;
    }
    space* temp=new space;
    temp=work->next;//Ϊʲô��������next
    cout<<"begin"<<"\t"<<"length"<<"\t"<<"������"<<endl;//��ӡ��ͷ
    while(temp)//δ������
    {
        cout<<temp->begin<<"\t"<<temp->length<<"\t"<<temp->name.c_str()<<endl;//��ӡ��ָ����������Ϣ��c_str()��������һ��ָ������C�ַ�����ָ�볣��, ���ݲ���
        temp=temp->next;//ָ��������һ��
    }
}

void allocate(Space empty, Space work)//�����ڴ�
{
    int choice;//����㷨ѡ��
    int length;//��ǰ���������Ҫ�ĳ���
    int m=0;
    char name[30];
    cout<<"ѡ���㷨��"<<endl;
    cout<<"1.������Ӧ�㷨"<<endl;
    cout<<"2.�����Ӧ�㷨"<<endl;
    cout<<"3.���Ӧ�㷨"<<endl;
    cin>>choice;
    sort(empty,choice);//������ѡ�㷨��empty��������
    displayEmpty(empty);//������ӡempty��
    cout<<"��ǰ���������Ҫ�ĳ��ȣ�"<<endl;
    cin>>length;
    if(length>total-used)//��ǰ������Ҫ���ȴ����ڴ�����ʣ�೤��
    {
        cout<<"ʣ��ռ䲻��"<<endl;
        return;
    }
    cout<<"���뵱ǰ�����������"<<endl;
    cin>>name;


    space* temp=new space;
	space* preTemp=new space;//������������preTemp������
	temp=empty;
	preTemp=empty;
	while(temp)
	{
		if(temp->length>length)//����ǰ��ָ�����������������Ҫ����
        {
			if(temp->length-length<=threshold)//����ֵС�ڵ�����ֵ
			{
				if(temp==empty)//�ж��Ƿ��ǵ�һ�����
                {
					space* prog=new space;//prog�������
					prog->begin=temp->begin;//���ڲ�ֵ<=��ֵ�������������ڴ���ʼ��ַ��Ϊ��ǰ����������ʼ��ַ
					prog->length=temp->length;//���ڲ�ֵ<=��ֵ�������������ڴ泤�ȼ�Ϊ��ǰ�������鳤��
					//prog->end=temp->end;//���ڲ�ֵ<=��ֵ�������������ڴ�ĩ��ַ��Ϊ��ǰ��������ĩ��ַ
					prog->name=(string)name;//����������ָ�������
					space* workList=new space;
					workList=work;
					while(workList->next)//�ҵ�work��ĩβ
					{
						workList=workList->next;
					}
					prog->next=workList->next;//���·����ڴ�ĳ���ռ���������work��ĩβ
					workList->next=prog;
					empty=empty->next;
				}
				else
				{
					space* prog=new space;
					preTemp->next=temp->next;//������������
					prog->begin=temp->begin;//���ڲ�ֵ<=��ֵ�������������ڴ���ʼ��ַ��Ϊ��ǰ����������ʼ��ַ
					prog->length=temp->length;//���ڲ�ֵ<=��ֵ�������������ڴ泤�ȼ�Ϊ��ǰ�������鳤��
					//prog->end=temp->end;//���ڲ�ֵ<=��ֵ�������������ڴ泤�ȼ�Ϊ��ǰ�������鳤��
					prog->name=(string)name;//����������ָ�������
					space* workList=new space;
					workList=work;
					while(workList->next)//�ҵ�work��ĩβ
					{
						workList=workList->next;
					}
					prog->next=workList->next;//���·����ڴ�ĳ���ռ���������work��ĩβ
					workList->next=prog;
				}
			}
			else//����ֵ��С����ֵ
            {
				space* prog=new space;
				prog->begin=temp->begin;//���ڲ�ֵ>��ֵ�������������ڴ���ʼ��ַ��Ϊ��ǰ����������ʼ��ַ
				prog->length=length;//���ڲ�ֵ>��ֵ�������������ڴ泤�ȼ�Ϊ��������
				temp->begin=temp->begin+length;//���ڲ�ֵ>��ֵ����ǰ��������ָ����ʣ���ֵ���ʼ��ַ��Ϊ�ָ�ǰ��ʼ��ַ+���򳤶�
				temp->length=temp->length-length;//���ڲ�ֵ>��ֵ����ǰ��������ָ����ʣ���ֵĳ��ȼ�Ϊ�ָ�ǰ����-���򳤶�
				prog->name=(string)name;//����������ָ�������
				space *workList=new space;
				workList=work;
				while(workList->next)//�ҵ�work��ĩβ
				{
					workList=workList->next;
				}
				prog->next=workList->next;//���·����ڴ�ĳ���ռ���������work��ĩβ
				workList->next=prog;
			}
			break;
		}
		temp=temp->next;//����ǰָ��������鲻���������Ҫ���ȣ���ָ����һ����������
		if(m==1)//������������
			preTemp=preTemp->next;
		m=1;
	}
	displayEmpty(empty);//��ӡ���º��empty��
	cout<<endl;
	displayWork(work);//��ӡ���º��work��
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
        if(temp->next)//������з�ĩβ��������
        {
            if(temp->begin+temp->length==temp->next->begin)//����ǰ���������ĩ��ַ�����¸������������ʼ��ַ��˵��������ϲ�
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
    cout<<"������ճ�������"<<endl;
    cin>>name;
    space* tempWork=new space;
    space* preTempWork=new space;
    space* tempEmpty=new space;
    tempWork=work->next;//����������Ϊʲô��work��ı������Ǵ�work->next��ʼ
    preTempWork=work;
    tempEmpty=empty;
    while(tempWork)
    {
        if(!tempWork->name.compare(name))
        {
            flag=1;//�ҵ�Ŀ�����
            begin=tempWork->begin;
            length=tempWork->length;
            len=getLength(empty);
            preTempWork->next=tempWork->next;//��work��ɾ���ýڵ�
            while(tempEmpty)//���ڻ����ڴ棬�����µĿ������飬����empty��״̬
            {
                space* newEmpty=new space;
                newEmpty->begin=begin;
                newEmpty->length=length;
                newEmpty->name="δ����";
                if(len>=2)//����ǰempty�����Ƿ����һ����������
                {
                    if(tempEmpty->begin>begin)//��֤tempEmpty����ָ��empty����׸���������
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
                else//����ǰempty����ֻ��һ����������
                {
                    if(tempEmpty->begin>begin)//��֤tempEmpty����ָ��empty����׸���������
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
                tempEmpty=tempEmpty->next;//��������Ǳ��ֶδ洢��
            }
        }
        tempWork=tempWork->next;
        preTempWork=preTempWork->next;
    }
    if(!flag)//δ�ҵ���Ӧ����
        cout<<"δ�ҵ���Ӧ����"<<endl;
    else
    {
        firstSuit(empty);//��empty������
        check(empty);//�����������ܷ�ϲ�
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
    cout<<"����ڴ�������"<<endl;
    cin>>length;
    total=length;
    init(empty,work,length);

    while(1)
    {
        cout<<"Menu"<<endl;
        cout<<"1.��ӳ���"<<endl;
        cout<<"2.�����ڴ�"<<endl;
        cout<<"3.�˳�"<<endl;
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


