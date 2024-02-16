
#include<iostream>
#include<fstream>
#include<stdexcept>
#include"BigInteger.h"

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 3){
        invalid_argument("There must be 2 command line arguments\n");
    }
    ifstream in;
    ofstream out;
    
    string line1, trash, line2;

    in.open(argv[1]);
    out.open(argv[2]);

    getline(in, line1);
    getline(in, trash);
    getline(in, line2);

    BigInteger A = BigInteger(line1);
    //out<<"A: " << A << "\n\n";
    BigInteger B = BigInteger(line2);
    //out<<"B: " << B << "\n\n";

    /* BigInteger A3;
    for (int i=0; i<3; i++){
        A3 += A;
    }
    //out<<"A3: " <<A3 << "\n\n";
    //out<<"A: " << A << "\n\n";

    BigInteger B2;
    for (int i=0; i<2; i++){
        B2 += B;
    }
    BigInteger AAAA;
    AAAA = A;
    for (int i=0; i<3; i++){
        AAAA *= A;
    }
    BigInteger AAAA9;
    for (int i=0; i<9; i++){
        AAAA9 += AAAA;
    }
    BigInteger BBBBB;
    BBBBB = B;
    for (int i=0; i<4; i++){
        BBBBB *= B;
    }
    BigInteger BBBBB16;
    for (int i=0; i<16; i++){
        BBBBB16 += BBBBB;
    }
    

    //cout<<"A^4: " << AAAA << "\n";
    //cout<<"A^4 should be: 2849 391667738 046148631 637623201" << "\n\n";

    //cout<<"9A^4: " << AAAA9 << "\n";
    //cout<<"9A^4 should be: 25644 525009642 415337684 738608809" << "\n\n";

    //cout<<"B^5: " << BBBBB << "\n";
    //cout<<"B^5 should be: 22231 769254561 231216196 150333531 610966585 385288557" << "\n\n";

    //cout<<"16B^5: " << BBBBB16 << "\n";
    //cout<<"16B^5 should be: 355708 308072979 699459138 405336505 775465366 164616912" << "\n\n";



    out<< A << "\n\n";
    out<< B << "\n\n";
    out<<(A+B) << "\n\n";
    out<<(A-B) << "\n\n";
    out<<(A-A) << "\n\n";
    out<<(A3-B2) << "\n\n";
    out<<(A*B) << "\n\n";
    out<<(A*A) << "\n\n";
    out<<(B*B) << "\n\n";
    out<<(AAAA9+BBBBB16) << "\n"; */


    BigInteger A3 = A * (BigInteger("3"));
    BigInteger B2 = B * (BigInteger("2"));

    BigInteger Asquared = A*A;
    BigInteger nineApower4 = Asquared * Asquared * BigInteger("9");
    BigInteger sixteenBpower5 = B*B*B*B*B * BigInteger("16");

    out<< A << "\n\n";
    out<< B << "\n\n";
    out<<(A+B) << "\n\n";
    out<<(A-B) << "\n\n";
    out<<(A-A) << "\n\n";
    out<<(A3-B2) << "\n\n";
    out<<(A*B) << "\n\n";
    out<<(Asquared) << "\n\n";
    out<<(B*B) << "\n\n";
    out<<(nineApower4+sixteenBpower5) << "\n";
    
    

    return 0;

}