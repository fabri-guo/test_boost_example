#include "class_ptr.h"
#include "boost/smart_ptr.hpp"

void test_class_ptr()
{

}


class_ptr::class_ptr()
{
}


class_ptr::~class_ptr()
{
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

		scoped_ptr<std::string> sp2 = sp;				// error scoped_ptr 不能拷贝构造
		
		// 是使代码变得更加清晰简单, 没有增加多余的操作, 安全的同时保证了效率.
	}
}

void class_ptr::test_unique_ptr()
{
	using namespace boost;
	{
		unique_ptr<int> up(new int);
		assert(up);
		*up = 10;
		std::cout << *up << std::endl;
		up.reset();
		assert(!up);
	}
}
