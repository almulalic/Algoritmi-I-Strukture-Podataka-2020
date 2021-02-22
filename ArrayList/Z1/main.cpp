#include <stdexcept>
#include <iostream>
#include <cmath>

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

//#region Niz Lista

template <typename T>
class NizLista : public Lista<T> {
private:
    int kapacitet; // Mjenja se ukoliko dodje do limita, te se tako postize efekat niza bez granica
    int duzina;
    int tekuci;
    T** niz;

public:
    NizLista(): kapacitet(10),duzina(0),tekuci(0), niz(new T*[kapacitet]{}) {};
    NizLista(const NizLista<T> &nova);
    NizLista<T> operator=(const NizLista<T> &nova);
    ~NizLista() { for(int i=0;i<kapacitet;++i) delete niz[i];  delete[] niz; }
    void prosiriKapacitet(int noviKapacitet);
    int brojElemenata() const { return duzina; }
    T& trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    T operator [](int i) const override;
    T& operator [](int i) override;
};

template <typename T>
NizLista<T>::NizLista(const NizLista<T> &nova) {
    if(&nova != this) {
        niz = new T*[nova.kapacitet]{};
        
        for(int i=0;i<nova.brojElemenata();++i)
            niz[i] = new T(*nova.niz[i]);
        
        kapacitet = nova.kapacitet;
        duzina = nova.brojElemenata();
        tekuci = 0;
    }
}

template <typename T>
NizLista<T> NizLista<T>::operator=(const NizLista<T> &nova) {
    if(&nova != this) {
        delete[] niz;
        
        niz = new T*[nova.kapacitet]{};
        
        for(int i=0;i<nova.brojElemenata();++i)
            niz[i] = new T(*nova.niz[i]);
        
        kapacitet = nova.kapacitet;
        duzina = nova.brojElemenata();
        tekuci = 0;
        
        return *this;
    }
    
    return nova;
}

template<typename T>
void NizLista<T>::prosiriKapacitet(int noviKapacitet) {
    T** novi(new T*[noviKapacitet]{});
        
    for(int i=0;i<duzina;++i)
        novi[i] = niz[i];
    
    delete[] niz;
    niz = novi;
    kapacitet = noviKapacitet;
}

template <typename T>
T& NizLista<T>::trenutni() const {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
        
    return *niz[tekuci];
}

template <typename T>
bool NizLista<T>::prethodni() {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
    
    if(tekuci - 1 < 0)
        return false;
    
    --tekuci;
    return true;
}

template <typename T>
bool NizLista<T>::sljedeci() {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
    
    if(tekuci + 1 > duzina - 1)
        return false;
    
    ++tekuci;
    return true;
}

template <typename T>
void NizLista<T>::pocetak() {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
    
    tekuci = 0;
}

template <typename T>
void NizLista<T>::kraj() {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
    
    tekuci = duzina - 1;
}

template <typename T>
void NizLista<T>::obrisi() {
    if(duzina == 0)
        throw std::domain_error("Lista je prazna");
    
    delete niz[tekuci];
    
	for (int i = tekuci; i < duzina - 1; ++i)
		niz[i] = niz[i + 1];
		
	niz[duzina - 1] = nullptr;
	
	if (tekuci == duzina - 1)
		tekuci--;
		
	duzina--;
}

template <typename T>
void NizLista<T>::dodajIspred(const T& el) { // lijevo
    
    if(duzina + 1 >= kapacitet)
        prosiriKapacitet(kapacitet * 10);
    
    for(int i=duzina;i>tekuci;--i)
        niz[i] = niz[i - 1];
    
    if(duzina == 0) {
        niz[0] = new T(el);
        tekuci = 0;  
    } else {
        niz[tekuci + 1] = niz[tekuci];
        niz[tekuci] = new T(el);
        ++tekuci;
    }
    
    ++duzina;
}

