#include <iostream>
#include <stdexcept>

using namespace std;
template<typename T>
class Iterator;

//#region Lista

template <typename T>
class Lista {

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual T& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T& el) = 0;
    virtual void dodajIza(const T& el) = 0;
    virtual T operator [](int i) const = 0;
    virtual T& operator [](int i) = 0;
};

//#endregion

//#region DvostrukaLista

template <typename T>
class DvostrukaLista : public Lista<T> {
    struct Node {
        T element;
        Node* previous;
        Node* next;
        
        Node(T element,Node *previous = nullptr,Node *next = nullptr):element(element),previous(previous),next(next) {}
    };
    
    Node *prvi;
    Node *zadnji;
    Node *tekuci;
    int duzina;
    
    void provjeraDuzine() const { if(duzina <= 0) throw std::domain_error("Lista je prazna"); }
    friend class Iterator<T>;
    
public:
    //#region Metode DvostrukaLista
    
    DvostrukaLista():prvi(nullptr),zadnji(nullptr),tekuci(nullptr),duzina(0) {}
    DvostrukaLista(const DvostrukaLista<T>& nova);
    DvostrukaLista<T> operator=(const DvostrukaLista<T>& nova);
    ~DvostrukaLista();
    int brojElemenata() const override { return duzina; }
    T& trenutni() const override { provjeraDuzine(); return tekuci -> element; }
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override { tekuci = prvi; }
    void kraj() override { tekuci = zadnji; }
    void obrisi() override;
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    T operator[](int i) const override;
    T& operator[](int i) override;

    //#endregion
    
};

//#endregion 

//#region Implementacija metoda DvostrukaLista

template <typename T>
DvostrukaLista<T>::DvostrukaLista(const DvostrukaLista<T> &nova) {
    if(this != &nova && nova.duzina > 0) {
        Node* poc = nova.prvi;
        Node* krj = nova.zadnji;
        Node* t = new Node(poc -> element,nullptr,nullptr);
        Node* np = t;
        
        poc = poc -> next;
        while(poc != krj) {
            t -> next = new Node(poc -> element,t,nullptr);
            t = t -> next;
            poc = poc -> next;
        }
        
        t -> next = new Node(krj -> element,t,nullptr);
        t = t -> next;
        
        prvi = np;
        zadnji = t;
        duzina = nova.duzina;
    }
}

template <typename T>
DvostrukaLista<T> DvostrukaLista<T>::operator=(const DvostrukaLista<T> &nova) {
    if(this != &nova && nova.duzina > 0) {
        Node* poc = nova.prvi;
        Node* krj = nova.zadnji;
        Node* t = new Node(poc -> element,nullptr,nullptr);
        Node* np = t;
        
        poc = poc -> next;
        while(poc != krj) {
            t -> next = new Node(poc -> element,t,nullptr);
            t = t -> next;
            poc = poc -> next;
        }
        
        t -> next = new Node(krj -> element,t,nullptr);
        t = t -> next;
        
        prvi = np;
        zadnji = t;
        duzina = nova.duzina;
        
        return *this;
    }
    
    return nova;
}

template <typename T>
DvostrukaLista<T>::~DvostrukaLista() {
    
    while(prvi != zadnji) {
        prvi = prvi -> next;
        delete prvi -> previous;
    }
    
    delete zadnji;
}

template<typename T>
bool DvostrukaLista<T>::prethodni() {
    provjeraDuzine();
    
    if(!tekuci -> previous)
        return false;
    
    tekuci = tekuci -> previous;
    
    return true;
}

template<typename T>
bool DvostrukaLista<T>::sljedeci() {
     provjeraDuzine();
    
    if(!tekuci -> next) 
        return false;
    
    tekuci = tekuci -> next;
    
    return true;
}

