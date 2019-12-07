#pragma once
#include "Schedule.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(const Schedule &schedule, int nodes_count, int generation_count);

	void generate(Schedule::class_to_lessons_for_week_map_t& result);

private:

	int get_fit_node(const std::vector<std::vector<int>> &generation);

	void do_chrossing(const std::vector<int> &first_chromosome, const std::vector<int> &second_chromosome, std::vector<int> &new_chromosome);

private:

	const Schedule &schedule_;
	int chromosome_lenght_;
	int nodes_count_;
	int generation_count_;
	int chromosome_values_;
};