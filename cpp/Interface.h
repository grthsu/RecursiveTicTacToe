//Copyright: Gregory Su
//May 2018

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <vector>
#include <string>
#include "Logic.h"

//CLASS: simple interface
class Interface {
public:

	//EFFECTS: creates a new Interface object for a game of depth size
	Interface(unsigned size);

	//EFFECTS: Returns true if legal move
	bool legal(const std::vector<int> & input, Logic::owner player) const;

	//MODIFIES: *this
	//EFFECTS: Adds the move to the storage in logic
	//NOTE: Does NOT check for move legality
	void addMove(const std::vector<int> & input, Logic::owner player);
	
	virtual ~Interface() {}

	//EFFECTS: Returns the last player
	Logic::owner getLastPlayer() const;

	//MODIFIES: *this::logic
	//EFFECTS: Returns the game's victor, if any.  Also updates any victors on subboards.
	Logic::owner victor();

	//EFFECTS: Returns the owner of the grid with the given coordinates
	Logic::owner getGridOwner(const std::vector<int> & coordinates) const;

	//Primary driver for game play.  Maybe.
	//virtual void playGame();

private:
	Logic logic;
};

//EFFECTS: Returns a pointer to an interface of the given game size and type.
//NOTE: default size = 2 (Ultimate Tic Tac Toe), no inputs = default interface of depth 2
//TYPES: default (or no input)
Interface* InterfaceFactory(unsigned size = 2, const std::string & type = "default");



void Interface::addMove(const std::vector<int> & input, Logic::owner player) {
	logic.addMove(input, player);
}

Interface::Interface(unsigned size)
	: logic(Logic(size)) {}

bool Interface::legal(const std::vector<int> & input, Logic::owner player) const {
	return logic.legal(input, player);
}

Interface* InterfaceFactory(unsigned size, const std::string & type) {
	return new Interface(size);
}

Logic::owner Interface::getLastPlayer() const {
	return logic.getLastPlayer();
}

Logic::owner Interface::victor() {
	return logic.victor();
}

Logic::owner Interface::getGridOwner(const std::vector<int> & coordinates) const {
	return logic.getGridOwner(coordinates);
}

#endif
