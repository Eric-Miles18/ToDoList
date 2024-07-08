#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	//main functions to control and handel evreything
	void CreatControls();
	void SetupSizers();
	void BindEventHandler();
	void AddSavedTasks();

	//event functions
	void OnAddButtonClicked(wxCommandEvent& event);
	void OnInputEnter(wxCommandEvent& event);
	void OnListKeyDown(wxKeyEvent& event);
	void OnClearButtonClicked(wxCommandEvent& event);
	void OnWindowClosed(wxCloseEvent& event);

	//helping functions
	void AddTaskFromInput();
	void DeleteSelectedTask();
	void MoveSelectedTask(int offset);
	void SwapTasks(int i, int j);

	//window content
	wxPanel* panel;
	wxStaticText* headTitle;
	wxTextCtrl* inputTask;
	wxButton* addButton;
	wxCheckListBox* checkListBox;
	wxButton* clearButton;
};

