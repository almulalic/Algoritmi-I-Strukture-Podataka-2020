#include <iostream>
#include <stdexcept>

//#region Stack

using namespace std;

template <typename T>
class Stek {
    struct Node {
        T element;
        Node* previous;
        Node(T element,Node* previous):element(element),previous(previous) {}
    };
    
    Node* tekuci; // ujedno i kraj, prvi odozgo
    Node *dno; // prvi odozdo
    int duzina;
    
public:
    Stek():tekuci(nullptr),dno(nullptr),duzina(0) {}
    Stek(const Stek<T> &novi);
    Stek operator=(const Stek <T> &novi);
    ~Stek();
    void brisi();
    void stavi(const T& el);
    T skini();
    T& vrh() const;
    int brojElemenata() const { return duzina; }
};

template <typename T>
Stek<T>::Stek(const Stek<T> &novi):tekuci(nullptr),duzina(novi.duzina) {

    if(this != &novi && novi.duzina != 0) {
        Node *temp = novi.tekuci;
        Node *t = new Node(temp -> element,nullptr);
        Node *poc = t;
        
        temp = temp -> previous;
        while(temp != novi.dno) {
            t -> previous = new Node(temp -> element,nullptr);
            temp = temp -> previous;
            t = t -> previous;
        }
        
        t -> previous = new Node(temp -> element,nullptr);
        
        tekuci = poc;
        duzina = novi.duzina;
        dno = t -> previous;
    }
}

template <typename T>
Stek<T> Stek<T>::operator=(const Stek<T> &novi) {
    if(this != &novi && novi.duzina != 0) {
        Node *temp = novi.tekuci;
        Node *t = new Node(temp -> element,nullptr);
        Node *poc = t;
        
        temp = temp -> previous;
        while(temp != novi.dno) {
            t -> previous = new Node(temp -> element,nullptr);
            temp = temp -> previous;
            t = t -> previous;
        }
        
        t -> previous = new Node(temp -> element,nullptr);
        
        tekuci = poc;
        duzina = novi.duzina;
        dno = t -> previous;
        
        return *this;
    }
    
    return novi;
}

template <typename T>
Stek<T>::~Stek() {
    if(duzina != 0) {
        
      while(tekuci != dno) {
          Node* old = tekuci;
          tekuci = tekuci -> previous;
          delete old;
      }
      
      delete tekuci;
    }
}

template <typename T>
void Stek<T>::brisi() {
    if(duzina != 0) {
        Node *temp = tekuci;
    
        while(temp -> previous != nullptr) {
            Node *old = temp;
            temp = temp -> previous;
            delete old;
        }
        
        delete temp;
        
        tekuci = nullptr;
        duzina = 0;
    }
}

template <typename T>
void Stek<T>::stavi(const T& el) {
    Node *novi = new Node(el,nullptr);
    
    if(duzina == 0) 
        dno = novi;
    else 
        novi -> previous = tekuci;
         
    tekuci = novi;
    
    ++duzina;
}

template <typename T>
T Stek<T>::skini() {
    if(duzina == 0)
        throw std::domain_error("Stek je prazan.");
    
    Node* temp = tekuci;
    T element = temp -> element;
    
    if(temp -> previous != nullptr)
        tekuci = tekuci -> previous;
    
    delete temp;
    
    --duzina;
    return element;
}

template <typename T>
T& Stek<T>::vrh() const {
    if(duzina == 0)
        throw std::domain_error("Stek je prazan.");
    
    return tekuci -> element;
}

//#endregion

//#region Testovi

bool TestirajDubokoKopiranje(Stek<int> stek) {
    for(int i{1};i<=10;++i)
        stek.stavi(i);
    
    Stek<int> stekKopija(stek);
    
    stekKopija.skini();
    stekKopija.skini();
    stekKopija.skini();
    
    if(stek.brojElemenata() == stekKopija.brojElemenata())
        return false;
    
    if(stek.vrh() == stekKopija.vrh())
        return false;
    
    Stek<int> stekKopijaDodjele = stek;
    
    stekKopijaDodjele.skini();
    stekKopijaDodjele.skini();
    stekKopijaDodjele.skini();
    stekKopijaDodjele.skini();
    stekKopijaDodjele.skini();
    
    if(stek.brojElemenata() == stekKopijaDodjele.brojElemenata())
        return false;
    
    if(stek.vrh() == stekKopijaDodjele.vrh())
        return false;
    
    return true;
}

