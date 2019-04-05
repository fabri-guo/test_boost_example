#include "test_tool.h"

#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <map>
#include <queue>

#include "boost/core/ignore_unused.hpp"
#include "boost/optional.hpp"
#include "boost/assign.hpp"
#include "boost/logic/tribool.hpp"
#include "boost/exception/exception.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
/*
class noncopyable
{
protected:
	noncopyable(){}				// 默认构造和析构是保护的  可继承
	~noncopyable(){}
private:
	noncopyable(const noncopyable&) = delete;	// 私有化拷贝构造和拷贝赋值
	const noncopyable& operator= (const noncopyable&) = delete;
};

*/

void test_tool_func()
{
	test_tool tt;
	tt.test_assign();
	tt.test_ignore_unused(10, 10);
	tt.test_optional();
	tt.test_stack();
	tt.test_tribool();
}

void test_tool::test_ignore_unused(int a, int c)
{
	using namespace boost;
	int i;
	ignore_unused(a, c, i);
}

void test_tool::test_optional()
{
	using namespace boost;
	optional<int> op0;
	optional<int> op1(none);
	assert(!op0);

	assert(op0 == op1);
	assert(op1.value_or(253) == 253);
	std::cout << op1.value_or_eval([]() {return 873; }) << std::endl;

	optional<std::string> ops("test");
	std::cout << *ops << std::endl;

	ops.emplace("monado", 3);		// 创建三个字符串内容 没有拷贝代价
	assert(*ops == "mon");

	std::vector<int> v(10);
	optional<std::vector<int>&> opv(v);
	assert(opv);
	opv->push_back(5);
	assert(opv->size() == 11);
	opv = none;
	assert(!opv);

	auto x = make_optional(5);
	assert(*x = 5);
	auto y = make_optional<double>((*x > 10, 1.0));
	assert(!y);
}

void test_tool::test_assign()
{
	using namespace boost::assign;
	{
		std::vector<int> v;
		v += 1, 2, 3, 4, 5, 6;
		std::set<std::string> s;
		s += "c", "cpp", "lua", "swift";

		std::map<int, std::string> m;
		m += std::make_pair(1, "one"), std::make_pair(2, "two");
	}
	
	{
		std::vector<int> v;
		push_back(v)(1)(2)(3)(4);

		std::list<std::string> l;
		push_front(l)("C")("cpp")("lua")("swift");

		//std::forward_list<std::string>
		push_front(l)("martix")("reload");

		std::map<int, std::string> m;
		insert(m)(1, "one")(2, "two");
	}
	{
		std::vector<int> v;
		push_back(v),1, 2, 3, 4, 5;

		push_back(v)(5), 7, 74 / 8, (9), 10;
		std::deque<std::string> d;
		push_front(d)() = "breath", "of", "the", "wild";
		assert(d.size() == 5);

		std::vector<int> v1 = list_of(1)(2)(3)(4);
		std::deque<std::string> d1 = (list_of("power")("bomb"), "phazon", "suit");

		std::set<int> s = (list_of(10), 20, 30, 40, 50);

		std::map<int, std::string> m = list_of(std::make_pair(1, "one"))(std::make_pair(2, "two"));
	}
	{
		std::map<int, int> m1 = map_list_of(1, 2)(3, 4)(5, 6);
		std::map<int, std::string> m2 = map_list_of(1, "one")(2, "two");
	}
	{
		std::vector<int> v = list_of(1).repeat(3, 2)(3)(4)(5);
		std::deque<int> d;
		push_front(d).range(v.begin(), v.begin()+ 5);
	}
}

void test_tool::test_stack()
{
	using namespace boost::assign;
	std::stack<int> stk = (list_of(1), 2, 3).to_adapter();
	stk += 4, 5, 6;
	for (; !stk.empty();)
	{
		std::cout << stk.top() << " ";
		stk.pop();
	}
	std::queue<std::string> q = (list_of("chain")("us")("uk")).repeat(2, "russia").to_adapter();
	push(q)("germany");
	for (; !q.empty();)
	{
		std::cout << q.front() << " ";
		q.pop();
	}
	std::priority_queue<double> pq = (list_of(1.2323), 1.566, 1.666, 2.6666).to_adapter();
	push(pq), 3.14, 2.7812;
	for ( ; !pq.empty();)
	{
		std::cout << pq.top() << " ";
		pq.pop();
	}

	int a = 1, b = 2, c = 3;
	std::vector<int> v = ref_list_of<3>(a)(b)(c);
	assert(v.size() == 3);
}

void test_tool::test_tribool()
{
	using namespace boost;
	tribool tb(true);
	tribool tb2(!tb);
	if (tb)
	{
		std::cout << "true" << std::endl;
	}
	tb2 = indeterminate;
	assert(indeterminate(tb2));
	std::cout << tb2 << std::endl;

	if(indeterminate(tb2))
	{
		std::cout << "indeterminate" << std::endl;
	}

	std::cout << (tb2 || true) << std::endl;
	std::cout << (tb2 && false) << std::endl;
}

void test_tool::test_uuid()
{
	using namespace boost::uuids;
	{
		uuid u;
		assert(uuid::static_size() == 16);
		assert(u.size() == 16);
		std::vector<unsigned int> v(16, 7);
		std::copy(v.begin(), v.end(), u.begin());
		assert(u.data[0] == u.data[1] && u.data[15] == 7);
		std::cout << u << std::endl;

		std::fill_n(u.data + 10, 6, 8);
		std::cout << u << std::endl;
	}
	{
		uuid u;
		//std::fill_n(u.begin(), u.end(), 0xab);
		assert(!u.size());
		assert(u.variant() == u.variant_rfc_4122);
		assert(u.version() == u.version_unknown);
		std::cout << u << std::endl;

		std::memset(u.data, 0, 16);
		assert(u.is_nil());
	}

	{
		uuid u1, u2;
		std::fill_n(u1.begin(), u1.size(), 0xab);
		std::fill_n(u2.begin(), u2.size(), 0x10);
		assert(u1 != u2 && u1 > u2);
		u2.data[0] = 0xff;
		assert(u1 < u2);
		std::memset(u1.data, 0, 16);
		std::memset(u2.data, 0, 16);
		assert(u1 == u2);
	}

	{
		//{480C1AD1-CF49-4E3A-8D4A-FE8D3E4C21B9}
		uuid u = nil_generator()();
		assert(u.is_nil());
		u = nil_uuid();
		assert(u.is_nil());
	}
	{
		string_generator sgen;
		uuid u1 = sgen("0123456789abcdefg0123456789abcdef");
		std::cout << u1 << std::endl;
		uuid u2 = sgen("480C1AD1-CF49-4E3A-8D4A-FE8D3E4C21B9");
		std::cout << u2 << std::endl;
		uuid u3 = sgen(L"{480C1AD1-CF49-4E3A-8D4A-FE8D3E4C21B9}");
		std::cout << u3 << std::endl;
	}
	{
		uuid www_xx_com = string_generator()("{480C1AD1-CF49-4E3A-8D4A-FE8D3E4C21B9}");
		name_generator ngen(www_xx_com);
		uuid u1 = ngen("mario");
		assert(!u1.is_nil() && u1.version() == uuid::version_name_based_sha1);
		uuid u2 = ngen("link");
		std::cout << u2 << std::endl;
	}
	{
		random_generator rgen;
		uuid u = rgen();
		assert(u.version() == uuid::version_random_number_based);
		std::cout << u << std::endl;
	}
}

struct my_exception : virtual std::exception, virtual boost::exception
{

};
