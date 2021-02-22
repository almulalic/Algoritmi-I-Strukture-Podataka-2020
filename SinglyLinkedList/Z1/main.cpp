#include <iostream>

#include <cmath>
#include <vector>
#include <stdexcept>

using namespace std;

#define FLOAT_EPSILON 0.000001
#define DOUBLE_EPSILON 0.00000000001

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

//region NizMapa

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

//region Metode NizMapa

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

//region Testovi

bool uporediFloat(float prvi,float drugi) {
    return fabs(prvi-drugi) < FLOAT_EPSILON;
}

bool uporediDouble(double prvi,double drugi) {
    return fabs(prvi-drugi) < DOUBLE_EPSILON;
}

// Test[1] -> Kljuc: string, Vrijednost: int
bool TestirajMapuStrignInt(NizMapa<std::string,int>& mapa) {
    mapa["Sarajevo"] = 12000;
    mapa["Zenica"] = 14000;
    mapa["Banja Luka"] = 10000;
    
    if(mapa["Sarajevo"] != 12000 || mapa["Zenica"] != 14000 || mapa["Banja Luka"] != 10000 || mapa.brojElemenata() != 3)
        return false;
    
    mapa.obrisi();
    
    mapa["Sarajevo"] = 14000;
    mapa["Zenica"] = 10000;
    mapa["Banja Luka"] = 12000;
    
    if(mapa["Sarajevo"] != 14000 || mapa["Sarajevo"] == 12000 || mapa["Zenica"] != 10000 || mapa["Zenica"] == 14000 
        || mapa["Banja Luka"] != 12000 || mapa["Banja Luka"] == 10000 || mapa.brojElemenata() != 3 ||
        mapa["Kirgistan"] != int())
        return false;
        
    return true;
}

// Test[2] -> Kljuc: int, vrijednost: float
bool TestirajMapuIntFloat(NizMapa<int,float> mapa) {
    mapa[1] = (float) 12.1;
    mapa[2] = (float) 18.4;
    mapa[3] = (float) 14.3;
    
    if(!uporediFloat(mapa[1],12.1) || !uporediFloat(mapa[2],18.4) || !uporediFloat(mapa[3],14.3) || mapa.brojElemenata() != 3)
        return true;
    
    mapa.obrisi();
    
    mapa[1] = (float) 18.4;
    mapa[2] = (float) 14.3;
    mapa[3] = (float) 12.1;
    
    if(!uporediFloat(mapa[1],18.4) || uporediFloat(mapa[1],12.1) || !uporediFloat(mapa[2],14.3) || uporediFloat(mapa[2],18.4) 
        || !uporediFloat(mapa[3],12.1) || uporediFloat(mapa[3],14.3) || mapa.brojElemenata() != 3 ||
        !uporediFloat(mapa[123],float()))
        return false;
        
    return true;
}

// Test[3] -> Kljuc: string, vrijednost: bool
bool TestirajMapuStringBool(NizMapa<std::string,bool> mapa) {
    mapa["jeAktivan"] = true;
    mapa["jeRegistrovan"] = false;
    
    if(mapa["jeAktivan"] != true || mapa["jeRegistrovan"] != false || mapa.brojElemenata() != 2)   
        return false;
    
    mapa.obrisi();
    
    mapa["jeAktivan"] = false;
    mapa["jeRegistrovan"] = true;
    
    if(mapa["jeAktivan"] != false || mapa["jeRegistrovan"] != true || mapa.brojElemenata() != 2)   
        return false;
        
    return true; 
}

