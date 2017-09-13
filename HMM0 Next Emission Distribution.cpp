#include <bits/stdc++.h>
using namespace std;

vector <double> Next_Emission_Distribution(vector< vector<double> > tranMat, vector<vector<double> > emiMat, vector<double> iniState)
{

}

int main()
{
    vector< vector<double> > tranMat;
    vector< vector<double> > emiMat;
    vector< double > iniState;

    //Input
    int h,w;
    cin>>h>>w;
    for(int i = 0 ; i < h; i++)
    {
        vector <double> aux;
        tranMat.push_back( aux );
        for(int j = 0 ; j < w; j++)
        {
            double x;
            cin>>x;
            tranMat[i].push_back(x);
        }
    }

    cin>>h>>w;
    for(int i = 0 ; i < h; i++)
    {
        vector <double> aux;
        emiMat.push_back( aux );
        for(int j = 0 ; j < w; j++)
        {
            double x;
            cin>>x;
            emiMat[i].push_back(x);
        }
    }

    cin>>h>>w;
    for(int i = 0 ; i < w; i++)
    {
            double x;
            cin>>x;
            iniState.push_back(x);
    }
    //
}