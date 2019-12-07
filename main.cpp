#include <iostream>
#include "Schedule.h"
#include "GeneticAlgorithm.h"
#include "OptionParser.h"

int main(int argc, char** argv)
{
	OptionParser parser;
	if (!parser.init(argc, argv))
	{
		std::cout << "Invalid arguments" << std::endl;
		parser.print_usage();
		return EXIT_FAILURE;
	}

	std::string subjects_file = parser.get_subjects_file();
	std::string teachers_file = parser.get_teachers_file();
	int number_of_lessons_for_day = parser.get_number_of_lessons_for_day();
	int number_of_school_days_in_week = parser.get_number_of_school_days_in_week();

	Schedule schedule(subjects_file, teachers_file, number_of_lessons_for_day, number_of_school_days_in_week);

	Schedule::class_to_lessons_for_week_map_t result_timetable;
	GeneticAlgorithm(schedule, 30, 1000).generate(result_timetable);

	Schedule::class_to_lessons_for_week_map_t::iterator it, it_end;
	it = result_timetable.begin();
	it_end = result_timetable.end();

	int days_in_week = schedule.get_number_of_days_in_week();
	int lessons_for_day = schedule.get_number_of_lessons_for_day();

	for (; it != it_end; ++it)
	{
		std::cout << "_____________" << std::endl;
		std::cout << it->first << std::endl;
		std::cout << std::endl;

		for (int j = 0; j < days_in_week; j++)
		{
			for (int k = 0; k < lessons_for_day; k++)
			{
				std::string subject = it->second[j*lessons_for_day + k];
				if (subject.empty())
				{
					std::cout << "---" << std::endl;
				}
				else
				{
					std::cout << subject << std::endl;
				}
			}

			std::cout << std::endl;
		}

		std::cout << "_____________" << std::endl;

	}

	return EXIT_SUCCESS;
}