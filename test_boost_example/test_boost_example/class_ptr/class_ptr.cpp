#include "class_ptr.h"
#include <iostream>
#include <map>
#include <vector>

#include "boost/smart_ptr.hpp"
#include "boost/smart_ptr/make_unique.hpp"
#include "boost/weak_ptr.hpp"
void test_class_ptr()
{
	class_ptr cp;
	cp.test_scoped_ptr();
	cp.test_unique_ptr();
	cp.test_shared_ptr();
}


class shared
{
private:
	boost::shared_ptr<int> m_p;
public:
	shared(boost::shared_ptr<int> p) :m_p(p){}
	void print()
	{
		std::cout << "count:" << m_p.use_count() << " v= " << *m_p << std::endl;
	}
};

void print_func(boost::shared_ptr<int> p)
{
	std::cout << "count:" << p.use_count() << " v= " << *p << std::endl;
}

void class_ptr::test_scoped_ptr()
{
	using namespace boost;
	{
		scoped_ptr<std::string> sp(new std::string("text"));
		assert(sp);
		assert(sp != nullptr);

		std::cout << *sp << std::endl;
		std::cout << sp->size() << std::endl;

		//scoped_ptr<std::string> sp2 = sp;				// error scoped_ptr ���ܿ�������
		
		// ��ʹ�����ø���������, û�����Ӷ���Ĳ���, ��ȫ��ͬʱ��֤��Ч��.
	}
}

void class_ptr::test_unique_ptr()
{
	using namespace std;
	{
		// ��C++��׼�еĶ�����µ�����ָ��, �������������auto_ptr
		// ͬ����֧�ֿ�������͸�ֵ, ��scoped_ptr��ܹ��� ֧��[]
		unique_ptr<int> up(new int);
		assert(up);
		*up = 10;
		std::cout << *up << std::endl;
		up.reset();
		assert(!up);

		auto p = std::make_unique<int>(10);
		assert(p && *p == 10);
	}
}

void class_ptr::test_shared_ptr()
{
	using namespace boost;
	{
		shared_ptr<int> spi( new int);
		assert(spi);
		*spi = 254;

		shared_ptr<std::string> sps(new std::string("smart"));
		assert(sps->size() == 5);
		//shared_ptr<int> done_do_this(new int[10]);			//������ȷ���ͷ��ڴ�

		typedef shared_ptr<std::string> sps_t;
		std::map<sps_t, int> m;
		sps_t sp(new std::string("one"));
		m[sp] = 11;
	}
	{
		shared_ptr<int> sp(new int(10));
		assert(sp.unique());			// ������shared_ptr��ָ���Ψһ������

		shared_ptr<int> sp2 = sp;
		assert(sp == sp2 && sp.use_count() == 2);
		*sp2 = 100;
		assert(*sp == 100);
		sp.reset();
		assert(!sp);
	}

	{
		shared_ptr<int> p(new int(100));
		shared s1(p), s2(p);
		s1.print();
		s2.print();

		*p = 20;
		print_func(p);
		s1.print();
	}

	{
		auto sp = make_shared<std::string>("make_ptr");
		auto spv = make_shared<std::vector<int>>((10, 2));
		assert(spv->size() == 10);

		typedef std::vector<shared_ptr<int>> vs;	
		vs v(10);			// ����ӵ��10��Ԫ�ص�����
		int i = 0;
		for (auto pos = v.begin(); pos != v.end(); ++pos)
		{
			(*pos) = make_shared<int>(++i);
			std::cout << *(*pos) << ", ";
		}
		std::cout << std::endl;

		shared_ptr<int> p = v[9];
		*p = 100;
		std::cout << *v[9] << std::endl;
	}
}

void class_ptr::test_weak_ptr()
{
	// �۲���Դ�����ü���
	using namespace boost;
	shared_ptr<int> sp(new int(10));
	assert(sp.use_count() == 1);
	weak_ptr<int> wp(sp);
	assert(wp.use_count() == 1);
	if ( !wp.expired())
	{
		shared_ptr<int> sp2 = wp.lock();
		*sp2 = 100;
		assert(wp.use_count() == 2);
	}
	assert(wp.use_count() == 1);
	sp.reset();
	assert(wp.expired());
	assert(!wp.lock());
}

struct counted_data
{
	int m_count = 0;
};

void class_ptr::test_intrusive_ptr()
{
	using namespace boost;
	typedef intrusive_ptr<counted_data> counted_ptr;
	counted_ptr p(new counted_data);
	assert(p);
	assert(p->m_count == 1);
	counted_ptr p2(p);
	assert(p->m_count == 2);
	counted_ptr weak_p(p.get(), false);
	assert(weak_p->m_count == 2);
	p2.reset();
	assert(!p2);
	assert(p->m_count == 1);
}

void intrusive_ptr_add_ref(counted_data* p)		// �������ü���
{
	++p->m_count;
}

void intrusive_ptr_release(counted_data* p)		// �������ü���
{
	if ( --p->m_count == 0)
	{
		delete p;
	}
}

/*
�Ž�ģʽ

class sample : public impl
{
class impl;
shared_ptr<impl> p;
sample():p(new impl){}
}
*/

/*
����ģʽ
class impl : public abstract
{
}

shared_ptr<abstract> create(){
	return make_shared<impl>();
}
*/

/*
�ͷŵ��ú���
socket_t* ps = open_socket();
shared_ptr<socket_t> p(ps, close_socket);
*/

