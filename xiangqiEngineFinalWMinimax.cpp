#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <climits>
using namespace std;

int evaluation(string board[10][9], map <char, int> &v){
	//check if both generals are present

	//black general
	bool bPresent = false;
	for (int row = 0; row < 3; ++row){
		for (int col = 3; col < 6; ++col){
			if (board[row][col] == "Bg"){
				bPresent = true;
				break;
			}
		}
	}
	if (!bPresent) return INT_MAX;

	//red general
	bool rPresent = false;
	for (int row = 7; row < 10; ++row){
		for (int col = 3; col < 6; ++col){
			if (board[row][col] == "Rg"){
				rPresent = true;
				break;
			}
		}
	}
	if (!rPresent) return -INT_MAX;

	//evaluate position
	int score = 0;
	for (int row = 0; row < 10; ++row){
		for (int col = 0; col < 9; ++col){
			string piece = board[row][col];
			if (piece == ".") continue;
			int value = v[piece[1]];
			if (piece[0] == 'B') score -= value;
			else score += value;
		}
	}
	return score;
}

string makeMove(string board[10][9], int startRow, int startCol, int endRow, int endCol){
	string p = board[endRow][endCol];
	board[endRow][endCol] = board[startRow][startCol];
	board[startRow][startCol] = ".";
	if (p == ".") return "";
	return p;
}

vector <int> genMove(int row, int col, int newRow, int newCol){
	vector <int> curMove;
	curMove.push_back(row);
	curMove.push_back(col);
	curMove.push_back(newRow);
	curMove.push_back(newCol);
	return curMove;
}

void undoMove(string board[10][9], int startRow, int startCol, int endRow, int endCol, string pieceCaptured){
	board[startRow][startCol] = board[endRow][endCol]; //move piece back to start
	if (pieceCaptured == "") board[endRow][endCol] = ".";
	else board[endRow][endCol] = pieceCaptured;
}

bool outOfBounds(int val, int min, int max){
	if (val < min) return true;
	if (val > max) return true;
	return false;
}

bool canMove(string board[10][9], int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	string piece = board[row][col];
	if (piece == ".") return true;
	else if (piece[0] == oppSide) return true;
	return false;
}

void rookMoves(string board[10][9], vector < vector <int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	int offset[4][2] = {
		{1, 0},
		{-1, 0},
		{0, 1},
		{0, -1}
	};
	for (int i = 0; i < 4; ++i){
		int curRow = row;
		int curCol = col;
		while (true){
			curRow += offset[i][0];
			curCol += offset[i][1];
			if (outOfBounds(curRow, 0, 9) || outOfBounds(curCol, 0, 8)) break;
			//not out of bounds - check if piece is on the same side
			if (canMove(board, curRow, curCol, side)) m.push_back(genMove(row, col, curRow, curCol)); //check if its empty/piece of opposing side
			else break; //cant capture the piece
		}
	}
}

void knightMoves(string board[10][9], vector < vector <int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	int offSet[8][4] = {
		{-2, -1, -1, 0},
		{-2, 1, -1, 0},
		{-1, 2, 0, 1},
		{1, 2, 0, 1},
		{2, 1, 1, 0},
		{2, -1, 1, 0},
		{1, -2, 0, -1},
		{-1, -2, 0, -1}
	};
	for (int i = 0; i < 8; ++i){
		int newRow = row + offSet[i][0];
		int newCol = col + offSet[i][1];
		//check if out of bounds
		if (outOfBounds(newRow, 0, 9) || outOfBounds(newCol, 0, 8)) continue;
		//piece at 马腿
		string pcMaTui = board[row + offSet[i][2]][col + offSet[i][3]]; //should be empty
		//piece at new position
		string newPosPc = board[newRow][newCol];
		if (pcMaTui == ".") {
    		if (newPosPc == ".") m.push_back(genMove(row, col, newRow, newCol));
    		else if (newPosPc[0] == oppSide) m.push_back(genMove(row, col, newRow, newCol));
		} 
	}
}