template<typename T>
void DvostrukaLista<T>::obrisi() {
    provjeraDuzine();
    Node* brisanje = tekuci;
    
    if(tekuci -> previous && tekuci -> next) {
        tekuci -> next -> previous = tekuci -> previous;
        tekuci -> previous -> next = tekuci -> next;
            
        tekuci = tekuci -> next;
      
    } else if(tekuci -> previous && !tekuci -> next) {
        tekuci = tekuci -> previous;
        tekuci -> next = nullptr;
        zadnji = tekuci;
    } else if(!tekuci -> previous && tekuci -> next) {
        tekuci = tekuci -> next;
        tekuci -> previous = nullptr;
        prvi = tekuci;
    } else {
        prvi = nullptr;
        zadnji = nullptr;
    }

    delete brisanje;
    --duzina;
}

template<typename T>
void DvostrukaLista<T>::dodajIspred(const T& el) { //lijevo
    Node* novi = new Node(el);
    
    if(!duzina) {
        tekuci = novi;
        prvi = novi; 
        zadnji = novi;
    } else {
        novi -> next = tekuci;
        novi -> previous = tekuci -> previous;
        
        if(tekuci -> previous) 
            tekuci -> previous -> next = novi;
        
        tekuci -> previous = novi;
            
        if(!novi -> previous)
            prvi = novi;
    }
    
    ++duzina;
}

template<typename T>
void DvostrukaLista<T>::dodajIza(const T& el) { //desno
    Node* novi = new Node(el);
    
    if(!duzina) {
        tekuci = novi;
        prvi = novi;
        zadnji = novi;
    } else {
        novi -> next = tekuci -> next;
        
        if(tekuci -> next)
            tekuci -> next -> previous = novi;
        
        tekuci -> next = novi;
        novi -> previous = tekuci;
        
        if(!novi -> next)
            zadnji = novi;
    }
    
    ++duzina;
}

template<typename T>
T DvostrukaLista<T>::operator[](int i) const {
    if(i < 0 || i > duzina)
        throw std::domain_error("Neispravan indeks");  
    
    Node* temp = prvi;
    
    int index=0;
    while(index++ != i) {
        temp = temp -> next;
    }
    
    return temp -> element;
}

template<typename T>
T& DvostrukaLista<T>::operator[](int i) {
  if(i < 0 || i > duzina)
        throw std::domain_error("Neispravan indeks");  
    
    Node* temp = prvi;
    
    int index=0;
    while(index++ != i) {
        temp = temp -> next;
    }
    
    return temp -> element;  
}

//#endregion

//#region Implementacija metoda iteratora

template <typename T> // Trebalo bi se napraviti genericnije posto klasa derivira iz aptraktne klase Niz, poboljsati rjesenje naknadno
class Iterator {
    const DvostrukaLista<T> &dl;
    typename DvostrukaLista<T>::Node *tekuci;

public:
    Iterator(const DvostrukaLista<T> &dl):dl(dl),tekuci(dl.prvi) {}
    T current() const { return tekuci -> element; }
    bool operator++() { if(tekuci -> next != nullptr) { tekuci = tekuci -> next; return true; } return false; }
    bool operator--() { if(tekuci -> previous != nullptr) { tekuci = tekuci -> previous; return true; } return false; }
    typename DvostrukaLista<T>::Node* begin() { return dl.prvi; }
    typename DvostrukaLista<T>::Node* end() { return dl.zadnji; }
    T first() { return dl.prvi -> element; }
    T last() { return dl.zadnji -> element; }
    bool operator!=(typename DvostrukaLista<T>::Node* comparer) { return tekuci != comparer; }
    bool operator==(typename DvostrukaLista<T>::Node* comparer) { return tekuci == comparer; }
};

//#endregion

//#region dajMaksimum()

template<typename T>
T dajMaksimum(const Lista<T>& n) {
    
    Iterator<T> it(dynamic_cast<const DvostrukaLista <T>&>(n));
    
    T max = it.first();
    while(it != it.end()) {
        if(it.current() > max)
            max = it.current();
        ++it;
    }
    
    return it.current() > max ? it.current() : max;
}

//#endregion

//#region Testovi

