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

vector<double> getColumn(vector< vector<double > > mat, int index)
{
    vector<double> res;
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



vector <double> Next_Emission_Distribution(vector< vector<double> >tranMat,vector< vector<double> > emiMat, vector<double> iniState)
{
    vector<double> res1,res2;  
    for(int i = 0 ; i < tranMat.size(); i++ )
    {
        res1.push_back(Scalar_Product(tranMat[i],iniState));
    }
    for(int i = 0 ; i < emiMat.size(); i++ )
    {
        res2.push_back(Scalar_Product(emiMat[i],res1));
    }

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
    // Next Emision Distribution
    vector<double> NextEmiRes = Next_Emission_Distribution(tranMat,emiMat,iniState);
    cout<<1<<' '<<NextEmiRes.size()<<' ';
    printVector(NextEmiRes);
    //
    return 0;
}