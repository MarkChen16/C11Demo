// C11Demo.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//��Ա����Ĭ�ϳ�ʼֵ
	int i = 1000;
	int j = 2000;
	int k;
};

//֧��move������
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

	//move���캯��
	MyString(MyString &&s)
	{
		//���ٷ����µĿռ䣬ֱ��ָ�����е��ڴ�
		pc = s.pc;

		//��������ָ��ָ���µĿռ�
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

//����move�������ĺ���
template<class T>
void MoveSwap(T &a, T &b)
{
	T tmp(move(a));

	a = move(b);

	b = move(tmp);
}

int main()
{
	//VS2010����֧��C11��VS2012���ϰ汾��ȫ֧��C11��

	///////////////////ͳһ��ʼ�������������ʼ��
	int arr[3]{1, 2, 3};
	vector<int> iv{1, 2, 3};
	map<int, string> mp{ {1, "GQ"}, {2, "CF"}, {3, "WD"} };

	MyStruct *pMS = new MyStruct{1, 2};

	///////////////////����ָ�룺���õ�����ָ�룬��Ҫ����memory.h
	shared_ptr<MyStruct> spMS(pMS);

	///////////////////���ڶ�������������������Զ��жϱ��������ͣ�����ʱ��������
	int i = 10;
	auto ai = i + 200;
	auto pNMS = pMS;

	///////////////////����ʽ�����ͣ�����ʱ��������
	decltype(mp) mp2;
	decltype(i + 200) ai2;

	///////////////////��ָ�룺����ת��Ϊ����
	int *pOld = NULL;
	int *pNew = nullptr;

	///////////////////���ڷ�Χ��forѭ�������ӷ����ݵ�ѭ����
	for (auto iter : mp)
	{
		auto iter1 = iter.first;
		auto iter2 = iter.second;
	}

	///////////////////��ֵ���ú�move���������
	MyString a("abc"), b("123");
	MoveSwap<MyString>(a, b);

	///////////////////��ϣ��(����ӳ��) ��Ҫ����unordered_map.h���������ӳ�䣬����ɾ������Ч�ʸ��ߣ����ռ���Ҫ���ࣻ
	unordered_map<int, string> um;
	um.insert(make_pair(1, "FA"));
	um.insert(make_pair(3, "UF"));
	um.insert(make_pair(2, "FAE"));
	um[5] = "AFEI";

	///////////////////������ʽ���ж��ַ����Ƿ����ĳ�ָ�ʽ
	regex reg("b.?p.*k");
	int iRev = 0;
	iRev = regex_match("bopggk", reg);
	iRev = regex_match("boopgggk", reg);

	regex reg2("\\d{3}([a-zA-Z]+).(\\d{2}|N/A)\\s\\1");
	//	\\d��������{3}����3�Σ�
	//	([a-zA-Z])������ĸ����1�����ϣ�
	//	һ���ʾ�κ��ַ���
	//	\\d{2}�����������֣�(\\d{2}|N/A)��ʾҪô���������֣�Ҫô��N/A��
	//	\\s��ʾ�ո�
	//	\\1��ʾ��һ����
	iRev = regex_match("123Hello N/A Hello", reg2);
	iRev = regex_match("123Hello 12 hello", reg2);

	//////////////////Lambda���ʽ�����ٶ���ֻ��һ�εĺ�����
	auto rev = [](double a, double b)->double {return a + b; }(1, 2);	//���庯����ֱ�ӵ���

	auto ff = [=](double a, double b)->double {		//���崫ֵ�����ĺ��������������������
		return a*b;
	};
	rev = ff(10, 20);

	function<long(long)> fib = [&fib](long i)->long {	//����һ���ݹ麯��
		return i <= 2 ? 1 : fib(i - 1) + fib(i - 2);
	};
	cout << fib(10) << endl;	//ע�⣬�ݹ����̫�࣬ջ�����ܴ�Ч�ʲ��ߣ�

	double dA=1, dB=2;
	auto fSwap = [&a, &b](double &a, double &b) {	//����һ�����ò����ĺ�����������������
		double tmp = a;
		a = b;
		b = tmp;
	};
	fSwap(dA, dB);

	for_each(arr, arr+3, [=](int iter) {cout << iter << " "; });	//��Ϊ����ָ��������ú���
	cout << endl;

	///////////////////ǿ��ת��
	/*
	dynamic_cast: ����̬����ָ��ת��Ϊ������ָ�룬��鰲ȫ�����ʧ�ܷ���NULL(ֻ�����ڰ����麯���Ļ���)
	static_cast���Ƚ���Ȼ�͵ͷ��յ�ת����������ת��Ϊ����
	reinterpret_cast: �߷��յ�ת����ֱ�����ڴ�ӳ��
	const_cast����������ת��Ϊ�ǳ������������ܸı�������ͣ�ֻ��ȥ��const��
	*/

	///////////////////�쳣����
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
			cout << "int�쳣" << endl;
		}
		catch (double ex)
		{
			cout << "double�쳣" << endl;
		}
		catch (...)
		{
			cout << "�����쳣" << endl;

			throw exception("����δ���������쳣");		//���׳��쳣
		}
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}

    return 0;
}

