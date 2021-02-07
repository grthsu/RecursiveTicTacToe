//Copyright: Gregory Su
//May 2018

#include <wx/wxprec.h>
#include "Interface.h"
#include "Logic.h"
#include "GraphicsInterface.h"
#include <string>

//wxWidgets Equivalent of Main
wxIMPLEMENT_APP(Game);

//Copy the documentation from header//...
bool Game::OnInit() {
	wxTextEntryDialog setup(nullptr, "Enter the number of Tic Tac Toe recursions (1 = normal)");
	unsigned level = 2;

	bool goodInput = false;
	while (!goodInput) {
		if (setup.ShowModal() == wxID_OK) {
			std::string input = setup.GetValue().ToStdString();
			int tempLevel = 2;
			try {
				tempLevel = std::stoi(input);
			}
			catch (...) {
				wxMessageBox("Bad Input.  Try Again.");
				continue;
			}
			if (tempLevel < 0) {
				wxMessageBox("Bad Input: Negative Number.  Try Again.");
				continue;
			}
			level = tempLevel;
			goodInput = true;
		}
		else {
			return false;
		}
	}

	Board* game = new Board(level);
	game->Show();
	return true;
}

void Board::OnClick(wxCommandEvent & event) {
	wxButton* button = (wxButton*)(event.GetEventObject());
	std::string moveID = std::to_string(button->GetId());
	while (moveID.find('9') != std::string::npos) {
		moveID[moveID.find('9')] = '0';
	}
	std::vector<int> move;
	for (size_t index = 0; index < moveID.size(); ++index) {
		move.push_back(moveID[index] - '0');
	}

	//wxMessageBox(std::to_string(button->GetGrandParent()->GetId()));

	Logic::owner player = convertToNextPlayer(game->getLastPlayer());
	if (game->legal(move, player)) {
		game->addMove(move, player);
		button->SetBackgroundColour(stateToColor(player));
		game->victor();

		int gridID = button->GetId() / 10;
		move.pop_back();
		while (move.size() > 0) {
			wxPanel* panel = (wxPanel*) FindWindowById(gridID);
			panel->SetBackgroundColour(stateToColor(game->getGridOwner(move), true));
			gridID /= 10;
			move.pop_back();
		}
		
		if (game->victor() != Logic::owner::none) {
			SetBackgroundColour(stateToColor(game->victor()));
			Refresh();
			wxMessageBox(convertToString(game->victor()) + " has won!");
		}

		markNewLegal();
		Refresh();	
	}
}