template <typename T>
void NizLista<T>::dodajIza(const T& el) { // desno

    if(duzina + 1 >= kapacitet)
        prosiriKapacitet(kapacitet * 10);
    
    if(duzina == 0) {
        niz[0] = new T(el);
        tekuci = 0;  
    } else {
        for(int i=duzina ;i>tekuci;--i)
            niz[i] = niz[i - 1];
        
        niz[tekuci + 1] = new T(el);
    }
    
    ++duzina;
}

template <typename T>
T NizLista<T>::operator [](int i) const {
    if(i < 0 || i >= duzina)
        throw std::range_error("Indeks nije validan");
    
    return *niz[i];
}

template <typename T>
T& NizLista<T>::operator [](int i) {
    if(i < 0 || i >= duzina)
        throw std::range_error("Indeks nije validan");
    
    return *niz[i];
}

//#endregion

//#region Jednostruka Lista

template <typename T>
class JednostrukaLista : public Lista<T> {

private:
    struct Node {
        T element;
        Node* next;
        Node(const T& element,Node* next):element(element),next(next){};
    };
    
    int lduzina;
    int dduzina;
    Node *tp;
    Node *t;
    Node *ti;
    Node *prvi;
    Node *zadnji;
    
public:
    Node* dajPrethodniTrenutni() const { return tp; }
    Node* dajTrenutni() const { return t; } 
    Node* dajIspredTrenutnog() const { return ti; }
    Node* dajPocetak() const { return prvi; }
    Node* dajZadnji() const { return zadnji; }
    JednostrukaLista(): lduzina(0),dduzina(0),tp(nullptr),t(nullptr),ti(nullptr),prvi(nullptr),zadnji(nullptr) {};
    JednostrukaLista(const JednostrukaLista<T> &nova);
    JednostrukaLista<T> operator=(const JednostrukaLista<T> &nova);
    ~JednostrukaLista() override;
    int brojElemenata() const override { return lduzina + dduzina; }
    T& trenutni() const override;
    bool sljedeci() override;
    bool prethodni() override;
    void pocetak() override {  tp = nullptr; t = prvi; ti = t -> next; }
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    T operator [](int i) const override;
    T& operator [](int i) override;
};

template <typename T>
JednostrukaLista<T>::JednostrukaLista(const JednostrukaLista<T> &nova) {
    if(&nova != this && nova.brojElemenata() > 0) {
        lduzina = nova.lduzina;
        dduzina = nova.dduzina;
        
        Node *np = nova.dajPocetak();
        Node *poc = new Node(np -> element,nullptr);
        np = np -> next;
        
        Node *tk = poc;
        
        if(nova.tp == nullptr)
            tp = nullptr;
        else if(nova.t == nullptr)
            t = nullptr;
        else if(nova.ti == nullptr)
            ti = nullptr;
        
        while(np != nullptr) {
            tk -> next = new Node(np -> element,nullptr);
            tk = tk -> next;
            
            if(np == nova.dajPrethodniTrenutni())
                tp = tk;
            else if(np == nova.dajTrenutni())
                t = tk;
            else if(np == nova.dajIspredTrenutnog())
                ti = tk;
            
            np = np -> next;
        }
        
        prvi = poc;
        zadnji = tk;
    }
}

template <typename T>
JednostrukaLista<T> JednostrukaLista<T>::operator=(const JednostrukaLista<T> &nova) {
    
    if(&nova != this && nova.brojElemenata() > 0) {
          lduzina = nova.lduzina;
        dduzina = nova.dduzina;
        
        Node *np = nova.dajPocetak();
        Node *poc = new Node(np -> element,nullptr);
        np = np -> next;
        
        Node *tk = poc;
        
        if(nova.tp == nullptr)
            tp = nullptr;
        else if(nova.t == nullptr)
            t = nullptr;
        else if(nova.ti == nullptr)
            ti = nullptr;
        
        while(np != nullptr) {
            tk -> next = new Node(np -> element,nullptr);
            tk = tk -> next;
            
            if(np == nova.dajPrethodniTrenutni())
                tp = tk;
            else if(np == nova.dajTrenutni())
                t = tk;
            else if(np == nova.dajIspredTrenutnog())
                ti = tk;
            
            np = np -> next;
        }
        
        prvi = poc;
        zadnji = tk;
        
        return *this;
    }
    
    return nova;
}

