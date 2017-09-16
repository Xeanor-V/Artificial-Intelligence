#include "Player.hpp"
#include "HMM.hpp"
#include <cstdlib>
#include <iostream>

namespace ducks
{

Player::Player()
{

}

Action Player::shoot(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to get the best action.
     * This skeleton never shoots.
     */
    // This line choose not to shoot
    return cDontShoot;
    //This line would predict that bird 0 will move right and shoot at it
    //return Action(0, MOVE_RIGHT);
}

std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue)
{
    /*
     * Here you should write your clever algorithms to guess the species of each bird.
     * This skeleton makes no guesses, better safe than sorry!
     */
	/*
    for(int i = 0 ; i < pState.getNumBirds(); i++)
    {
        Bird bird = pState.getBird(i);
        vector<int> observation;
		for(int j = 0 ; j < bird.getSeqLength(); j++)
        {
            switch (bird.getObservation(j))
            {
                case MOVE_DEAD:
                observation.push_back(-1);
                break;
                case MOVE_UP_LEFT:
                observation.push_back(0);
                break;
                case MOVE_UP:
                observation.push_back(1);
                break;
                case MOVE_UP_RIGHT:
                observation.push_back(2);
                break;
                case MOVE_LEFT:
                observation.push_back(3);
                break;
                case MOVE_STOPPED:
                observation.push_back(4);
                break;
                case MOVE_RIGHT:
                observation.push_back(5);
                break;
                case MOVE_DOWN_LEFT:
                observation.push_back(6);
                break;
                case MOVE_DOWN:
                observation.push_back(7);
                break;
                case MOVE_DOWN_RIGHT:
                observation.push_back(8);
                break;
                case COUNT_MOVE:
                observation.push_back(9);
                break;
            }
        }
	int NoStates = this -> Player::NoStates;
        HMM hmm(NoStates,NoStates,NoStates,10,10);
        //cerr<<"BIRD NO."<<i<<": \n";
        //hmm.Print_HMM();
	
    }
	*/
    std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
    return lGuesses;
}

void Player::hit(const GameState &pState, int pBird, const Deadline &pDue)
{
    /*
     * If you hit the bird you are trying to shoot, you will be notified through this function.
     */
    std::cerr << "HIT BIRD!!!" << std::endl;
}

void Player::reveal(const GameState &pState, const std::vector<ESpecies> &pSpecies, const Deadline &pDue)
{
    /*
     * If you made any guesses, you will find out the true species of those birds in this function.
     */
}


} /*namespace ducks*/
