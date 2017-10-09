#include <bits/stdc++.h>
using namespace std;

typedef pair < vector< vector<long double> >, long double> PairErrorMat;
typedef pair<vector< long double >, long double> PairErrorVect;
void printMatrix( vector< vector<long double> > mat)
{
	for(int i = 0 ; i <  mat.size(); i++)
	{
		for(int j = 0 ; j < mat[i].size(); j++)
			cout<<mat[i][j]<<' ';
		cout<<'\n';
	}
	return;
}
// WARNING
// Matrixes must be of the same size.
PairErrorMat errorMatrix(vector< vector<long double> > A, vector< vector< long double> > B )
{
    long double sum = 0;
    vector< vector<long double> > res(A.size());
    for(int i = 0 ; i < A.size(); i++)
        for(int j = 0 ; j < A[i].size(); j++)
        {
            res[i].push_back( abs(A[i][j] - B[i][j]));
            sum += abs(A[i][j] - B[i][j]);
        }
    return make_pair(res,sum);

}

PairErrorVect erroVector(vector <long double> A, vector<long double> B)
{
    long double sum = 0;
    for(int i = 0 ; i < A.size(); i++)
     sum += A[i] = abs(A[i] - B[i]);
    return make_pair(A,sum);
}
void printMatrixOneLine( vector< vector<long double> > mat)
{
	cout<<mat.size()<<' '<<mat[0].size()<<' ';
	for(int i = 0 ; i <  mat.size(); i++)
	{
		for(int j = 0 ; j < mat[i].size(); j++)
			cout<<mat[i][j]<<' ';
	}
	cout<<'\n';
	return;
}

void printVector( vector<long double> vec)
{
	for(int i = 0 ; i < vec.size(); i++)
		cout<<vec[i]<<' ';
	cout<<'\n';
	return;
}

void printVectorInt( vector<int> vec)
{
	for(int i = 0 ; i < vec.size(); i++)
		cout<<vec[i]<<' ';
	cout<<'\n';
	return;
}

