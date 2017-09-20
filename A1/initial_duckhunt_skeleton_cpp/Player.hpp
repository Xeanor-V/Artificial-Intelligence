#ifndef _DUCKS_PLAYER_HPP_
#define _DUCKS_PLAYER_HPP_

#include "Deadline.hpp"
#include "GameState.hpp"
#include "Action.hpp"
#include <vector>
#include "HMM.hpp"

namespace ducks
{

class Player
{
public:
    /**
     * Constructor
     * There is no data in the beginning, so not much should be done here.
     */
    Player();
    vector <HMM> playerModels;
    vector < pair<HMM,int> > HMMAvg;
	// Parameters we can play with :
	int NoStatesG = 1; // For the guessing models
	int NoStatesS = 5; // For the shooting models
	int maxitersG = 500;
	int maxitersS = 50;
	long double thresholdGuess = -1e1; // Threshold on guesses is useless
	long double thresholdShoot = 0.85; // Only shoot when it is sufficiently probable
	long double thresholdBS = 1e-80; // Threshold on the proba of BS from which we don't shoot anymore
	long double previousCheck = 5;
	VVI triedShots; // Don't try the same shot twice
	VVI predictions;
	//int startShoot = 110; // The dumb shooting method is actually bad
    /**
     * Shoot!
     *
     * This is the function where you start your work.
     *
     * You will receive a variable pState, which contains information about all
     * birds, both dead and alive. Each birds contains all past actions.
     *
     * The state also contains the scores for all players and the number of
     * time steps elapsed since the last time this function was called.
     *
     * @param pState the GameState object with observations etc
     * @param pDue time before which we must have returned
     * @return the prediction of a bird we want to shoot at, or cDontShoot to pass
     */
    Action shoot(const GameState &pState, const Deadline &pDue);

    /**
     * Guess the species!
     * This function will be called at the end of each round, to give you
     * a chance to identify the species of the birds for extra points.
     *
     * Fill the vector with guesses for the all birds.
     * Use SPECIES_UNKNOWN to avoid guessing.
     *
     * @param pState the GameState object with observations etc
     * @param pDue time before which we must have returned
     * @return a vector with guesses for all the birds
     */
    std::vector<ESpecies> guess(const GameState &pState, const Deadline &pDue);

    /**
     * If you hit the bird you were trying to shoot, you will be notified
     * through this function.
     *
     * @param pState the GameState object with observations etc
     * @param pBird the bird you hit
     * @param pDue time before which we must have returned
     */
    void hit(const GameState &pState, int pBird, const Deadline &pDue);

    /**
     * If you made any guesses, you will find out the true species of those
     * birds through this function.
     *
     * @param pState the GameState object with observations etc
     * @param pSpecies the vector with species
     * @param pDue time before which we must have returned
     */
    void reveal(const GameState &pState, const std::vector<ESpecies> &pSpecies, const Deadline &pDue);
};

} /*namespace ducks*/

#endif
