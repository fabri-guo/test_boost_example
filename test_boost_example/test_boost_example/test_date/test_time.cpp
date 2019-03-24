#include "test_time.h"

#include "boost/timer.hpp"
#include "boost/progress.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
//#include <stdlib.h>
#include <windows.h>


using namespace boost;

void time_date()
{
	test_time t;
	t.test_timer();

	t.test_progress_timer();

	t.test_progress_display();

}


void test_time::test_timer()
{
	timer t;
	std::cout << "max timespan:" << t.elapsed_max() / 3600 << "h" << std::endl;
	std::cout << "min timespan:" << t.elapsed_min() << "s" << std::endl;
	std::cout << "now time elapsed:" << t.elapsed() << std::endl;

}

void test_time::test_progress_timer()
{
	{
		progress_timer t;
		Sleep(500);
	}

	std::stringstream ss;
	{
		progress_timer t(ss);
		Sleep(20);
	}
	std::cout << ss.str() << std::endl;
}

void test_time::test_progress_display()
{
	std::vector<std::string> v(100);
	std::ofstream fs("./test.txt");
	progress_display pd(v.size());
	for (auto& x : v)
	{
		fs << x << std::endl;
		++pd;
	}

	{
		std::vector<std::string> v(100, "aaa");
		v[10] = "";
		v[24] = "";
		std::ofstream fs("/test1.txt");
		progress_display pd(v.size());
		for (auto pos = v.begin(); pos != v.end(); ++pos)
		{
			fs << *pos << std::endl;
			++pd;
			if (pos->empty())
			{
				std::cout << "null string # " << (pos - v.begin()) << std::endl;
			}
		}
	}
}
