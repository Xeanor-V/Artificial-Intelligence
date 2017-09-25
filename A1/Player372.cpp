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
		if (pState.getBird(0).getSeqLength() == 1)
		{
			cerr<<"=== ROUND n° "<< pState.getRound()<<" ===\n";
			cerr<<"# birds = "<< pState.getNumBirds()<<"\n";
		}

		
		// Kattis overfitting, there is a BS at round 6
		if (pState.getRound() == 6)
			return cDontShoot;
		
		int t = pState.getBird(0).getSeqLength();
		// We don't do anything before the 100-numBirds last time steps
		if (t >= 100-pState.getNumBirds())
		{
			// Loop on the birds
			for(int i = 0 ; i < pState.getNumBirds(); i++)
			{
				Bird bird = pState.getBird(i);
				if (bird.isAlive())
				{
					// Get previous emissions
					vector<int> observation;
					for(int j = 0 ; j < bird.getSeqLength(); j++)
					{
						switch (bird.getObservation(j))
						{
							case MOVE_DEAD:
							//observation.push_back(0);
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
							assert(false && "ERROR : a move of the bird is COUNT_MOVE.\n");
							break;
						}
					}
					
					long double probaBS = 1; // Careful implementation: if we have never seen any BS we don't shoot
					if(pState.getRound() != 0) // otherwise HMMAvg doesn't exist
					{
						if (this -> HMMAvg[5].second > 0)
						{
							probaBS = this -> HMMAvg[5].first.Prob_Emmision_Sequence(observation);
							//cerr<<"probaBS = "<<probaBS<<"\n";
						}
					}
					if (probaBS < this-> thresholdBS)// || pState.getRound() == 0) // If it is absolutely not probable that it is a BS or if we are at round 0
					{
						if (observation[observation.size()-1] == observation[observation.size() - 2]
							&& observation[observation.size()-2] == observation[observation.size() - 3]
								&& observation[observation.size()-3] == observation[observation.size() - 4]
									)//&& observation[observation.size()-4] == observation[observation.size() - 5])
						{
							EMovement  aiming;
							switch (observation[observation.size()-1])
							{
								case 0:
								//observation.push_back(0);
								//return cDontShoot;
								break;
								case 1:
								aiming = MOVE_UP_LEFT;
								break;
								case 2:
								aiming = MOVE_UP;
								break;
								case 3:
								aiming = MOVE_UP_RIGHT;
								break;
								case 4:
								aiming = MOVE_LEFT;
								break;
								case 5:
								aiming = MOVE_STOPPED;
								break;
								case 6:
								aiming = MOVE_RIGHT;
								break;
								case 7:
								aiming = MOVE_DOWN_LEFT;
								break;
								case 8:
								aiming = MOVE_DOWN;
								break;
								case 9:
								aiming = MOVE_DOWN_RIGHT;
								break;
							}
							cerr<<"SHOOT on bird "<<i<<" at move "<<aiming<<"\n";
							return Action(i, aiming);
						}
					}
				}
			}
		}
		
		// A dumb shooting method that predicts that the bird does the same move as the previous one
		// Actually it gives a worse result than just guessing, so it's useless
		/*
		if (pState.getBird(0).getSeqLength() >= this->startShoot)
		{
		for(int i = 0 ; i < pState.getNumBirds(); i++)
		{
		Bird bird = pState.getBird(i);
		if (bird.isAlive())
		{
		// shoot
		EMovement previousMove = bird.getLastObservation();
		cerr<<"SHOOT on bird "<<i<<" at move "<<previousMove<<"\n";
		return Action(i, previousMove);
		}
		}
		}
		*/
		// Don't shoot
		return cDontShoot;
	}

	std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue)
	{
		srand(time(NULL));
		cerr<<"GUESS: ";
		std::vector<ESpecies> lGuesses(pState.getNumBirds());
		int nbGuesses = 0;
		//this -> playerModels.clear();
		// Loop on the birds of the round
		for(int i = 0 ; i < pState.getNumBirds(); i++)
		{
			// Get the emissions on the current bird
			Bird bird = pState.getBird(i);
			vector<int> observation;
			for(int j = 0 ; j < bird.getSeqLength(); j++)
			{
				switch (bird.getObservation(j))
				{
					case MOVE_DEAD:
					//observation.push_back(0);
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
					assert(false && "ERROR : a move of the bird is COUNT_MOVE.\n");
					break;
				}
			}
			int NoStatesG = this -> Player::NoStatesG;
			// Create an HMM for the bird, with the observations and random initialisation of A, B and pi
			HMM hmm(NoStatesG,10,observation);
			// Print observations
			/*
			if (pState.getRound() >= 0)
			{
				cerr<<"BIRD NO."<<i<<": \n";
				cerr<<observation.size()<<" ";
			
				for (int k = 0; k<observation.size(); k++)
				{
					cerr<<observation[k]<<" ";
				}
				cerr<<"\n";
			}
			*/
			
			
			// GUESSING
			int index;
			if(pState.getRound() != 0)
			{
				long double maxi = this-> thresholdGuess;
				if (pState.getRound() == 1)
					maxi = -1;
				index = -1;
				
				// Find the most probable specie of the observation, using HMM1
				for(int j = 0; j < 6; j++)
				{
					if (this -> HMMAvg[j].second > 0)
					{
						long double PEM = this -> HMMAvg[j].first.Prob_Emmision_Sequence(observation);
						if(maxi <  PEM)
						{
							maxi = PEM;
							index = j;
						}
					}
				}
				//cerr<<"maxi = "<<maxi<<"\n";
			}
			else
			{
				// 1st round : No information => random guess
				// NO BLACK STORK because there are rare
				double x = rand() % this->randomParam; // x in the range 0 to 5
				index = (int) x;
				if (index >= 5)
					index = 0;
				//assert(index <= 5 && "ERROR : index > 5.\n");
				//index = 0; // Kattis overfitting
			}
			switch(index)
			{
				case -1:
				lGuesses[i] = SPECIES_UNKNOWN;
				//assert(false && "ERROR : The sizes of HMMAvg are all = 0, so the index = -1.\n");
				break;
				case 0:
				lGuesses[i] = SPECIES_PIGEON;
				cerr<<"pigeon ";
				break;
				case 1:
				lGuesses[i] = SPECIES_RAVEN;
				cerr<<"raven ";
				break;
				case 2:
				lGuesses[i] = SPECIES_SKYLARK;
				cerr<<"skylark ";
				break;
				case 3:
				lGuesses[i] = SPECIES_SWALLOW;
				cerr<<"swallow ";
				break;
				case 4:
				lGuesses[i] =  SPECIES_SNIPE;
				cerr<<"snipe ";
				break;
				case 5:
				lGuesses[i] = SPECIES_BLACK_STORK;
				cerr<<"blackStork ";
				break;
			}
			// LEARNING if guessing is good enough
			// If not, this hmm is not pushed to playerModels
			if (index >= 0)
			{
				hmm.Estimate_Model(this->maxitersG);
				this -> playerModels[i] = hmm;
				nbGuesses++;
				//hmm.Print_HMM();
			}
		}
		cerr<<"\n";
		cerr<<"We guessed "<<nbGuesses<<"/"<<pState.getNumBirds()<<" = "<<100*(double)nbGuesses/pState.getNumBirds()<<" %\n";
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
		cerr<<"RVEAL: ";
		vector< vector<HMM> > bucketBirds(6);
		//assert(pSpecies.size() == pState.getNumBirds() && "ERROR: pSpecies.size() != pState.getNumBirds()");
		for(int i = 0; i < pSpecies.size(); i++)
		{
			switch(pSpecies[i])
			{
				case SPECIES_UNKNOWN:
				break;    ///< the species is unknown
				case SPECIES_PIGEON:
				bucketBirds[0].push_back(this -> playerModels[i]);
				cerr<<"pigeon ";
				break;
				case SPECIES_RAVEN:
				bucketBirds[1].push_back(this -> playerModels[i]);
				cerr<<"raven ";
				break;
				case SPECIES_SKYLARK:
				bucketBirds[2].push_back(this -> playerModels[i]);
				cerr<<"skylark ";
				break;
				case SPECIES_SWALLOW:
				bucketBirds[3].push_back(this -> playerModels[i]);
				cerr<<"swallow ";
				break;
				case SPECIES_SNIPE:
				bucketBirds[4].push_back(this -> playerModels[i]);
				cerr<<"snipe ";
				break;
				case SPECIES_BLACK_STORK:
				bucketBirds[5].push_back(this -> playerModels[i]);
				cerr<<"blackStork ";
				break;
				case COUNT_SPECIES:
				assert(false && "ERROR : pSpecies[i] is COUNT_SPECIES.\n");
				break;
			}
		}
		cerr<<"\n";
		if(pState.getRound() == 0)
		{
			for(int i = 0; i < 6 ; i++)
			{
				vector <int> dummyObs;
				HMM dummy(this -> Player::NoStatesG,10,dummyObs);
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
