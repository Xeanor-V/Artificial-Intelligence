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
            HMM();
            // HMM0
            VLD Next_Emmision(VVLD tranMat,VVLD emiMat,VLD iniState);
            long double Scalar_Product(VLD A, VLD B);
            // HMM1
            VLD Element_Wise_Product(VLD A, VLD B);
            long double Prob_Emmision_Sequence(VVLD tranMat,VVLD emiMat,VLD iniState,VI obs);
            // HMM2
            pair<long double, int> Best_Index_Vector(VLD vec);
            DeltaTable Estimate_Sequence_Of_States(VVLD tranMat,VVLD emiMat,VLD iniState,VI obs);
            VI Backtracking(DeltaTable deltaR);
            // HMM4
            pair<VVLD,VVLD> Estimate_Model(VVLD tranMat,VVLD emiMat,VLD iniState,VI obs);
    };
}