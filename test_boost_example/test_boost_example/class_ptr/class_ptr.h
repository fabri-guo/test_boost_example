#pragma once
void test_class_ptr();


class class_ptr
{
public:
	void test_scoped_ptr();

	void test_unique_ptr();

	void test_shared_ptr();

	void test_weak_ptr();

	void test_intrusive_ptr();
};

// template<class T>
// namespace boost {
// 	class shared_ptr<T>;
// }


//void print_func(boost::shared_ptr<int> p);

