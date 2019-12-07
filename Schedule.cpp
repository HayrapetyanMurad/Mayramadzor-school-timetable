#include <fstream>
#include <set>
#include "Schedule.h"

Schedule::Schedule(const std::string &subjects_file, const std::string &teachers_file, int number_of_lessons_for_day, int number_of_days_in_week)
	: number_of_days_in_week_(number_of_days_in_week)
	, number_of_lessons_for_day_(number_of_lessons_for_day)
{
	init(subjects_file, teachers_file);
}

int Schedule::get_fitness(const std::vector<int> &chromosome) const
{
	int fitness = 0;

	int lessons_count_for_week = number_of_days_in_week_ * number_of_lessons_for_day_;
	std::size_t classes_count = classes_.size();


	//check that class doesn't have 2 lessons for same time
	for (int i = 0; i < classes_count; i++)
	{
		//for every day
		for (int j = 0; j < lessons_count_for_week; j++)
		{
			for (int k = j + 1; k < lessons_count_for_week; k++)
			{
				if (chromosome[i * lessons_count_for_week + j] == chromosome[i * lessons_count_for_week + k])
				{
					fitness += 10;
				}
			}
		}
	}

	//check that teacher doesn't have 2 lessons for same time

	std::set<const Teacher*> teachers_for_time;
	std::map<Class, std::vector<const Lesson*>>::const_iterator class_it, class_it_end;
	class_it_end = classes_.end();

	//for each hour in week
	for (int i = 0; i < lessons_count_for_week; i++)
	{
		//for each class
		teachers_for_time.clear();
		class_it = classes_.begin();
		for (int j = 0; class_it != class_it_end; ++class_it, ++j)
		{

			int index = chromosome[j * lessons_count_for_week + i];
			Lesson const *lesson = class_it->second[index];
			if (lesson != NULL)
			{
				if (teachers_for_time.insert(lesson->teacher_).second == false)
				{
					fitness += 10;
				}
			}


		}
	}

	return fitness;
}

void Schedule::get_schedule(const std::vector<int> &chromosome, Schedule::class_to_lessons_for_week_map_t& result) const
{
	std::map<Class, std::vector<const Lesson*>>::const_iterator it, it_end;
	it = classes_.begin();
	it_end = classes_.end();

	for (int i=0; it != it_end; ++it, ++i)
	{
		for (int j = 0; j < number_of_days_in_week_*number_of_lessons_for_day_; j++)
		{
			int index = i * number_of_days_in_week_*number_of_lessons_for_day_ + j;
			const Lesson *lesson = it->second[chromosome[index]];
			if (lesson)
			{
				result[it->first.name_].push_back(lesson->subject_name);
			}
			else
			{
				result[it->first.name_].push_back("");
			}
		}
	}
}

int Schedule::get_number_of_classes() const
{
	return (int)classes_.size();
}

int Schedule::get_number_of_lessons_for_day() const
{
	return number_of_lessons_for_day_;
}

int Schedule::get_number_of_days_in_week() const
{
	return number_of_days_in_week_;
}

void Schedule::init(const std::string &subjects_file, const std::string &teachers_file)
{
	class_to_lessons_map_t class_to_lessons_map;

	init_classes(subjects_file, class_to_lessons_map);

	teacher_to_lessons_map_t teacher_to_lessons_map;

	init_teachers(teachers_file, teacher_to_lessons_map);

	teacher_to_lessons_map_t::iterator teacher_it, teacher_it_end;
	class_to_lessons_for_teacher_map_t::iterator teacher_class_it, teacher_class_it_end;

	teacher_it = teacher_to_lessons_map.begin();
	teacher_it_end = teacher_to_lessons_map.end();

	for (; teacher_it != teacher_it_end; ++teacher_it)
	{
		std::map<Teacher, std::vector<const Lesson*>>::iterator teacher;
		teacher = teachers_.insert(std::make_pair(Teacher(teacher_it->first), std::vector<const Lesson*>())).first;

		teacher_class_it = teacher_it->second.begin();
		teacher_class_it_end = teacher_it->second.end();

		for (; teacher_class_it != teacher_class_it_end; ++teacher_class_it)
		{
			class_to_lessons_map_t::iterator class_lessons_it = class_to_lessons_map.find(teacher_class_it->first);
			if (class_lessons_it == class_to_lessons_map.end())
			{
				std::string error = "Class " + teacher_class_it->first + " for teacher " + teacher_it->first + " does not exist in classes file.";
				throw std::exception(error.c_str());
			}

			std::map<Class, std::vector<const Lesson*>>::iterator school_class;
			school_class = classes_.find(Class(class_lessons_it->first));
			if (school_class == classes_.end())
			{
				school_class = classes_.insert(std::make_pair(Class(class_lessons_it->first), std::vector<const Lesson*>())).first;
			}

			std::size_t lessons_count_in_class = teacher_class_it->second.size();

			for (std::size_t i = 0; i < lessons_count_in_class; i++)
			{
				lesson_to_lesson_count_map_t::iterator lesson_count_it = class_lessons_it->second.find(teacher_class_it->second[i]);
				if (lesson_count_it == class_lessons_it->second.end())
				{
					std::string error = "Class does not have lesson with name " + teacher_class_it->second[i];
					throw std::exception(error.c_str());
				}

				int lesson_count = class_lessons_it->second.find(teacher_class_it->second[i])->second;

				for (int j = 0; j < lesson_count; j++)
				{
					Lesson *new_lesson = &(*lessons_.insert(lessons_.end(), Lesson(&teacher->first, &school_class->first, teacher_class_it->second[i])));
					teacher->second.push_back(new_lesson);
					school_class->second.push_back(new_lesson);
				}

				class_lessons_it->second.erase(teacher_class_it->second[i]);
			}
		}

	}

	class_to_lessons_map_t::iterator class_lessons_it, class_lessons_it_end;
	class_lessons_it = class_to_lessons_map.begin();
	class_lessons_it_end = class_to_lessons_map.end();

	for (; class_lessons_it != class_lessons_it_end; ++class_lessons_it)
	{
		if (!class_lessons_it->second.empty())
		{
			std::string error = "For class " + class_lessons_it->first + " some lesons do not have teachers.";
			throw std::exception(error.c_str());
		}
	}

	//adding Null lessons for free classes
	int max_lessos_count = number_of_lessons_for_day_ * number_of_days_in_week_;
	std::size_t lessons_count;

	std::map<Class, std::vector<const Lesson*>>::iterator classes_it, classes_it_end;
	classes_it = classes_.begin();
	classes_it_end = classes_.end();

	for (; classes_it != classes_it_end; ++classes_it)
	{
		lessons_count = classes_it->second.size();
		for (std::size_t j = lessons_count; j < max_lessos_count; j++)
		{
			classes_it->second.push_back(NULL);
		}
	}
}

