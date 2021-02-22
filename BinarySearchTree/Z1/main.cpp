#include <iostream>

#include <time.h>
#include <vector>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

//region Virtual Mapa

template <typename TK,typename TV>
class Mapa {

public:
    Mapa(){}
    virtual ~Mapa() {};
    virtual TV operator[](const TK& kljuc) const = 0;
    virtual TV& operator[](const TK& kljuc) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi(const TK& kljuc) = 0;
    virtual void obrisi() = 0;
};

//endregion

//region Binary Search Tree

//region Binary Search Node

template <typename TK, typename TV>
class BSNode {
public:
    TK key;
    TV value;
    BSNode *left,*right,*parent;        
    
    BSNode() { left = right = nullptr; }
    BSNode(const TK& k, const TV& v, BSNode* p = nullptr, BSNode* l  = nullptr, BSNode* r = nullptr) {
        key=k;
        value=v;
        parent=p;
        left=l;
        right=r;
    }
};

//endregion

//region Definicija klase

template <typename TK, typename TV>
class BinStabloMapa : public Mapa<TK,TV> {
    BSNode<TK,TV>* root;
    int duzina;
    void umetni(BSNode<TK,TV>* &trenutni,  BSNode<TK,TV>* z, BSNode<TK,TV>* rod = nullptr);
    BSNode<TK,TV>* pronadji(BSNode<TK,TV>* trenutni, const TK& kljuc) const;
    void brisi(BSNode<TK,TV> *&r, const TK& kljuc);
    void brisiSve(BSNode<TK,TV> *r);
    BSNode<TK,TV>* nextMinNode(BSNode<TK,TV>* pocetak);
    BSNode<TK,TV>* kloniraj(BSNode<TK,TV>* trenutna) const;
public:
    BinStabloMapa() { root = nullptr; duzina = 0; }
    BinStabloMapa(const BinStabloMapa &nova);
    BinStabloMapa operator=(const BinStabloMapa &nova);
    ~BinStabloMapa() { brisiSve(root); }
    TV operator[] (const TK& kljuc) const;
    TV& operator[] (const TK& kljuc);
    int brojElemenata() const { return duzina; }
    void obrisi(const TK& kljuc);
    void obrisi();
};

//endregion

//region Implementacija metoda

template <typename TK, typename TV>
BSNode<TK,TV>* BinStabloMapa<TK,TV>::kloniraj(BSNode<TK,TV>* trenutna) const {
  if (trenutna) {
    BSNode<TK,TV>* nova = new BSNode<TK,TV>(trenutna -> key,trenutna -> value);
    
    nova -> left = kloniraj(trenutna -> left);
    nova -> right = kloniraj(trenutna -> right);
    
    return nova;
  }
  
  return nullptr;
}

template <typename TK, typename TV>
BinStabloMapa<TK,TV>::BinStabloMapa(const BinStabloMapa &nova) {
    if(&nova != this)  {
        root = kloniraj(nova.root);
        duzina = nova.duzina;
    }
}

template <typename TK, typename TV>
BinStabloMapa<TK,TV> BinStabloMapa<TK,TV>::operator=(const BinStabloMapa &nova) {
    if(&nova != this)  {
        delete root;
        root = kloniraj(nova.root);
        duzina = nova.duzina;
    }
    
    return *this;
}
    
template <typename TK, typename TV>
void BinStabloMapa<TK,TV>::umetni(BSNode<TK,TV>* &trenutni, BSNode<TK,TV>* z, BSNode<TK,TV>* rod) {
    BSNode<TK,TV>* p = trenutni;
    
    if(trenutni == nullptr) {
        z -> parent = rod;
        trenutni = z;
    } else {
        if(z -> key < p -> key)
            umetni(p -> left, z, p);
        else
            umetni(p -> right, z, p);
    }
}

template <typename TK, typename TV>
BSNode<TK,TV>* BinStabloMapa<TK,TV>::pronadji(BSNode<TK,TV>* trenutni, const TK& kljuc) const {
    if(trenutni == nullptr || kljuc == trenutni -> key)
        return trenutni;
    
    if(kljuc < trenutni -> key)
        return pronadji(trenutni -> left, kljuc);
    else
        return pronadji(trenutni -> right, kljuc);
}

