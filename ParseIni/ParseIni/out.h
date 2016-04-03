#include <string>
#include <map>
#include <fstream>
#include <stdexcept>
#include "minicsv.h"
class MyIniFile
{
private:
	std::map<std::string, std::string> m_NameValueMap;
public:
	bool ParseFile(const std::string& file, /* OUT */ std::string& error)
	{
		m_NameValueMap.clear();
		csv::ifstream is(file.c_str());
		is.set_delimiter('=', "$$");
		if (is.is_open())
		{
			while (is.read_line())
			{
				std::string name;
				std::string value;
				is >> name;
				value = is.get_rest_of_line();
				m_NameValueMap[name] = value;
			}
			is.close();
			return Validate(error);
		}
		else
		{
			std::ostringstream oss;
			oss << "File cannot be opened:" << file;
			error = oss.str();
			return false;
		}
	}
	bool Exists(const std::string& name)
	{
		typedef std::map<std::string, std::string> Map;
		typedef Map::const_iterator MapConstIter;
		MapConstIter it = m_NameValueMap.find(name);
		return it != m_NameValueMap.end();
	}
	std::string StartDate()
	{
		if(!Exists("StartDate"))
		{
			throw std::runtime_error("StartDate does not exist");
		}
		std::string val;
		std::istringstream iss(m_NameValueMap["StartDate"]);
		iss >> val;
		return val;
	}
	bool IsValidStartDate()
	{
		bool ret = false;
		try
		{
			ret = true;
		}
		catch(std::exception&)
		{
		}
		return ret;
	}
	std::string EndDate()
	{
		if(!Exists("EndDate"))
		{
			throw std::runtime_error("EndDate does not exist");
		}
		std::string val;
		std::istringstream iss(m_NameValueMap["EndDate"]);
		iss >> val;
		return val;
	}
	bool IsValidEndDate()
	{
		bool ret = false;
		try
		{
			ret = Exists("StartDate")&&EndDate()>=StartDate();;
		}
		catch(std::exception&)
		{
		}
		return ret;
	}
	int Alpha()
	{
		if(!Exists("Alpha"))
		{
			throw std::runtime_error("Alpha does not exist");
		}
		int val;
		std::istringstream iss(m_NameValueMap["Alpha"]);
		iss >> val;
		return val;
	}
	bool IsValidAlpha()
	{
		bool ret = false;
		try
		{
			ret = Alpha()>=0&&Alpha()<=255;
		}
		catch(std::exception&)
		{
		}
		return ret;
	}
	bool CheckFolder()
	{
		if(!Exists("CheckFolder"))
		{
			throw std::runtime_error("CheckFolder does not exist");
		}
		bool val = false;
		std::string s(m_NameValueMap["CheckFolder"]);
		if(s=="Y"||s=="1"||s=="true") val=true;
		else if(s=="N"||s=="0"||s=="false") val=false;
		return val;
	}
	bool IsValidCheckFolder()
	{
		bool ret = false;
		try
		{
			ret = true;
		}
		catch(std::exception&)
		{
		}
		std::string s(m_NameValueMap["CheckFolder"]);
		return ret&&(s=="Y"||s=="1"||s=="true"||s=="N"||s=="0"||s=="false");
	}
	bool Validate(std::string& error)
	{
		std::ostringstream oss;
		if(!IsValidStartDate())
		{
			oss << "StartDate validation fails!" << std::endl;
		}
		if(!IsValidEndDate())
		{
			oss << "EndDate validation fails!" << std::endl;
		}
		if(!IsValidAlpha())
		{
			oss << "Alpha validation fails!" << std::endl;
		}
		if(!IsValidCheckFolder())
		{
			oss << "CheckFolder validation fails!" << std::endl;
		}
		error = oss.str();
		return error.empty();
	}
};
