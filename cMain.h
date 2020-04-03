#pragma once

#include <wx/wx.h>

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

public:
	int nFieldWidth = 10;
	int nFieldHeight = 10;
	int fieldCount = nFieldWidth * nFieldHeight;
	int mines = 20;
	wxButton*** btn;

	int** nField = nullptr;
	bool bFirstClick = true;

	void checkMines(int x, int y);
	void resetGame();

	void OnButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

