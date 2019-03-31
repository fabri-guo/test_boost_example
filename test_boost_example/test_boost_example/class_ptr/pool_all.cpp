#include "pool_all.h"
#include <iostream>
#include <string>
#include <vector>

#include "boost/pool/pool.hpp"
#include "boost/pool/object_pool.hpp"
#include "boost/pool/singleton_pool.hpp"
#include "boost/pool/pool_alloc.hpp"

void test_pool_all()
{
	pool_all a;
	a.test_object_pool();
	a.test_pool();
	a.test_singleton_pool();
}

void pool_all::test_pool()
{
	using namespace boost;
	pool<> p1(sizeof(int));				// һ���ɷ���int���ڴ��
	int* p = static_cast<int*>(p1.malloc());	// �����void* ת������Ҫ������
	assert(p1.is_from(p));
	p1.free(p);						// �ͷ��ڴ�ط�����ڴ��
	for (int i = 0; i < 100; ++i)	// ��������������ڴ�
	{
		p1.ordered_malloc(10);
	}
}				// �ڴ�ض�������, ���з�����ڴ������ﱻ�ͷ�

struct demo_class
{
public:
	int a, b, c;
	demo_class(int x = 1, int y = 2, int z = 3):a(x), b(y), c(z){}
};

void pool_all::test_object_pool()
{
	using namespace boost;
	object_pool<demo_class> p1;					// �����ڴ��
	auto p = p1.malloc();						// ����һ��ԭʼ�ڴ��
	assert(p1.is_from(p));
	assert(p->a != 1 || p->b != 2 || p->c != 3);
	p = p1.construct(7, 8, 9);					// ����һ������, ���Դ��ݲ���
	object_pool<std::string> pls;				// ����һ������string �����ڴ��
	for ( int i = 0; i < 10; i++)				// ������������ڴ�
	{
		std::string* ps = pls.construct("hello object_pool");
		std::cout << *ps << std::endl;
	}
}

struct pool_tag 
{
	int a;
	int b;
	int c;
};

void pool_all::test_singleton_pool()
{
	using namespace boost;
	typedef singleton_pool<pool_tag, sizeof(int)> sp1;
	int* p = (int*)sp1::malloc();
	assert(sp1::is_from(p));
	sp1::release_memory();

	std::vector<int, pool_allocator<int> > v;

	v.push_back(10);
	std::cout << v.size() << std::endl;
}


