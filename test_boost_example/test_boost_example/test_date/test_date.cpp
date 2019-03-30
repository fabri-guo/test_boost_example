#include "test_date.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;

void test_date_func()
{
	test_date d1;
	d1.test_mdate();
}

void test_date::test_mdate()
{
	{
		date d1;
		date d2(2010, 1, 1);
		date d3(2010, Jan, 1);
		date d4(d2);
		assert(d1 == date(not_a_date_time));
		assert(d2 == d4);
		assert(d3 < d4);
	}
	
	{
		date d1 = from_string("1993-7-2");
		date d2 = from_string("2015/1/1");
		date d3 = from_undelimited_string("20011118");

		std::cout << day_clock::local_day() << std::endl;
		std::cout << day_clock::universal_day() << std::endl;
	}

	{
		date d1(neg_infin);
		date d2(pos_infin);
		date d3(not_a_date_time);
		date d4(max_date_time);
		date d5(min_date_time);
	}

	{
		// 异常  
		date d1(1399, 1, 1);		// 超过日期上线	
		date d2(10000, 1, 1);		// 超过日期下线
		date d3(2017, 2, 29);		// 不存在的日期
	}
	{
		date d1(2017, 6, 1);
		assert(d1.year() == 2017);
		assert(d1.month() == 6);
		assert(d1.day() == 1);

		date::ymd_type ymd = d1.year_month_day();
		assert(ymd.year == 2017);
		assert(ymd.month == 6);
		assert(ymd.day == 1);
		std::cout << d1.day_of_week() << std::endl;					// thu
		std::cout << d1.day_of_year() << std::endl;					// 152
		assert( d1.end_of_month() == date(2017, 6, 30));

		std::cout << date(2015, 1, 10).week_number() << std::endl;		//2
		std::cout << date(2016, 1, 10).week_number() << std::endl;		//1
		std::cout << date(2017, 1, 10).week_number() << std::endl;		//2
	}
	{

		assert(date(pos_infin).is_infinity());				// 无限日期
		assert(date(pos_infin).is_pos_infinity());			// 正无穷
		assert(date(neg_infin).is_neg_infinity());			// 负无穷
		assert(date(not_a_date_time).is_not_a_date());		// 无效日期
		assert(date(not_a_date_time).is_special());			// 特殊日期
		assert(!date(2017, 5, 31).is_special());
	}
	{
		date d(2017, 11, 23);
		std::cout << to_simple_string(d) << std::endl;			// 2017-jan-23
		std::cout << to_iso_string(d) << std::endl;				// 20171123
		std::cout << to_iso_extended_string(d) << std::endl;	// 2017-11-23
		std::cout << d << std::endl;							// 2017-jan-23

		//std::cin >> d;
		//std::cout << d;
	}
	{
		date d(2017, 5, 20);
		tm t = to_tm(d);
		assert(t.tm_hour == 0 && t.tm_min == 0);
		assert(t.tm_year == 117 && t.tm_mday == 20);

		date d2 = date_from_tm(t);
		assert(d == d2);
	}
}