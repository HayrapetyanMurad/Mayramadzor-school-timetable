#pragma once
#include <string>

class OptionParser
{
public:
	OptionParser();
	bool init(int argc, char** argv);
	void print_usage();

	std::string get_subjects_file();
	std::string get_teachers_file();
	int get_number_of_lessons_for_day();
	int get_number_of_school_days_in_week();
private:
	std::string subjects_file_;
	std::string teachers_file_;
	int number_of_lessons_for_day_;
	int number_of_school_days_in_week_;
};

