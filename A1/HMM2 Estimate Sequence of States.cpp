#include <bits/stdc++.h>
using namespace std;

typedef vector<long double> VLD;
typedef vector< VLD >  VVLD;

// A[0] * B[0] + A[1] * B[1] ....
long double Scalar_Product(VLD A, VLD B)
{
    long double res = 0 ;
    for(int i = 0 ; i < A.size(); i++)
    res += ( A[i] * B[i] );
    return res;
}

// [ A[0] * B[0] ; A[1] * B[1] .... ]
VLD Element_Wise_Product(VLD A, VLD B)
{

    VLD res;
	for (int i = 0; i < A.size(); i++)
	{
		res.push_back(A[i]*B[i]);
	}
	return res;
}

void Print_Matrix( VVLD  mat)
{
    for(int i = 0 ; i <  mat.size(); i++)
    {
        for(int j = 0 ; j < mat[i].size(); j++)
            cout<<mat[i][j]<<' ';
        cout<<'\n';
    }
    return;
}

//Print_vector it's a general method for printing vector
// @Order determines if it's going to be printed forward(true) or backwards(false)
template<typename Type>
void Print_Vector( vector<Type> vec,bool order)
{
    if(order)
    {
        for(int i = 0 ; i < vec.size(); i++)
        {
            if(i)cout<<' ';
            cout<<vec[i];
        }
    }
    else
    {
        for(int i = vec.size() - 1; i>=0 ; i--)
        {
            if(i == vec.size() - 1)cout<<vec[i];
            else cout<<' '<<vec[i];
        }
    }
    cout<<'\n';
    return;
}

void  Print_Pair_Matrix(vector< vector< pair<long double, int> > > mat)
{
    cout<<"-----\n";
    for(int i = 0 ; i < mat.size(); i++)
    {
        for(int j = 0 ; j < mat[i].size(); j++)
        {
            cout<<mat[i][j].first<<' '<<mat[i][j].second<<'\n';
        }
        cout<<"-----\n";
    }
}


// Best_Index_Vector finds the value and the index of the maximun element in a VLD
pair<long double, int> Best_Index_Vector(VLD vec)
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


vector < pair<long double, int> > Next_delta(VVLD tranMat, VLD delta, VLD obs)
{
    vector< pair<long double,int> >  res;
    for(int i = 0; i < tranMat.size(); i++)
    {
        VLD aux;
        long double maxi = -1e9;
        for(int j = 0; j < tranMat[i].size(); j++)
        aux.push_back(tranMat[i][j] * delta[j] * obs[i]) ;
        res.push_back( Best_Index_Vector (aux) );
    }
    return res;    
}

int main()
{
    //VVLD  tranMat;
    vector< long double > iniState;

    //Input
    int h,w;
    cin>>h>>w;
    VVLD  tranMat(w);
    for(int i = 0 ; i < h; i++)
    {
        for(int j = 0 ; j < w; j++)
        {
            long double x;
            cin>>x;
            tranMat[j].push_back(x);
        }
    }
    cin>>h>>w;
    VVLD  emiMat(w);
    for(int i = 0 ; i < h; i++)
    {
        for(int j = 0 ; j < w; j++)
        {
            long double x;
            cin>>x;
            emiMat[j].push_back(x);
        }
    }
    cin>>h>>w;
    for(int i = 0 ; i < w; i++)
    {
            long double x;
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
    vector< long double > delta = Element_Wise_Product(iniState, emiMat[obs[0]]);
    vector< int > bestPossible;
    vector< vector< pair<long double, int> > > DeltaResults; 
    // Delta procedure (Forward algorithm)
	for (int i = 1; i < obs.size(); i++)
	{
        vector< pair<long double,int> > res = Next_delta(tranMat, delta, emiMat[obs[i]]);
        for(int j = 0 ; j < res.size(); j++) delta[j] = res[j].first;
        DeltaResults.push_back(res);
    }
    // Backtracking
    double maxi = -1e9;
    int index = -1;
    // Finding the max of the last result
    for(int i = 0; i < obs.size(); i++)
    {   
        long double aux =  DeltaResults[DeltaResults.size() -1 ][i].first;
        if( aux > maxi)
        {
            maxi = aux;
            index = i;
        }
    }
    vector<int>backtracking;
    backtracking.push_back(index);
    index = DeltaResults[DeltaResults.size() -1 ][index].second;
    backtracking.push_back(index);
    // Going back in the results
    for(int i = DeltaResults.size() - 2; i>=0; i--)
    {
        backtracking.push_back(DeltaResults[i][index].second);
        index = DeltaResults[i][index].second;
    }
    Print_Vector(backtracking,false);
    return 0;
}