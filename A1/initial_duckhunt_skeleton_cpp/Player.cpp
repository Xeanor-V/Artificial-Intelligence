#include "Player.hpp"
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
    playerModels.clear();
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
		int NoStates = 5;//this -> Player::NoStates;
        HMM hmm(NoStates,10,observation);
        hmm.Estimate_Model();
        //cerr<<"BIRD NO."<<i<<": \n";
        //hmm.Print_HMM();
        playerModels.push_back(hmm);
    }

    if(pState.getRound() != 0)
    {
        std::vector<ESpecies> lGuesses(pState.getNumBirds());
        for(int i = 0 ; i < pState.getNumBirds(); i++)
        {
            long double maxi = -1e9;
            int index = 0;
            for(int j = 0; j < 6; j++)
            {
                //maxi = max(maxi, HMMAvg[j].Prob_Emmision_Sequence() );
                long double PEM = HMMAvg[j].first.Prob_Emmision_Sequence();
                if(maxi <  PEM)
                {
                    maxi =  PEM;
                    index = j;
                }
            }
            switch(index)
            {
                case 0:
                lGuesses[i] = SPECIES_PIGEON;
                break;
                case 1:
                lGuesses[i] = SPECIES_RAVEN;
                break;
                case 2:
                lGuesses[i] = SPECIES_SKYLARK;
                break;
                case 3:
                lGuesses[i] = SPECIES_SWALLOW;
                break;
                case 4:
                lGuesses[i] =  SPECIES_SNIPE;
                break;
                case 5:
                lGuesses[i] = SPECIES_BLACK_STORK;
                break;
            }
        }
        return lGuesses;
    }
    else
    {
        std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
        return lGuesses;
    }
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
    vector< vector<HMM> > bucketBirds(6);
    for(int i = 0; i < pSpecies.size(); i++)
    {
        switch(pSpecies[i])
        {
            case SPECIES_UNKNOWN:
            break;    ///< the species is unknown
            case SPECIES_PIGEON:
            bucketBirds[0].push_back(this -> playerModels[i]);
            break;
            case SPECIES_RAVEN:
            bucketBirds[1].push_back(this -> playerModels[i]);
            break;
            case SPECIES_SKYLARK:
            bucketBirds[2].push_back(this -> playerModels[i]);
            break;
            case SPECIES_SWALLOW:
            bucketBirds[3].push_back(this -> playerModels[i]);
            break;
            case SPECIES_SNIPE:
            bucketBirds[4].push_back(this -> playerModels[i]);
            break;
            case SPECIES_BLACK_STORK:
            bucketBirds[5].push_back(this -> playerModels[i]);
            break;
        }
    }
    if(pState.getRound() == 0)
    {
        for(int i = 0; i < 6 ; i++)
        {
            if(bucketBirds[i].size() > 0)
            {
                vector <int> dummyObs;
                HMM dummy(bucketBirds[i][0].tranMat.size(),bucketBirds[i][0].iniState.size(),dummyObs);
                HMM avgHMM = bucketBirds[i][0].Avg_HMM(bucketBirds[i],dummy,abs(DBL_MIN));
                HMMAvg.push_back( make_pair(avgHMM,bucketBirds[i].size()));
            }
        }
    }
    else
    {
        for(int i = 0; i < 6 ; i++)
        {
            if(bucketBirds[i].size() > 0)
            {
                HMM avgHMM = bucketBirds[i][0].Avg_HMM(bucketBirds[i],HMMAvg[i].first, HMMAvg[i].second);
                HMMAvg[i].first = avgHMM;
                HMMAvg[i].second = HMMAvg[i].second + bucketBirds[i].size();
            }
        }
    }
}


} /*namespace ducks*/
