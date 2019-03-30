#include "test_date.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;

void test_date_func()
{
	test_date d1;
	d1.test_mdate();

	d1.test_date_period();

	d1.test_time_duration();
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
		//assert(d3 < d4);
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
	//	date d2(10000, 1, 1);		// 超过日期下线
	//	date d3(2017, 2, 29);		// 不存在的日期
	//	date d1(1399, 1, 1);		// 超过日期上线	
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
	{
		days dd1(10), dd2(-100), dd3(255);
		assert(dd1 > dd2 && dd1 < dd3);
		assert(dd1 + dd2 == days(-90));
		assert((dd1 + dd3).days() == 265);
		assert(dd3 / 5 == days(51));

		weeks w(3);
		assert(w.days() == 21);
		months m(5);
		years y(2);
		months m2 = y + m;
		assert(m2.number_of_months() == 29);
		assert((y*2).number_of_years() == 4);

		date d1(2000, 1, 1), d2(2017, 11, 18);
		std::cout << d2 - d1 << std::endl;

		assert(d1 + (d2 - d1) == d2);

		d1 += days(10);
		assert(d1.day() == 11);
		d1 += months(2);
		assert(d1.month() == 3 && d1.day() ==11);
		d1 -= weeks(1);
		assert(d1.day() ==4);
		d2 -= years(10);
		assert(d2.year() ==d1.year() + 7);
	}

	{
		date d1(2017, 1, 1);
		date d2 = d1 + days(pos_infin);
		assert(d2.is_pos_infinity());
		d2 = d1 + days(not_a_date_time);
		d2 = date(neg_infin);
		days dd = d1 - d2;
		assert(dd.is_special() && !dd.is_negative());

		date d(2017, 3, 30);
		d -= months(1);					// 2-28
		d -= months(1);					// 1-31
		d += months(2);					// 3-31
		assert( d.day() == 31);			// 
	}
}

void test_date::test_date_period()
{		// 日期区间
	{
		date_period dp1(date(2017, 1, 1), days(20));
		date_period dp2(date(2017, 1, 1), date(2016, 1, 1));
		date_period dp3(date(2017, 3, 1), days(-20));

		date_period dp(date(2017, 1, 1), days(20));
		assert(!dp.is_null());

		assert(dp.begin().day() == 1);
		assert(dp.last().day() == 20);
		assert(dp.end().day() == 21);
		assert(dp.length().days() == 20);
	}

	{
		date_period dp1(date(2017, 1, 1), days(20));
		date_period dp2(date(2017, 2, 19), days(10));
		std::cout << dp1;

		assert( dp1 < dp2 );
	}
	{
		date_period dp(date(2017, 1, 1), days(20));
		dp.shift(days(3));
		assert(dp.begin().day() == 4);
		assert(dp.length().days() == 20);
		dp.expand(days(3));
		assert(dp.begin().day() == 1);
		assert(dp.length().days() == 26);
	}
	{
		// 日期区间运算
		date_period dp(date(2010, 1, 1), days(20));
		assert(dp.is_after(date(2009, 12, 1)));
		assert(dp.is_before(date(2010, 2, 1)));
		assert(dp.contains(date(2010, 1, 10)));

		date_period dp2(date(2010, 1, 5), days(10));
		assert(dp.contains(dp2));
		assert(dp.intersects(dp2));
		assert(dp.intersection(dp2) == dp2);

		date_period dp3(date(2010, 1, 21),days(5));
		assert(!dp3.intersects(dp2));
		assert(dp3.intersection(dp2).is_null());

		assert(dp.is_adjacent(dp3));
		assert(!dp.intersects(dp3));
	}
	{
		date_period dp1(date(2010, 1, 1), days(20));
		date_period dp2(date(2010, 1, 5), days(10));
		date_period dp3(date(2010, 2, 1), days(5));
		date_period dp4(date(2010, 1, 15), days(10));

		assert(dp1.contains(dp2) && dp1.merge(dp2) == dp1);
		assert(!dp1.intersects(dp3) && dp1.merge(dp3).is_null());
		assert(dp1.intersects(dp2) && dp1.merge(dp4).end() == dp4.end());
		assert(dp1.span(dp3).end() == dp3.end());
	}
	{
		date d(2007, 9, 28);
		day_iterator d_iter(d);

		assert(d_iter == d);

		++d_iter;
		assert(d_iter == date(2007, 9, 28));
		year_iterator y_iter(*d_iter, 10);
		assert(y_iter == d + days(1));
		++y_iter;
		assert(y_iter->year() == 2017);

		day_iterator iter(day_clock::local_day());
		++iter;
		//iter += 5;
		//std::advance(iter, 5);
	}
	{
		date d(2017, 1, 23);
		date d_start(d.year(), d.month(), 1);
		date d_end = d.end_of_month();
		for ( day_iterator d_iter(d_start); d_iter != d_end; ++d_iter)
		{
			std::cout << *d_iter << " " << d_iter->day_of_week() << std::endl;
		}
	}
	{
		date d(2017, 1, 23);
		date d18year = d + years(18);
		std::cout << d18year << " is " << d18year.day_of_week() << std::endl;
		int count = 0;
		for (day_iterator d_iter(date(d18year.year(), 1, 1)); d_iter != d18year.end_of_month(); ++d_iter)
		{
			if ( d_iter->day_of_week() == Sunday)
			{
				++count;
			}
		}
		std::cout << "total " << count << " Sunday." << std::endl;
		count = 0;
		for ( month_iterator m_iter(date(d18year.year(), 1, 1)); m_iter < date(d18year.year() + 1, 1, 1); ++m_iter)
		{
			count += m_iter->end_of_month().day();
		}
		std::cout << " total " << count << " day of year " << std::endl;

		std::cout << (gregorian_calendar::is_leap_year(d18year.year()) ? 365 : 366);
	}
}