// Test[4] -> Kljuc: float, vrijednost: vector<double>
bool TestirajMapuFloatVectorDouble(NizMapa<float,std::vector<double>> mapa) {
    std::vector<double> ocekivaniPrvi = {3.123232523545,3.134157345252,3.15152324624614,3.1254125215215};
    std::vector<double> ocekivaniDrugi = {8.125215215,8.23124214,8.4643574534,8.426525325,8.231231234124};
    
    mapa[3.14] = ocekivaniPrvi;
    mapa[8.00] = ocekivaniDrugi;
    
    for(int j{0};j<4;++j)
        if(!uporediDouble(mapa[3.14][j], ocekivaniPrvi[j]))
            return false;
            
    for(int j{0};j<4;++j)
        if(!uporediDouble(mapa[8.00][j], ocekivaniDrugi[j]))
            return false;
  
    if(mapa.brojElemenata() != 2)
        return false;
        
    mapa[3.14] = ocekivaniDrugi;
    mapa[8.00] = ocekivaniPrvi;
    
    for(int j{0};j<4;++j)
        if(!uporediDouble(mapa[3.14][j], ocekivaniDrugi[j]))
            return false;
            
    for(int j{0};j<4;++j)
        if(!uporediDouble(mapa[8.00][j], ocekivaniPrvi[j]))
            return false;
    
    if(mapa.brojElemenata() != 2)
        return false;
    
    return true;
}

// Test[5] -> Kljuc: int, vrijednost: vector<int>
bool TestirajMapuIntVectorInt(NizMapa<int,std::vector<int>> mapa) {
    std::vector<int> ocekivaniPrvi = {1,2,3,4,5,6,7};
    std::vector<int> ocekivaniDrugi = {8,9,10,11,12,13,14,15};
    std::vector<int> ocekivaniTreci = {16,17,18,19,20,21,22,23};

    mapa[1] = ocekivaniPrvi;
    mapa[2] = ocekivaniDrugi;
    mapa[3] = ocekivaniTreci;
    
    for(int j{0};j<4;++j) 
        if(mapa[1][j] != ocekivaniPrvi[j])
            return false;
    
    for(int j{0};j<4;++j) 
        if(mapa[2][j] != ocekivaniDrugi[j])
            return false;
            
    for(int j{0};j<4;++j) 
        if(mapa[3][j] != ocekivaniTreci[j])
            return false;
    
    if(mapa.brojElemenata() != 3)
        return false;
        
    mapa[1] = ocekivaniDrugi;
    mapa[2] = ocekivaniTreci;
    mapa[3] = ocekivaniPrvi;
    
    for(int j{0};j<4;++j) 
        if(mapa[1][j] != ocekivaniDrugi[j])
            return false;
    
    for(int j{0};j<4;++j) 
        if(mapa[2][j] != ocekivaniTreci[j])
            return false;
            
    for(int j{0};j<4;++j) 
        if(mapa[3][j] != ocekivaniPrvi[j])
            return false;
    
    return true;
}

//endregion

int main() {
 
    std::cout << std::boolalpha;
    
    NizMapa<std::string,int> mapa1;
    std::cout << "Test[1] -> Kljuc: string, Vrijednost: int: " << (TestirajMapuStrignInt(mapa1) ? "USPJESAN" : "PADA") << std::endl;
    
    NizMapa<int,float> mapa2;
    std::cout << "Test[2] -> Kljuc: int, vrijednost: float: " << (TestirajMapuIntFloat(mapa2) ? "USPJESAN" : "PADA") << std::endl;
    
    NizMapa<std::string,bool> mapa3;
    std::cout << "Test[3] -> Kljuc: string, vrijednost: bool: " << (TestirajMapuStringBool(mapa3) ? "USPJESAN" : "PADA") << std::endl;

    NizMapa<float,std::vector<double>> mapa4;
    std::cout << "Test[4] -> Kljuc: float, vrijednost: vector<double>: " << (TestirajMapuFloatVectorDouble(mapa4) ? "USPJESAN" : "PADA") << std::endl;
    
    NizMapa<int,std::vector<int>> mapa5;
    std::cout << "Test[5] -> Kljuc: int, vrijednost: vector<int>: " << (TestirajMapuIntVectorInt(mapa5) ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