void elephantMoves(string board[10][9], vector < vector <int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	int offSet[4][4] = {
		{-2, -2, -1, -1},
		{-2, 2, -1, 1},
		{2, -2, 1, -1},
		{2, 2, 1, 1}
	};
	for (int i = 0; i < 4; ++i){
		int newRow = row + offSet[i][0];
		if (side == 'R' && newRow < 5) continue; // Red elephant can't cross
		if (side == 'B' && newRow > 4) continue; // Black elephant can't cross
		int newCol = col + offSet[i][1];
		//check if out of bounds
		if (outOfBounds(newRow, 0, 9) || outOfBounds(newCol, 0, 8)) continue;
		//piece at 马腿
		string pcMaTui = board[row + offSet[i][2]][col + offSet[i][3]]; //should be empty
		//piece at new position
		string newPosPc = board[newRow][newCol];
		if (pcMaTui == ".") {
    		if (newPosPc == ".") m.push_back(genMove(row, col, newRow, newCol));
    		else if (newPosPc[0] == oppSide) m.push_back(genMove(row, col, newRow, newCol));
		} 
	}
}

void advisorMoves(string board[10][9], vector < vector <int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	int jiuGong[4];
	if (side == 'R'){
		jiuGong[0] = 7;
		jiuGong[1] = 3;
		jiuGong[2] = 9;
		jiuGong[3] = 5;
	}
	else{
		jiuGong[0] = 0;
		jiuGong[1] = 3;
		jiuGong[2] = 2;
		jiuGong[3] = 5;
	}
	int offset[4][2] = {
		{1, 1},
		{1, -1},
		{-1, 1},
		{-1, -1}
	};
	for (int i = 0; i < 4; ++i){
		int newRow = row + offset[i][0];
		int newCol = col + offset[i][1];
		if (outOfBounds(newRow, jiuGong[0], jiuGong[2]) || outOfBounds(newCol, jiuGong[1], jiuGong[3])) continue;
		if (canMove(board, newRow, newCol, side)) m.push_back(genMove(row, col, newRow, newCol));
	}
}

void generalMoves(string board[10][9], vector < vector<int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	int jiuGong[4];
	if (side == 'R'){
		jiuGong[0] = 7;
		jiuGong[1] = 3;
		jiuGong[2] = 9;
		jiuGong[3] = 5;
	}
	else{
		jiuGong[0] = 0;
		jiuGong[1] = 3;
		jiuGong[2] = 2;
		jiuGong[3] = 5;
	}
	int offset[4][2] = {
		{0, 1},
		{0, -1},
		{1, 0},
		{-1, 0}
	};
	for (int i = 0; i < 4; ++i){
		int newRow = row + offset[i][0];
		int newCol = col + offset[i][1];
		if (outOfBounds(newRow, jiuGong[0], jiuGong[2]) || outOfBounds(newCol, jiuGong[1], jiuGong[3])) continue;
		if (canMove(board, newRow, newCol, side)){
			m.push_back(genMove(row, col, newRow, newCol));
		}
	}
}

void soldierMoves(string board[10][9], vector < vector <int> > &m, int row, int col, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	if (side == 'R'){
		//can only move 1 space forward if 还没过河, but 要是过河了就能左右移动了
		if (row >= 5){
			//haven't 过河 - can only move 1 space forward at a time
			int newRow = row - 1;
			if (canMove(board, newRow, col, side)) m.push_back(genMove(row, col, newRow, col));
		}
		else{
			int offset[3][2] = {
				{0, 1},
				{0, -1},
				{-1, 0}
			};
			for (int i = 0; i < 3; ++i){
				int newRow = row + offset[i][0];
				int newCol = col + offset[i][1];
				if (outOfBounds(newRow, 0, 9) || outOfBounds(newCol, 0, 8)) continue;
				if (canMove(board, newRow, newCol, side)) m.push_back(genMove(row, col, newRow, newCol));
			}
		}
	}
	else{
		if (row <= 4){
			//haven't 过河 - can only move 1 space forward at a time
			int newRow = row + 1;
			if (canMove(board, newRow, col, side)) m.push_back(genMove(row, col, newRow, col));
		}
		else{
			int offset[3][2] = {
				{0, 1},
				{0, -1},
				{1, 0}
			};
			for (int i = 0; i < 3; ++i){
				int newRow = row + offset[i][0];
				int newCol = col + offset[i][1];
				if (outOfBounds(newRow, 0, 9) || outOfBounds(newCol, 0, 8)) continue;
				if (canMove(board, newRow, newCol, side)) m.push_back(genMove(row, col, newRow, newCol));
			}
		}
	}
}

