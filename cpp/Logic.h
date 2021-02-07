//Copyright: Gregory Su
//May 2018

#ifndef __LOGIC_H__
#define __LOGIC_H__

#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <string>

//CLASS: Calculates logic and stores information about the boards
class Logic {
public:

	//ENUMERATION: Refers to the current owner of a given object
	//  none = no owner, one = owned by player one, two = owned by player two
	//  draw = an object owned by neither played that has been completely claimed
	enum owner {
		none, one, two, draw
	};

	//EFFECTS: Initializes storage for memory of a game of depth size
	Logic(unsigned size);

	//EFFECTS: Returns the legality of the given move.  Only considers the last move made.
	//EXAMPLE: If the last move was [8, 6, 1], [6, 1, 2] and [6, 1, 4] would both be legal,
	//	but [8, 6, 2], [6, 2, 1], and [4, 5, 3] would be illegal.
	bool legal(const std::vector<int> & input, owner player) const;

	//MODIFES: *this::lastMove
	//EFFECTS: records the given move as the last move made
	//NOTE: Does NOT check legality
	void addMove(const std::vector<int> & input, owner player);

	//EFFECTS: Returns the last player
	owner getLastPlayer() const;

	//EFFECTS: Returns the game's victor, if any.  Also updates victors for subboards.
	owner victor();

	//EFFECTS: Returns the owner of the grid with the given coordinates
	owner getGridOwner(const std::vector<int> & coordinates) const;

	//Think about writing moves to file for later reloading

private:

	//VARIABLE: record of the last move
	std::vector<int> lastMove;

	//VARIABLE: record of how many level of board are being played
	unsigned depth;

	//VARIABLE: record of the last player to make a move
	owner lastPlayer;

	//CLASS: Any object placed in a grid
	class GridItem {
	public:
		//EFFECTS: Constructor, Initializes state to none
		GridItem();

		//EFFECTS: Destructor
		virtual ~GridItem() {}

		//MODIFIES: state
		//EFFECTS: sets state to player
		void setOwner(owner player);

		//EFFECTS: returns state/current player owner
		owner getOwner() const;

		//MODIFIES: *this::memory (the master board)
		//EFFECTS: returns state/current player owner
		virtual owner victor();

		//REQUIRES: input.size() == 0 (there should no coordinate if this is not a Grid)
		//MODIFIES: state
		//EFFECTS: sets state to player
		virtual void addMove(const std::vector<int> & input, owner player);

		//EFFECTS: returns getOwner()
		virtual owner getGridItemOwner(const std::vector<int> & coordinates) const;

		//EFFECTS: returns true if state == owner::none
		virtual bool legal(const std::vector<int> & input) const;

		//EFFECTS: Returns empty set.  Provided because used by derived class Grid
		virtual std::vector<int> checkInput(const std::vector<int> & input, int returnValue) const {
			return {};
		}

	private:
		//VARIABLE: current player owner of the object
		owner state;
	};

	//CLASS: grid of nine GridItems, can be placed in another grid
	class Grid : public GridItem {
	public:

		//REQUIRES: level > 0
		//EFFECTS: sets GridItem::state to none, creates and fills a grid of nine unique_ptrs
		//  to GridItems
		//NOTE: If level > 1, calls nine Grid(level - 1); if level == 1, calls nine GridItems
		Grid(unsigned level);

		//EFFECTS: destructor
		virtual ~Grid() {}

		//REQUIRES: input.size() > 0
		//MODIFIES: recursion
		//EFFECTS: calls addMove on GridItem in recursion at position input[0], passing input after [0]
		//NOTE: does not check state before or after
		virtual void addMove(const std::vector<int> & input, owner player);

		//MODIFIES: GridItem::state
		//EFFECTS: returns current state/player owner of the Grid/board,
		//  checks for a new victor if GridItem::state == none, sets state to any new owner or tie
		virtual owner victor();

		//EFFECTS: Returns the owner of the GridItem with the given coordinates.
		//	Returns its own owner if coordinates.size() == 0.
		virtual owner getGridItemOwner(const std::vector<int> & coordinates) const;

		//EFFECTS: Returns true if the button at the coordinates defined by input is unclaimed
		//	and no boards that the button is in have been claimed.
		//	Returns getOwner() == owner::none if input.size() == 0.
		virtual bool legal(const std::vector<int> & input) const;


		//REQUIRES: input has no elements greater than 8
		//EFFECTS: If the grid is claimed or complete, returns the empty vector.
		//	If input is empty, returns a vector containing returnValue.
		//	Otherwise, returns {input[0], checkInput(input starting at 1, input[0])}
		//		If returnValue == 9, does not prepend input[0] to the recursive answer.
		virtual std::vector<int> checkInput(const std::vector<int> & input, int returnValue) const;

	private:
		//VARIABLE: storage for the GridItems that make up the Grid
		std::vector < std::unique_ptr<GridItem> > recursion;
	};

	//VARIABLE: the master board
	Grid memory;

	//EFFECTS: if every GridItem at each number of increments starting from the start index
	//	to the terminate index have the same owner, returns true; otherwise, false
	//EXAMPLE: checkEquality(board, 3, 1, 8) or (board, 3, 1, 7): 1 == 4 == 7
	static bool checkEquality(const std::vector< std::unique_ptr<GridItem> > & board, unsigned increment,
		unsigned start, unsigned terminate);
};

//EFFECTS: returns the next player given the last player.
//	Returns owner::two if passed owner::one; otherwise, returns owner::one
inline Logic::owner convertToNextPlayer(Logic::owner state);

