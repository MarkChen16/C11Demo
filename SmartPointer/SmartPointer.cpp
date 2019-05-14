// SmartPointer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <assert.h>

using namespace std;

class A {
public:
	A(int i) :ival(i) {
		printf("create  %d \n", ival);
	}
	~A() {
		printf("delete %d \n", ival);
	}

	int getvalue() { return ival; }
private:
	int ival;
};


class C;
class B
{
public:
	//shared_ptr<C> pc_value;
	weak_ptr<C> pc_value;		//����һ������ָ�뻻��weak_ptr�����share_ptr�������õ����⣻
	~B()
	{
		cout << "B delete\n";
	}
};
class C
{
public:
	shared_ptr<B> pb_value;
	~C()
	{
		cout << "C delete\n";
	}
};



int main()
{
	//auto_ptr��unique_ptr��share_ptr��weak_ptr

	//////auto_ptr�Զ�����ָ�룬�ѱ����ã����ܽ��и��Ʋ�������ֵ��ͬ���ƶ���Դ����Ȩ����������Ĭ�ϵ���delete�������޷����أ�
	{
		auto_ptr<A> ptr1(new A(1));
		auto_ptr<A> ptr2(new A(2));

		assert(ptr1->getvalue() == 1);
		assert(ptr2->getvalue() == 2);

		ptr1 = ptr2;
		assert(ptr2.get() == NULL); // now ptr2 is NULL
	}

	//////unique_ptrΨһ����ָ�룬�������auto_ptr�����ƵĻ���Ҫ��ʾmove��
	{
		unique_ptr<A> ptr3(new A(1));
		unique_ptr<A> ptr4(new A(2));

		//ptr3 = ptr4;		//����
		ptr3 = move(ptr4);	//��ȷ, ptr2���NULL
		assert(ptr4 == NULL);

		unique_ptr<A[]> ptr5(new A[2]{ 3, 4 });	//array
		assert(ptr5[0].getvalue() == 3);
		assert(ptr5[1].getvalue() == 4);
	}

	////////shared_ptr����ָ�룬����������Դ��ʹ�����ü�����¼��Դ������ָ�빲��
	///////����release�����������������߱����Ƹ�ֵʱ��ԭ���������Դ�����ü��������1������Ϊ0ʱ�ͷ���Դ��
	{
		shared_ptr<A> ptr6(new A(1));
		shared_ptr<A> ptr7(new A(2));
		assert(ptr6->getvalue() == 1);
		assert(ptr6.use_count() == 1);

		assert(ptr7->getvalue() == 2);
		assert((ptr7.use_count()) == 1);

		ptr6 = ptr7;
		assert(ptr6.use_count() == 2);
		assert(ptr7.use_count() == 2);
		assert(ptr6->getvalue() == 2);
		assert(ptr7->getvalue() == 2);
	}

	////////weak_ptr��ָ�룬���share_ptr��������ʱ�޷����������⣻
	////////��������ǽ�pa_value��pb_value������һ������Ϊ weak_ptr���͡�
	{
		shared_ptr<C> pc(new C());
		shared_ptr<B> pb(new B());

		pc->pb_value = pb;	//��������
		pb->pc_value = pc;

		//assert(pb.use_count() == 2);
		//assert(pc.use_count() == 2);
	}

    return 0;
}

