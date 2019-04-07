#include "test_string.h"

#include <iostream>

#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include "boost/utility/string_ref.hpp"
#include "boost/algorithm/algorithm.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/xpressive/xpressive.hpp"

void test_option_string()
{

}


void test_string::test_lexical_cast()
{
	using namespace boost;
	int x = lexical_cast<int>("100");
	long y = lexical_cast<long>("2000");
	float pai = lexical_cast<float>("3.1416935");
	double e = lexical_cast<double>("2.71928");
	double r = lexical_cast<double>("1.414,x", 5);
	std::cout << x << y << pai << e << r << std::endl;

	std::string str = lexical_cast<std::string>(456);
	std::cout << str << std::endl;
	std::cout << lexical_cast<std::string>(0.618) << std::endl;
	std::cout << lexical_cast<std::string>(0x10) << std::endl;

	std::cout << lexical_cast<bool>("1") << std::endl;

	try {
		std::cout << lexical_cast<int>("0x100");
		std::cout << lexical_cast<double>("helloworld");
		std::cout << lexical_cast<long>("1000L");
		std::cout << lexical_cast<bool>("false") << std::endl;
	}
	catch (bad_lexical_cast & e)
	{
		std::cout << "error:" << e.what() << std::endl;
	}
	{
		//assert(stoi("42") == 42);
	}
	//assert(num_vaild<double>("3.14"));
}

void test_string::test_format()
{
	using namespace boost;
	{
		std::cout << format("%s:%d+%d=%d\n") % "sum" % 1 % 2 % (1 + 2);
		format fmt("(%1+ %2%) * 2%2% = %3%\n");
		fmt % 2 % 4;
		fmt % ((2 + 5) * 5);
		std::cout << fmt.str();
	}
	{
		//using namespace boost::io::group;
		format fmt("%050d\n%-8.3f\n% 10s\n%05X\n");
		std::cout << fmt % 62 % 2.345 % "123456789" % 48;

		const format fmt1("%10d %020.0f %010X %10.5se\n");
		std::cout << format(fmt1) % 62 % 2.346 % 255 % 0.6677;

		format fmt2("%1% %2% %3% %2% %1% \n");
		std::cout << fmt2 % 1 % 2 % 3;
		fmt2.bind_arg(2, 10);
		std::cout << fmt2 % 1 % 3;
		fmt2.clear();

		std::cout << fmt2 % io::group(std::ios::showbase, std::ios::oct, 111) % 333;
		fmt2.clear_binds();

		fmt2.modify_item(1, io::group(std::ios::hex, std::ios::right, std::ios::showbase, 8));
		std::cout << fmt2 % 49 % 20 % 100;
	}
}

void test_string::test_string_ref()
{
	using namespace boost;
	const char* ch = "days of future past";
	std::string str(ch);
	string_ref s1(ch);
	string_ref s2(str);

	assert(s1 == s2 && s1 == ch && s2 == str);
	string_ref s3(ch, 4);
	assert(s3 == str.substr(0, 4));

	string_ref s4, s5;
	s4 = ch;
	s5 = str;
	assert(s4 == s5);

	const char* ch1 = "apple iphone ipad";
	string_ref str_ref(ch);
	assert(!str.empty());
	assert(str.size() == strlen(ch));
	for (auto& x : str)
	{
		std::cout << x;
	}
	std::cout << std::endl;
	assert(str.find('i') == 6);
	auto substr = str.substr(6, 6);
	assert(substr == "iphone");
	std::string s = str_ref.to_string();
	assert(str_ref == s);
	str_ref.clear();
	assert(str_ref.empty());

	str_ref.remove_prefix(6);
	assert(str_ref.starts_with("IP"));

	str_ref.remove_prefix(5);
	assert(str_ref.ends_with("one"));
}

