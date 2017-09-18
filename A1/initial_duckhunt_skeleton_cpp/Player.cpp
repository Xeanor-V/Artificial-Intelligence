#include "Player.hpp"
#include <cstdlib>
#include <iostream>

namespace ducks
{

	Player::Player() : playerModels(20, HMM(1,1, vector<int>(1)))
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
		//cerr<<"=== ROUND n° "<< pState.getRound()<<" ===\n";
		//cerr<<"# birds = "<< pState.getNumBirds()<<"\n";
		//cerr<<"GUESS: ";
		//this -> playerModels.clear();
		std::vector<ESpecies> lGuesses(pState.getNumBirds());
		
		for(int i = 0 ; i < pState.getNumBirds(); i++)
		{
			Bird bird = pState.getBird(i);
			vector<int> observation;
			for(int j = 0 ; j < bird.getSeqLength(); j++)
			{
				switch (bird.getObservation(j))
				{
					case MOVE_DEAD:
					observation.push_back(0);
					break;
					case MOVE_UP_LEFT:
					observation.push_back(1);
					break;
					case MOVE_UP:
					observation.push_back(2);
					break;
					case MOVE_UP_RIGHT:
					observation.push_back(3);
					break;
					case MOVE_LEFT:
					observation.push_back(4);
					break;
					case MOVE_STOPPED:
					observation.push_back(5);
					break;
					case MOVE_RIGHT:
					observation.push_back(6);
					break;
					case MOVE_DOWN_LEFT:
					observation.push_back(7);
					break;
					case MOVE_DOWN:
					observation.push_back(8);
					break;
					case MOVE_DOWN_RIGHT:
					observation.push_back(9);
					break;
					case COUNT_MOVE:
					cerr<<"ERROR : a move of the bird is COUNT_MOVE.\n";
					break;
				}
			}
			int NoStates = this -> Player::NoStates;
			HMM hmm(NoStates,10,observation);
			//hmm.Print_HMM();
			// cerr<<"BIRD NO."<<i<<": \n";
			// cerr<<observation.size()<<" ";
			/*
			for (int k = 0; k<observation.size(); k++)
			{
			cerr<<observation[i]<<" ";
			}
			cerr<<"\n";
			*/
			hmm.Estimate_Model();
			//hmm.Print_HMM();
			this -> playerModels[i] = hmm;
		
			// GUESS
			if(pState.getRound() != 0)
			{
				// cerr<<"GUESSING: \n";
				// cerr<<"BIRD NO. "<<i<<": \n";
				long double maxi = -1e9;
				int index = -1;
				for(int j = 0; j < 6; j++)
				{
					// cerr<<"SPECIES NO. "<<j<<": \n";
					// cerr<<"HMMAvg[j].second = "<<this -> HMMAvg[j].second<<": \n";
					if (this -> HMMAvg[j].second > 0)
					{
						long double PEM = this -> HMMAvg[j].first.Prob_Emmision_Sequence(observation);
						if(maxi <  PEM)
						{
							// cerr<<"CHANGE : index = "<<j<<": \n";
							maxi = PEM;
							index = j;
						}
					}
				}
				switch(index)
				{
					case -1:
					lGuesses[i] = SPECIES_UNKNOWN;
					cerr<<"ERROR : The sizes of HMMAvg are all = 0, so the index = -1.\n";
					break;
					case 0:
					lGuesses[i] = SPECIES_PIGEON;
					//cerr<<"pigeon ";
					break;
					case 1:
					lGuesses[i] = SPECIES_RAVEN;
					//cerr<<"raven ";
					break;
					case 2:
					lGuesses[i] = SPECIES_SKYLARK;
					//cerr<<"skylark ";
					break;
					case 3:
					lGuesses[i] = SPECIES_SWALLOW;
					//cerr<<"swallow ";
					break;
					case 4:
					lGuesses[i] =  SPECIES_SNIPE;
					//cerr<<"snipe ";
					break;
					case 5:
					lGuesses[i] = SPECIES_BLACK_STORK;
					//cerr<<"blackStork ";
					break;
				}
			}
		}
		cerr<<"\n";
		if(pState.getRound() == 0)
		{
			std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_PIGEON); // CHANGE IT RANDOM !!!!!!!!!!!
			//cerr<<"pigeon pigeon pigeon ...\n";
		}
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
		//cerr<<"RVEAL: ";
		vector< vector<HMM> > bucketBirds(6);
		assert(pSpecies.size() == pState.getNumBirds() && "ERROR: pSpecies.size() != pState.getNumBirds()");
		for(int i = 0; i < pSpecies.size(); i++)
		{
			switch(pSpecies[i])
			{
				case SPECIES_UNKNOWN:
				break;    ///< the species is unknown
				case SPECIES_PIGEON:
				bucketBirds[0].push_back(this -> playerModels[i]);
				//cerr<<"pigeon ";
				break;
				case SPECIES_RAVEN:
				bucketBirds[1].push_back(this -> playerModels[i]);
				//cerr<<"raven ";
				break;
				case SPECIES_SKYLARK:
				bucketBirds[2].push_back(this -> playerModels[i]);
				//cerr<<"skylark ";
				break;
				case SPECIES_SWALLOW:
				bucketBirds[3].push_back(this -> playerModels[i]);
				//cerr<<"swallow ";
				break;
				case SPECIES_SNIPE:
				bucketBirds[4].push_back(this -> playerModels[i]);
				//cerr<<"snipe ";
				break;
				case SPECIES_BLACK_STORK:
				bucketBirds[5].push_back(this -> playerModels[i]);
				//cerr<<"blackStork ";
				break;
				case COUNT_SPECIES:
				cerr<<"ERROR : pSpecies[i] is COUNT_SPECIES.\n";
				break;
			}
		}
		//cerr<<"\n";
		if(pState.getRound() == 0)
		{
			for(int i = 0; i < 6 ; i++)
			{
				vector <int> dummyObs;
				HMM dummy(this -> Player::NoStates,10,dummyObs);
				if(bucketBirds[i].size() > 0)
				{
					HMM avgHMM = bucketBirds[i][0].Avg_HMM(bucketBirds[i],dummy,abs(DBL_MIN));
					this -> HMMAvg.push_back( make_pair(avgHMM,bucketBirds[i].size()));
				}
				else
				{
					this -> HMMAvg.push_back( make_pair(dummy,0));
				}
				//this -> HMMAvg[i].first.Print_HMM();
			}
		}
		else
		{
			for(int i = 0; i < 6 ; i++)
			{
				if(bucketBirds[i].size() > 0)
				{
					HMM avgHMM = bucketBirds[i][0].Avg_HMM(bucketBirds[i],this -> HMMAvg[i].first, this -> HMMAvg[i].second);
					this -> HMMAvg[i].first = avgHMM;
					this -> HMMAvg[i].second = this -> HMMAvg[i].second + bucketBirds[i].size();
				}
			}
		}
	}


} /*namespace ducks*/