template <typename T>
JednostrukaLista<T>::~JednostrukaLista() {

    Node *temp = prvi;
    
    while(temp != zadnji) {
        Node *old = temp;
        temp = temp -> next;
        delete old;
    }
    
    delete temp;
}

template <typename T>
T& JednostrukaLista<T>::trenutni() const {
    if(brojElemenata() == 0)
        throw std::domain_error("Lista je prazna");
    
    return t -> element;
}

template <typename T>
bool JednostrukaLista<T>::sljedeci() {
    if(brojElemenata() == 0)
        throw std::domain_error("Lista je prazna");
    
    if(ti != nullptr) {
        
        tp = t;
        
        t = ti;
        tp -> next = t;
        
        ti = ti -> next;
        
        ++lduzina;
        --dduzina;
        
        return true;
        
    } else 
        return false;
}

template <typename T>
bool JednostrukaLista<T>::prethodni() {
    if(brojElemenata() == 0)
        throw std::domain_error("Lista je prazna");
    
    if(tp != nullptr) {
        if(tp != prvi) {
            Node *pr = prvi;
            
            if(prvi -> next != tp) 
                while(pr -> next != tp)
                    pr = pr -> next;

            ti = t;
                        
            t = tp;
            t -> next = ti;
            
            tp = pr;
        } else {
            ti = t;
            t = tp;
            tp = nullptr;
        }
        
        --lduzina;
        ++dduzina;
        return true;
    } else
        return false;

    // std::std::cout << "Tp: " << tp -> element << " T: " << t -> element << " Ti: " << t -> element;
}

template <typename T>
void JednostrukaLista<T>::kraj() { 
    Node* np = prvi; 
    
    while(np -> next -> next != nullptr) 
        np = np -> next; 
    
    tp = np;
    t = np -> next;
    ti = nullptr;
}

template <typename T>
void JednostrukaLista<T>::obrisi() {
    if(brojElemenata() == 0)
        throw std::domain_error("Lista je prazna");
    
    Node *brisanje = t;
    
    if(ti != nullptr) {
        
        t = ti;
        ti = ti -> next;
        t -> next = ti;
        
        if(tp == nullptr)
            prvi = t;
        else 
            tp -> next = t;
        
        if(t -> next == nullptr)
            zadnji = t;
            
        --dduzina;
        
    } else if(tp != nullptr) {
        
        if(tp != prvi) {
            Node *np = prvi;
            
            while(np -> next != tp)
                np = np -> next;
                
            t = tp;
            t -> next = ti;
            tp = np;
            zadnji = t;
        } else {
            t = prvi;
            t -> next = ti;
            tp = nullptr;
            prvi = t;
            zadnji = t;
        }
        
        --lduzina;
    } else {
        prvi = nullptr;
        zadnji = nullptr;
        t = nullptr;
        tp = nullptr;
        ti = nullptr;
        --lduzina;
    }
    
    delete brisanje;
}

template <typename T>
void JednostrukaLista<T>::dodajIspred(const T& el) { // lijevo
    Node *novi = new Node(el,nullptr);

    if(tp == nullptr && t == nullptr && ti == nullptr) {
        prvi = novi;
        t = prvi;
        
        if(zadnji == nullptr)
            zadnji = novi;
            
    } else if(tp == nullptr && t != nullptr && ti == nullptr) {
        tp = novi;
        novi -> next = t;
        prvi = novi;
    } else if(tp != nullptr && t != nullptr && ti == nullptr) {
        tp -> next = novi;
        novi -> next = t;
        tp = novi;
    }
    
    lduzina++;
}

