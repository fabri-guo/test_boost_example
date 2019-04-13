#include "test_container.h"

#include <iostream>
#include "boost/array.hpp"
#include "boost/dynamic_bitset.hpp"
#include "boost/utility/binary.hpp"
#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "boost/bimap.hpp"
#include "boost/circular_buffer.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/any.hpp"
#include "boost/variant.hpp"

void test_container__()
{

}

void test_container::test_array()
{
	using namespace boost;
	{
		array<int, 10> ar;
		ar[0] = 1;
		ar.back() = 10;
		assert(ar[ar.max_size() - 1] == 10);
		ar.assign(777);

		for (auto& x : ar)
		{
			std::cout << x << ",";
		}
		int* p = ar.c_array();
		*(p + 5) = 255;
		std::cout << ar[5] << std::endl;
		ar.at(8) = 666;
		std::sort(ar.begin(), ar.end());
	}
	{
		array<std::string, 3> ar = { "ablice", "bob", "carl" };
		int a[10] = { 0 };
		array<int, 10> arr = { 0 };
		assert(std::equal(arr.begin(), arr.end(), a));
		array<std::string, 3> ar2 = { "recer" };
		assert(ar2.at(1).empty());
		//using namespace boost::assign;
		//array<int, 3> arr(list_of(2)(4)(6));
	}
}

void test_container::test_dynamic_bitset()
{
	using namespace boost;
	{
		dynamic_bitset<> db;
		db.resize(10, true);
		std::cout << db << std::endl;
		db.resize(5);
		std::cout << db << std::endl;
	}
	{
		dynamic_bitset<> db(5, BOOST_BINARY(01110));
		std::cout << db << std::endl;
		assert(db.size() == 5);
		db.clear();
		assert(db.empty() && db.size() == 0);
		assert(dynamic_bitset<>(64).num_blocks() == 1);
		assert(dynamic_bitset<>(65).num_blocks() == 2);
		dynamic_bitset<> db2(5, BOOST_BINARY(01001));
		db2.append(BOOST_BINARY(101));
		assert(db2.size() == sizeof(unsigned long) * 8 + 5);
		std::cout << db2 << std::endl;

		dynamic_bitset<> db3(4, BOOST_BINARY(1010));
		db3[0] &= 1;
		db3[1] ^= 1;
		std::cout << db3 << std::endl;
		dynamic_bitset<> db4(4, BOOST_BINARY(0101));
		assert(db3 > db4);
		std::cout << (db3 ^ db4) << std::endl;
		std::cout << (db3 | db4) << std::endl;
	}
	{
		dynamic_bitset<> db(4, BOOST_BINARY(0101));
		db.flip();
		assert(db.to_ulong() == BOOST_BINARY(1010));
		db.set();
		assert(!db.none());
		db.reset();
		assert(!db.any());
		db.set(1, 1);
		assert(db.count() == 1);
	}
	{
		dynamic_bitset<> db(5, BOOST_BINARY(001101));
		auto pos = db.find_first();
		assert(pos == 0);
		pos = db.find_next(pos);
		assert(pos == 2);
	}
}

void test_container::test_unordered()
{
	using namespace boost;

	unordered_set<int> s = { 1,2,3,4,5 };
	for (auto& x : s) {
		std::cout << x << std::endl;
	}
	std::cout << std::endl;
	std::cout << s.size() << std::endl;

	s.clear();
	std::cout << s.empty() << std::endl;

	s.insert(8);
	s.insert(45);

	std::cout << s.size() << std::endl;
	std::cout << *s.find(8) << std::endl;
	s.erase(45);
	//using namespace boost::assign;
	//unordered_set<int> us1 = list_of(1)(2)(3);
	//unordered_set<int> us2 = list_of(3)(2)(1);
	
}

void test_container::test_bimap()
{
	using namespace boost;
	{
		bimap<int, std::string> bm;
		bm.left.insert(std::make_pair(1, "111"));
		bm.left.insert(std::make_pair(2, "222"));

		bm.right.insert(std::make_pair("string", 10));
		bm.right.insert(std::make_pair("bimap", 20));

		for (auto pos = bm.left.begin(); pos != bm.left.end(); ++pos)
		{
			std::cout << "left{" << pos->first << "}=" << pos->second << std::endl;
		}
	}
}

void test_container::test_circular_buffer()
{
	using namespace boost;
	circular_buffer<int> cb(5);
	assert(cb.empty());
	cb.push_back(1);
	cb.push_front(2);
	assert(cb.front() == 2);
	cb.insert(cb.begin(), 3);

	for (auto pos = cb.begin(); pos != cb.end(); ++pos )
	{
		std::cout << *pos << std::endl;
	}
	std::cout << std::endl;
	cb.pop_front();
	assert(cb.size() == 2);
	cb.pop_back();
	assert(cb[0] = 2);
}

void test_container::test_tuple()
{
	using namespace boost;
	typedef tuple<int, std::string, double, int> my_tuple;
	my_tuple t1;
	my_tuple t2(1, "1234");
	my_tuple t3(t1);
	t2 = t3;

	int x = 10;
	tuple<int&> t4(x);
}

void test_container::test_any()
{
	using namespace boost;
	{
		any a(10);
		int n = any_cast<int>(a);
		assert(n == 10);
		any_cast<int&>(a) = 20;
		assert(any_cast<int>(a) == 20);
	}
	{
		any a1, a2(2.0);
		assert(any_cast<int*>(&a1) == nullptr);
		assert(any_cast<std::string*>(&a2) == nullptr);

		any a3(100);
		a3 = std::string("char");
		a3 = std::vector<std::vector<int>>();

		std::string* ps = new std::string("abc");
		a3 = ps;
		if (!a3.empty() && any_cast<std::string*>(a3))
		{
			std::cout << *any_cast<std::string*>(a3) << std::endl;
		}
	}
}

void test_container::test_variant()
{
	using namespace boost;
	variant<int, double, std::string> v;
	v = "123";
	std::cout << v << std::endl;

	std::cout << get<std::string>(v) << std::endl;

	typedef variant<int, double, std::string> var_t;
	var_t mm;
	assert(mm.type() == typeid(int));
	assert(mm.which() == 0);
	mm = "variant demo";
	std::cout << "get<std::string>(&v)" << std::endl;
}
