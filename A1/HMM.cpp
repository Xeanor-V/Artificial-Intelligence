#include "HMM.hpp"

HMM::HMM()
{

}
HMM::double Scalar_Product(vector<double> A, vector<double> B)
{
    double res = 0 ;
    for(int i = 0 ; i < A.size(); i++)
    res += ( A[i] * B[i] );
    return res;
}
HMM::vector<double>  Next_Emmision(VVLD tranMat,VVLD emiMat,VLD iniState)
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
HMM::vector<double> Element_Wise_Product(vector<double> A, vector<double> B)
{
    vector<double> res;
	for (int i = 0; i < A.size(); i++)
	{
		res.push_back(A[i]*B[i]);
	}
	return res;
}
HMM::double Prob_Emmision_Sequence(VVLD tranMat,VVLD emiMat,VLD iniState)
{
    // Initialization
	vector< double > alpha = Element_Wise_Product(iniState, emiMat[obs[0]]);
	for (int i = 1; i < obs.size(); i++)
	{
        alpha = Next_Alpha(tranMat, alpha, emiMat[obs[i]]);
        vector <double> b = emiMat[obs[i]];
        vector<double> res1,res2;  
        for(int i = 0 ; i < tranMat.size(); i++ )
        {
            res1.push_back(Scalar_Product(tranMat[i],alpha));
        }
        res2 = Element_Wise_Product(res1, b);
        alpha = res2;
    }
	double res = 0;
	for (int i = 0; i < alpha.size(); i++)
	{
		res += alpha[i];
	}
	return res;
}