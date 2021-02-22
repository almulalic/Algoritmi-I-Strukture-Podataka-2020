#include <iostream>

#include <iostream>
#include <stdexcept>

//#region DvostruktiRed

using namespace std;

template <typename T>
class DvostraniRed {
    struct Node {
        T element;
        Node* previous;
        Node* next;
        Node(T element,Node* previous = nullptr,Node* next = nullptr):element(element),previous(previous),next(next) {}
    };
    
    Node* zadnji; // zadnji u redu ->
    Node *prvi;  // prvi u redu <-
    int duzina;
    
public:
    DvostraniRed():zadnji(nullptr),prvi(nullptr),duzina(0) {}
    DvostraniRed(const DvostraniRed<T> &novi);
    DvostraniRed operator=(const DvostraniRed <T> &novi);
    ~DvostraniRed();
    void brisi();
    void staviNaCelo(const T& el);
    void staviNaVrh(const T& el);
    T skiniSaCela();
    T skiniSaVrha();
    T& celo() const;
    T& vrh() const;
    int brojElemenata() const { return duzina; }
};

template <typename T>
DvostraniRed<T>::DvostraniRed(const DvostraniRed<T> &novi):zadnji(nullptr),prvi(nullptr),duzina(novi.duzina) {

    if(this != &novi && novi.duzina != 0) {
        Node *zad = novi.zadnji;
        Node *prv = novi.prvi;
        Node *iter = new Node(novi.zadnji -> element);
        Node *nz = iter;
        
        zad = zad -> next;
        while(zad != prv) {
            iter -> next = new Node(zad -> element,iter,nullptr);
            iter = iter -> next;
            zad = zad -> next;
        }
        
        iter -> next = new Node(zad -> element,iter,nullptr);
        iter = iter -> next;
        
        zadnji = nz;
        prvi = iter;
        duzina = novi.duzina;
    }
}

template <typename T>
DvostraniRed<T> DvostraniRed<T>::operator=(const DvostraniRed<T> &novi) {
    if(this != &novi && novi.duzina != 0) {
        if(duzina > 0)
            this -> brisi();
        
        Node *zad = novi.zadnji;
        Node *prv = novi.prvi;
        Node *iter = new Node(novi.zadnji -> element);
        Node *nz = iter;
        
        zad = zad -> next;
        while(zad != prv) {
            iter -> next = new Node(zad -> element,iter,nullptr);
            iter = iter -> next;
            zad = zad -> next;
        }
        
        iter -> next = new Node(zad -> element,iter,nullptr);
        iter = iter -> next;
        
        zadnji = nz;
        prvi = iter;
        duzina = novi.duzina;
        
        return *this;
    }
    
    return novi;
}

template <typename T>
DvostraniRed<T>::~DvostraniRed() {
    if(duzina != 0) {
        
      while(zadnji != prvi) {
          Node* old = zadnji;
          zadnji = zadnji -> next;
          delete old;
      }
      
      delete zadnji;
    }
    
}

template <typename T>
void DvostraniRed<T>::brisi() {
    if(duzina != 0) {
        
      while(zadnji != prvi) {
          Node* old = zadnji;
          zadnji = zadnji -> next;
          delete old;
      }
      
      delete zadnji;
      
      prvi = nullptr;
      zadnji = nullptr;
      duzina = 0;
    }
}

template <typename T>
void DvostraniRed<T>::staviNaCelo(const T& el) { // -> zadnji
    Node *novi = new Node(el);
    
    if(duzina == 0) {
        zadnji = novi;
        prvi = novi;
    } else {
        novi -> next = zadnji;
        zadnji -> previous = novi;
        zadnji = novi;
    }
    
    ++duzina;
}

template <typename T>
void DvostraniRed<T>::staviNaVrh(const T& el) {
    Node *novi = new Node(el);
    
    if(duzina == 0) {
        zadnji = novi;
        prvi = novi;
    } else {
        novi -> previous = prvi;
        prvi -> next = novi;
        prvi = novi;
    }
    
    ++duzina;
}

