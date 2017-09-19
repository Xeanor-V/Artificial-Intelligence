#include <bits/stdc++.h>
using namespace std;
typedef vector< vector <long double> > VVLD;
typedef vector< long double > VLD;
typedef vector< int > VI;
typedef vector< vector< pair<long double, int> > > DeltaTable;

namespace ducks
{
    class HMM
    {
        public:
            HMM(int numState,int numEmi,VI obs);
            HMM(VVLD tranMat, VVLD emiMat, VLD iniState, VI obs);
            // HMM0
            VLD Next_Emmision();
            long double Scalar_Product(VLD A, VLD B);
            // HMM1
            VLD Element_Wise_Product(VLD A, VLD B);
            long double Prob_Emmision_Sequence(VI obs);
            // HMM2
            pair<long double, int> Best_Index_Vector(VLD vec);
            DeltaTable Estimate_Sequence_Of_States();
            VI Backtracking(DeltaTable deltaResults);
            // HMM4
            void Estimate_Model(int maxIters);
            //Help methods
            void Print_HMM();
			//void Print_DeltaTable(DeltaTable tab);
            HMM Avg_HMM(vector<HMM> hmms, HMM previous, double weight);
            VVLD Matrix_Sum(VVLD A, VVLD B);
            VVLD Matrix_Division(VVLD A, double B);
            VLD Vector_Sum(VLD A, VLD B);
            VLD Vector_Division(VLD A, double B);
            VVLD tranMat, emiMat;
            VLD iniState;
            VI obs;
    };
}