void test_date::test_time_duration()
{
	{
		time_duration td(1, 10, 30, 1000);
		time_duration td1(1, 60, 60, 1000 * 1000 * 6 + 1000);
		hours h(1);
		minutes m(10);
		seconds s(30);
		milliseconds ms(1);

		time_duration dt = h + m + s + ms;
		time_duration dt1 = hours(2) + seconds(10);

		time_duration dt2 = duration_from_string("1:10:30:001");
	}
	{
		time_duration td(1, 10, 30, 1000);
		assert(td.hours() == 1 && td.minutes() == 10 && td.seconds() == 30);
		assert(td.total_seconds() == 1* 3600 + 10* 60+ 30);
		assert(td.total_milliseconds() == td.total_seconds() * 1000 + 1);
		assert(td.fractional_seconds() == 1000);
		hours h(-10);
		assert(h.is_negative());
		time_duration h2 = h.invert_sign();
		assert( !h2.is_negative() && h2.hours() == 10 );

		time_duration td1(not_a_date_time);
		assert(td1.is_special() && td1.is_not_a_date_time());
		time_duration td2(neg_infin);
		assert(td2.is_negative() && td2.is_neg_infinity());

		time_duration td3 = hours(1);
		time_duration td4 = hours(2) + minutes(30);
		assert(td3 < td4);
		assert( (td3 + td4).hours() ==3);
		assert((td3-td4).is_negative());
		assert(td3*5==td4*2);
		assert((td1/2).minutes() == td2.minutes());
		time_duration td5(1, 10, 30, 1000);
		std::cout << to_simple_string(td5) << std::endl;
		std::cout << to_iso_string(td5) << std::endl;
	}
}

void test_date::test_ptime()
{
	using namespace boost::gregorian;
	{
		ptime p(date(2017, 7, 7), hours(1));
		ptime p1 = time_from_string("2017-7-7 01:00:00");
		ptime p2 = from_iso_string("20170707T01000");

		ptime p3 = second_clock::local_time();
		ptime p4 = microsec_clock::universal_time();
		std::cout << p3 << std::endl;
	}
	{
		ptime p1(not_a_date_time);							// 无效时间
		assert(p1.is_not_a_date_time());					
		ptime p2(pos_infin);								// 正无限时间	
		assert(p2.is_special() && p2.is_pos_infinity());	
	}
	{
		ptime p(date(2010, 3, 20), hours(12)+minutes(30));
		date d = p.date();
		time_duration td = p.time_of_day();
		assert(d.month() == 3 && d.day() == 20);
		assert(td.total_seconds() == 12 * 3600 + 30 * 60);
		ptime p1(date(2010, 3, 20), hours(12)+minutes(30));
		ptime p2 = p1 + hours(3);
		assert(p1 < p2);
		assert(p2- p1 == hours(3));
		p2 += months(1);
		assert(p2.date().month() == 4);

		std::cout << to_simple_string(p2) << std::endl;
		std::cout << to_iso_string(p2) << std::endl;
		std::cout << to_iso_extended_string(p2) << std::endl;

	}
	{
		ptime p(date(2017, 5, 20), hours(14));
		tm t = to_tm(p);
		assert(t.tm_year == 117 && t.tm_hour == 14);
		assert(ptime_from_tm(t) == p);
		ptime p1 = from_time_t(std::time(0));
		assert(p.date() == day_clock::local_day());
		std::cout << to_time_t(p1);
	}
	{
		ptime p(date(2017, 1, 1), hours(12));
		time_period tp1(p, hours(8));
		time_period tp2(p + hours(8), hours(1));
		assert(tp1.end() == tp2.begin() && tp1.is_adjacent(tp2));
		assert(!tp1.intersects(tp2));

		tp1.shift(hours(1));
		assert(tp1.is_after(p));
		assert(tp1.intersects(tp2));
		tp2.expand(hours(10));
		assert(tp2.contains(p) && tp2.contains(tp1));

		ptime p3(date(2017, 5, 31), hours(10));
		for ( time_iterator t_iter(p, minutes(10)); t_iter < p + hours(1); ++t_iter)
		{
			std::cout << *t_iter << std::endl;
		}
	}
}