// Testiramo da li je zaista duboka kopija, i da li su svi privatni elementi uredno kopirani
template <typename T>
bool TestirajKopiju(Lista<T> &lista,Lista<T> &kopijaListe) {
   
    for(int i=0;i<lista.brojElemenata();++i)
        if(lista[i] != kopijaListe[i])
            return false;
    
    lista.obrisi();
    
    if(lista.brojElemenata() == kopijaListe.brojElemenata())
        return false;
    
    return true;
}

// Testiranje metoda za pomjeranje prethodni() i sljedeci() te metode trenutni()
template <typename T>
bool TestirajPomjeranjeITrenutni(Lista<T> &lista) {
    if(lista.trenutni() != 1)
        return false;
   
    lista.sljedeci();
    
    if(lista.trenutni() != 5)
        return false;
    
    lista.sljedeci();
    
    if(lista.trenutni() != 4)
        return false;
    
    lista.prethodni();
    
    if(lista.trenutni() != 5)
        return false;
    
    lista.prethodni();
    
    if(lista.trenutni() != 1)
        return false;
    
    if(lista.prethodni() != false) // ne moze dalje nazad
        return false;
    
    if(lista.trenutni() != 1)
        return false;
        
    lista.sljedeci();
    lista.sljedeci();
    lista.sljedeci();
    lista.sljedeci();
    lista.sljedeci();
    lista.sljedeci();
    
    if(lista.trenutni() != 2) 
        return false;
        
    if(lista.sljedeci() != false) // ne moze dalje naprijed
        return false;
    
    return true;
}

// Testiranje metoda za pomjeranje trenutnog elementa na prvi pocetak() i zadnji kraj() elemenat u nizu
template <typename T>
bool TestirajPocetakIKraj(Lista<T> &lista) {
    if(lista.trenutni() != 1)
        return false;
    
    lista.pocetak();
    
    if(lista.trenutni() != 1 || lista.prethodni())
        return false;
        
    lista.pocetak();
    lista.pocetak();
    lista.pocetak();
    
    if(lista.trenutni() != 1)
        return false;
    
    lista.kraj();
    
    if(lista.trenutni() != 2 || lista.sljedeci())
        return false;
    
    lista.kraj();
    lista.kraj();
    lista.kraj();
    
    lista.pocetak();
    
    if(lista.trenutni() != 1 || lista.prethodni())
        return false;
    
    return true;
}

// Testiranje metode za brisanje obrisi() i ponasanja tekuceg u tom slucaju
template <typename T>
bool TestirajBrisanje(Lista<T> &lista) {
    
    if(lista.trenutni() != 1) 
        return false;
    
    lista.obrisi();
    
    if(lista.brojElemenata() != 4)
        return false;
    else if(lista.trenutni() != 5)
        return false;
    
    return true;
}

// Testiranje metode dodajIspred() za dodavanje elemenata ispred niza
template <typename T>
bool TestirajDodajIspred(Lista<T> &lista) {
    if(lista.brojElemenata() != 0) 
        return false;
    
    lista.dodajIspred(1243);
    
    if(lista.brojElemenata() != 1)
        return false;
    else if (lista.trenutni() != 1243)
        return false;
    
    lista.dodajIspred(781);
    lista.dodajIspred(141);
    lista.dodajIspred(241);
    lista.dodajIspred(31);
    lista.dodajIspred(3);
    
    if(lista.brojElemenata() != 6)
        return false;
    else if(lista.trenutni() != 1243)
        return false;
    
    int testni[6] = {781,141,241,31,3,1243};
    
    for(int i=0;i<lista.brojElemenata();++i)
        if(lista[i] != testni[i])
            return false;
            
    return true;
}

// Testiranje metode dodajIza() za dodavanje elemenata ispred niza
template <typename T>
bool TestirajDodajIza(Lista<T> &lista) {
    if(lista.brojElemenata() != 0) 
        return false;
    
    lista.dodajIza(8132);
    
    if(lista.brojElemenata() != 1)
        return false;
    else if (lista.trenutni() != 8132)
        return false;
    
    lista.dodajIza(132);
    lista.dodajIza(5123);
    lista.dodajIza(13);
    lista.dodajIza(547);
    lista.dodajIza(5);
    
    if(lista.brojElemenata() != 6)
        return false;
    else if(lista.trenutni() != 8132)
        return false;
    
    int testni[6] = {8132,5,547,13,5123,132};
    
    for(int i=0;i<lista.brojElemenata();++i)
        if(lista[i] != testni[i])
            return false;
    
    return true;
}

