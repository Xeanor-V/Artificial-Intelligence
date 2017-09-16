#include "HMM.hpp"

namespace ducks
{


    HMM::HMM(int tranMatH,int tranMatW,int emiMatH,int emiMatW, int iniStateN):
    tranMat(tranMatH), emiMat(emiMatH), iniState()
    {
        srand(time(NULL));
        double coef;
        for(int i = 0 ; i < tranMatH; i++)
        {
            coef = 0;
            for(int j = 0 ; j < tranMatW; j++)
            {
                double x = rand();
                tranMat[i].push_back(x);
                coef+=x;
            }
            coef = 1/coef;
            for(int j = 0 ; j < tranMat[i].size(); j++) tranMat[i][j] *= coef;
        }

        for(int i = 0 ; i < emiMatH; i++)
        {
            coef = 0;
            for(int j = 0 ; j < emiMatW; j++)
            {
                double x = rand();
                emiMat[i].push_back(x);
                coef+=x;
            }
            coef = 1/coef;
            for(int j = 0 ; j < tranMat[i].size(); j++) emiMat[i][j] *= coef;
        }


        coef = 0;
        for(int i = 0 ; i < iniStateN; i++)
        {
            double x = rand();
            iniState.push_back(x);
            coef+=x;
        }
        coef = 1/coef;
        for(int i = 0 ; i < iniStateN; i++) iniState[i] *= coef;
    }    
    HMM::HMM(VVLD tranMat, VVLD emiMat, VLD iniState, VI obs)
    {
        this -> tranMat = tranMat;
        this -> emiMat = emiMat;
        this -> iniState = iniState;
        this -> obs = obs;
    }
    long double HMM::Scalar_Product(VLD A, VLD B)
    {
        long double res = 0 ;
        for(int i = 0 ; i < A.size(); i++)
        res += ( A[i] * B[i] );
        return res;
    }
    VLD HMM::Next_Emmision()
    {
        VLD res1,res2;  
        for(int i = 0 ; i < tranMat.size(); i++ )
        {
            res1.push_back(this->HMM::Scalar_Product(tranMat[i],iniState));
        }
        for(int i = 0 ; i < emiMat.size(); i++ )
        {
            res2.push_back(this->HMM::Scalar_Product(emiMat[i],res1));
        }
        return res2;
    }
    VLD HMM::Element_Wise_Product(VLD A, VLD B)
    {
        VLD res;
        for (int i = 0; i < A.size(); i++)
        {
            res.push_back(A[i]*B[i]);
        }
        return res;
    }
    long double HMM::Prob_Emmision_Sequence()
    {
        // Initialization
        VLD alpha = this->HMM::Element_Wise_Product(iniState, emiMat[obs[0]]);
        for (int i = 1; i < obs.size(); i++)
        {
            //alpha = this->HMM::Next_Alpha(tranMat, alpha, emiMat[obs[i]]);
            VLD b = emiMat[obs[i]];
            VLD res1,res2;  
            for(int j = 0 ; j < tranMat.size(); j++ )
            {
                res1.push_back(this->HMM::Scalar_Product(tranMat[j],alpha));
            }
            res2 = this->HMM::Element_Wise_Product(res1, b);
            alpha = res2;
        }
        long double res = 0;
        for (int i = 0; i < alpha.size(); i++)
        {
            res += alpha[i];
        }
        return res;
    }
    pair<long double, int> HMM::Best_Index_Vector(VLD vec)
    {   
        long double aux = -1e9;
        int index = -1;
        for(int j = 0 ; j < vec.size(); j++) 
        {
            if(aux < vec[j])
            {
                aux = vec[j];
                index = j;
            }
        }
        return make_pair(aux,index);
    }
    DeltaTable HMM::Estimate_Sequence_Of_States()
    {
        VLD delta = this->HMM::Element_Wise_Product(iniState, emiMat[obs[0]]);
        vector< int > bestPossible;
        DeltaTable DeltaResults; 
        // Delta procedure (Forward algorithm)
        for (int i = 1; i < obs.size(); i++)
        {
            vector< pair<long double,int> > res;
            for(int j = 0; j < tranMat.size(); j++)
            {
                VLD aux;
                long double maxi = -1e9;
                for(int k = 0; k < tranMat[j].size(); j++)
                aux.push_back(tranMat[j][k] * delta[k] * obs[k]) ;
                res.push_back(this->HMM::Best_Index_Vector (aux) );
            }
            for(int j = 0 ; j < res.size(); j++) delta[j] = res[j].first;
            DeltaResults.push_back(res);
        }
        return DeltaResults;
    }
    VI HMM::Backtracking(DeltaTable DeltaResults)
    {
        // Backtracking
        double maxi = -1e9;
        int index = -1;
        // Finding the max of the last result
        for(int i = 0; i < DeltaResults[0].size(); i++)
        {   
            long double aux =  DeltaResults[DeltaResults.size() -1 ][i].first;
            if( aux > maxi)
            {
                maxi = aux;
                index = i;
            }
        }
        VI backtracking;
        backtracking.push_back(index);
        index = DeltaResults[DeltaResults.size() -1 ][index].second;
        backtracking.push_back(index);
        // Going back in the results
        for(int i = DeltaResults.size() - 2; i>=0; i--)
        {
            backtracking.push_back(DeltaResults[i][index].second);
            index = DeltaResults[i][index].second;
        }
        return backtracking;
    }
    void HMM::Estimate_Model()
    {
        int i,j,t;
        int T = obs.size();
        int N = iniState.size();
        int M = emiMat[0].size();
        VLD c(T);
        VVLD alpha(T, VLD(N)); // dim = (T,N)
        VVLD beta(T, VLD(N));  // dim = (T,N)
        VVLD gamma2D(T, VLD(N)); // dim = (T,N)
        vector< VVLD > gamma3D(T, VVLD(N, VLD(N))); // dim = (T,N,N)
        
        // cerr<<"Variables ok\n";
        // 1. Initialization ======================
        int maxIters = 500;
        int iters = 0;
        long double oldLogProb = -DBL_MAX;
        
        // cerr<<"Initialization ok\n";
        // cerr<<"alpha[T-1][N-1] = "<<alpha[T-1][N-1]<<"\n";
        //int z = 200;
        while (true)
        {
        
            // ========================================
            // 2. The alpha-pass ======================
            // ========================================
            // compute alpha_0(i)
            c[0] = 0;
            for (i=0; i<N ; i++)
            {
                alpha[0][i] = iniState[i] * emiMat[i][obs[0]];
                c[0] += alpha[0][i];
            }
    
            // scale the alpha_0(i)
            c[0] = 1.0/c[0];
            for (i=0; i<N ; i++)
            {
                alpha[0][i] = c[0] * alpha[0][i];
            }
            
            // cerr<<"foobar ok\n";
            // compute alpha_t(i)
            for (t=1; t<T ; t++)
            {
                c[t] = 0.0;
                for (i=0; i<N ; i++)
                {
                    alpha[t][i] = 0.0;
                    for (j=0; j<N; j++)
                    {
                        alpha[t][i] += alpha[t-1][j]*tranMat[j][i];
                    }
                    alpha[t][i] = alpha[t][i] * emiMat[i][obs[t]];
                    c[t] = c[t] + alpha[t][i];
                }
                // scale alpha_t(i)
                c[t] = 1.0/c[t];
                for (i=0; i<N ; i++)
                {
                    alpha[t][i] = c[t]*alpha[t][i];
                }
            }
            // cerr<<"Alpha pass ok\n";
            // ========================================
            // 3. The beta-pass =======================
            // ========================================
        
            // Let beta_T−1(i) = 1, scaled by c_T−1
            for (i=0; i<N ; i++)
            {
                beta[T-1][i] = c[T-1];
            }
        
            // beta-pass
            for (t=T-2 ; t>=0 ; t--)
            {
                for (i=0; i<N ; i++)
                {
                    beta[t][i] = 0.0;
                    for (j=0; j<N; j++)
                    {
                        beta[t][i] += tranMat[i][j] * emiMat[j][obs[t+1]] * beta[t+1][j];
                    }
                    // scale beta_t(i) with same scale factor as alpha_t(i)
                    beta[t][i] = c[t] * beta[t][i];
                }
            }
            // cerr<<"Beta pass ok\n";
            // ===============================================================
            // 4. Compute gamma_t(i, j) and gamma_t(i) =======================
            // ===============================================================
            long double denom;
            for (t=0; t<T-1; t++)
            {
                denom = 0.0;
                for (i=0; i<N ; i++)
                {
                    for (j=0; j<N; j++)
                    {
                        denom += alpha[t][i] * tranMat[i][j] * emiMat[j][obs[t+1]] * beta[t+1][j];
                    }
                }
                for (i=0; i<N ; i++)
                {
                    gamma2D[t][i] = 0.0;
                    for (j=0; j<N; j++)
                    {
                        gamma3D[t][i][j] = (alpha[t][i] * tranMat[i][j] * emiMat[j][obs[t+1]] * beta[t+1][j]) / denom;
                        gamma2D[t][i] += gamma3D[t][i][j];
                    }
                }
            }
            // Special case for gamma_T−1(i)
            denom = 0.0;
            for (i=0; i<N ; i++)
            {
                denom += alpha[T-1][i];
            }
            for (i=0; i<N ; i++)
            {
                gamma2D[T-1][i] = alpha[T-1][i] / denom;
            }
            
            // cerr<<"Gamma computations ok\n";
            // ==================================================
            // 5. Re-estimate tranMat, emiMat and iniState =======================
            // ==================================================
            // re-estimate iniState
            for (i=0; i<N ; i++)
            {
                iniState[i] = gamma2D[0][i];
            }
        
            long double numer;
            // re-estimate tranMat
            for (i=0; i<N ; i++)
            {
                for (j=0; j<N; j++)
                {
                    numer = 0.0;
                    denom = 0.0;
                    for (t=0; t<T-1; t++)
                    {
                        numer += gamma3D[t][i][j];
                        denom += gamma2D[t][i];
                    }
                    tranMat[i][j] = numer/denom;
                }
            }
        
            // re-estimate emiMat
            for (i=0; i<N ; i++)
            {
                for (j=0; j<M; j++)
                {
                    numer = 0.0;
                    denom = 0.0;
                    for (t=0; t<T; t++)
                    {
                        if (obs[t] == j)
                        {
                            numer += gamma2D[t][i];
                        }
                        denom += gamma2D[t][i];
                    }
                    emiMat[i][j] = numer/denom;
                }
            }
            
            // ======================================================
            // 6. Compute log[P (O | lambda)] =======================
            // ======================================================
            long double logProb = 0.0;
            for (i=0; i<T ; i++)
            {
                logProb += log(c[i]);
            }
            logProb = -logProb;
            
            // ===============================================================
            // 7. To iterate or not to iterate, that is the question... ======
            // ===============================================================
            iters++;
            if (iters<maxIters && logProb > oldLogProb)
                oldLogProb = logProb;
            else
                break;
        }
        this -> HMM::iniState = iniState;
        this -> HMM::tranMat = tranMat;
        this -> HMM::emiMat = emiMat;
        //return make_pair(iniState, make_pair(tranMat,emiMat));
    }
    void HMM::Print_HMM()
    {
        cerr<<"Transition matrix: \n";
        for(int i = 0 ; i < tranMat.size(); i++)
        {
            for(int j = 0 ; j < tranMat[i].size();j++)
            cerr<<tranMat[i][j]<<' ';
            cerr<<'\n';
        }
        cerr<<"Emision Matrix: \n";
        for(int i = 0 ; i < emiMat.size(); i++)
        {
            for(int j = 0 ; j < emiMat[i].size();j++)
            cerr<<emiMat[i][j]<<' ';
            cerr<<'\n';
        }
        cerr<<"Initial state probability: \n";
        for(int i = 0 ; i < iniState.size(); i++)
        {
            cerr<<iniState[i]<<' ';
        }
        cerr<<'\n';
    }
}
/*
int main()
{
    /*
    ducks::HMM hmm;
    ducks::HMM hmm;
    VLD iniState;
    
        //Input
        int h,w;
        cin>>h>>w;
        VVLD tranMat(w);
        for(int i = 0 ; i < h; i++)
        {
            for(int j = 0 ; j < w; j++)
            {
                double x;
                cin>>x;
                tranMat[j].push_back(x);
            }
        }
       // printMatrix(tranMat);
    
        cin>>h>>w;
        VVLD emiMat(w);
        for(int i = 0 ; i < h; i++)
        {
            for(int j = 0 ; j < w; j++)
            {
                double x;
                cin>>x;
                emiMat[j].push_back(x);
            }
        }
        //printMatrix(emiMat);
    
        cin>>h>>w;
        for(int i = 0 ; i < w; i++)
        {
                double x;
                cin>>x;
                iniState.push_back(x);
        }
        
        vector< int > obs;
        cin>>w;
        for(int i = 0 ; i < w; i++)
        {
                int x;
                cin>>x;
                obs.push_back(x);
        }
        cerr<<hmm.Prob_Emmision_Sequence(tranMat,emiMat,iniState,obs)<<endl;
    //cerr<<"It's ok"<<endl;

    ducks::HMM hmm(4,4,4,4,4);
    hmm.Print_HMM();
}
*/
