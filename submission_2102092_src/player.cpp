#include "player.hpp"
#include <cstdlib>
#include <limits>
#include <algorithm>

using namespace std;

namespace TICTACTOE3D
{	
	// Needed to sort the sons by their gamma
	bool Player::compareByGamma(pair<int,int> a, pair<int,int> b)
	{
	    return a.second < b.second;
	}
	
	// Clever evaluation function
	// if terminal state, it returns -intmax, 0 or intmax for OWin, Draw, XWin
	// Otherwise :
	// 1 X in a free valid row gives 1 point, 2 give 10, 3 give 100
	// 1 O in a free valid row gives -1 point, 2 give -10, 3 give -100
	int Player::gammafct(const GameState &pState)
	{
		int intmax = numeric_limits<int>::max();
		if (pState.isXWin())
			return intmax - 10;
		else if (pState.isOWin())
			return -(intmax - 10);
		else if (pState.isDraw())
			return 0;
		else
		{
			// Clever evaluation function
			int v = 0;
			int i,j,k;
			int vX, vO;
			bool freeforX, freeforO;
			// ==============================================
			// ============ ORTHOGONAL ROWS =================
			// ==============================================
			// Vertical rows
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++)
				{
					vX = 0; vO = 0;
					freeforX = true; freeforO = true;
					for (k=0;k<4;k++)
					{
						switch (pState.at(i,j,k))
						{
							case CELL_EMPTY:
							break;
							case CELL_X:
							vX++;
							freeforO = false;
							break;
							case CELL_O:
							vO++;
							freeforX = false;
							break;
							case CELL_INVALID:
							cerr<<"ERROR: The cell falls outside the board.\n";
							break;
						}
					}
					if (freeforX)
						v += weights[vX];
					if (freeforO)
						v -= weights[vO];
				}
			}
			// Horizontal rows
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++)
				{
					vX = 0; vO = 0;
					freeforX = true; freeforO = true;
					for (k=0;k<4;k++)
					{
						switch (pState.at(i,k,j))
						{
							case CELL_EMPTY:
							break;
							case CELL_X:
							vX++;
							freeforO = false;
							break;
							case CELL_O:
							vO++;
							freeforX = false;
							break;
							case CELL_INVALID:
							cerr<<"ERROR: The cell falls outside the board.\n";
							break;
						}
					}
					if (freeforX)
						v += weights[vX];
					if (freeforO)
						v -= weights[vO];
				}
			}
			// "Wideness" rows
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++)
				{
					vX = 0; vO = 0;
					freeforX = true; freeforO = true;
					for (k=0;k<4;k++)
					{
						switch (pState.at(k,i,j))
						{
							case CELL_EMPTY:
							break;
							case CELL_X:
							vX++;
							freeforO = false;
							break;
							case CELL_O:
							vO++;
							freeforX = false;
							break;
							case CELL_INVALID:
							cerr<<"ERROR: The cell falls outside the board.\n";
							break;
						}
					}
					if (freeforX)
						v += weights[vX];
					if (freeforO)
						v -= weights[vO];
				}
			}
			// ============================================
			// ============ DIAGONAL ROWS =================
			// ============================================
			// === YZ plane ===
			for (i=0;i<4;i++)
			{
				// Descending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(i,j,j))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
				
				// Ascending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(i,3-j,j))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
			}
			// === XY plane ===
			for (i=0;i<4;i++)
			{
				// Descending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(j,j,i))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
				
				// Ascending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(j,3-j,i))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
			}
			// === XZ plane ===
			for (i=0;i<4;i++)
			{
				// Descending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(j,i,j))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
				
				// Ascending diagonal
				vX = 0; vO = 0;
				freeforX = true; freeforO = true;
				for (j=0;j<4;j++)
				{
					switch (pState.at(3-j,i,j))
					{
						case CELL_EMPTY:
						break;
						case CELL_X:
						vX++;
						freeforO = false;
						break;
						case CELL_O:
						vO++;
						freeforX = false;
						break;
						case CELL_INVALID:
						cerr<<"ERROR: The cell falls outside the board.\n";
						break;
					}
				}
				if (freeforX)
					v += weights[vX];
				if (freeforO)
					v -= weights[vO];
			}
			// =============================================
			// ============ MAIN DIAGONALS =================
			// =============================================
			// Diag 1
			vX = 0; vO = 0;
			freeforX = true; freeforO = true;
			for (i=0;i<4;i++)
			{
				switch (pState.at(i,i,i))
				{
					case CELL_EMPTY:
					break;
					case CELL_X:
					vX++;
					freeforO = false;
					break;
					case CELL_O:
					vO++;
					freeforX = false;
					break;
					case CELL_INVALID:
					cerr<<"ERROR: The cell falls outside the board.\n";
					break;
				}
			}
			if (freeforX)
				v += weights[vX];
			if (freeforO)
				v -= weights[vO];
			// Diag 2
			vX = 0; vO = 0;
			freeforX = true; freeforO = true;
			for (i=0;i<4;i++)
			{
				switch (pState.at(3-i,i,i))
				{
					case CELL_EMPTY:
					break;
					case CELL_X:
					vX++;
					freeforO = false;
					break;
					case CELL_O:
					vO++;
					freeforX = false;
					break;
					case CELL_INVALID:
					cerr<<"ERROR: The cell falls outside the board.\n";
					break;
				}
			}
			if (freeforX)
				v += weights[vX];
			if (freeforO)
				v -= weights[vO];
			// Diag 3
			vX = 0; vO = 0;
			freeforX = true; freeforO = true;
			for (i=0;i<4;i++)
			{
				switch (pState.at(i,3-i,i))
				{
					case CELL_EMPTY:
					break;
					case CELL_X:
					vX++;
					freeforO = false;
					break;
					case CELL_O:
					vO++;
					freeforX = false;
					break;
					case CELL_INVALID:
					cerr<<"ERROR: The cell falls outside the board.\n";
					break;
				}
			}
			if (freeforX)
				v += weights[vX];
			if (freeforO)
				v -= weights[vO];
			// Diag 4
			vX = 0; vO = 0;
			freeforX = true; freeforO = true;
			for (i=0;i<4;i++)
			{
				switch (pState.at(i,i,3-i))
				{
					case CELL_EMPTY:
					break;
					case CELL_X:
					vX++;
					freeforO = false;
					break;
					case CELL_O:
					vO++;
					freeforX = false;
					break;
					case CELL_INVALID:
					cerr<<"ERROR: The cell falls outside the board.\n";
					break;
				}
			}
			if (freeforX)
				v += weights[vX];
			if (freeforO)
				v -= weights[vO];
			return v;
		}
	}
	/*
	* AB pruning where we explore the sons by beginning with those with the largest gamma
	*/
	int Player::alphabeta(const GameState &pState, int depth, int alpha, int beta, uint8_t me)
	{
		int intmax = numeric_limits<int>::max();
		vector<GameState> lNextStates;
		pState.findPossibleMoves(lNextStates);
		int nbMoves = lNextStates.size();
		int v;
		if (depth == 0 || lNextStates.size() == 0)
			v = gammafct(pState);
		else
		{
			// Begin by searching sons with the largest gammafct
			// We create a table with the sons and their gamma
			// Then we sort it
			// Then we explore the sons by beginning with those with the largest gamma
			// => More efficient AB pruning
			vector < pair<int,int> > indGamma;
			for (int i=0; i<nbMoves; i++)
				indGamma.push_back(make_pair(i,gammafct(lNextStates[i])));
			sort(indGamma.begin(), indGamma.end(), compareByGamma);
			
			if (pState.getNextPlayer() == me)
			{
				v = -intmax;
				for (int i=0; i<lNextStates.size(); i++)
				{
					int ind = indGamma[i].first;
					v = max(v, alphabeta(lNextStates[ind], depth-1, alpha, beta, me));
					alpha = max(alpha, v);
					if (beta <= alpha)
						break;
				}
			}
			else
			{
				v = intmax;
				for (int i=0; i<lNextStates.size(); i++)
				{
					int ind = indGamma[i].first;
					v = min(v, alphabeta(lNextStates[ind], depth-1, alpha, beta, me));
					beta = min(beta, v);
					if (beta <= alpha)
						break;
				}
			}
		}
		return v;
	}

	GameState Player::play(const GameState &pState,const Deadline &pDue)
	{
		cerr << "Processing " << pState.toMessage() << endl;

		vector<GameState> lNextStates;
		pState.findPossibleMoves(lNextStates);
		int nbMoves = lNextStates.size();

		if (lNextStates.size() == 0) return GameState(pState, Move());

		uint8_t me = pState.getNextPlayer();
		int intmax = numeric_limits<int>::max();
		
		// Weights for the heuristic
		weights.push_back(0);   // 0 aligned X/O
		weights.push_back(1);   // 1 aligned X/O
		weights.push_back(10);  // 2 aligned X/O
		weights.push_back(100); // 3 aligned X/O
		
		int v = -intmax;
		int bestMove = -1;
		int depth = 1;
		int alpha = -intmax;
		int beta = intmax;
		
		// Begin by searching sons with the largest gammafct
		vector < pair<int,int> > indGamma;
		for (int i=0; i<nbMoves; i++)
			indGamma.push_back(make_pair(i,gammafct(lNextStates[i])));
		sort(indGamma.begin(), indGamma.end(), compareByGamma);
		
		// First step of AB pruning
		for (int i=0; i<nbMoves; i++)
		{
			int ind = indGamma[i].first;
			int childV = alphabeta(lNextStates[ind], depth-1, alpha, beta, me);
			if (childV > v)
			{
				v = childV;
				bestMove = ind;
				alpha = v;
			}
		}
		return lNextStates[bestMove];
	}
} /*namespace TICTACTOE3D*/
