
#include<iostream>
#include<string>
#include<stdexcept>
#include<sstream>
#include<cmath>
#include"BigInteger.h"

//************-------------------************************
//ALTER WHEN SUBMITTING: it should be
//const int power = 9;
//const long base = 1000000000;

const int power = 9;
const long base = 1000000000;



//***************-----------------*********************
// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m){
    L.moveFront();
    while (L.position() < L.length()){
        ListElement scaled = L.moveNext() * m;
        L.setBefore(scaled);
    }
}

//normalizeList(), will be used only in mult()
void normalizeList(List& L){


    L.moveBack();
    while (L.position() > 1){
        long value = L.movePrev();
        if (value >= base){
            L.setAfter(value%base);
            L.setBefore(L.peekPrev() + value/base);
        }
    }
    L.moveFront();
    long value = L.peekNext();
    if (value >= base){
        L.setAfter(value%base);
        L.insertBefore(value/base);

    }
}

void sumList(List& S, List A, List B){
    List a = A;
    List b = B;
    List added;

    a.moveBack(); b.moveBack();
    while (a.position() > 0 && b.position() > 0){
        added.insertAfter(a.movePrev() + b.movePrev());
    }
    while (a.position() > 0){
        added.insertAfter(a.movePrev());
    }
    while (b.position() > 0){
        added.insertAfter(b.movePrev());
    }

    S = added;


}

List makeList(long s){
    List mylist;
    if (s < base){
        //std::cout<< "numlen was less than power\n";
        mylist.insertAfter(s);
    }else{
        while (s>=base){
            mylist.insertAfter(s%base);
            s = s/base;
        }
        if (s != 0){
            mylist.insertAfter(s);
        }
    }
    return mylist;
}

BigInteger::BigInteger(){
    signum = 0;
    digits.clear();
}