template <typename T>
T DvostraniRed<T>::skiniSaCela() {
     if(duzina == 0)
        throw std::domain_error("DvostraniRed je prazan.");
    
    Node* temp = zadnji;
    T element = zadnji -> element;
    
    if(zadnji -> next != nullptr) 
        zadnji = zadnji -> next;
    else
        zadnji = prvi;
        
    delete temp;
    
    --duzina;
    return element;
}

template <typename T>
T DvostraniRed<T>::skiniSaVrha() {
    if(duzina == 0)
        throw std::domain_error("DvostraniRed je prazan.");
    
    Node* temp = prvi;
    T element = prvi -> element;
    
    if(prvi -> previous != nullptr) 
        prvi = prvi -> previous;
    else
        prvi = zadnji;
        
    delete temp;
    
    --duzina;
    return element;
}

template <typename T>
T& DvostraniRed<T>::vrh() const {
    if(duzina == 0)
        throw std::domain_error("DvostraniRed je prazan.");
    
    return prvi -> element;
}

template <typename T>
T& DvostraniRed<T>::celo() const {
    if(duzina == 0)
        throw std::domain_error("DvostraniRed je prazan.");
    
    return zadnji -> element;
}

//#endregion

//#region Testovi

template <typename T>
bool TestirajDubokoKopiranje(DvostraniRed<T> red) {
    DvostraniRed<T> redKopija(red);
    
    if(redKopija.brojElemenata() != red.brojElemenata())
        return false;
    
    if(redKopija.celo() != red.celo())
        return false;
        
    if(redKopija.vrh() != red.vrh())
        return false;
    
    DvostraniRed<T> redDodjela = red;
    
    if(redDodjela.brojElemenata() != red.brojElemenata())
        return false;
    
    if(redDodjela.celo() != red.celo())
        return false;
    
    if(redKopija.vrh() != red.vrh())
        return false;
    
    DvostraniRed<T> destruktivni(red);
    destruktivni = red;
    
    while(redKopija.brojElemenata() != 0) {
        if(destruktivni.skiniSaVrha() != redKopija.skiniSaVrha())
            return false;
    }
    
    while(redDodjela.brojElemenata() != 0) {
        if(red.skiniSaCela() != redDodjela.skiniSaCela())
            return false;
    }
    
    return true;
}

template <typename T>
bool TestirajMetoduBrisi(DvostraniRed<T> red) {
    
    red.brisi();
    
    if(red.brojElemenata() != 0)
        return false;

    return true;
}

template <typename T>
bool TestirajMetoduStaviNaCelo(DvostraniRed<T> red) {
    for(int i{1};i<=10;++i)
        red.staviNaCelo(i);
    
    if(red.celo() != 10)
        return false;
    
    if(red.brojElemenata() != 10)
        return false;
    
    for(int i{10};i>=1;--i)
        if(i != red.skiniSaCela())
            return false;
        
    return true;
}

template <typename T>
bool TestirajMetoduStaviNaVrh(DvostraniRed<T> red) {
    for(int i{1};i<=10;++i)
        red.staviNaVrh(i);
    
    if(red.celo() != 1)
        return false;
    
    if(red.brojElemenata() != 10)
        return false;
    
    for(int i{10};i<=10;++i)
        if(red.skiniSaVrha() != i)
            return false;
        
    return true;
}

template <typename T>
bool TestirajMetoduSkiniSaCela(DvostraniRed<T> red) {
    for(int i{1};i<=10;++i)
        red.staviNaCelo(i);
    
    for(int i{10};i>=1;--i) {
        if(i != red.skiniSaCela())
            return false;
    }
    
    if(red.brojElemenata() != 0)
        return false;
    
    return true;
}

template <typename T>
bool TestirajMetoduSkiniSaVrha(DvostraniRed<T> red) {
    for(int i{1};i<=10;++i)
        red.staviNaCelo(i);
    
    for(int i{1};i<=10;++i) {
        if(i != red.skiniSaVrha())
            return false;
    }
    
    if(red.brojElemenata() != 0)
        return false;
    
    return true;
}

