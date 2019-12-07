#include <iostream>
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(const Schedule &schedule, int nodes_count, int generation_count)
	: schedule_(schedule)
	, chromosome_values_(schedule.get_number_of_lessons_for_day() * schedule.get_number_of_days_in_week())
	, chromosome_lenght_(schedule.get_number_of_lessons_for_day() * schedule.get_number_of_days_in_week() * schedule.get_number_of_classes())
	, nodes_count_(nodes_count)
	, generation_count_(generation_count)
{}

void GeneticAlgorithm::generate(Schedule::class_to_lessons_for_week_map_t& result)
{
	std::vector<std::vector<std::vector<int>>> geneartions(2);

	std::vector<std::vector<int>> &old_generation = geneartions[0];
	std::vector<std::vector<int>> &new_generation = geneartions[1];

	old_generation.resize(nodes_count_);
	new_generation.resize(nodes_count_);

	for (int i = 0; i < nodes_count_; i++)
	{
		old_generation[i].resize(chromosome_lenght_);
		new_generation[i].resize(chromosome_lenght_);

		for (int j = 0; j < chromosome_lenght_; j++)
		{
			old_generation[i][j] = rand() % chromosome_values_;
		}
	}

	int current_generation_index;
	int next_generation_index;
	for (int i = 0; i < generation_count_; i++)
	{
		current_generation_index = i % 2;
		next_generation_index = (i + 1) % 2;

		old_generation = geneartions[current_generation_index];
		new_generation = geneartions[next_generation_index];

		for (int j = 0; j < nodes_count_; j++)
		{
			int first_index = get_fit_node(old_generation);
			int second_index = get_fit_node(old_generation);

			do_chrossing(old_generation[first_index], old_generation[second_index], new_generation[j]);
		}

	}

	int fit_node_index = get_fit_node(new_generation);

	//std::cout << schedule_.get_fitness(new_generation[fit_node_index]) << std::endl;

	schedule_.get_schedule(new_generation[fit_node_index], result);
}

int GeneticAlgorithm::get_fit_node(const std::vector<std::vector<int>> &generation)
{
	//chose random three nodes and retun fittest

	int index;
	int fitness = INT_MAX;

	for (int i = 0; i < 3; i++)
	{
		int new_index = rand() % nodes_count_;
		int new_fitness = schedule_.get_fitness(generation[new_index]);
		if (new_fitness < fitness)
		{
			fitness = new_fitness;
			index = new_index;
		}
	}

	return index;
}

void GeneticAlgorithm::do_chrossing(const std::vector<int> &first_chromosome, const std::vector<int> &second_chromosome, std::vector<int> &new_chromosome)
{
	int crossing_point = rand() % (chromosome_lenght_ - 2) + 1;

	for (int i = 0; i <= crossing_point; i++)
	{
		new_chromosome[i] = first_chromosome[i];
	}

	for (int i = crossing_point + 1; i < chromosome_lenght_; i++)
	{
		new_chromosome[i] = second_chromosome[i];
	}

	int mutation_probability = rand() % 10;
	if (mutation_probability == 0)
	{
		if (rand() % 2 == 0)
		{
			int first_index = rand() % chromosome_lenght_;
			int second_index = rand() % chromosome_lenght_;

			std::swap(new_chromosome[first_index], new_chromosome[second_index]);

		}
		else
		{
			int gene_to_change = rand() % chromosome_lenght_;
			int new_value = rand() % chromosome_values_;

			new_chromosome[gene_to_change] = new_value;
		}
	}
}