template <typename TK, typename TV>
TV BinStabloMapa<TK,TV>::operator[] (const TK& kljuc) const {
    BSNode<TK,TV>* trazeni = pronadji(root,kljuc);
  
    if(trazeni == nullptr)
        return TV();
    
    return trazeni -> value;
}

template <typename TK, typename TV>
TV& BinStabloMapa<TK,TV>::operator[] (const TK& kljuc) {
    BSNode<TK,TV>* trazeni = pronadji(root,kljuc);
    
    if(trazeni == nullptr) {
        BSNode<TK,TV>* novi = new BSNode<TK,TV>(kljuc,TV());
        umetni(root,novi);
        
        ++duzina;
        
        if(duzina == 1) root = novi;
        
        return novi -> value;
    }
    
    return trazeni -> value;
}

template <typename TK, typename TV>
BSNode<TK,TV>* BinStabloMapa<TK,TV>::nextMinNode(BSNode<TK,TV>* pocetak) {
  
    while(pocetak -> left != nullptr) 
        pocetak = pocetak -> left;

    return pocetak;
}

template <typename TK, typename TV>
void BinStabloMapa<TK,TV>::brisi(BSNode<TK,TV>* &trenutni, const TK& kljuc) {
    BSNode<TK,TV>* trazeni = pronadji(trenutni,kljuc);
 
    if(trazeni == nullptr)
        return;
       
    if(trazeni -> left == nullptr && trazeni -> right == nullptr) {
        if(trazeni != root) {
            if(trazeni -> parent -> left == trazeni)
                trazeni -> parent -> left = nullptr;
            else
                trazeni -> parent -> right = nullptr;
        }
            
        delete trazeni;
        
    } else if(trazeni -> left && trazeni -> right) {
        BSNode<TK,TV>* naredni = nextMinNode(trazeni -> right);
        
        TK kljuc = naredni -> key;
        TV vrijednost = naredni -> value;
        BSNode<TK,TV>* rod = naredni -> parent;
        
        brisi(root, naredni -> key);
        
        trazeni -> key = kljuc;
        trazeni -> value = vrijednost;
        trazeni -> parent = rod;
    } else if(trazeni -> left != nullptr && trazeni -> right == nullptr) {
        trazeni -> left -> parent = trazeni -> parent;

        if(trazeni != root) 
            trazeni -> parent -> left = trazeni -> left;
        else
            root = trazeni -> left;
        
        delete trazeni;
        
    } else if(trazeni -> left == nullptr && trazeni -> right != nullptr) {
        
        trazeni -> right -> parent = trazeni -> parent;
            
        if(trazeni != root)
            trazeni -> parent -> right = trazeni -> right;
        else
            root = trazeni -> right;
            
        delete trazeni;
    }
    
}

template <typename TK, typename TV>
void BinStabloMapa<TK,TV>::obrisi(const TK& kljuc) {
   brisi(root,kljuc);
    --duzina;
}

template <typename TK, typename TV>
void BinStabloMapa<TK,TV>::brisiSve(BSNode<TK,TV> *r) {
    if (r) {
        brisiSve(r->left);
        brisiSve(r->right);
        delete r;
    } 
    
}

template <typename TK, typename TV>
void BinStabloMapa<TK,TV>::obrisi() {
    brisiSve(root);
    duzina = 0;
    root = nullptr;
}

//endregion

//endregion

//region NizMapa

//region Definicija klase

template<typename TK, typename TV>
class NizMapa : public Mapa<TK,TV> {
    int kapacitet;
    int duzina;
    TK* kljucevi;
    TV* vrijednosti;

public:
    NizMapa():kapacitet(50),duzina(0),kljucevi(new TK[kapacitet]{}),vrijednosti(new TV[kapacitet]{}) {}
    NizMapa(const NizMapa& nova);
    NizMapa operator=(const NizMapa& nova);
    ~NizMapa() { delete[] kljucevi; delete[] vrijednosti; }
    void ProsiriKapacitet(int duzinaNiza);
    TV operator[](const TK& kljuc) const;
    TV& operator[](const TK& kljuc);
    int brojElemenata() const { return duzina; }
    void obrisi();
    void obrisi(const TK& kljuc);
};

//endregion

//region Implementacija metoda

