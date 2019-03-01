// C11Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include <string>
#include <memory>

#include <vector>
#include <map>

#include <unordered_map>

#include <regex>

#include <functional>

#include <exception>

using namespace std;

struct MyStruct
{
	int i;
	int j;
};

class A
{
public:
	A():k(3000)
	{

	}

private:
	//成员变量默认初始值
	int i = 1000;
	int j = 2000;
	int k;
};

//支持move的类型
class MyString
{
public:
	MyString()
	{
		pc = new char[1]{ 0 };
	}

	MyString(const char *pcStr)
	{
		pc = new char[strlen(pcStr)+1];
		strcpy(pc, pcStr);
	}

	MyString(const MyString &s)
	{
		pc = new char[strlen(s.pc)+1];
		strcpy(pc, s.pc);
	}

	MyString &operator=(const MyString &s)
	{
		if (pc != s.pc)
		{
			delete[] pc;
			pc = new char[strlen(s.pc)+1];
			strcpy(pc, s.pc);
		}

		return *this;
	}

	//move构造函数
	MyString(MyString &&s)
	{
		//不再分配新的空间，直接指向已有的内存
		pc = s.pc;

		//将参数的指针指向新的空间
		s.pc = new char[1]{ 0 };
	}

	MyString &operator=(MyString &&s)
	{
		if (pc != s.pc)
		{
			delete[] pc;
			pc = s.pc;

			s.pc = new char[1]{ 0 };
		}

		return *this;
	}

	~MyString()
	{
		delete[] pc;
	}

	char *pc;
};

//采用move做交换的函数
template<class T>
void MoveSwap(T &a, T &b)
{
	T tmp(move(a));

	a = move(b);

	b = move(tmp);
}

int main()
{
	//VS2010部分支持C11、VS2012以上版本完全支持C11；

	///////////////////统一初始化方法：数组初始化
	int arr[3]{1, 2, 3};
	vector<int> iv{1, 2, 3};
	map<int, string> mp{ {1, "GQ"}, {2, "CF"}, {3, "WD"} };

	MyStruct *pMS = new MyStruct{1, 2};

	///////////////////智能指针：内置的智能指针，需要引入memory.h
	shared_ptr<MyStruct> spMS(pMS);

	///////////////////用于定义变量，编译器可以自动判断变量的类型：编译时定义类型
	int i = 10;
	auto ai = i + 200;
	auto pNMS = pMS;

	///////////////////求表达式的类型：运行时定义类型
	decltype(mp) mp2;
	decltype(i + 200) ai2;

	///////////////////空指针：不能转换为整数
	int *pOld = NULL;
	int *pNew = nullptr;

	///////////////////基于范围的for循环：更加方便快捷的循环体
	for (auto iter : mp)
	{
		auto iter1 = iter.first;
		auto iter2 = iter.second;
	}

	///////////////////右值引用和move：减少深拷贝
	MyString a("abc"), b("123");
	MoveSwap<MyString>(a, b);

	///////////////////哈希表(无序映射) 需要引入unordered_map.h：不排序的映射，插入删除查找效率更高，但空间需要更多；
	unordered_map<int, string> um;
	um.insert(make_pair(1, "FA"));
	um.insert(make_pair(3, "UF"));
	um.insert(make_pair(2, "FAE"));
	um[5] = "AFEI";

	///////////////////正则表达式：判断字符串是否符合某种格式
	regex reg("b.?p.*k");
	int iRev = 0;
	iRev = regex_match("bopggk", reg);
	iRev = regex_match("boopgggk", reg);

	regex reg2("\\d{3}([a-zA-Z]+).(\\d{2}|N/A)\\s\\1");
	//	\\d代表数字{3}出现3次；
	//	([a-zA-Z])代表字母出现1次以上；
	//	一点表示任何字符；
	//	\\d{2}代表两个数字；(\\d{2}|N/A)表示要么是两个数字，要么是N/A；
	//	\\s表示空格；
	//	\\1表示第一个项
	iRev = regex_match("123Hello N/A Hello", reg2);
	iRev = regex_match("123Hello 12 hello", reg2);

	//////////////////Lambda表达式：快速定义只用一次的函数体
	auto rev = [](double a, double b)->double {return a + b; }(1, 2);	//定义函数并直接调用

	auto ff = [=](double a, double b)->double {		//定义传值参数的函数，返回两个数字相乘
		return a*b;
	};
	rev = ff(10, 20);

	function<long(long)> fib = [&fib](long i)->long {	//定义一个递归函数
		return i <= 2 ? 1 : fib(i - 1) + fib(i - 2);
	};
	cout << fib(10) << endl;	//注意，递归调用太多，栈开销很大，效率不高；

	double dA=1, dB=2;
	auto fSwap = [&a, &b](double &a, double &b) {	//定义一个引用参数的函数，交换两个数字
		double tmp = a;
		a = b;
		b = tmp;
	};
	fSwap(dA, dB);

	for_each(arr, arr+3, [=](int iter) {cout << iter << " "; });	//作为函数指针参数调用函数
	cout << endl;

	///////////////////强制转换
	/*
	dynamic_cast: 将多态基类指针转换为派生类指针，检查安全性如果失败返回NULL(只能用于包含虚函数的基类)
	static_cast：比较自然和低风险的转换，浮点型转换为整型
	reinterpret_cast: 高风险的转换，直接做内存映射
	const_cast：常量变量转换为非常量变量，不能改变变量类型，只是去掉const；
	*/

	///////////////////异常处理
	try
	{
		try
		{
			//auto i = 1;
			//auto i = 1.0;
			auto i = "1";

			throw i;
		}
		catch (int ex)
		{
			cout << "int异常" << endl;
		}
		catch (double ex)
		{
			cout << "double异常" << endl;
		}
		catch (...)
		{
			cout << "其他异常" << endl;

			throw exception("发现未处理其他异常");		//再抛出异常
		}
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}

    return 0;
}

