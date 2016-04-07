#include "minicsv.h"

struct Color
{
	Color() : r(0), g(0), b(0){}
	Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b){}
	int r;
	int g;
	int b;
};

inline std::ostream & operator<<(std::ostream & os, Color const & val)
{
	csv::ostringstream csv_os;
	csv_os.set_delimiter(',', "^^");
	csv_os << val.r << val.g << val.b;
	return os << csv_os.get_text();
}

inline std::istream & operator>>(std::istream & is, Color & val)
{
	std::string str;
	is >> str;
	csv::istringstream csv_is(str.c_str());
	csv_is.set_delimiter(',', "^^");
	if(csv_is.read_line())
	{
		csv_is >> val.r >> val.g >> val.b;
	}

	return is;
}