template <typename TK, typename TV>
NizMapa<TK,TV>::NizMapa(const NizMapa<TK,TV>& nova) {
    
    if(&nova != this) {
        
        kljucevi = new TK[nova.kapacitet]{};
        vrijednosti = new TV[nova.kapacitet]{};
      
        for(int i{0};i<nova.duzina;++i) {
            kljucevi[i] = nova.kljucevi[i];
            vrijednosti[i] = nova.vrijednosti[i];
        }
        
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
    }
}

template <typename TK, typename TV>
NizMapa<TK,TV> NizMapa<TK,TV>::operator=(const NizMapa<TK,TV>& nova) {
 
    if(&nova != this) {
        delete[] kljucevi;
        delete[] vrijednosti;
        
        kljucevi = new TK[nova.kapacitet]{};
        vrijednosti = new TV[nova.kapacitet]{};
      
        for(int i{0};i<nova.duzina;++i) {
            kljucevi[i] = nova.kljucevi[i];
            vrijednosti[i] = nova.vrijednosti[i];
        }
        
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
        
    }
    
    return *this;
}

template <typename TK, typename TV>
void NizMapa<TK,TV>::ProsiriKapacitet(int noviKapacitet) {
     TK* noviKljucevi(new TK[noviKapacitet]{});
     TV* noveVrijednosti(new TV[noviKapacitet]{});
    
    for(int i=0;i<duzina;++i)
        noviKljucevi[i] = kljucevi[i];
        
    for(int i=0;i<duzina;++i)
        noveVrijednosti[i] = vrijednosti[i];
    
    delete[] kljucevi;
    delete[] vrijednosti;
    
    kljucevi = noviKljucevi;
    vrijednosti = noveVrijednosti;
    kapacitet = noviKapacitet;
}

template <typename TK, typename TV>
TV NizMapa<TK,TV>::operator[](const TK& kljuc) const {
    int index = -1;
    
    for(int i{0};i<duzina;++i) {
        if(kljucevi[i] == kljuc) {
            index = i;
            break;
        }
    }
    
    if(index == -1)
        return TV();
    
    return vrijednosti[index];
}

template <typename TK, typename TV>
TV& NizMapa<TK,TV>::operator[](const TK& kljuc) {
    int index = -1;
    
    for(int i{0};i<duzina;++i) {
        if(kljucevi[i] == kljuc) {
            index = i;
            break;
        }
    }
    
    if(index == -1) {
        
        if(duzina >= kapacitet)
            this -> ProsiriKapacitet(duzina+kapacitet);
        
        kljucevi[duzina] = kljuc;
        return vrijednosti[duzina++];
    }
    
    return vrijednosti[index];
}

template <typename TK,typename TV>
void NizMapa<TK,TV>::obrisi() {
    delete[] kljucevi;
    delete[] vrijednosti;
    
    kljucevi = new TK[kapacitet]{};
    vrijednosti = new TV[kapacitet]{};
    duzina = 0;
}

template <typename TK,typename TV>
void NizMapa<TK,TV>::obrisi(const TK& kljuc) {
    int index = -1;
    
    for(int i{0};i<duzina;++i)
        if(kljucevi[i] == kljuc) {
            index = i;
            break;
        }
    
    if(index == -1)
        throw std::range_error("Kljuc ne postoji.");
    
    for(int i{index};i<duzina;++i) {
        kljucevi[i] = kljucevi[i+1];
        vrijednosti[i] = vrijednosti[i+1];
    }
    
    --duzina;
}

//endregion

//endregion

//region MetodaPoredjenja

void potrosiVrijeme(int vrijednost) {}

