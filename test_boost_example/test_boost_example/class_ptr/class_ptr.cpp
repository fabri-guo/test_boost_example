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

		//scoped_ptr<std::string> sp2 = sp;				// error scoped_ptr 不能拷贝构造
		
		// 是使代码变得更加清晰简单, 没有增加多余的操作, 安全的同时保证了效率.
	}
}

void class_ptr::test_unique_ptr()
{
	using namespace std;
	{
		// 是C++标准中的定义的新的智能指针, 用来替代曾经的auto_ptr
		// 同样不支持拷贝构造和赋值, 比scoped_ptr多很功能 支持[]
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
		//shared_ptr<int> done_do_this(new int[10]);			//不能正确的释放内存

		typedef shared_ptr<std::string> sps_t;
		std::map<sps_t, int> m;
		sps_t sp(new std::string("one"));
		m[sp] = 11;
	}
	{
		shared_ptr<int> sp(new int(10));
		assert(sp.unique());			// 现在是shared_ptr是指针的唯一持有者

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
		vs v(10);			// 申明拥有10个元素的容器
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
	// 观测资源的引用计数
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

void intrusive_ptr_add_ref(counted_data* p)		// 增加引用计数
{
	++p->m_count;
}

void intrusive_ptr_release(counted_data* p)		// 减少引用计数
{
	if ( --p->m_count == 0)
	{
		delete p;
	}
}

/*
桥接模式

class sample : public impl
{
class impl;
shared_ptr<impl> p;
sample():p(new impl){}
}
*/

/*
工厂模式
class impl : public abstract
{
}

shared_ptr<abstract> create(){
	return make_shared<impl>();
}
*/

/*
释放调用函数
socket_t* ps = open_socket();
shared_ptr<socket_t> p(ps, close_socket);
*/

