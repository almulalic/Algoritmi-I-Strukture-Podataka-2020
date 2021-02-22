#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

int fib_petlja (int n) { // kopirano iz postavke,O(2 na N), ne koristi se u zadatk
    if (n<=1) return n; // 0 i 1
    
    int pretprosli(0), prosli(1), rezultat;
    
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    
    return rezultat;
}

int fib2_0(int n,int rezultat = 1,int pretprosli=0,int prosli=1) { // mora biti 0(n)
    int novi = prosli + pretprosli;
    if(n<=1) return n;
    else if(n == 2) return novi;
    else return fib2_0(--n,novi,prosli,novi);
}

int nzd(int x,int y) {
    if(y == 0) return x;
    else if(y > 0) return nzd(y, x % y);
    else return x;
}

int main() {
    
    std::cout << "Najmanji zajednicki djelilac za 412 i 12242 je: " << nzd(412,12242) << std::endl;
    std::cout << "Faktorijel od 20 iznosi: " << fib2_0(20);
    
    return 0;
}
