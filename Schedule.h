#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

class Schedule
{
public:
	typedef std::map<std::string, int> lesson_to_lesson_count_map_t;
	typedef std::map<std::string, lesson_to_lesson_count_map_t> class_to_lessons_map_t;

	typedef std::map<std::string, std::vector<std::string>> class_to_lessons_for_teacher_map_t;
	typedef std::map<std::string, class_to_lessons_for_teacher_map_t> teacher_to_lessons_map_t;

	typedef std::map<std::string, std::vector<std::string>> class_to_lessons_for_week_map_t;

	struct Teacher
	{
		std::string name_;

		Teacher(const std::string &name)
			: name_(name)
		{}

		bool operator<(const Teacher& rhs) const
		{
			return name_ < rhs.name_;
		}
	};

	struct Class
	{
		std::string name_;

		Class(const std::string &name)
			: name_(name)
		{}

		bool operator<(const Class& rhs) const
		{
			return name_ < rhs.name_;
		}
	};

	struct Lesson
	{
		const Teacher *teacher_;
		const Class *class_;
		std::string subject_name;

		Lesson(const Teacher *t, const Class *c, const std::string& subj)
			: teacher_(t)
			, class_(c)
			, subject_name(subj)
		{}
	};

	Schedule(const std::string &subjects_file, const std::string &teachers_file, int number_of_lessons_for_day, int number_of_days_in_week);

	int get_fitness(const std::vector<int> &chromosome) const;

	void get_schedule(const std::vector<int> &chromosome, Schedule::class_to_lessons_for_week_map_t& result) const;

	int get_number_of_classes() const;

	int get_number_of_lessons_for_day() const;

	int get_number_of_days_in_week() const;

private:
	void init(const std::string &subjects_file, const std::string &teachers_file);

	void init_teachers(const std::string &teachers_file, teacher_to_lessons_map_t &teacher_to_lessons_map);

	void init_classes(const std::string &subjects_file, class_to_lessons_map_t &class_to_lessons_map);

	bool process_teacher_line(const std::string& line, teacher_to_lessons_map_t &teacher_to_lessons_map);

	bool process_class_line(const std::string& line, class_to_lessons_map_t &class_to_lessons_map);

	void trim(std::string &str);

private:

	std::list<Lesson> lessons_;
	std::map<Class, std::vector<const Lesson*>> classes_;
	std::map<Teacher, std::vector<const Lesson*>> teachers_;

	int number_of_lessons_for_day_;
	int number_of_days_in_week_;
};
