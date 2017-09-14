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

void printVector( vector<double> vec)
{
    for(int i = 0 ; i < vec.size(); i++)
    cout<<vec[i]<<' ';
    cout<<'\n';
    return;
}

vector <double> Next_Alpha(vector< vector<double> >tranMat, vector<double> alpha, vector<double> b)
{
    vector<double> res1,res2;  
    for(int i = 0 ; i < tranMat.size(); i++ )
    {
        res1.push_back(Scalar_Product(tranMat[i],alpha));
    }
    
	res2 = Element_Wise_Product(res1, b);

    return res2;
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
	vector< double > alpha = Element_Wise_Product(iniState, emiMat[obs[0]]);
	
	//cout<<"first\n";
	for (int i = 1; i < obs.size(); i++)
	{
       //cout<<i<<" "<<"loop\n";
		alpha = Next_Alpha(tranMat, alpha, emiMat[obs[i]]);
	}
	
	double res = 0;
	for (int i = 0; i < alpha.size(); i++)
	{
		res += alpha[i];
	}
	
    cout<< res <<'\n';
    
    return 0;
}