void cannonMoves(string board[10][9], vector < vector<int> > &m, int row, int col, char s){
	char oppSide = (s == 'R' ? 'B' : 'R');
	int offset[4][2] = {
		{1, 0}, //down
		{-1, 0}, //up
		{0, 1}, //right
		{0, -1} //left
	};

	bool blocked = false;

	for (int i = 0; i < 4; ++i){
		blocked = false;
		
		int curRow = row;
		int curCol = col;

		//move like a rook until there's a piece blocking it
		while (true){
			curRow += offset[i][0];
			curCol += offset[i][1];

			if (outOfBounds(curRow, 0, 9) || outOfBounds(curCol, 0, 8)) break; //if out of bounds

			if (board[curRow][curCol] == ".") m.push_back(genMove(row, col, curRow, curCol));
			else{
				blocked = true;
				break; //oh damn theres a piece there
			}
		}

		//jump over pieces
		if (!blocked) continue; //did not break out of loop because of a piece

		//now we actually jump over pieces
		curRow += offset[i][0];
		curCol += offset[i][1];
		while (!(outOfBounds(curRow, 0, 9) || outOfBounds(curCol, 0, 8))){
			//while were not out of bounds lol whaddya expect
			if (board[curRow][curCol] != "."){
				//if theres a piece there
				if (board[curRow][curCol][0] == oppSide) m.push_back(genMove(row, col, curRow, curCol));
				break;
			}
			curRow += offset[i][0];
			curCol += offset[i][1];
		}
	}
}

void generateMoves(string board[10][9], vector < vector <int> > &m, char s){
	//generate all legal moves
	m.clear(); //clear vector
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 9; ++j){
			string piece = board[i][j];
			if (piece == ".") continue;
			char type = piece[1];
			char side = piece[0];
			if (side != s) continue;
			if (type == 'r'){
				//generate rook moves for this
				rookMoves(board, m, i, j, side);
			}
			else if (type == 'h'){
				//generate knight moves
				knightMoves(board, m, i, j, side);
			}
			else if (type == 'e'){
				//generate elephant moves
				elephantMoves(board, m, i, j, side);
			}
			else if (type == 'a'){
				//advisor moves
				advisorMoves(board, m, i, j, side);
			}
			else if (type == 'g'){
				//general moves
				generalMoves(board, m, i, j, side);
			}
			else if (type == 's'){
				//soldier moves
				soldierMoves(board, m, i, j, side);
			}
			else{
				//cannon moves
				cannonMoves(board, m, i, j, side);
			}
		}
	}
}


