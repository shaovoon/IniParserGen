// ParseIni.cpp : Defines the entry point for the console application.
//

#include "minicsv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "MyIniFile.h"
#include "IniParserGen.h"

int main()
{
	std::string schema_file = "schema.ini";
	std::string out_file = "MyIniFileOut.h";
	
	IniParserGen generator;
	if(generator.ParseFile(schema_file))
	{
		std::string output;
		if(generator.GenerateCode(output))
		{
			std::ofstream ofs(out_file.c_str());
			if(ofs.is_open())
			{
				ofs << output;
				ofs.flush();
				ofs.close();
				std::cout << "Done." << std::endl;
			}
			else
			{
				std::cerr << "out_file cannot be opened:" << out_file << std::endl;
			}
		}
		else
		{
			std::cerr << "GenerateCode failed" << std::endl;
		}
	}
	else
	{
		std::cerr << "ParseFile failed" << std::endl;
	}

	try
	{
		MyIniFile ini_file;
		if (ini_file.ParseFile("test.ini"))
		{
			std::cout << ini_file.StartDate() << std::endl;
			std::cout << ini_file.EndDate() << std::endl;
			std::cout << ini_file.Alpha() << std::endl;
			std::cout << std::boolalpha << ini_file.CheckFolder() << std::endl;
			std::cout << ini_file.TintedColor() << std::endl;
			ini_file.SetAlpha(120);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception thrown:" << e.what() << std::endl;
	}

	return 0;
}

