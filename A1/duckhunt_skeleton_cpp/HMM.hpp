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
            HMM(int tranMatH,int tranMatW,int emiMatH,int emiMatW, int iniStateN);
            HMM(VVLD tranMat, VVLD emiMat, VLD iniState, VI obs);
            // HMM0
            VLD Next_Emmision();
            long double Scalar_Product(VLD A, VLD B);
            // HMM1
            VLD Element_Wise_Product(VLD A, VLD B);
            long double Prob_Emmision_Sequence();
            // HMM2
            pair<long double, int> Best_Index_Vector(VLD vec);
            DeltaTable Estimate_Sequence_Of_States();
            VI Backtracking(DeltaTable deltaR);
            // HMM4
            void Estimate_Model();
            //Help methods
            void Print_HMM();
        private:
            VVLD tranMat, emiMat;
            VLD iniState;
            VI obs;
    };
}