// Testiranje metode dajMaksimum()
template <typename T>
bool TestirajDajMaksimum(Lista<T> &lista) {
    
    for(int i{0};i<50;i++)
        lista.dodajIspred(i * i + 1);

    if(dajMaksimum(lista) != 2402)
        return false;
    
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    
    if(dajMaksimum(lista) != 2402)
        return false;
    
    return true;
}

//#endregion

int main() {
    
    std::cout << std::boolalpha;
    
    std::cout << "Testovi vracaju true ako su uspjesno zavrseni i a false ako nisu.";
    std::cout << std::endl;
    std::cout << std::endl;
    
    //#region Test Kopirajuceg Konstruktora 
    
        DvostrukaLista<int> dl1;
        
        for(int i=0;i<10;++i)
            dl1.dodajIza(i+1);
        
        DvostrukaLista<int> dl1Kopija(dl1);
        
        std::cout << "Test kopirajuceg konstruktora: " << (TestirajKopiju(dl1,dl1Kopija) ? "USPJESAN" : "PADA");
        
    //#endregion
    
    std::cout << std::endl;
        
    //#region Test Operatora Dodjele
    
        DvostrukaLista<int> dl2;
        
        for(int i=0;i<10;++i)
            dl2.dodajIza(i+1);
        
        DvostrukaLista<int> dl2Kopija = dl2;
        
        std::cout << "Test operatora dodjele: " << (TestirajKopiju(dl2,dl2Kopija) ? "USPJESAN" : "PADA");

    
    //#endregion
    
    std::cout << std::endl;
     
    //#region Test Metoda Za Pomjeranje prethodni() i sljedeci() te metode trenutni()
        
        DvostrukaLista<int> dl3;
        
        for(int i=0;i<5;++i)
            dl3.dodajIza(i+1);

        std::cout << "Test metoda za pomjeranje prethodni() i sljedeci() te metode trenutni(): " << (TestirajPomjeranjeITrenutni(dl3) ? "USPJESAN" : "PADA");
    
    //#endregion
    
    std::cout << std::endl;
    
    //#region Test Metoda pocetak() i kraj()
    
        DvostrukaLista<int> dl4;
        
        for(int i=0;i<5;++i)
            dl4.dodajIza(i+1);
        
        std::cout << "Test metoda pocetak() i kraj(): " << (TestirajPocetakIKraj(dl4) ? "USPJESAN" : "PADA");
    
    //#endregion
    
    std::cout << std::endl;
    
    //#region Test Metode obrisi()
    
        DvostrukaLista<int> dl5;
        
        for(int i=0;i<5;++i)
            dl5.dodajIza(i+1);
        
        std::cout << "Test metode obirsi(): " << (TestirajBrisanje(dl5) ? "USPJESAN" : "PADA");

    //#endregion
    
    std::cout << std::endl;
    
    //#region Test Metode dodajIspred()
    
        DvostrukaLista<int> dl6;
        std::cout << "Test metode dodajIspred(): " << (TestirajDodajIspred(dl6) ? "USPJESAN" : "PADA");
    
    //#endregion
    
    std::cout << std::endl;
    
    //#region Test Metode dodajIza()
        
        DvostrukaLista<int> dl7;
        std::cout << "Test metode dodajIza(): " << (TestirajDodajIza(dl7) ? "USPJESAN" : "PADA");
    
    //#endregion
    
    std::cout << std::endl;
    
    //#region Test Metode dajMaksimum()
    
        DvostrukaLista<int> dl8;
        std::cout << "Test metode dodajIza(): " << (TestirajDajMaksimum(dl8) ? "USPJESAN" : "PADA");
        
    //#endregion
    
    // Testiranje ostalih metoda poput brojElemenata(), operatora[] i slicne su testirane unutar ostalih testova
    
    return 0;
}
