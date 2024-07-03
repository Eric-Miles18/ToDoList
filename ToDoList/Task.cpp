#include "Task.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename) //Saves tasks in the app to a txt file
{
	std::ofstream ostream(filename);
	ostream << tasks.size();

	//loop over all the tasks and take its name and remove the spaces to speed the loading process and take the done state
	for (const Task& task : tasks)
	{
		std::string title = task.title;
		std::replace(title.begin(), title.end(), ' ', '_');

		ostream << "\n" << title << ' ' << task.done;
	}
}

std::vector<Task> LoadTasksFromFile(const std::string& filename) //load the tasks from the txt file to the app
{
	if (!std::filesystem::exists(filename))
	{
		return std::vector<Task>();
	}

	std::vector<Task> tasks;
	std::ifstream istream(filename);

	int n;
	istream >> n;

	//loop over every task in the txt file and load them into the app
	for (int i = 0; i < n; i++)
	{
		std::string title;
		bool done;
		
		istream >> title >> done;
		std::replace(title.begin(), title.end(), '_', ' ');
		tasks.push_back(Task{ title, done });
	}

	return tasks;
}