template <typename T>
void JednostrukaLista<T>::dodajIza(const T& el) { // desno
   Node *novi = new Node(el,nullptr);
    
    if(tp == nullptr && t == nullptr && ti == nullptr) {
        prvi = novi;
        t = prvi;
        
        if(zadnji == nullptr)
            zadnji = novi;
    } else if(tp == nullptr && t != nullptr && ti == nullptr) {
        t -> next = novi;
        ti = novi;
        zadnji = novi;
    } else if(tp == nullptr && t != nullptr && ti != nullptr) {
        t -> next = novi;
        novi -> next = ti;
        ti = novi;
    } else if(tp != nullptr && t != nullptr && ti != nullptr) {
        t -> next = novi;
        novi -> next = t -> next;
        ti = novi;
    } else if(tp != nullptr && t != nullptr && ti == nullptr) {
        t -> next = novi;
        ti = novi;
        zadnji = novi;
    } 

    ++dduzina;
}

template <typename T>
T JednostrukaLista<T>::operator [](int i) const {
    int brojac = 0;
    
    if(i > brojElemenata())
        throw std::domain_error("Neispravan indeks");
    
    Node *trazeni = prvi;
    
    while(brojac != i) {
        trazeni = trazeni -> next;
        brojac++;
    }
    
    return trazeni -> element;
}

