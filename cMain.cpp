#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)

wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Main Window", wxPoint(50, 50), wxSize(800, 600)) {
	// Create button array
	btn = new wxButton** [nFieldHeight];
	for (int i = 0; i < nFieldHeight; i++) {
		btn[i] = new wxButton*[nFieldWidth];
	}

	// Create grid
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	// Create mine field
	nField = new int*[nFieldHeight];
	for (int i = 0; i < nFieldHeight; i++) {
		nField[i] = new int[nFieldWidth];
	}

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	// Fill button array, assign font, grid, event / init minefield
	for (int i = 0; i < nFieldHeight; i++) {
		for (int j = 0; j < nFieldWidth; j++) {
			btn[i][j] = new wxButton(this, 10000 + (i * 100) + j);
			btn[i][j]->SetFont(font);
			grid->Add(btn[i][j], 1, wxEXPAND | wxALL);

			btn[i][j]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);

			nField[i][j] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain() {
	for (int i = 0; i < nFieldHeight; i++) {
		delete[] btn[i];
		delete[] nField[i];
	}
	delete[] btn;
	delete[] nField;
}

void cMain::OnButtonClicked(wxCommandEvent& evt) {
	// Get button coordinates from button id
	int x = (evt.GetId() - 10000) % 100;
	int y = floor((evt.GetId() - 10000) / 100);
	
	// Generate all mines at first click
	if (bFirstClick) {
		int mines = 20;

		while (mines) {
			// Get random coordinate for each mine
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			// Check if there is no mine yet in the the coordinates of field
			// Check that the user has not clicked these coordinates yet
			if (nField[ry][rx] == 0 && rx != x && ry != y) {
				nField[ry][rx] = -1;
				mines--;
			}
		}

		bFirstClick = false;
	}

	// Disable button, preventing it being pressed again
	btn[y][x]->Enable(false);

	// Check if player hit mine
	if (nField[y][x] == -1) {
		wxMessageBox("Game over!");

		// Reset Game
		bFirstClick = true;
		for (int i = 0; i < nFieldHeight; i++) {
			for (int j = 0; j < nFieldWidth; j++) {
				nField[i][j] = 0;
				btn[i][j]->SetLabel("");
				btn[i][j]->Enable(true);
			}
		}
	}
	else {
		checkMines(x, y);
	}
	evt.Skip();
}

void cMain::checkMines(int x, int y) {
	btn[y][x]->Enable(false);

	// Check surrounding mines and count mines
	int mineCount = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			// See if surrounding mines are in the field
			if (x + j >= 0 && x + j < nFieldWidth && y + i >= 0 && y + i < nFieldHeight) {
				//Check if field has mine
				if (nField[y + i][x + j] == -1) {
					mineCount++;
				}
			}
		}
	}

	if (mineCount > 0) {
		btn[y][x]->SetLabel(std::to_string(mineCount));
	}
	else {
		// Check and show surrounding fields
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				// See if surrounding mines are in the field
				if (x + j >= 0 && x + j < nFieldWidth && y + i >= 0 && y + i < nFieldHeight) {
					//Check if field has mine
					if (btn[y+i][x+j]->IsEnabled()) {
						checkMines(x + j, y + i);
					}
				}
			}
		}
	}
}