void Schedule::init_teachers(const std::string &teachers_file, teacher_to_lessons_map_t &teacher_to_lessons_map)
{
	std::ifstream file(teachers_file);

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!process_teacher_line(line, teacher_to_lessons_map))
			{
				std::string error = "Invalid format in " + teachers_file + " file";
				throw std::exception(error.c_str());
			}

		}

		file.close();
	}
	else
	{
		std::string error = "Unable to open " + teachers_file + " file";
		throw std::exception(error.c_str());
	}
}

void Schedule::init_classes(const std::string &subjects_file, class_to_lessons_map_t &class_to_lessons_map)
{
	std::ifstream file(subjects_file);

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!process_class_line(line, class_to_lessons_map))
			{
				std::string error = "Invalid format in " + subjects_file + " file";
				throw std::exception(error.c_str());
			}

		}

		file.close();
	}
	else
	{
		std::string error = "Unable to open " + subjects_file + " file";
		throw std::exception(error.c_str());
	}
}

bool Schedule::process_teacher_line(const std::string& line, teacher_to_lessons_map_t &teacher_to_lessons_map)
{
	std::string teacher_name;

	size_t pos_start = 0, pos_end;
	std::string delimiter(":");

	pos_end = line.find(delimiter, pos_start);
	if (pos_end != std::string::npos)
	{
		teacher_name = line.substr(pos_start, pos_end - pos_start);
		trim(teacher_name);
		pos_start = pos_end + delimiter.size();
	}
	else
	{
		return false;
	}

	delimiter = ",";

	std::string token;
	size_t pos;
	std::string class_name;
	std::string class_subject;

	while ((pos_end = line.find(delimiter, pos_start)) != std::string::npos) {
		token = line.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delimiter.size();

		pos = token.find("-");
		if (pos != std::string::npos)
		{
			class_name = token.substr(0, pos);
			trim(class_name);
			class_subject = token.substr(pos + 1);

			teacher_to_lessons_map[teacher_name][class_name].push_back(class_subject);
		}
		else
		{
			return false;
		}
	}

	token = line.substr(pos_start);
	pos = token.find("-");
	if (pos != std::string::npos)
	{
		class_name = token.substr(0, pos);
		trim(class_name);
		class_subject = token.substr(pos + 1);

		teacher_to_lessons_map[teacher_name][class_name].push_back(class_subject);
	}
	else
	{
		return false;
	}

	return true;
}

bool Schedule::process_class_line(const std::string& line, class_to_lessons_map_t &class_to_lessons_map)
{
	std::string class_name;

	size_t pos_start = 0, pos_end;
	std::string delimiter(":");

	pos_end = line.find(delimiter, pos_start);
	if (pos_end != std::string::npos)
	{
		class_name = line.substr(pos_start, pos_end - pos_start);
		trim(class_name);
		pos_start = pos_end + delimiter.size();
	}
	else
	{
		return false;
	}

	delimiter = ",";

	std::string token;
	size_t pos;
	std::string lesson_name;
	int lesson_count;

	while ((pos_end = line.find(delimiter, pos_start)) != std::string::npos) {
		token = line.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delimiter.size();

		pos = token.find("-");
		if (pos != std::string::npos)
		{
			lesson_name = token.substr(0, pos);
			trim(lesson_name);
			lesson_count = std::stoi(token.substr(pos + 1));

			class_to_lessons_map[class_name][lesson_name] = lesson_count;
		}
		else
		{
			return false;
		}
	}

	token = line.substr(pos_start);
	pos = token.find("-");
	if (pos != std::string::npos)
	{
		lesson_name = token.substr(0, pos);
		trim(lesson_name);
		lesson_count = std::stoi(token.substr(pos + 1));

		class_to_lessons_map[class_name][lesson_name] = lesson_count;
	}
	else
	{
		return false;
	}

	return true;
}

void Schedule::trim(std::string &str)
{
	const std::string chars = "\t\n\v\f\r ";
	str.erase(str.find_last_not_of(chars) + 1);
	str.erase(0, str.find_first_not_of(chars));
}