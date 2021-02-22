#include <iostream>

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//region Mapa

template <typename TK,typename TV>
class Mapa {

public:
    Mapa(){}
    virtual ~Mapa() {};
    virtual TV operator[](const TK& kljuc) const = 0;
    virtual TV& operator[](const TK& kljuc) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TK& kljuc) = 0;
};

//endregion

//region HashMapa


template<typename TK, typename TV>
class HashMapa : public Mapa<TK,TV> {
    int kapacitet;
    unsigned int duzina;
    TK* kljucevi;
    TV* vrijednosti;
    unsigned int(*hashFunkcija)(const TK ulaz,unsigned int max);
    
public:
    HashMapa():kapacitet(50),duzina(0),kljucevi(new TK[kapacitet]{}),vrijednosti(new TV[kapacitet]{}),hashFunkcija(nullptr) {}
    HashMapa(const HashMapa& nova);
    HashMapa operator=(const HashMapa& nova);
    ~HashMapa() { delete[] kljucevi; delete[] vrijednosti; }
    void ProsiriKapacitet(int duzinaNiza);
    TV Trazi(const TK& kljuc) const;
    TV operator[](const TK& kljuc) const;
    TV& operator[](const TK& kljuc);
    int brojElemenata() const { return duzina; }
    void obrisi();
    void obrisi(const TK& kljuc);
    void definisiHashFunkciju(unsigned int(*f)(const TK ulaz,unsigned int max))  {
        hashFunkcija = f;
    };
};

//endregion

//region Metode HashMapa

template <typename TK, typename TV>
HashMapa<TK,TV>::HashMapa(const HashMapa<TK,TV>& nova) {
    
    if(&nova != this) {
        
        kljucevi = new TK[nova.kapacitet]{};
        vrijednosti = new TV[nova.kapacitet]{};
      
        for(int i{0};i<nova.kapacitet;++i) {
            if(nova.kljucevi[i] != TK()) {
                kljucevi[i] = nova.kljucevi[i];
                vrijednosti[i] = nova.vrijednosti[i];
            }
        }
        
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
        hashFunkcija = nova.hashFunkcija;
    }
}

template <typename TK, typename TV>
HashMapa<TK,TV> HashMapa<TK,TV>::operator=(const HashMapa<TK,TV>& nova) {
 
    if(&nova != this) {
        delete[] kljucevi;
        delete[] vrijednosti;
        
        kljucevi = new TK[nova.kapacitet]{};
        vrijednosti = new TV[nova.kapacitet]{};
      
        for(int i{0};i<nova.kapacitet;++i) {
            if(nova.kljucevi[i] != TK()) {
                kljucevi[i] = nova.kljucevi[i];
                vrijednosti[i] = nova.vrijednosti[i];
            }
        }
        
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
        hashFunkcija = nova.hashFunkcija;
        
    }
    
    return *this;
}

template <typename TK, typename TV>
void HashMapa<TK,TV>::ProsiriKapacitet(int noviKapacitet) {
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
TV HashMapa<TK,TV>::operator[](const TK& kljuc) const {
    
    if(hashFunkcija == nullptr)
        throw std::domain_error("Hash Funckija nije definisana");
        
    int index = hashFunkcija(kljuc,kapacitet);

    if(vrijednosti[index] == TV())
        return TV();
    
    return vrijednosti[index];
}

template <typename TK, typename TV>
TV HashMapa<TK,TV>::Trazi(const TK& kljuc) const {
    int index,i(1);
 
    do {
        index = hashFunkcija(kljuc, i);
        
        if (kljucevi[index] == kljuc)
            return vrijednosti[index];
    else
        i += 1;
    
        
    } while (kljucevi[index] != TK() && i != duzina);
     
     return TV();
} 

template <typename TK, typename TV>
TV& HashMapa<TK,TV>::operator[](const TK& kljuc) {
    if(hashFunkcija == nullptr)
        throw std::domain_error("Hash Funckija nije definisana");
        
    int index = hashFunkcija(kljuc,kapacitet);
  
    if(kljucevi[index] == TK()) {
        
        if(duzina >= kapacitet)
            this -> ProsiriKapacitet(duzina+kapacitet);
        
        kljucevi[index] = kljuc;
        ++duzina;
        return vrijednosti[index];
    } else {
        
        TV trazeni = Trazi(kljuc);
        
        if(trazeni != TV())
            return vrijednosti[trazeni];
        
        int i=1;
        
        do {
            index = hashFunkcija(kljuc,i);
            
            if(kljucevi[index] == TK()) {
                kljucevi[index] = kljuc;
                ++duzina;
                return vrijednosti[index];
            } else 
                i+=1;
        } while(i != duzina);
        
    }
    
    return vrijednosti[index];
}

template <typename TK,typename TV>
void HashMapa<TK,TV>::obrisi() {
    delete[] kljucevi;
    delete[] vrijednosti;
    
    kljucevi = new TK[kapacitet]{};
    vrijednosti = new TV[kapacitet]{};
    duzina = 0;
}

template <typename TK,typename TV>
void HashMapa<TK,TV>::obrisi(const TK& kljuc) {
    int index = hashFunkcija(kljuc,kapacitet);
 
    if(vrijednosti[index] == TV())
        throw std::range_error("Kljuc ne postoji.");

    kljucevi[index] = TK();
    vrijednosti[index] = TV();

    duzina--;
}

//#endregion

unsigned int mojhash(int ulaz, unsigned int max) {
	unsigned int suma=23; 
	suma = suma*31 + ulaz;
	return suma % max;
}

int main() {
    
    // Const korektnost i test rada sa tipom int
    HashMapa <int, int> m;
    m.definisiHashFunkciju(mojhash);
    m[100] = 200;
    const HashMapa <int, int>& m2(m);
    m[200] = 300;
    cout << m2.brojElemenata() << " "; // 2
    cout << m2[200] << " "; // 300
    
    // Ovdje se sigurno nece kreirati objekat m2[300] jer je m2 const
    cout << m2[300] << " "; // 0
    cout << m2.brojElemenata() << " "; // 2
    m[300] = 400;
    cout << m2[300] << " "; // 400
    cout << m2.brojElemenata(); // 3
  
    return 0;
}