pair < vector <int>, int > minimax(string board[10][9], int depth, int alpha, int beta, bool maximisingPlayer, char side){
	char oppSide = (side == 'R' ? 'B' : 'R');
	map <char, int> values;
	values['r'] = 9;
	values['h'] = 4;
	values['e'] = 2;
	values['a'] = 2;
	values['c'] = 5;
	values['s'] = 1;
	values['g'] = 100;
	if (depth == 0){
		pair < vector <int>, int > returnData;
		returnData.first = genMove(-1, -1, -1, -1);
		returnData.second = evaluation(board, values);
		return returnData;
	}

	//depth isn't 0
	if (maximisingPlayer){
		//red's turn
		int bestEval = -INT_MAX;
		vector <int> bestMove = genMove(-1, -1, -1, -1);
		vector < vector <int> > moves;

		generateMoves(board, moves, 'R');
		for (const auto mv : moves){
			//start of minimax
			string c = makeMove(board, mv[0], mv[1], mv[2], mv[3]);

			//check for flying general rule
			
			//1. find location of red general
			int genRow = -1, genCol = -1;
			bool found = false;
			for (int row = 7; row < 10; ++row){
				for (int col = 3; col < 6; ++col){
					if (board[row][col] == "Rg"){
						genRow = row;
						genCol = col;
						found = true;
						break;
					}
				}
				if (found) break;
			}
			//2. look down the column and find if there's any pieces between red and black general
			bool foundPiece = false; //found a piece between black general and red general
			for (int row = genRow; row >= 0; --row){
				if (board[row][genCol] == "Bg") break;
				else if (board[row][genCol] != "."){
					foundPiece = true;
					break;
				}
			}
			if (!foundPiece){
				undoMove(board, mv[0], mv[1], mv[2], mv[3], c);
				continue; //flying general rule violated
			}

			//calculate best move
			int eval = minimax(board, depth-1, alpha, beta, false, 'B').second;

			undoMove(board, mv[0], mv[1], mv[2], mv[3], c);

			if (eval > bestEval){
				bestEval = eval;
				bestMove = mv;
			}

			alpha = max(alpha, eval);
			if (alpha >= beta) break; //prune
		}

		pair < vector <int>, int > returnData;
		returnData.first = bestMove;
		returnData.second = bestEval;

		return returnData;
	}
	else{
		//black's turn
		int bestEval = INT_MAX;
		vector <int> bestMove;
		vector < vector <int> > moves;
		for (int i = 0; i < 4; ++i) bestMove.push_back(-1);

		generateMoves(board, moves, 'B');
		for (const auto mv : moves){
			string c = makeMove(board, mv[0], mv[1], mv[2], mv[3]);

			int eval = minimax(board, depth-1, alpha, beta, true, 'R').second;

			undoMove(board, mv[0], mv[1], mv[2], mv[3], c);

			if (eval < bestEval){
				bestEval = eval;
				bestMove = mv;
			}
			beta = min(beta, eval);
			if (alpha >= beta) break;
		}

		pair < vector <int>, int > returnData;
		returnData.first = bestMove;
		returnData.second = bestEval;

		return returnData;
	}
}

int main(){
	//declare variables
	string board[10][9] = {
		{"Br", "Bh", "Be", "Ba", "Bg", "Ba", "Be", "Bh", "Br"},
		{".", ".", ".", ".", ".", ".", ".", ".", "."},
		{".", "Bc", ".", ".", ".", ".", ".", "Bc", "."},
		{"Bs", ".", "Bs", ".", "Bs", ".", "Bs", ".", "Bs"},
		{".", ".", ".", ".", ".", ".", ".", ".", "."},
		{".", ".", ".", ".", ".", ".", ".", ".", "."},
		{"Rs", ".", "Rs", ".", "Rs", ".", "Rs", ".", "Rs"},
		{".", "Rc", ".", ".", ".", ".", ".", "Rc", "."},
		{".", ".", ".", ".", ".", ".", ".", ".", "."},
		{"Rr", "Rh", "Re", "Ra", "Rg", "Ra", "Re", "Rh", "Rr"}
	};

	int inf = INT_MAX;

	map <char, int> values;
	values['r'] = 9;
	values['h'] = 4;
	values['e'] = 2;
	values['a'] = 2;
	values['c'] = 5;
	values['s'] = 1;
	values['g'] = 100;


	//input depth
	int depth = 5;
	//cin >> depth;

	/*auto result = minimax(board, depth, -INT_MAX, INT_MAX, true, 'R');

	cout << result.second << "\n";
	cout << "Best move: (" << result.first[0] << "," << result.first[1]
     << ") -> (" << result.first[2] << "," << result.first[3] << ")\n";*/

	while (true){
		int userInput[4];
		for (int i = 0; i < 4; ++i) cin >> userInput[i];
		makeMove(board, userInput[0], userInput[1], userInput[2], userInput[3]);
		cout << "You have made a move - when you get checkmated or you checkmate the AI, please press ctrl-c to exit\n";
		auto result = minimax(board, depth, -INT_MAX, INT_MAX, false, 'B');
		cout << "Best Eval: " << result.second << "\n";
		if (result.second == INT_MAX){
			cout << "the bot resigns. it sees a checkmate. ggs!\n";
			break;
		}
		cout << "Best move: (" << result.first[0] << "," << result.first[1] << ") -> (" << result.first[2] << "," << result.first[3] << ")\n";
		makeMove(board, result.first[0], result.first[1], result.first[2], result.first[3]);
	}
}