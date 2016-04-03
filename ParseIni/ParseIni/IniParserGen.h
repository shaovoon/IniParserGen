// Ini file parser generator


#ifndef INI_PARSER_GENERATOR_H
#define INI_PARSER_GENERATOR_H

#include "minicsv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

class IniParserGen
{
public:
	bool ParseFile(const std::string& file)
	{
		csv::ifstream is(file.c_str());
		is.set_delimiter('=', "$$");
		if (is.is_open())
		{
			while (is.read_line())
			{
				Info info;
				is >> info.name;
				is.set_delimiter(';', "$$");
				is >> info.type;

				if(info.type=="string")
					info.type="std::string";

				info.validator = is.get_rest_of_line();

				m_InfoVec.push_back(info);
				is.set_delimiter('=', "$$");

				// display 
				//std::cout << info.name << "," << info.type << "," << info.validator << std::endl;
			}
			return m_InfoVec.size() > 0;
		}
		return false;
	}

	bool GenerateCode(std::string& output)
	{
		output = "";
		if(m_InfoVec.size()==0)
		{
			return false;
		}
		std::ostringstream oss;
		const char* prologue = 
		"#include <string>\n"
		"#include <map>\n"
		"#include <fstream>\n"
		"#include <stdexcept>\n"
		"#include \"minicsv.h\"\n"
		"class MyIniFile\n"
		"{\n"
		"private:\n"
		"	std::map<std::string, std::string> m_NameValueMap;\n"
		"public:\n"
		"	bool ParseFile(const std::string& file, /* OUT */ std::string& error)\n"
		"	{\n"
		"		m_NameValueMap.clear();\n"
		"		csv::ifstream is(file.c_str());\n"
		"		is.set_delimiter('=', \"$$\");\n"
		"		if (is.is_open())\n"
		"		{\n"
		"			while (is.read_line())\n"
		"			{\n"
		"				std::string name;\n"
		"				std::string value;\n"
		"				is >> name;\n"
		"				value = is.get_rest_of_line();\n"
		"				m_NameValueMap[name] = value;\n"
		"			}\n"
		"			is.close();\n"
		"			return Validate(error);\n"
		"		}\n"
		"		else\n"
		"		{\n"
		"			std::ostringstream oss;\n"
		"			oss << \"File cannot be opened:\" << file;\n"
		"			error = oss.str();\n"
		"			return false;\n"
		"		}\n"
		"	}\n"
		"	bool Exists(const std::string& name)\n"
		"	{\n"
		"		typedef std::map<std::string, std::string> Map;\n"
		"		typedef Map::const_iterator MapConstIter;\n"
		"		MapConstIter it = m_NameValueMap.find(name);\n"
		"		return it != m_NameValueMap.end();\n"
		"	}\n";

		oss << prologue;
		for(size_t i=0; i<m_InfoVec.size(); ++i)
		{
			if(m_InfoVec[i].type=="bool")
			{
				oss << "\t" << m_InfoVec[i].type << " " << m_InfoVec[i].name << "()\n";
				oss << "\t{\n";
				oss << "\t\tif(!Exists(\"" << m_InfoVec[i].name << "\"))\n";
				oss << "\t\t{\n";
				oss << "\t\t\tthrow std::runtime_error(\"" << m_InfoVec[i].name << " does not exist\");\n";
				oss << "\t\t}\n";
				oss << "\t\tbool val = false;\n";
				oss << "\t\tstd::string s(m_NameValueMap[\"" << m_InfoVec[i].name << "\"]);\n";
				oss << "\t\tif(s==\"Y\"||s==\"1\"||s==\"true\") val=true;\n";
				oss << "\t\telse if(s==\"N\"||s==\"0\"||s==\"false\") val=false;\n";
				oss << "\t\treturn val;\n";
				oss << "\t}\n";
			}
			else
			{
				oss << "\t" << m_InfoVec[i].type << " " << m_InfoVec[i].name << "()\n";
				oss << "\t{\n";
				oss << "\t\tif(!Exists(\"" << m_InfoVec[i].name << "\"))\n";
				oss << "\t\t{\n";
				oss << "\t\t\tthrow std::runtime_error(\"" << m_InfoVec[i].name << " does not exist\");\n";
				oss << "\t\t}\n";
				oss << "\t\t" << m_InfoVec[i].type << " val;\n";
				oss << "\t\tstd::istringstream iss(m_NameValueMap[\"" << m_InfoVec[i].name << "\"]);\n";
				oss << "\t\tiss >> val;\n";
				oss << "\t\treturn val;\n";
				oss << "\t}\n";
			}

			oss << "\tbool IsValid" << m_InfoVec[i].name << "()\n";
			oss << "\t{\n";
			oss << "\t\tbool ret = false;\n";
			oss << "\t\ttry\n";
			oss << "\t\t{\n";
			oss << "\t\t\tret = " << m_InfoVec[i].validator << ";\n";
			oss << "\t\t}\n";
			oss << "\t\tcatch(std::exception&)\n";
			oss << "\t\t{\n";
			oss << "\t\t}\n";
			if(m_InfoVec[i].type=="bool")
			{
				oss << "\t\tstd::string s(m_NameValueMap[\"" << m_InfoVec[i].name << "\"]);\n";
				oss << "\t\treturn ret&&(s==\"Y\"||s==\"1\"||s==\"true\"||s==\"N\"||s==\"0\"||s==\"false\");\n";
			}
			else
			{
				oss << "\t\treturn ret;\n";
			}
			oss << "\t}\n";

		}

		oss << "\tbool Validate(std::string& error)\n";
		oss << "\t{\n";
		oss << "\t\tstd::ostringstream oss;\n";

		for(size_t i=0; i<m_InfoVec.size(); ++i)
		{
			oss << "\t\tif(!IsValid" << m_InfoVec[i].name << "())\n";
			oss << "\t\t{\n";
			oss << "\t\t\toss << \"" << m_InfoVec[i].name << " validation fails!\" << std::endl;\n";
			oss << "\t\t}\n";
		}

		oss << "\t\terror = oss.str();\n";
		oss << "\t\treturn error.empty();\n";
		oss << "\t}\n";

		oss << "};\n";

		output = oss.str();
		return true;
	}
private:
	struct Info
	{
		std::string name;
		std::string type;
		std::string validator;
	};

	std::vector<Info> m_InfoVec;
};

#endif // INI_PARSER_GENERATOR_H