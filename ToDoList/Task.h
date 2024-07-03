#pragma once
#include <string>
#include <vector>

struct Task
{
	std::string title;
	bool done;
};

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
std::vector<Task> LoadTasksFromFile(const std::string& filename);