template <typename T>
bool TestirajMetoduVrh(DvostraniRed<T> red) {
    red.staviNaVrh(5);
    
    if(red.vrh() != 5)
        return false;
    
    red.staviNaCelo(6);
    red.staviNaCelo(7);
    
    if(red.vrh() != 5)
        return false;
    
    red.staviNaVrh(6);
        
    if(red.vrh() != 6)
        return false;
    
    red.skiniSaVrha();
    
    if(red.vrh() != 5)
        return false;

    return true;
}

template <typename T>
bool TestirajMetoduCelo(DvostraniRed<T> red) {
    red.staviNaCelo(1);
    
    if(red.celo() != 1)
        return false;
    
    red.staviNaVrh(3);
    red.staviNaVrh(2);
    
    if(red.celo() != 1)
        return false;
    
    red.staviNaCelo(2);
        
    if(red.celo() != 2)
        return false;
    
    red.skiniSaCela();
    
    if(red.celo() != 1)
        return false;

    return true;
}

template <typename T>
bool TestirajMetoduBrojElemenata(DvostraniRed<T> red) {
    for(int i={1};i<=10;++i)
        red.staviNaCelo(i);
    
    if(red.brojElemenata() != 10)
        return false;
    
    red.skiniSaCela();
    
    if(red.brojElemenata() != 9)
        return false;
    
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    red.skiniSaCela();
    
    if(red.brojElemenata() != 1)
        return false;
    
    red.skiniSaCela();
    
    if(red.brojElemenata() != 0)
        return false;
    
    return true;
}

template <typename T>
bool TestirajIzuzetke(DvostraniRed<T> red) {
    red.brisi();
    
    bool jeBacenIzuzetak = false;

    try {
        red.skiniSaVrha();
    } catch(std::exception e) {
        jeBacenIzuzetak = true;
    }
    
    if(!jeBacenIzuzetak)
        return false;
        
    jeBacenIzuzetak = false;
    
    try {
        red.skiniSaCela();
    } catch(std::exception e) {
        jeBacenIzuzetak = true;
    }
    
    if(!jeBacenIzuzetak)
        return false;
        
    jeBacenIzuzetak = false;
    
    try {
        red.vrh();
    } catch(std::exception e) {
        jeBacenIzuzetak = true;
    }
    
    if(!jeBacenIzuzetak)
        return false;
        
    jeBacenIzuzetak = false;
    
    try {
        red.celo();
    } catch(std::exception e) {
        jeBacenIzuzetak = true;
    }
    
    if(!jeBacenIzuzetak)
        return false;
    
    return true;
}

//#endregion

int main() {
    
    std::cout << std::boolalpha;
    
    DvostraniRed<int> red1;
    for(int i{1};i<=10;++i) red1.staviNaVrh(i);
    std::cout << "Test dobuke kopije: " << (TestirajDubokoKopiranje(red1) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red2;
    for(int i{1};i<=10;++i) red2.staviNaCelo(i);
    std::cout << "Test metode brisi: " << (TestirajMetoduBrisi(red2) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red3;
    std::cout << "Test metode staviNaCelo: " << (TestirajMetoduStaviNaCelo(red3) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red4;
    std::cout << "Test metode staviNaVrh: " << (TestirajMetoduStaviNaVrh(red4) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red5;
    std::cout << "Test metode skiniSaCela: " << (TestirajMetoduSkiniSaCela(red5) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red6;
    std::cout << "Test metode skiniSaVrha: " << (TestirajMetoduSkiniSaVrha(red6) ? "USPJESAN" : "PADA") << std::endl;

    DvostraniRed<int> red7;
    std::cout << "Test metode vrh: " << (TestirajMetoduVrh(red7) ? "USPJESAN" : "PADA") << std::endl;

    DvostraniRed<int> red8;
    std::cout << "Test metode celo: " << (TestirajMetoduCelo(red8) ? "USPJESAN" : "PADA") << std::endl;

    DvostraniRed<int> red9;
    std::cout << "Test metode brojElemeanta: " << (TestirajMetoduBrojElemenata(red9) ? "USPJESAN" : "PADA") << std::endl;
    
    DvostraniRed<int> red10;
    std::cout << "Test bacanja izuzetaka: " << (TestirajIzuzetke(red10) ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
