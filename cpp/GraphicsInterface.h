//Copyright: Gregory Su
//May 2018

#ifndef __GRAPHICS_INTERFACE_H__
#define __GRAPHICS_INTERFACE_H__

#include <wx/wxprec.h>
#include <memory>
#include "Interface.h"
#include <vector>

//CONSTANT: Global constant value for the ID of the Master Board
const int Board_ID = 0;

//CONSTANT: Global constant value for client width
const int clientWidth = 700;

//CONSTANT: Global constant value for client height
const int clientHeight = 650;

//CLASS: Main Driver -- derives from wxApp to run program
class Game : public wxApp {
public:

	//EFFECTS: Required to initialize game, //...cancel out of setup closes program...
	virtual bool OnInit();
};

//CLASS: Interfaces the user interface with the logical rules of the game
class Board : public wxFrame {
public:

	//REQUIRES: recursionCount > 0
	//EFFECTS: Constructor.  recursionCount is the number of recursions to be played.
	//	Creates the frame and fills the board.
	Board(unsigned recursionCount);

	//REQUIRES: aboveGrid != nullptr, level > 0, parent != nullptr
	//MODIFIES: *aboveGrid, *parent
	//EFFECTS: Populates aboveGrid with nine buttons (level == 1) or grids.
	//	gap sets the distance between the boards.  parent is the ultimate parent of the buttons.
	//	ID is the coordinates of the grid being populated.  buttons are given the coordinates of
	//	their grid followed by their coordinate within the grid.  Calls recursively.
	static void FillBoard(wxGridSizer* aboveGrid, unsigned level, unsigned gap,
		const std::string & ID, wxWindow* parent, Board* handler);

	//EFFECTS: Controls the result when a button is clicked.  //...
	void OnClick(wxCommandEvent & event);

	//MODIFIES: button colors
	//EFFECTS: If possibleMove has length equal to the depth,
	//	makes an illegal unclaimed move with possibleMove coordinates black;
	//		otherwise, makes legal move with possibleMove coordinates yellow.
	//	Otherwise, calls itself nine times appending a coordinate to the end of possibleMove
	void markNewLegal(const std::vector<int> & possibleMove = {});

private:

	//VARIABLE: Number of levels of boards in the game
	unsigned depth;

	//VARIABLE: Holds all logical information that has occurred in the game
	std::unique_ptr<Interface> game;
};

//EFFECTS: Converts a player into a color for a wxWindow.
//	owner::none returns black, owner::one returns red,
//	owner::two returns blue, owner::draw returns green
//	If panel is passed as true, owner::none returns white
inline wxColour stateToColor(Logic::owner state, bool panel = false);



Board::Board(unsigned recursionCount) 
	: wxFrame(NULL, Board_ID, "Recursive Tic Tac Toe"), depth(recursionCount), game(nullptr) {
	game = std::unique_ptr<Interface>(InterfaceFactory(depth));
	unsigned gap = 4 * depth;
	wxGridSizer* grid = new wxGridSizer(3, 3, gap, gap);
	SetClientSize(clientWidth, clientHeight);
	Center();
	FillBoard(grid, depth, gap - 4, "", this, this);
	SetSizer(grid);
}

void Board::FillBoard(wxGridSizer* aboveGrid, unsigned level, unsigned gap,
		const std::string & ID, wxWindow* parent, Board* handler) {
	assert(level > 0);
	assert(aboveGrid);
	assert(parent);

	for (unsigned index = 0; index < 9; ++index) {
		if (level > 1) {
			wxGridSizer* grid = new wxGridSizer(3, 3, gap, gap);
			std::string copyID = ID;
			copyID.append(std::to_string(index));
			while (copyID.find('0') != std::string::npos) {
				copyID[copyID.find('0')] = '9';
			}
			wxPanel* panel = new wxPanel(parent, std::stoi(copyID));
			FillBoard(grid, level - 1, gap - 4, copyID, panel, handler);
			panel->SetSizer(grid);
			panel->SetBackgroundColour(*wxWHITE);
			aboveGrid->Add(panel, 0, wxEXPAND);
		}
		else {
			std::string copyID = ID;
			copyID.append(std::to_string(index));
			while (copyID.find('0') != std::string::npos) {
				copyID[copyID.find('0')] = '9';
			}
			wxButton* button = new wxButton(parent, std::stoi(copyID));
			button->Bind(wxEVT_BUTTON, &Board::OnClick, handler);
			button->SetBackgroundColour(*wxBLACK);
			aboveGrid->Add(button, 0, wxEXPAND);
		}
	}
}

wxColour stateToColor(Logic::owner state, bool panel) {
	switch (state) {
	case Logic::owner::none:
		if (panel) {
			return *wxWHITE;
		}
		return *wxBLACK;
	case Logic::owner::one:
		return *wxRED;
	case Logic::owner::two:
		return *wxBLUE;
	case Logic::owner::draw:
		return *wxGREEN;
	}
	return *wxWHITE;
}

void Board::markNewLegal(const std::vector<int> & possibleMove) {
	if (possibleMove.size() == depth) {
		std::string buttonID = "";
		for (size_t index = 0; index < possibleMove.size(); ++index) {
			if (possibleMove[index] == 0) {
				buttonID += "9";
			}
			else {
				buttonID += std::to_string(possibleMove[index]);
			}
		}
		wxButton* button = (wxButton*)FindWindowById(std::stoi(buttonID));
		
		if (game->legal(possibleMove, convertToNextPlayer(game->getLastPlayer()))) {
			button->SetBackgroundColour(*wxYELLOW);
		}
		else {
			if (button->GetBackgroundColour() == *wxYELLOW) {
				button->SetBackgroundColour(*wxBLACK);
			}
		}
	}
	else {
		for (int index = 0; index < 9; ++index) {
			std::vector<int> appendCoordinate = possibleMove;
			appendCoordinate.push_back(index);
			markNewLegal(appendCoordinate);
		}
	}
}

#endif
