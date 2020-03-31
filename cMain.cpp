#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)

wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Main Window", wxPoint(50, 50), wxSize(800, 600)) {
	// Create buttons and grid
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	// Create mine field
	nField = new int[nFieldWidth * nFieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < nFieldWidth; i++) {
		for (int j = 0; j < nFieldHeight; j++) {
			// Create buttons and add to grid
			btn[j * nFieldWidth + i] = new wxButton(this, 10000 + (j * nFieldWidth + i));
			btn[j * nFieldWidth + i]->SetFont(font);
			grid->Add(btn[j * nFieldWidth + i], 1, wxEXPAND | wxALL);

			// Bind buttons to event table
			btn[j * nFieldWidth + i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);

			nField[j * nFieldWidth + i] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain() {
	delete[] btn;
	delete[] nField;
}

void cMain::OnButtonClicked(wxCommandEvent& evt) {
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	// Generate all mines at first click
	if (bFirstClick) {
		int mines = 30;

		while (mines) {
			// Get random coordinate for each mine
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			// Check if there is no mine yet in the the coordinates of field
			// Check that the user has not clicked these coordinates yet
			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y) {
				nField[ry * nFieldWidth + ry] = -1;
				mines--;
			}
		}

		bFirstClick = false;
	}

	// Disable button, preventing it being pressed again
	btn[y * nFieldWidth + x]->Enable(false);

	// Check if player hit mine
	if (nField[y * nFieldWidth + x] == -1) {
		wxMessageBox("Game over!");

		// Reset Game
		bFirstClick = true;
		for (int i = 0; i < nFieldWidth; i++) {
			for (int j = 0; j < nFieldHeight; j++) {
				nField[j * nFieldWidth + i] = 0;
				btn[j * nFieldWidth + i]->SetLabel("");
				btn[j * nFieldWidth + i]->Enable(true);
			}
		}
	}
	else {
		// Check surrounding mines and count mines
		int mineCount = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j <= 2; j++) {
				// See if surrounding mines are in the field
				if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight) {
					//Check if field has mine
					if (nField[(y + j) * nFieldWidth + (x + i)] == -1) {
						mineCount++;
					}
				}
			}
		}

		if (mineCount > 0) {
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(mineCount));
		}
		else {
			// show every surrounding block that has a minecount of 0
		}
	}

	evt.Skip();
}