void uporediImplementacijeMapa() {
    // Generisemo 2 mape, zasada prazne tipa int i int
    BinStabloMapa<int,int> bsm;
    NizMapa<int,int> nm;
    
    
    // Dodajemo po 5000 (zbog C9 limitacija za vrijeme kompajliranja, radio bi bilo koji proizvoljan broj) elemenata u jednu i u drugu, te mjerimo vrijeme pristupa
    
    // Prvo dodajemo u drvo
    
    time_t beginBSTM,endBSTM;
    double vrijemeBSTMWrite,vrijemeBSTMRead;
    
    // Mjerimo vrijeme pisanja
    
    beginBSTM = clock();
    
    for(int i{0};i<5000;++i)
        bsm[i] = i;
    
    endBSTM = clock();
    
    vrijemeBSTMWrite = (endBSTM - beginBSTM) / (CLOCKS_PER_SEC / 1000);
    
    // Mjerimo vrijeme citanja
    
    beginBSTM = clock();
    
    for(int i{0};i<5000;++i)
        potrosiVrijeme(bsm[i]);
    
    endBSTM = clock();
    
    vrijemeBSTMRead = (endBSTM - beginBSTM) / (CLOCKS_PER_SEC / 1000);

    // Zatim racunamo za nizmapu
    
    // Prvo dodajemo u drvo
    
    time_t beginNM,endNM;
    double vrijemeNMWrite,vrijemeNMRead;
    
    // Mjerimo vrijeme pisanja
    
    beginNM = clock();
    
    for(int i{0};i<5000;++i)
        nm[i] = i;
    
    endNM = clock();
    
    vrijemeNMWrite = (endNM - beginNM) / (CLOCKS_PER_SEC / 1000);
    
    // Mjerimo vrijeme citanja
    
    beginNM = clock();
    
    for(int i{0};i<5000;++i)
        potrosiVrijeme(bsm[i]);
    
    endNM = clock();
    
    vrijemeNMRead = (endNM - beginNM) / (CLOCKS_PER_SEC / 1000);
    
    // Ispis rezultata za pisanje
    
    double razlika;
    
    std::cout << std::setprecision(2);
    
    std::cout << "Vrijeme pisanja 5000 elemenata je brže za ";
    if(vrijemeBSTMWrite > vrijemeNMWrite) {
        std::cout << "Binary Search Tree Mapu";
        razlika = vrijemeBSTMWrite - vrijemeNMWrite;  
    } else {
        std::cout << "Niz mapu";
        razlika = vrijemeNMWrite - vrijemeBSTMWrite;
    }
    
    std::cout << std::endl;
    
    std::cout << "Vrijeme Binary Search Tree Map (u ms): " << vrijemeBSTMWrite << std::endl;
    std::cout << "Vrijeme Niz Mape (u ms): " << vrijemeNMWrite << std::endl;
    std::cout << "Razlika (u ms): " << razlika << std::endl;
    
    std::cout << std::endl;
    
    // Ispis rezultata za čitanje
    
    std::cout << "Vrijeme čitanja 5000 elemenata je brže za ";
    if(vrijemeBSTMRead > vrijemeNMRead) {
        std::cout << "Binary Search Tree Mapu";
        razlika = vrijemeBSTMRead - vrijemeNMRead;  
    } else {
        std::cout << "Niz mapu";
        razlika = vrijemeNMRead - vrijemeBSTMRead;
    }
    
    std::cout << std::endl;
    
    std::cout << "Vrijeme Binary Search Tree Map (u ms): " << vrijemeBSTMRead << std::endl;
    std::cout << "Vrijeme Niz Mape (u ms): " << vrijemeNMRead << std::endl;
    std::cout << "Razlika (u ms): " << razlika << std::endl;
}

// P.S. pošto se radi o manjem broju podataka (koji nije mogao biti povećan zbog ograničenja c9 kompajlera) ovaj test može dati iste rezultate
// za čitanje kod nizmape i binarnog stabla pretraživanja dok bi teoretski vrijeme pristupa na eksponencijalno većem broju elemenata
// trebao biti efikasniji nego u stablu
// vrijeme pisanje je u najviše slučajeva bolje za binarno stablo

//endregion

// Nakon kompajliranja i izvrsavanja mozemo primjetiti da je u oba slučaja brži binary tree
// Razlog tome je što binary tree za upis ali i za ispis može u svakom koraku preskočiti provjeru za više elemenata odjednom
// dok niz mapa u jednom koraku provjerava tačno jedan elemenat
// u slučaju binarnog stabla do traženog rezultata se može doći u samo nekoliko koraka bez obzira na poredak ili na vrijednost samog ključa
// dok se u niz mapi prisutpa istim redosljedom po kojim je dolazilo tako da elementi koji zadnji uđu zadnji se i provjere
// za male kapacitete možda bi ove dvije strukture imale slično vrijeme pristupa ali za veće strukture je prikladnije binarsno stablo
// takođe binarno stablo zauzima tačno onliko prostora koliko je potrebno (uz mali trošak pokazivača na roditelja) dok niz mapa mora
// zauzeti puno više memorije čak i ako je ne koristi

int main() {
    uporediImplementacijeMapa();
    return 0;
}