template <typename T>
T& JednostrukaLista<T>::operator [](int i) {
    int brojac = 0;
    
    if(i > brojElemenata())
        throw std::domain_error("Neispravan indeks");
    
    Node *trazeni = prvi;
    
    while(brojac != i) {
        trazeni = trazeni -> next;
        brojac++;
    }
    
    return trazeni -> element;
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
    else if(lista.trenutni() != 4)
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

// Testiranje kombinacije metoda dodajIza() i dodajIspred()
template <typename T>
bool TestirajKombinacijeDodavanja(Lista<T> &lista) {
    try{
        
    lista.dodajIza(4);
    lista.dodajIza(1);
    lista.dodajIspred(10);
    lista.dodajIza(11);
    //#region test
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.sljedeci();
    // lista.sljedeci();
    // lista.sljedeci();
    // lista.prethodni();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.prethodni();
    // lista.prethodni();
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.dodajIza(11);
    // lista.prethodni();
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.prethodni();
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.dodajIza(11);
    // lista.dodajIspred(10);
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();

    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();

    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    // lista.obrisi();
    //#endregion
    } catch(std::exception ex) {
        std::cout << "belaj: " << ex.what();
    }
    
    
    for(int i{0};i<lista.brojElemenata();++i)
        std::cout << lista[i] << " ";
    
    return true;
} 

//#endregion

int main() {

    std::cout << std::boolalpha;
    
    // std::cout << "Testovi vracaju true ako su uspjesno zavrseni i a false ako nisu.";
    // std::cout << std::endl;
    // std::cout << std::endl;
    
    // //#region Test Kopirajuceg Konstruktora 
    
    //     //#region NizLista 
    
    //         NizLista<int> nl1;
            
    //         for(int i=0;i<10;++i)
    //             nl1.dodajIza(i+1);
            
    //         NizLista<int> nl1Kopija(nl1);
            
    //         std::cout << "=> NizLista <= - Test kopirajuceg konstruktora: " << (TestirajKopiju(nl1,nl1Kopija) ? "USPJESAN" : "PADA");
        
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl1;
            
    //         for(int i=0;i<10;++i)
    //             jl1.dodajIza(i+1);
            
    //         JednostrukaLista<int> jl1Kopija(jl1);
        
    //         std::cout << "=> JednostrukaLista <= - Test kopirajuceg konstruktora: " << (TestirajKopiju(jl1,jl1Kopija) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
        
    // //#region Test Operatora Dodjele
    
    //     //#region NizLista 
    
    //         NizLista<int> nl2;
            
    //         for(int i=0;i<10;++i)
    //             nl2.dodajIza(i+1);
            
    //         NizLista<int> nl2Kopija = nl2;
            
    //         std::cout << "=> NizLista <= - Test operatora dodjele: " << (TestirajKopiju(nl2,nl2Kopija) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl2;
            
    //         for(int i=0;i<10;++i)
    //             jl2.dodajIza(i+1);
            
    //         JednostrukaLista<int> jl2Kopija = jl2;
        
    //         std::cout << "=> JednostrukaLista <= - Test operatora dodjele: " << (TestirajKopiju(jl2,jl2Kopija) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
     
    // //#region Test Metoda Za Pomjeranje prethodni() i sljedeci() te metode trenutni()
        
    //     //#region NizLista 
    
    //         NizLista<int> nl3;
            
    //         for(int i=0;i<5;++i)
    //             nl3.dodajIza(i+1);
            
    //         NizLista<int> nl3Kopija = nl3;
            
    //         std::cout << "=> NizLista <= - Test metoda za pomjeranje prethodni() i sljedeci() te metode trenutni(): " << (TestirajPomjeranjeITrenutni(nl3) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl3;
            
    //         for(int i=0;i<5;++i)
    //             jl3.dodajIza(i+1);
            
    //         JednostrukaLista<int> jl3Kopija = jl3;
        
    //         std::cout << "=> JednostrukaLista <= - Test metoda za pomjeranje prethodni() i sljedeci() te metode trenutni(): " << (TestirajPomjeranjeITrenutni(jl3) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
    
    // //#region Test Metoda pocetak() i kraj()
        
    //     //#region NizLista 
    
    //         NizLista<int> nl4;
            
    //         for(int i=0;i<5;++i)
    //             nl4.dodajIza(i+1);
            
    //         NizLista<int> nl4Kopija = nl4;
            
    //         std::cout << "=> NizLista <= - Test metoda pocetak() i kraj(): " << (TestirajPocetakIKraj(nl4) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl4;
            
    //         for(int i=0;i<5;++i)
    //             jl4.dodajIza(i+1);
            
    //         JednostrukaLista<int> jl4Kopija = jl4;
        
    //         std::cout << "=> JednostrukaLista <= - Test metoda pocetak() i kraj(): " << (TestirajPocetakIKraj(jl4) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
    
    // //#region Test Metode obrisi()
        
    //     //#region NizLista 
    
    //         NizLista<int> nl5;
            
    //         for(int i=0;i<5;++i)
    //             nl5.dodajIza(i+1);
            
    //         NizLista<int> nl5Kopija = nl5;
            
    //         std::cout << "=> NizLista <= - Test metode obirsi(): " << (TestirajBrisanje(nl5) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl5;
            
    //         for(int i=0;i<5;++i)
    //             jl5.dodajIza(i+1);
            
    //         JednostrukaLista<int> jl5Kopija = jl5;
        
    //         std::cout << "=> JednostrukaLista <= - Test metode obrisi(): " << (TestirajBrisanje(jl5) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
    
    // //#region Test Metode dodajIspred()
        
    //     //#region NizLista 
    
    //         NizLista<int> nl6;
    //         std::cout << "=> NizLista <= - Test metode dodajIspred(): " << (TestirajDodajIspred(nl6) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl6;
    //         std::cout << "=> JednostrukaLista <= - Test metode dodajIspred(): " << (TestirajDodajIspred(jl6) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    // std::cout << std::endl;
    // std::cout << std::endl;
    
    // //#region Test Metode dodajIza()
        
    //     //#region NizLista 
    
    //         NizLista<int> nl7;
    //         std::cout << "=> NizLista <= - Test metode dodajIza(): " << (TestirajDodajIza(nl7) ? "USPJESAN" : "PADA");
    
    //     //#endregion
        
    //     std::cout << std::endl;
        
    //     //#region Jednostruka lista 
    
    //         JednostrukaLista<int> jl7;
    //         std::cout << "=> JednostrukaLista <= - Test metode dodajIza(): " << (TestirajDodajIza(jl7) ? "USPJESAN" : "PADA");
    
    //     //#endregion
    
    // //#endregion
    
    std::cout << std::endl;
    std::cout << std::endl;
    
    //#region Test kombinacije dodajIza() i dodajIspred()
    
        //#region NizLista 
    
            NizLista<int> nl8;
            std::cout << "=> NizLista <= - Test metode dodajIza(): " << (TestirajKombinacijeDodavanja(nl8) ? "USPJESAN" : "PADA");
    
        //#endregion
        

        
    //#endregion
    
    // Testiranje ostalih metoda poput brojElemenata(), operatora[] i slicne su testirane unutar ostalih testova
    
    return 0;
}