bool TestirajMetoduBrisi(Stek<int> stek) {
    for(int i{0};i<10;++i)
        stek.stavi(i);
        
    stek.brisi();
    
    if(stek.brojElemenata() != 0)
        return false;
    
    stek.stavi(1);
    stek.stavi(2);
    stek.stavi(3);
    
    stek.brisi();
    
    bool jeBacenIzuzetak = false;
    
    try {
        stek.vrh();
    } catch(...) {
        jeBacenIzuzetak = true;
    }

    if(stek.brojElemenata() == stekKopijaDodjele.brojElemenata())
        return false;
    
    if(stek.vrh() == stekKopijaDodjele.vrh())
        return false;
    
    return true;
}

bool TestirajMetoduBrisi(Stek<int> stek) {
    for(int i{0};i<10;++i)
        stek.stavi(i);
        
    stek.brisi();
    
    if(stek.brojElemenata() != 0)
        return false;
    
    stek.stavi(1);
    stek.stavi(2);
    stek.stavi(3);
    
    stek.brisi();
    
    bool jeBacenIzuzetak = false;
    
    try {
        stek.vrh();
    } catch(...) {
        jeBacenIzuzetak = true;
    }
    
    return jeBacenIzuzetak;
}

bool TestirajMetoduStavi(Stek<int> stek) {
    for(int i{1};i<=5;i++)
        stek.stavi(i);
    
    int ocekivani[5] = {5,4,3,2,1};
    
    if(stek.brojElemenata() != 5)
        return false;
        
    for(int i{0};i<5;i++)
        if(stek.skini() != ocekivani[i])
            return false;
    
    if(stek.brojElemenata() != 0)
        return false;
    
    return true;
}

bool TestirajMetoduSkini(Stek<int> stek) {
    for(int i{1};i<=15;i++)
        stek.stavi(i);
        
    int ocekivani[3] = {15,14,13};
    
    for(int i{0};i<3;++i)
        if(stek.skini() != ocekivani[i])
            return false;
        
    if(stek.brojElemenata() != 12)
        return false;
    
    stek.skini();
    stek.skini();
    stek.skini();
    stek.skini();
    
    if(stek.skini() != 8)
        return false;
        
    if(stek.brojElemenata() != 7)
        return false;
    
    stek.skini();
    stek.skini();
    stek.skini();
    stek.skini();
    stek.skini();
    stek.skini();
    stek.skini();
    
    if(stek.brojElemenata() != 0)
        return false;
    
    bool jeBacenIzuzetak = false;
    
    try {
        stek.skini();        
    } catch(...) {
        jeBacenIzuzetak = true;
    }
    
    return jeBacenIzuzetak;
}

bool TestirajMetoduVrh(Stek<int> stek) {
    stek.stavi(1);
    
    if(stek.vrh() != 1)
        return false;
    
    for(int i{0};i<10;++i) {
        stek.stavi(i+1);
        if(stek.vrh() != i + 1)
            return false;
    }
    
    if(stek.vrh() != 10)
        return false;
    
    stek.brisi();
    
    bool jeBacenIzuzetak = false;
    
    try {
        stek.vrh();        
    } catch(...) {
        jeBacenIzuzetak = true;
    }
    
    return jeBacenIzuzetak;
}

bool TestirajMetoduBrojElemenata(Stek<int> stek) {
    for(int i{0};i<10;++i)
        stek.stavi(i);
    
    if(stek.brojElemenata() != 10)
        return false;
    
    stek.skini();
    
    if(stek.brojElemenata () != 9)
        return false;
    
    for(int i{0};i<9;++i) {
        stek.skini();
        if(stek.brojElemenata() != 8 - i)
            return false;
    }
    
    return true;
}

//#endregion

int main() {
    
    std::cout << std::boolalpha;
    
    Stek<int> stek1;
    std::cout << "Test dobuke kopije: " << (TestirajDubokoKopiranje(stek1) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek2;
    std::cout << "Test metode brisi(): " << (TestirajMetoduBrisi(stek2) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek3;
    std::cout << "Test metode stavi(): " << (TestirajMetoduStavi(stek3) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek4;
    std::cout << "Test metode skini(): " << (TestirajMetoduSkini(stek4) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek5;
    std::cout << "Test metode skini(): " << (TestirajMetoduSkini(stek5) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek6;
    std::cout << "Test metode vrh(): " << (TestirajMetoduVrh(stek5) ? "USPJESAN" : "PADA") << std::endl;
    
    Stek<int> stek7;
    std::cout << "Test metode brojElemenata(): " << (TestirajMetoduBrojElemenata(stek5) ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
