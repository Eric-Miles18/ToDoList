#include "MainFrame.h"
#include <wx/wx.h>
#include <string>
#include <vector>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreatControls();
	BindEventHandler();
	AddSavedTasks();
}

void MainFrame::CreatControls() //Will control all ui controlos from here
{
	wxFont headTitleFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headTitle = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headTitle->SetFont(headTitleFont);

	inputTask = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
}

void MainFrame::BindEventHandler()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputTask->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

void MainFrame::AddSavedTasks() //Load tasks into file
{
	std::vector<Task> tasks = LoadTasksFromFile("Tasks.txt");
	
	for (const Task& task : tasks)
	{
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.title, index);
		checkListBox->Check(index, task.done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& event)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& event) //Add task when the user use enter key
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& event) //Check waht key pressed to start its jop
{
	switch (event.GetKeyCode())
	{
		case WXK_DELETE:
			DeleteSelectedTask();
			break;
		case WXK_UP:
			MoveSelectedTask(-1);
			break;
		case WXK_DOWN:
			MoveSelectedTask(1);
			break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& event) //Clear all the list
{
	if (checkListBox->IsEmpty())
	{
		return;
	}

	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();

	if (result == wxID_YES)
	{
		checkListBox->Clear();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& event) //Save when app is closed
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++)
	{
		Task task;
		task.title = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "Tasks.txt");
	event.Skip();
}

void MainFrame::AddTaskFromInput() //Add the task to the check box
{
	wxString title = inputTask->GetValue();

	if (!title.IsEmpty())
	{
		checkListBox->Insert(title, checkListBox->GetCount());
		inputTask->Clear();
	}

	inputTask->SetFocus();
}

void MainFrame::DeleteSelectedTask() //Delete the selected task
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	checkListBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset) //Move the selected task in up or down to change the order
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	int newIndex = selectedIndex + offset;

	if (newIndex >= 0 && newIndex < checkListBox->GetCount())
	{
		SwapTasks(selectedIndex, newIndex);
		checkListBox->SetSelection(newIndex, true);
	}
}

void MainFrame::SwapTasks(int i, int j) //Swap the name and the check bettwen 2 tasks
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.title);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.title);
	checkListBox->Check(j, taskI.done);
}