//EFFECTS: Returns a string describing the state.
//	owner::one returns "Player 1", owner::two returns "Player 2," otherwise returns "Nobody"
inline std::string convertToString(Logic::owner state);



void Logic::addMove(const std::vector<int> & input, owner player) {
	lastMove = input;
	lastPlayer = player;
	memory.addMove(input, player);
}

void Logic::GridItem::addMove(const std::vector<int> & input, owner player) {
	assert(input.size() == 0);
	setOwner(player);
}

void Logic::Grid::addMove(const std::vector<int> & input, owner player) {
	recursion.at(input.at(0))->addMove(std::vector<int>(input.begin() + 1, input.end()), player);
}

bool Logic::legal(const std::vector<int> & input, owner player) const {
	if (!memory.legal(input)) {
		return false;
	}
	if (lastMove.size() == 0 || lastMove.size() == 1) {
		return true;
	}
	if (player == lastPlayer) {
		return false;
	}

	std::vector<int> compare(lastMove.begin() + 1, lastMove.end());
	compare = memory.checkInput(compare, 9);

	for (size_t index = 0; index < compare.size(); ++index) {
		if (input[index] != compare[index]) {
			return false;
		}
	}

	return true;
}

Logic::Logic(unsigned size)
	: depth(size), lastPlayer(none), memory(Grid(size)) {}

Logic::owner Logic::getLastPlayer() const {
	return lastPlayer;
}

Logic::owner Logic::getGridOwner(const std::vector<int> & coordinates) const {
	return memory.getGridItemOwner(coordinates);
}

Logic::GridItem::GridItem()
	: state(none) {}

Logic::Grid::Grid(unsigned level) {
	assert(level > 0);
	for (unsigned index = 0; index < 9; ++index) {
		if (level == 1) {
			recursion.emplace_back(new GridItem);
		}
		else {
			recursion.emplace_back(new Grid(level - 1));
		}
	}
}

Logic::owner Logic::GridItem::victor() {
	return getOwner();
}

void Logic::GridItem::setOwner(owner player) {
	state = player;
}

Logic::owner Logic::GridItem::getOwner() const {
	return state;
}

Logic::owner Logic::GridItem::getGridItemOwner(const std::vector<int> & coordinates) const {
	return getOwner();
}

bool Logic::GridItem::legal(const std::vector<int> & input) const {
	return state == owner::none;
}

bool Logic::Grid::legal(const std::vector<int> & input) const {
	if (input.size() == 0) {
		return getOwner() == owner::none;
	}
	return (getOwner() == owner::none
		&& recursion.at(input.at(0))->legal(std::vector<int>(input.begin() + 1, input.end())));
}

std::vector<int> Logic::Grid::checkInput(const std::vector<int> & input, int returnValue) const {
	if (getOwner() != owner::none) {
		return {};
	}
	if (input.size() == 0) {
		return { returnValue };
	}
	std::vector<int> temporary =
		recursion[input[0]]->checkInput(std::vector<int>(input.begin() + 1, input.end()), input[0]);
	if (returnValue != 9) {
		temporary.insert(temporary.begin(), returnValue);
	}
	return temporary;
}

Logic::owner Logic::Grid::getGridItemOwner(const std::vector<int> & coordinates) const {
	if (coordinates.size() == 0) {
		return getOwner();
	}
	return recursion.at(coordinates.at(0))->
		getGridItemOwner(std::vector<int>(coordinates.begin() + 1, coordinates.end()));
}

bool Logic::checkEquality(const std::vector< std::unique_ptr<GridItem> > & board, unsigned increment,
		unsigned start, unsigned terminate) {
	Logic::owner player = board.at(start)->victor();
	for (unsigned index = start; index <= terminate; index += increment) {
		if (board.at(index)->victor() != player) {
			return false;
		}
	}
	return true;
}

Logic::owner Logic::Grid::victor() {
	if (getOwner() != owner::none) {
		return getOwner();
	}
	else {
		for (size_t index = 0; index < recursion.size(); index += 3) {
			if (recursion[index]->victor() != owner::none &&
					checkEquality(recursion, 1, index, index + 2)) {
				setOwner(recursion[index]->getOwner());
				return getOwner();
			}
		}
		for (size_t index = 0; index < 3; index += 1) {
			if (recursion[index]->victor() != owner::none
					&& checkEquality(recursion, 3, index, index + 6)) {
				setOwner(recursion[index]->getOwner());
				return getOwner();
			}
		}
		if (recursion[0]->victor() != owner::none && checkEquality(recursion, 4, 0, 8)) {
			setOwner(recursion[0]->getOwner());
			return getOwner();
		}
		if (recursion[2]->victor() != owner::none && checkEquality(recursion, 2, 2, 6)) {
			setOwner(recursion[2]->getOwner());
			return getOwner();
		}
		bool allTaken = true;
		for (size_t index = 0; index < recursion.size(); ++index) {
			if (recursion[index]->getOwner() == owner::none) {
				allTaken = false;
				break;
			}
		}
		if (allTaken) {
			setOwner(owner::draw);
		}
	}
	for (size_t index = 0; index < recursion.size(); ++index) {
		recursion[index]->victor();
	}
	return getOwner();
}

inline Logic::owner convertToNextPlayer(Logic::owner state) {
	if (state == Logic::owner::one) {
		return Logic::owner::two;
	}
	return Logic::owner::one;
}

inline std::string convertToString(Logic::owner state) {
	switch (state) {
	case Logic::owner::one:
		return "Player 1";
	case Logic::owner::two:
		return "Player 2";
	default:
		return "Nobody";
	}
}

Logic::owner Logic::victor() {
	return memory.victor();
}

#endif