int main()
{
    //Matrix to converge
    vector< long double > OriginalPi;
    int i,j,t;
    int h,w;
    cin>>h>>w;
    vector< vector<long double> > OriginalA(h);
    for(i = 0 ; i < h; i++ )
        for(j = 0 ; j < w; j++)
        {
            long double x;
            cin>>x;
            OriginalA[i].push_back(x);
        }
    cin>>h>>w;
    vector< vector<long double> > OriginalB(h);
    for(i = 0 ; i < h; i++ )
        for(j = 0 ; j < w; j++)
        {
            long double x;
            cin>>x;
            OriginalB[i].push_back(x);
        }

    cin>>h>>w;
    for(i = 0 ; i < w; i++)
    {
        long double x;
        cin>>x;
        OriginalPi.push_back(x);
    }

	//Input
    cin>>h>>w;
	vector< long double > pi;
	vector< vector<long double> > A(h);
	for(i = 0 ; i < h; i++)
	{
		for(j = 0 ; j < w; j++)
		{
			long double x;
			cin>>x;
			A[i].push_back(x);
		}
	}
	// printMatrix(A);

	cin>>h>>w;
	vector< vector<long double> > B(h);
	for(i = 0 ; i < h; i++)
	{
		for(j = 0 ; j < w; j++)
		{
			long double x;
			cin>>x;
			B[i].push_back(x);
		}
	}
	// printMatrix(B);

	cin>>h>>w;
	for(i = 0 ; i < w; i++)
	{
		long double x;
		cin>>x;
		pi.push_back(x);
	}
	// printVector(pi);
	
	cin>>w;
	vector< int > obs(w);
	for(i = 0 ; i < w; i++)
	{
		int x;
		cin>>x;
		obs[i]=x;
	}
	// printVectorInt(obs);
	
	// 0. Variables ===========================
	int T = obs.size();
	int N = pi.size();
	int M = B[0].size();
	vector <long double> c(T);
	vector< vector<long double> > alpha(T, vector<long double>(N)); // dim = (T,N)
	vector< vector<long double> > beta(T, vector<long double>(N));  // dim = (T,N)
	vector< vector<long double> > gamma2D(T, vector<long double>(N)); // dim = (T,N)
	vector< vector< vector<long double> > > gamma3D(T, vector< vector<long double> >(N, vector<long double>(N))); // dim = (T,N,N)
	
	// 1. Initialization ======================
	int maxIters = 3000;
	int iters = 0;
	long double oldLogProb = -DBL_MAX;
	
	while (true)
	{
	
		// ========================================
		// 2. The alpha-pass ======================
		// ========================================

		// compute alpha_0(i)
		c[0] = 0.0;
		for (i=0; i<N ; i++)
		{
			alpha[0][i] = pi[i] * B[i][obs[0]];
			c[0] += alpha[0][i];
		}

		// scale the alpha_0(i)
		c[0] = 1.0/c[0];
		for (i=0; i<N ; i++)
		{
			alpha[0][i] = c[0] * alpha[0][i];
		}
		
		// compute alpha_t(i)
		for (t=1; t<T ; t++)
		{
			c[t] = 0.0;
			for (i=0; i<N ; i++)
			{
				alpha[t][i] = 0.0;
				for (j=0; j<N; j++)
				{
					alpha[t][i] += alpha[t-1][j] * A[j][i];
				}
				alpha[t][i] = alpha[t][i] * B[i][obs[t]];
				c[t] += alpha[t][i];
			}
			// scale alpha_t(i)
			c[t] = 1.0/c[t];
			for (i=0; i<N ; i++)
			{
				alpha[t][i] = c[t] * alpha[t][i];
			}
		}

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
					beta[t][i] += A[i][j] * B[j][obs[t+1]] * beta[t+1][j];
				}
				// scale beta_t(i) with same scale factor as alpha_t(i)
				beta[t][i] = c[t] * beta[t][i];
			}
		}
		
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
					denom += alpha[t][i] * A[i][j] * B[j][obs[t+1]] * beta[t+1][j];
				}
			}
			for (i=0; i<N ; i++)
			{
				gamma2D[t][i] = 0.0;
				for (j=0; j<N; j++)
				{
					gamma3D[t][i][j] = (alpha[t][i] * A[i][j] * B[j][obs[t+1]] * beta[t+1][j]) / denom;
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
		
		// ==================================================
		// 5. Re-estimate A, B and pi =======================
		// ==================================================
		// re-estimate pi
		for (i=0; i<N ; i++)
		{
			pi[i] = gamma2D[0][i];
		}
	
		long double numer;
		// re-estimate A
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
				A[i][j] = numer/denom;
			}
		}
	
		// re-estimate B
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
				B[i][j] = numer/denom;
			}
		}
		/*cout<<"iters = "<<iters<<"\n";
		cout<<"A = \n";
		printMatrix(A);
		cout<<"B = \n";
		printMatrix(B);
		cout<<"pi = \n";
		printVector(pi);*/
		
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
		if (iters < maxIters && logProb > oldLogProb)
			oldLogProb = logProb;
		else
			break;
	}
	cout<<"iters = "<<iters<<"\n";
	//printMatrixOneLine(A);
    //printMatrixOneLine(B);
    cout<<"A = \n";
    printMatrix(A);
    cout<<"B = \n";
    printMatrix(B);
    cout<<"pi = \n";
    printVector(pi);

    cout<<"Error: \n";
    cout<<"A error = \n";
    PairErrorMat errorA = errorMatrix(A,OriginalA);
    cout<<"// " << errorA.second <<'\n';
    printMatrix(errorA.first);
    cout<<"B error = \n";
    PairErrorMat errorB = errorMatrix(B,OriginalB);
    cout<<"// " << errorB.second <<'\n';
    printMatrix(errorB.first);
    cout<<"Pi error = \n";
    PairErrorVect errorPi = erroVector(pi,OriginalPi);
    cout<<"// " << errorPi.second <<'\n';
    printVector(errorPi.first);     
	// cout<<"-DBL_MAX = "<<-DBL_MAX<<"\n";
	
	return 0;
}

