void test_string::test_algorithm()
{
	using namespace boost;
	std::string str("readme.txt");
	if (ends_with(str, "txt"))
	{
		std::cout << to_upper_copy(str) + " UPPER" << std::endl;
		assert(ends_with(str, "txt"));
	}
	replace_first(str, "readme", "followme");
	std::cout << str << std::endl;
	std::vector<char> v(str.begin(), str.end());
	std::vector<char> v2 = to_upper_copy(erase_first_copy(v, "txt"));
	for (auto ch : v2)
	{
		std::cout << ch;
	}
	std::string ss("fireemblem herose\n");
	std::cout << to_upper_copy(ss);
	to_lower(ss);
	std::cout << ss << std::endl;

	assert(iends_with(ss, "herose"));
	assert(ends_with(ss, "herose"));
	assert(starts_with(ss, "Fireemble"));
	assert(contains(ss, "le"));
	std::string ss2 = to_lower_copy(ss);
	assert(iequals(ss, ss2));
	std::string s3("power suit");
	assert(ilexicographical_compare(ss, s3));
	assert(all(ss2.substr(0, 5), is_lower()));

	std::string str1("samus"), str2("Samus");
	assert(!is_equal()(str, str2));
	assert(is_less()(str1, str2));
	assert(!is_equal()(str1, string_ref(str2)));

	format fmt("|%s|\n");
	std::string str4 = " samus aran  ";
	std::cout << fmt % trim_copy(str4);
	std::cout << fmt % trim_left_copy(str4);
	trim_right(str4);
	std::cout << fmt % str4;
	
	std::string str5 = "2017 happy new year!!";
	std::cout << fmt % trim_left_copy_if(str5, is_digit());
	std::cout << fmt % trim_right_copy_if(str5, is_punct());
	std::cout << fmt % trim_copy_if(str5, is_punct() || is_digit() || is_space());

	format fmt1("|%s|. pos = %d\n");
	std::string str6 = "long long ago. there was a king";
	iterator_range<std::string::iterator> rge;
	rge = find_first(str, "long");
	std::cout << fmt1 % rge % (rge.begin() - str.begin());
	rge = ifind_first(str, "long");
	std::cout << fmt1 % rge % (rge.begin() - str.begin());
	rge = find_nth(str, "ng", 2);
	std::cout << fmt1 % rge % (rge.begin() - str.begin());
	rge = find_head(str, 4);
	std::cout << fmt1 % rge % (rge.begin() - str.begin());
	rge = find_tail(str, 5);
	std::cout << fmt1 % rge % (rge.begin() - str.begin());
	rge = find_first(str, "samus");
	assert(rge.empty() && !rge);

	std::string pstr = "Samus beat the monster.\n";
	std::cout << replace_first_copy(pstr, "Samus", "samus");
	replace_last(pstr, "best", "kill");
	std::cout << pstr;
	replace_tail(pstr, 9, "ridley\n");
	std::cout << pstr;
	std::cout << ierase_all_copy(pstr, "samus");
	std::cout << replace_nth_copy(pstr, "l", 1, "L");
	std::cout << erase_tail_copy(pstr, 8);

	std::string pstr1 = "Samus,Link,Zelda::Mario-Luigi+zelda";
	std::deque<std::string> d;
	ifind_all(d, pstr1, "zELDA");
	assert(d.size() == 2);
	for (auto& x : d)
	{
		std::cout << "[" << x << "]";
	}
	std::list<iterator_range<std::string::iterator>> l;
	split(l, pstr1, is_any_of(",.:-="));
	for (auto& x : l)
	{
		std::cout << "[" << x << "]";
	}
	l.clear();
	split(l, pstr1, is_any_of(",.:-="), token_compress_on);
	for (auto& x : l)
	{
		std::cout << "[" << x << "]";
	}
	auto pos = make_find_iterator(pstr, first_finder("samus", is_equal()));
}

void test_string::test_xpressive()
{
	using namespace boost::xpressive;
	{
		cregex reg = cregex::compile("a.c");
		assert(regex_match("abc", reg));
		assert(regex_match("a+c", reg));
		assert(!regex_match("ac", reg));
		assert(!regex_match("abd", reg));
	}
	{
		cregex reg = cregex::compile(R"---(\d{6}(1|2)\d{3}(0|1)\d[0-3]\d\d{3}(X|\d))---", icase);
		assert(regex_match("99999999991123566", reg));
		cmatch what;
		assert(regex_match("999124536363", what, reg));
		for (auto& x : what)
		{
			std::cout << "[" << x << "]";
		}
		std::cout << std::endl;
		std::cout << "date:" << what[1] << what[3] << what[5] << std::endl;
	}
	{
		// regex_match  <==> regex_search
		std::string str("readme.txt");
		sregex start_reg = sregex::compile("^re.*");
		sregex end_reg = sregex::compile(".*text$");
		assert(regex_match(str, start_reg));
		assert(regex_match(str, end_reg));
	}
	{
		std::string str("readme.txt");
		sregex reg1 = sregex::compile("(.*)(me)");
		sregex reg2 = sregex::compile("(t)(.)(t)");
		std::cout << regex_replace(str, reg1, "manual") << std::endl;
		std::cout << regex_replace(str, reg1, "$lyou") << std::endl;
		std::cout << regex_replace(str, reg2, "$&$&") << std::endl;
		std::cout << regex_replace(str, reg2, "lN$3") << std::endl;
		str = regex_replace(str, reg2, "$l$3");
		std::cout << str << std::endl;

		std::string str1("2010 happy new year");
		sregex reg3 = sregex::compile("^(\\d| )*");
		sregex reg4 = sregex::compile("!*$");
		std::cout << regex_replace(str1, reg1, "") << std::endl;
		std::cout << regex_replace(str1, reg2, "") << std::endl;
		str = regex_replace(str, reg1, "$200");
		std::cout << str << std::endl;
	}
}