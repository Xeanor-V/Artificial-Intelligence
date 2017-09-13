#include <bits/stdc++.h>
using namespace std;


// A[0] * B[0] + A[1] * B[1] ....
double Scalar_Product(vector<double> A, vector<double> B)
{
    double res = 0 ;
    for(int i = 0 ; i < A.size(); i++)
    res += ( A[i] * B[i] );
    return res;
}

// [ A[0] * B[0] ; A[1] * B[1] .... ]
vector<double> Element_Wise_Product(vector<double> A, vector<double> B)
{
    vector<double> res;
	for (int i = 0; i < A.size(); i++)
	{
		res.push_back(A[i]*B[i]);
	}
	return res;
}

void printMatrix( vector< vector<double> > mat)
{
    for(int i = 0 ; i <  mat.size(); i++)
    {
        for(int j = 0 ; j < mat[i].size(); j++)
            cout<<mat[i][j]<<' ';
        cout<<'\n';
    }
    return;
}
template<typename Type>
void printVector( vector<Type> vec)
{
    for(int i = 0 ; i < vec.size(); i++)
    cout<<vec[i]<<' ';
    cout<<'\n';
    return;
}

vector <double> Next_Gamma(vector< vector<double> >tranMat, vector<double> gamma, vector<double> obs)
{
    vector<double> res;
    for(int i = 0; i < tranMat.size(); i++)
    {
        double maxi = -1e9;
        for(int j = 0; j < tranMat[i].size(); j++)
        maxi = max(maxi,tranMat[i][j] * gamma[i] * obs[i]);
        res.push_back(maxi);
    }
    return res;    
}

int main()
{
    //vector< vector<double> > tranMat;
    vector< double > iniState;

    //Input
    int h,w;
    cin>>h>>w;
    vector< vector<double> > tranMat(w);
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
    vector< vector<double> > emiMat(w);
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
	
	// Initialization
    vector< double > gamma = Element_Wise_Product(iniState, emiMat[obs[0]]);
    vector< int > bestPossible;
	
	//cout<<"first\n";
	for (int i = 1; i < obs.size(); i++)
	{
       //cout<<i<<" "<<"loop\n";
        gamma= Next_Gamma(tranMat, gamma, emiMat[obs[i]]);
        double aux = -1e9;
        int index = -1;
        for(int j = 0 ; j < gamma.size(); j++) 
        {
            //aux = max(aux,gamma[j] );
            if(aux < gamma[j])
            {
                aux = gamma[j];
                index = j;
            }
        }
        bestPossible.push_back(index);
        
	}
	printVector(bestPossible);
    return 0;
}