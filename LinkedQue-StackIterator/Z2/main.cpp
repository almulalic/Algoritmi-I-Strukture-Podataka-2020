#include <iostream>

#include <algorithm>
#include <cmath>
#include <ostream>  
#include <sstream>
#include <vector>

using namespace std;

//#region Stack

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

//#region Binarna pretraga

template <typename T>
int binarna_pretraga(std::vector<T> v, T trazeni) {
    int prvi = 0;
    int zadnji = v.size() - 1;
    int pola;
    
    while(zadnji >= prvi) {
        pola = (prvi + zadnji) / 2;

        if(v.at(pola) == trazeni)
            return pola;
        else if(v.at(pola) > trazeni)
            zadnji = pola - 1;
        else
            prvi = pola + 1;
    }
    
    return -1;
}

//#endregion

//#region Pretraga

void pretraga(Stek<std::vector<int>> &s, int trazeni,std::ostream &izlaz = std::cout) {
    
    if(s.brojElemenata() == 0) {
        izlaz << "Nema elementa";
        return;
    }
    
    std::vector<int> temp = s.vrh();
    s.skini();
    
    int index = binarna_pretraga(temp,trazeni);
    if(index != -1) {
        izlaz << index << " " << s.brojElemenata();
        return s.stavi(temp);
    } 
    
    pretraga(s,trazeni,izlaz);
    s.stavi(temp);
}

//#endregion

//#region Testovi 

// Napomena da zaista nisam znao za drugi način testiranja bez da u potpunosti mjenjam parametre pretraga() funkcije, ovim je očuvano da radi u oba slučaja

// Trazeni element se nalazi na pocetku
bool TestMetodePretragaPrvi() {
    Stek<std::vector<int>> stek;
    
    std::vector<int> vektor1 = {1,2,3};
    std::vector<int> vektor2 = {4,5,6,7,8,9};
    std::vector<int> vektor3 = {10,11,12,13,14,15,16,17};
    
    stek.stavi(vektor1);
    stek.stavi(vektor2);
    stek.stavi(vektor3);
    
    std::ostringstream buffer;
    pretraga(stek,3,buffer);    
    
    return buffer.str() == "2 0";
}

// Trazeni element se nalazi u sredini
bool TestMetodePretragaDrugi() {
    Stek<std::vector<int>> stek;
    
    std::vector<int> vektor1 = {125,1353};
    std::vector<int> vektor2 = {3412,5123,6141,7844,8755,9532};
    std::vector<int> vektor3 = {10022,11031,13512,16214,18294,20123,24124,24324};
    
    stek.stavi(vektor1);
    stek.stavi(vektor2);
    stek.stavi(vektor3);
    
    std::stringstream buffer;
    pretraga(stek,6141,buffer);    
    
    return buffer.str() == "2 1";
}

// Trazeni element se nalazi u zadnjem redu
bool TestMetodePretragaTreci() {
    Stek<std::vector<int>> stek;
    
    std::vector<int> vektor1 = {1225,13453};
    std::vector<int> vektor2 = {43412,51423,61421,78454,87515,95332};
    std::vector<int> vektor3 = {100223,110315,134512,162514,186294,201723,243124,123322451};
    
    stek.stavi(vektor1);
    stek.stavi(vektor2);
    stek.stavi(vektor3);
    
    std::stringstream buffer;
    pretraga(stek,123322451,buffer);  
    
    return buffer.str() == "7 2";
}

// Test treba da vraca da nema elemenata
bool TestMetodePretragaCetvrti() {
    Stek<std::vector<int>> stek;
    
    std::vector<int> vektor1 = {14,24,56};
    std::vector<int> vektor2 = {78,82,91,103,112,145};
    std::vector<int> vektor3 = {201,1001,2002,3014,4245,6123,7812,9812};
    
    std::stringstream buffer;
    pretraga(stek,5124,buffer);  
    
    return buffer.str() == "Nema elementa";
}

//#endregion

int main() {
    
    std::cout << std::boolalpha;
    
    std::cout << "Test kada se trazeni element nalazi u prvom redu: " << (TestMetodePretragaPrvi() ? "USPJESAN" : "PADA") << std::endl;
    std::cout << "Test kada se trazeni element nalazi u sredini: " << (TestMetodePretragaDrugi() ? "USPJESAN" : "PADA") << std::endl;
    std::cout << "Test kada se trazeni element nalazi u zadnjem redu: " << (TestMetodePretragaTreci() ? "USPJESAN" : "PADA") << std::endl;
    std::cout << "Test kada se trazeni element ne nalazi u steku: " << (TestMetodePretragaCetvrti() ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
