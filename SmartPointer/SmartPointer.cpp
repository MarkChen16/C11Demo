// SmartPointer.cpp : 定义控制台应用程序的入口点。
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
	weak_ptr<C> pc_value;		//其中一个智能指针换成weak_ptr，解决share_ptr互相引用的问题；
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
	//auto_ptr、unique_ptr、share_ptr、weak_ptr

	//////auto_ptr自动智能指针，已被弃用，不能进行复制操作，赋值等同于移动资源管理权；析构函数默认调用delete，而且无法重载；
	{
		auto_ptr<A> ptr1(new A(1));
		auto_ptr<A> ptr2(new A(2));

		assert(ptr1->getvalue() == 1);
		assert(ptr2->getvalue() == 2);

		ptr1 = ptr2;
		assert(ptr2.get() == NULL); // now ptr2 is NULL
	}

	//////unique_ptr唯一智能指针，用于替代auto_ptr，复制的话需要显示move；
	{
		unique_ptr<A> ptr3(new A(1));
		unique_ptr<A> ptr4(new A(2));

		//ptr3 = ptr4;		//错误
		ptr3 = move(ptr4);	//正确, ptr2变成NULL
		assert(ptr4 == NULL);

		unique_ptr<A[]> ptr5(new A[2]{ 3, 4 });	//array
		assert(ptr5[0].getvalue() == 3);
		assert(ptr5[1].getvalue() == 4);
	}

	////////shared_ptr智能指针，它允许共享资源，使用引用计数记录资源被几个指针共享。
	///////调用release（），析构函数或者被复制赋值时，原来管理的资源的引用计数都会减1，计数为0时释放资源。
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

	////////weak_ptr弱指针，解决share_ptr互相引用时无法析构的问题；
	////////解决方法是将pa_value和pb_value中任意一个声明为 weak_ptr类型。
	{
		shared_ptr<C> pc(new C());
		shared_ptr<B> pb(new B());

		pc->pb_value = pb;	//互相引用
		pb->pc_value = pc;

		//assert(pb.use_count() == 2);
		//assert(pc.use_count() == 2);
	}

    return 0;
}

