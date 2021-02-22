#include <iostream>

#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

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

//#region Zadatak 1

void radixSort(std::vector<int> &a) {
    
    int maxCifara = ceil(log10(*std::max_element(a.begin(), a.end())));
    int djelilac = 1;
    
    Red<int> redovi[10];
  
    for(int i{0};i<maxCifara;++i) {
        for(int j{0};j<a.size();++j) 
            redovi[((int) round(a[j] / djelilac) % 10)].stavi(a[j]);
        
        a.clear();
    
        for(int k{0};k<10;++k) {
            int brEl = redovi[k].brojElemenata();
       
            for(int l{0};l<brEl;++l) 
                a.push_back(redovi[k].skini());
        }
        
        djelilac *= 10;
    }
}

//#endregion

//#region Zadatak 2

bool jeList(std::vector<int> &a, int i,int vel) {
    return i >= (vel / 2) && (i < vel);
}

void zamjeni(std::vector<int> &a,int i,int j) {
    int temp(a[i]);
    a[i] = a[j];
    a[j] = temp;
}

int leftChild(int i) {
    return 2*i+1;    
}

int rightChild(int i) {
    return 2*i+2;
}

int parent(int i) {
    return (i-1) / 2;
}

void popraviDole(std::vector<int> &a, int i, int vel) {
    while(!jeList(a,i,vel)) {
        int veci = leftChild(i);
        int desni = rightChild(i);
        
        if((desni < vel) && a[desni] > a[veci]) 
            veci = desni;
            
        if(a[i] <= a[veci]) {
            zamjeni(a,i,veci);
            i = veci;
        } else
            break;
    }
}

void popraviGore(vector<int> &a, int i) {
   while((i != 0) && a[i] > a[parent(i)]) {
       zamjeni(a,i,parent(i));
       i = parent(i);
   }
}

void stvoriGomilu(std::vector<int> &a) {
    for(int i{(int) (a.size() / 2 - 1)}; i >= 0; --i)
        popraviDole(a,i,a.size());
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &vel) {
    if(vel == a.size())
        a.push_back(0);
    
    a[vel++] = umetnuti;
    popraviGore(a,vel-1);
}

int izbaciPrvi(vector<int> &a, int &vel) {
    if(vel == 0)
        throw std::domain_error("Gomila prazna");
    
    zamjeni(a,0,--vel);
    
    if(vel != 0)
        popraviDole(a,0,vel);
    
    return a[vel];
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int duz = a.size();
    int vel = a.size();

    for(int i = 0; i <= duz - 2; ++i) {
        izbaciPrvi(a,vel);
    }
}

//#endregion

//#region Testovi

bool TestirajRadixSort() {
    std::vector<int> radix = {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625};
    
    radixSort(radix);
    
    int result[12] = {174, 248, 271, 293, 329, 347, 455, 625, 643, 673, 788, 820};
    
    for(int i{0};i<radix.size();++i)
        if(radix[i] != result[i])
            return false;
    
    return true;
}

bool TestirajStavaranjeGomile() {
    std::vector<int> vektor = {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625};
    
    stvoriGomilu(vektor);
    
    int result[12] = {820, 643, 788, 293, 455, 625, 673, 174, 248, 347, 271, 329};
    
    for(int i{0};i<12;++i)
        if(vektor[i] != result[i])
            return false;
    
    return true;
}

bool TestirajUmetanjeUGomilu() {
    std::vector<int> vektor = {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625};
    
    int vel = vektor.size();
    stvoriGomilu(vektor);
    
    umetniUGomilu(vektor,10,vel);  
    umetniUGomilu(vektor,40,vel);   
    umetniUGomilu(vektor,20,vel);   
    umetniUGomilu(vektor,30,vel);   
    umetniUGomilu(vektor,50,vel);     
    
    int result[17] = {820, 643, 788, 293, 455, 625, 673, 174, 248, 347, 271, 329, 10, 40, 20, 30, 50};
    
    for(int i{0};i<vel;++i)
        if(vektor[i] != result[i])
            return false;
    
    return true;
}

bool TestirajIzbacivanjePrvog() {
    std::vector<int> vektor = {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625};
    
    int vel = vektor.size();
    stvoriGomilu(vektor);
    
    izbaciPrvi(vektor,vel); 
    
    if(vektor[0] != 788)
        return false;
    
    
    izbaciPrvi(vektor,vel); 
    
    if(vektor[0] != 673)
        return false;
    
        
    izbaciPrvi(vektor,vel); 
    
    if(vektor[0] != 643)
        return false;
        
        
    izbaciPrvi(vektor,vel); 
    
    if(vektor[0] != 625)
        return false;
    
    int result[8] = {625, 455, 329, 293, 347, 271, 248, 174};
    
    for(int i{0};i<8;++i)
        if(vektor[i] != result[i])
            return false;
    
    return true;
}

bool TestirajGomilaSort() { // Heap sort
     std::vector<int> heap = {248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625};
    
    radixSort(heap);
    
    int result[12] = {174, 248, 271, 293, 329, 347, 455, 625, 643, 673, 788, 820};
    
    for(int i{0};i<heap.size();++i)
        if(heap[i] != result[i])
            return false;
    
    return true;   
}

//#endregion

int main() {
    
    std::cout << std::boolalpha;
    
    std::cout << "Test[1] -> Testiranje Radix Sorta: " << (TestirajRadixSort() ? "USPJESAN" : "PADA") << std::endl;
    
    std::cout << "Test[2] -> Testiranje Stvaranja Gomile: " << (TestirajStavaranjeGomile() ? "USPJESAN" : "PADA") << std::endl;
    
    std::cout << "Test[3] -> Testiranje Umetanja U Gomilu: " << (TestirajUmetanjeUGomilu() ? "USPJESAN" : "PADA") << std::endl;

    std::cout << "Test[4] -> Testiranje Izbacivanja Prvog: " << (TestirajIzbacivanjePrvog() ? "USPJESAN" : "PADA") << std::endl;
    
    std::cout << "Test[5] -> Testiranje Gomila (Heap) Sorta: " << (TestirajGomilaSort() ? "USPJESAN" : "PADA") << std::endl;
    
    return 0;
}
