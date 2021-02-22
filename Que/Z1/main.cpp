#include <iostream>
#include <stdexcept>

//#region Red

using namespace std;

template <typename T>
class Red {
    struct Node {
        T element;
        Node* previous;
        Node* next;
        Node(T element,Node* previous = nullptr,Node* next = nullptr):element(element),previous(previous),next(next) {}
    };
    
    Node* zadnji; // zadnji u redu
    Node *prvi;  // prvi u redu
    int duzina;
    
public:
    Red():zadnji(nullptr),prvi(nullptr),duzina(0) {}
    Red(const Red<T> &novi);
    Red operator=(const Red <T> &novi);
    ~Red();
    void brisi();
    void stavi(const T& el);
    T skini();
    T& celo() const;
    int brojElemenata() const { return duzina; }
};

template <typename T>
Red<T>::Red(const Red<T> &novi):zadnji(nullptr),prvi(nullptr),duzina(novi.duzina) {

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
Red<T> Red<T>::operator=(const Red<T> &novi) {
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
        
        return *this;
    }
    
    return novi;
}

template <typename T>
Red<T>::~Red() {
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
void Red<T>::brisi() {
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
void Red<T>::stavi(const T& el) {
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
T Red<T>::skini() {
    if(duzina == 0)
        throw std::domain_error("Red je prazan.");
    
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
T& Red<T>::celo() const {
    if(duzina == 0)
        throw std::domain_error("Red je prazan.");
    
    return prvi -> element;
}

//#endregion

//#region Testovi

template <typename T>
bool TestirajDubokoKopiranje(Red<T> red) {
    Red<T> redKopija(red);
    
    if(redKopija.brojElemenata() != red.brojElemenata())
        return false;
    
    if(redKopija.celo() != red.celo())
        return false;
    
    Red<T> redDodjela = red;
    
    if(redDodjela.brojElemenata() != red.brojElemenata())
        return false;
    
    if(redDodjela.celo() != red.celo())
        return false;
    
    Red<T> destruktivni(red);
    destruktivni = red;
    
    while(redKopija.brojElemenata() != 0) {
        if(destruktivni.skini() != redKopija.skini())
            return false;
    }
    
    while(redDodjela.brojElemenata() != 0) {
        if(red.skini() != redDodjela.skini())
            return false;
    }
    
    return true;
}

template <typename T>
bool TestirajMetoduBrisi(Red<T> red) {
    
    red.brisi();
    
    if(red.brojElemenata() != 0)
        return false;

    return true;
}

template <typename T>
bool TestirajMetoduStavi(Red<T> red) {
    for(int i{1};i<=10;++i)
        red.stavi(i);
    
    if(red.celo() != 1)
        return false;
    
    if(red.brojElemenata() != 10)
        return false;
    
    return true;
}

template <typename T>
bool TestirajMetoduSkini(Red<T> red) {
    for(int i{1};i<=10;++i)
        red.stavi(i);
    
    for(int i{1};i<=10;++i) {
        if(i != red.skini())
            return false;
    }
    
    if(red.brojElemenata() != 0)
        return false;
    
    return true;
}

template <typename T>
bool TestirajMetoduCelo(Red<T> red) {
    red.stavi(1);
    
    if(red.celo() != 1)
        return false;
    
    red.stavi(3);
    red.stavi(2);
    
    if(red.celo() != 1)
        return false;
    
    red.skini();
        
    if(red.celo() != 3)
        return false;
    
    red.skini();
    
    if(red.celo() != 2)
        return false;

    return true;
}

template <typename T>
bool TestirajMetoduBrojElemenata(Red<T> red) {
    for(int i={1};i<=10;++i)
        red.stavi(i);
    
    if(red.brojElemenata() != 10)
        return false;
    
    red.skini();
    
    if(red.brojElemenata() != 9)
        return false;
    
    red.skini();
    red.skini();
    red.skini();
    red.skini();
    red.skini();
    red.skini();
    red.skini();
    red.skini();
    
    if(red.brojElemenata() != 1)
        return false;
    
    red.skini();
    
    if(red.brojElemenata() != 0)
        return false;
    
    return true;
}

template <typename T>
bool TestirajIzuzetke(Red<T> red) {
    red.brisi();
    
    bool jeBacenIzuzetak = false;
    
    try {
        red.skini();
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
    
    Red<int> red1;
    for(int i{1};i<=10;++i) red1.stavi(i);
    std::cout << "Test dobuke kopije: " << (TestirajDubokoKopiranje(red1) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red2;
    for(int i{1};i<=10;++i) red2.stavi(i);
    std::cout << "Test metode brisi: " << (TestirajMetoduBrisi(red2) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red3;
    std::cout << "Test metode stavi: " << (TestirajMetoduStavi(red3) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red4;
    std::cout << "Test metode skini: " << (TestirajMetoduSkini(red4) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red5;
    std::cout << "Test metode celo: " << (TestirajMetoduCelo(red5) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red6;
    std::cout << "Test metode brojElemenata: " << (TestirajMetoduBrojElemenata(red6) ? "USPJESAN" : "PADA") << std::endl;
    
    Red<int> red7;
    std::cout << "Test bacanja izuzetaka: " << (TestirajIzuzetke(red7) ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
