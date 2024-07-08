#include "MainFrame.h"
#include <wx/wx.h>
#include <string>
#include <vector>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreatControls();
	SetupSizers();
	BindEventHandler();
	AddSavedTasks();
}

void MainFrame::CreatControls() //Will control all ui controlos from here
{
	wxFont headTitleFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headTitle = new wxStaticText(panel, wxID_ANY, "To-Do List");
	headTitle->SetFont(headTitleFont);

	inputTask = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add");
	checkListBox = new wxCheckListBox(panel, wxID_ANY);
	clearButton = new wxButton(panel, wxID_ANY, "Clear");
}

void MainFrame::SetupSizers()
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(headTitle, wxSizerFlags().CenterHorizontal());
	mainSizer->AddSpacer(25);

	wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
	inputSizer->Add(inputTask, wxSizerFlags().Proportion(1));
	mainSizer->AddSpacer(5);
	inputSizer->Add(addButton);

	mainSizer->Add(inputSizer, wxSizerFlags().Expand());
	mainSizer->AddSpacer(5);
	mainSizer->Add(checkListBox, wxSizerFlags().Expand().Proportion(1));
	mainSizer->AddSpacer(5);
	mainSizer->Add(clearButton);

	wxGridSizer* outerSizer = new wxGridSizer(1);
	outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 25).Expand());

	panel->SetSizer(outerSizer);
	outerSizer->SetSizeHints(this);
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