BigInteger::BigInteger(std::string s){
    if (s.length() == 0){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    int i = 0;
    long len = s.length();
    long numlen = s.length();
    if (s[0] == '-'){
        if (s.length() == 1){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
        signum = -1;
        i = 1;
        numlen -= 1;
    }else if (s[0] == '+'){
        if (s.length() == 1){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
        signum = 1;
        i = 1;
        numlen -= 1;
        
    }else{
        signum = 1;
        i = 0;
    }

    int notzeroanymore = 0;
    int starts = 0;
    while (i<len){
        if (!isdigit(s[i])){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
        if (s[i] != '0'){
            if (notzeroanymore == 0){
                notzeroanymore = 1;
                starts = i;
            }
            
        }
        i++;
    }

    digits.clear();
    if (!notzeroanymore){
        digits.insertBefore(0);
        signum = 0;
    }else{
        if (numlen <= power){
            //std::cout<< "numlen was less than power\n";
            long num = std::stol(s);
            digits.insertBefore(num);
        }else if (numlen > power){
            i = len - 1;
            while (i >= starts){
                std::string buffer = "";
                for (int q=0; q<power; q++){
                    if (i<starts){
                        break;
                    }
                    buffer.insert(0, 1, s[i]);
                    i--;
                }
                digits.insertAfter(std::stol(buffer));
            }
        }
    }



}

BigInteger::BigInteger(const BigInteger &N){
    signum = N.signum;
    digits.clear();
    digits = N.digits;
}

int BigInteger::sign() const{
    return signum;
}

int BigInteger::compare(const BigInteger& N) const{
    if (signum > N.signum){
        return 1;
    }else if (signum < N.signum){
        return -1;
    }else if (signum == 0 && N.signum == 0){
        return 0;
    }

    if (digits.length() > N.digits.length()){
        return 1;
    }else if (digits.length() < N.digits.length()){
        return -1;
    }else{
        List x, y;
        x = digits;
        y = N.digits;
        x.moveFront(); 
        y.moveFront();

        while (x.position() < x.length()){
            long a = x.moveNext();
            long b = y.moveNext();
            if (a == b){
                continue;
            }else if (a > b){
                return 1;
            }else if (a < b){
                return -1;
            }
        }

        return 0;

    }
}

void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}

void BigInteger::negate(){
    signum *= -1;
}

BigInteger BigInteger::add(const BigInteger& N) const{
    if (signum == 1 && N.signum == -1){
        BigInteger Ncopy;
        Ncopy = N;
        Ncopy.signum = 1;
        BigInteger done = this->sub(Ncopy);
        return done;
    }else if (signum == -1 && N.signum == 1){
        BigInteger thiscopy = *this;
        thiscopy.negate();
        BigInteger done = N.sub(thiscopy);
        return done;
    }else if (signum == -1 && N.signum == -1){
        BigInteger thiscopy = *this;
        BigInteger Ncopy = N;
        thiscopy.negate(); Ncopy.negate();
        BigInteger done = thiscopy.add(Ncopy);
        done.signum = -1;
        return done;
    }else if (signum == 0 && N.signum == 0){
        BigInteger H;
        H.digits.insertAfter(0);
        return H;
    }else if (signum == 0 || N.signum == 0){
        if (signum == 0){
            BigInteger copy;
            copy = N;
            return copy;
        }else if (N.signum == 0){
            BigInteger copy;
            copy = *this;
            return copy;
        }
    }

    List added;
    List A;
    A = digits;
    List B;
    B = N.digits;
    int carry = 0;
    //[111 |122223333]
    //[222 |211110000]


    A.moveBack();
    B.moveBack();
    while (A.position() > 0 && B.position() > 0){
        long a = A.movePrev();
        long b = B.movePrev();
        long sum = a + b + carry;
        //std::cout<<"sum: "<<sum<<"\n";
        if (sum >= base){
            sum -= base;
            carry = 1;
        }else{
            carry = 0;
        }
        added.insertAfter(sum);
    }

    while (A.position() > 0){
        long insert = (A.movePrev() + carry);
        if (insert >= base){
            insert -= base;
            carry = 1;
        }else{
            carry = 0;
        }
        added.insertAfter(insert);
        /* if (A.position() == 0 && carry == 1){
            added.insertAfter(1);
        } */
        
    }
    while (B.position() > 0){
        long insert = (B.movePrev() + carry);
        if (insert >= base){
            insert -= base;
            carry = 1;
        }else{
            carry = 0;
        }
        added.insertAfter(insert);
        /* if (B.position() == 0 && carry == 1){
            added.insertAfter(1);
        } */
    }
    if (carry == 1){
        added.insertAfter(1);
    }

    BigInteger returnthis;
    returnthis.digits = added;
    returnthis.signum = 1;

    return returnthis;

}

BigInteger BigInteger::sub(const BigInteger& N) const{
    // A = [1 2 3 4 5]
    // B = [8 7 6]
    // carry = 1
    //              [025 123 456 245]
    //  [444 000 000 111 000 000 226]
    //   443 999 999 914 123 455 981 <- FALSE

    //  [125 123 456 245]
    //  [211 000 000 226]
    //   914 123 456 019 <- FALSE

    if (signum == 1 && N.signum == 1){
        if (N.digits.length() > digits.length()){
            BigInteger done = N.sub(*this);
            done.signum = -1;
            return done;

        }else if (N.digits.length() == digits.length()){
            if (N.digits.front() > digits.front()){
                BigInteger done = N.sub(*this);
                done.signum = -1;
                return done;
            }

        }
    }else if (signum == -1 && N.signum == 1){
        BigInteger Ncopy = N;
        Ncopy.signum = -1;
        BigInteger done = this->add(Ncopy);
        done.signum = -1;
        return done;

    }else if (signum == 1 && N.signum == -1){
        BigInteger Ncopy = N;
        Ncopy.signum = 1;
        BigInteger done = this->add(Ncopy);
        return done;
    }else if (signum == -1 && N.signum == -1){
        BigInteger Ncopy;
        BigInteger thiscopy;
        Ncopy = N;
        thiscopy = *this;
        Ncopy.signum = 1;
        thiscopy.signum = 1;
        BigInteger done = Ncopy.sub(thiscopy);
        return done;
    }else if (signum == 0 && N.signum == 0){
        BigInteger H;
        H.digits.insertAfter(0);
        return H;
    }

    // return list:
    // [444]
    List added;
    List A = digits;
    List B = N.digits;
    int carry = 0;
    //int i;
    //int diff = A.length() - B.length();
    //int AorB = 0; // if A is longer, 1. If B is longer, 2



    A.moveBack();
    B.moveBack();
    while (A.position() > 0 && B.position() > 0){
        long a = A.movePrev();
        long b = B.movePrev();
        long diff = a - b - carry;
        if (diff < 0){
            diff = base + diff;
            carry = 1;
        }else{
            carry = 0;
        }
        added.insertAfter(diff);
    }

    while (A.position() > 0){
        long insert = (A.movePrev() - carry);
        if (insert < 0){
            insert = base + insert;
            carry = 1;
        }else{
            carry = 0;
        }
        
        added.insertAfter(insert);
        /* if (A.position() == 0 && carry == 1){
            added.insertAfter(1);
        } */
        
        
    }
    while (B.position() > 0){
        long insert = (B.movePrev() - carry);
        if (insert < 0){
            insert = base + insert;
            carry = 1;
        }else{
            carry = 0;
        }
        added.insertAfter(insert);
        /* if (B.position() == 0 && carry == 1){
            added.insertAfter(1);
        } */
    }

    if (carry == 1){
        added.insertAfter(1);
    }

    //std::cout<<"added: " << added << "\n";
    added.moveFront();
    while (added.front() == 0){
        if (added.length() == 1){
            break;
        }
        added.eraseAfter();
        
    }

    BigInteger returnthis;
    returnthis.digits = added;
    //std::cout<<added;
    if (added.front() == 0){
        returnthis.signum = 0;
    }else{
        returnthis.signum = signum;
    }

    return returnthis;

}



BigInteger BigInteger::mult(const BigInteger& N) const{
    if (signum == 0 || N.signum == 0){
        BigInteger done;
        done.digits.insertAfter(0);
        return done;
    }
    List A = digits;
    List B = N.digits;
    List total;
    List mylist;
    std::string buffer;
    A.moveBack();
    
    int Acount = 0;
    while (A.position() > 0){
        B.moveBack();
        int Bcount = 0;
        long cursor = A.movePrev();
        if (cursor == 0){
            Acount++;
            continue;
        }
        while (B.position() > 0){
            long Bdata = B.movePrev();
            if (Bdata == 0){
                Bcount++;
                continue;
            }
            mylist = makeList(cursor * Bdata);
            //std::cout<<"mylist: "<<mylist<<"\n";
            mylist.moveBack();
            for (int i=0; i<(Acount+Bcount); i++){
                mylist.insertBefore(0);
            }

            sumList(total, total, mylist);
            //std::cout<< "total: " << total << "\n";
            Bcount++;
        }
        Acount++;
    }
    normalizeList(total);
    BigInteger fin;
    fin.digits = total;

    if (signum * N.signum == -1){
        fin.signum = -1;
    }else{
        fin.signum = 1;
    }
    return fin;
    
}

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
    std::string s = "";
    if (signum == -1){
        s += '-';
    }else if (signum == 0){
        s += '0';
        return s;
    }
    digits.moveFront();
    while (digits.position() < digits.length()){
        long element = digits.moveNext();
        if (element == 0){
            if (digits.position() == 1){
                s += '0';
                //s += ' ';
            }else{
                std::string buffer(power, '0');
                s += buffer;
                //s += ' ';
            }
            
        }else{
            std::stringstream ss;
            std::string buffer;
            ss << element;
            ss >> buffer;
            if (buffer.length() < power){
                if (digits.position() != 1){
                    int chai = power - buffer.length();
                    buffer.insert(0, chai, '0'); 
                }
            }
            
            s += buffer;
            //s += " ";
        }
    }
    //std::cout<<digits.to_string()<<"\n";

    return s;
    
};

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream<< N.to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    }else{
        return false;
    }
};

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1){
        return true;
    }else{
        return false;
    }
};

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    int comp = A.compare(B);
    if (comp == 0 || comp == -1){
        return true;
    }else{
        return false;
    }
};

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1){
        return true;
    }else{
        return false;
    }
};

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    int comp = A.compare(B);
    if (comp == 0 || comp == 1){
        return true;
    }else{
        return false;
    }
};

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
};

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    return A = A+B;
};

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
};

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    return A = A-B;
};

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
};

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    return A = A*B;
};