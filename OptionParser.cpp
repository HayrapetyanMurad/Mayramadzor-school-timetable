#include <iostream>
#include "OptionParser.h"

OptionParser::OptionParser()
	: number_of_lessons_for_day_(7)
	, number_of_school_days_in_week_(5)
{}

bool OptionParser::init(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i],"-subjects_file") == 0 )
		{
			if (i + 1 == argc)
			{
				return false;
			}

			subjects_file_ = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i],"-teachers_file") == 0)
		{
			if (i + 1 == argc)
			{
				return false;
			}

			teachers_file_ = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i],"-number_of_lessons_for_day"))
		{
			if (i + 1 == argc)
			{
				return false;
			}

			number_of_lessons_for_day_ = std::atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i],"-number_of_school_days_in_week"))
		{
			if (i + 1 == argc)
			{
				return false;
			}

			number_of_school_days_in_week_ = std::atoi(argv[i + 1]);
			i++;
		}
		else
		{
			return false;
		}
	}

	if (subjects_file_.empty() || teachers_file_.empty())
	{
		return false;
	}

	return true;
}

void OptionParser::print_usage()
{
	std::cout << "Usage: \n";
	std::cout << "\t-subjects_file <value> (mandatory)\n";
	std::cout << "\t\t file for describing classes and subjects\n";
	std::cout << "\t\t each line of file should contain record about classes in this format\n";
	std::cout << "\t\t class_name: subject_name1-number_of_lessons_for_week, subject_name2-number_of_lessons_for_week, ... \n";
	std::cout << "\t-teachers_file <value> (mandatory)\n";
	std::cout << "\t\t file for describing teachers\n";
	std::cout << "\t\t each line of file should contain record about teacher in this format\n";
	std::cout << "\t\t teacher_name: class1-subject1, class2-subject2, ... \n";
	std::cout << "\t-number_of_lessons_for_day <value> (optional: default value: 7)\n";
	std::cout << "\t\t maximum number of lessons for day\n ";
	std::cout << "\t-number_of_school_days_in_week <value> (optional: default value: 5)\n";
	std::cout << "\t\t number of school days in week\n ";
	
}

std::string OptionParser::get_subjects_file()
{
	return subjects_file_;
}

std::string OptionParser::get_teachers_file()
{
	return teachers_file_;
}

int OptionParser::get_number_of_lessons_for_day()
{
	return number_of_lessons_for_day_;
}

int OptionParser::get_number_of_school_days_in_week()
{
	return number_of_school_days_in_week_;
}