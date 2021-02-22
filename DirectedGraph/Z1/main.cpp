#include <iostream>

class Grana {
    
}

class Cvor {
    
}

template <typename TipOznake>
class UsmjereniGraf {
    int brojCvorova;
    int brojGrana;
    int **M;
    TipOznake *oznake;
    
public:
    UsmjereniGraf(int brojCvorova) : brojCvorova(brojCvorova), M(new int*[brojCvorova]{}), oznake(new TipOznake[brojCvorova]{}) {}
    ~UsmjereniGraf() {}
    int dajBrojCvorova() const { return brojCvorova; }
    void postaviBrojCvorova(int noviBroj) { if(noviBroj < brojCvorova) throw std::domain_error("Novi broj mora biti veći"); brojCvorova = noviBroj; }
    void dodajGranu(int polazniCvor,int dolazniCvor,float tezinaGrane = 0);
    void obrisiGranu(int polazniCvor,int dolazniCvor);
    int dajTezinuGrane(int polazniCvor,int dolazniCvor);
    bool postojiGrana(int polazniCvor, int dolazniCvor);
    void postaviOznakuCvora(int brojCvora,TipOznake oznaka);
};

template <typename TipOznake>
void UsmjereniGraf<TipOznake>::dodajGranu(int polazniCvor,int dolazniCvor,float tezinaGrane=0) {
    if(polazniCvor < 0 || polazniCvor >= brojGrana || dolazniCvor < 0 || dolazniCvor >= brojGrana)
        throw std::domain_error("Cvor nije validan");
    
    if(M[polazniCvor][dolazniCvor] != 0)
        brojGrana++;
    
    M[polazniCvor][dolazniCvor] = tezinaGrane;
}

template <typename TipOznake>
void UsmjereniGraf<TipOznake>::obrisiGranu(int polazniCvor,int dolazniCvor) {
    if(polazniCvor < 0 || polazniCvor >= brojGrana || dolazniCvor < 0 || dolazniCvor >= brojGrana)
        throw std::domain_error("Cvor nije validan");
    
    if(M[polazniCvor][dolazniCvor] != 0)
        brojGrana--;
    
    M[polazniCvor][dolazniCvor] = 0;
}

template <typename TipOznake>
void UsmjereniGraf<TipOznake>::postaviTezinuGranu(int polazniCvor,int dolazniCvor,float tezinaGrane) {
    if(polazniCvor < 0 || polazniCvor >= brojGrana || dolazniCvor < 0 || dolazniCvor >= brojGrana)
        throw std::domain_error("Cvor nije validan");

    M[polazniCvor][dolazniCvor] = tezinaGrane;
}

template <typename TipOznake>
int UsmjereniGraf<TipOznake>::dajTezinuGrane(int polazniCvor,int dolazniCvor,float tezinaGrane) {
    if(polazniCvor < 0 || polazniCvor >= brojGrana || dolazniCvor < 0 || dolazniCvor >= brojGrana)
        throw std::domain_error("Cvor nije validan");

    return M[polazniCvor][dolazniCvor] != 0;
}

template <typename TipOznake>
void UsmjereniGraf<TipOznake>::postaviOznakuCvora(int brojCvora, TipOznake oznaka) {
    if(brojCvora < 0 || brojCvora >= brojGrana)
        throw std::domain_error("Cvor nije validan");
    
    oznake[brojCvora] = oznaka;
}



int main() {
    std::cout << "Pripremna Zadaca Za Vjezbu 10, Zadatak 1";
    return 0;
}
