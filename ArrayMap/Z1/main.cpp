#include <iostream>

#include <vector>
#include <stdexcept>

using namespace std;

//region Mapa

template <typename TK,typename TV>
class Mapa {

public:
    Mapa(){}
    virtual TV operator[](const TK& kljuc) const;
    virtual TV operator[](const TK& kljuc);
    virtual int brojElemenata();
    virtual void obrisi();
    virtual void obrisi(const TK& kljuc);
};

//endregion

//region NizLista

template<typename TK, typename TV>
class NizMapa {
    int kapacitet;
    int duzina;
    TK** kljucevi;
    TV** vrijednosti;

public:
    NizMapa():kapacitet(50),duzina(0),kljucevi(new TK*{}),vrijednosti(new TV*[kapacitet]{}) {}
    void ProsiriDuzinu(int duzinaNiza);
    TV operator[](const TK& kljuc) const ;
    TV operator[](const TK& kljuc) ;
    int brojElemenata() { return duzina; }
    void obrisi() ;
    void obrisi(const TK& kljuc) ;
};

//endregion

//region Metode NizMapa

template <typename TK, typename TV>
void NizMapa<TK,TV>::ProsiriDuzinu(int noviKapacitet) {
     TK** noviKljucevi(new TK*[noviKapacitet]{});
     TV** noveVrijednosti(new TV*[noviKapacitet]{});
        
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
    
    for(int i(0);i<kapacitet;++i) 
        if(*kljucevi[i] == kljuc) {
            index = i;
            break;
        }
    
    if(index == -1)
        throw std::domain_error("Kljuc nije pronadjen");
    
    return *vrijednosti[index];
}

template <typename TK, typename TV>
TV NizMapa<TK,TV>::operator[](const TK& kljuc) {
    int index = -1;
    
    kljucevi[duzina] = new int
    
    if(index == -1)
        throw std::domain_error("Kljuc nije pronadjen");
    
    return *vrijednosti[index];
}

template <typename TK,typename TV>
void NizMapa<TK,TV>::obrisi() {
    delete[] kljucevi;
    delete[] vrijednosti;
    
    kljucevi = new TK*[kapacitet]{};
    vrijednosti = new TV*[kapacitet]{};
}

template <typename TK,typename TV>
void NizMapa<TK,TV>::obrisi(const TK& kljuc) {
    int index = -1;
    
    for(int i{0};i<duzina;++i)
        if(*kljucevi[i] == kljuc) {
            index = 1;
            break;
        }
    
    if(index == -1)
        throw std::range_error("Kljuc ne postoji.");
    
    for(int i{0};i<duzina;++i) {
        *kljucevi[i] = *kljucevi[i+1];
        *vrijednosti[i] = *vrijednosti[i+1];
    }
}

//endregion

int main() {
    
    // Osnovni test funkcionalnosti NizMape
    NizMapa<std::string,std::string> m;
    
    m["Sarajevo"] = "BiH";
    m["Zagreb"] = "Hrvatska";
    
    cout << m.brojElemenata() << " " << m["Zagreb"] << " "; 
    
    // 2 Hrvatska
    // Test praznih elemenata
    
    // cout << m["Beograd"] << " "; 
    // cout << m["zagreb"] << " "; 
    
    // m.obrisi("Zagreb");
    
    // Potpuno korektna NizMapa nece kreirati objekte "Beograd" i "zagreb"
    // jer oni nemaju vrijednosti pa ce se ispod ispisati 1
    // Ali to zahtijeva implementaciju "Pristupnika" pa cemo priznavati
    // rjesenje koje ispise broj 3
    
    // cout << m.brojElemenata(); 
    // 3 ili 1
    // Test brisanja
    // cout << " " << m["Zagreb"] << " "; 
    
    // m.obrisi();
    
    // cout << m.brojElemenata();
    // cout << " " << m["Sarajevo"] << " ";
    
    return 0;
}
