#include <iostream>

#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

//region Sorting algoritmi

template <typename T>
void swap(T &a, T &b) {
    T temp(a);
    a = b;
    b = temp;
}

template <typename T>
void bubble_sort(T* niz,int vel) {
    for(int i=0;i<vel;++i) {
        for(int j=0;j<vel-1;j++) {
            if(niz[j] > niz[j+1])
                ::swap(niz[j],niz[j+1]);
        }
    }
}

template <typename T>
void selection_sort(T* niz,int vel) {
    for(int i=0;i<vel;++i) {
        int indexMin = i;
        for(int j=i;j<vel;++j) {
            if(niz[j] < niz[indexMin])
                indexMin = j;
        }
        
        ::swap(niz[i],niz[indexMin]);
    }
}

template <typename T>
int partition(T* A,int prvi, int zadnji) {
    T pivot = A[prvi];
    int p = prvi + 1;
    
    while(p <= zadnji) 
        if(A[p] <= pivot) 
            p++; 
        else 
            break;
    
    for(int i=p+1;i<=zadnji;++i)
        if(A[i] < pivot)
            ::swap(A[i],A[p++]);
    
    ::swap(A[prvi],A[p-1]);
    
    return p-1;
}

template <typename T>
void quickSort(T* niz,int p, int z) {
    if(p < z) {
        int part = partition(niz,p,z);
        quickSort(niz,p,part-1);
        quickSort(niz,part+1,z);
    }
}

template <typename T>
void quick_sort(T* niz,int vel) {
    quickSort(niz,0,vel-1);
}

template<typename T>
void merge(T* A,int l, int p, int q,int d) {
    int i(0), j(q-l), k(l);
    int lhs(p-l), rhs(d-l);
    
    T *B = new T[d-l+1];
    
    for(int m=0;m<=rhs;++m)
        B[m] = A[l+m];

    while(i <= lhs && j <= rhs) {
        if(B[i] < B[j]) {
            A[k] = B[i];
            i++;
        } else {
            A[k] = B[j];
            j++;
        }
        k++;
    }
    
    while(i <= lhs) A[k++] = B[i++];
    while(j <= rhs) A[k++] = B[j++];
    
    delete[] B;
}

template <typename T>
void mergeSort(T* niz,int l,int d) {
    if(d > l) {
        int p = (l+d-1)/2;
        int q = p + 1;
        mergeSort(niz,l,p);
        mergeSort(niz,q,d);
        merge(niz,l,p,q,d);
    }
}

template <typename T>
void merge_sort(T* niz,int vel) {
    mergeSort(niz,0,vel-1);
}

template <typename T>
void ucitaj(string filename, T*& niz, int &vel) {
    std::fstream ulaz(filename,std::fstream::in | std::fstream::out);
    
    niz = new int[vel];
    
    T value;
    int i=0;
    
    if(!ulaz) return;
    
    while(!ulaz.eof()) {
    	char separator;
    
        if(!(ulaz >> value))
             break;
    
        if((separator = ulaz.peek()) == ' ') {
            ulaz.get();
        } else {
            if(separator == EOF) ulaz.clear();
            break;
        }
        
        niz[i++] = value;
       
        if(ulaz.bad())
             throw std::logic_error("Problemi pri citanju datoteke");
	}
    
    ulaz.close();
}

void generisi(std::string filename, int vel) {
    std::fstream izlaz(filename,std::fstream::in | std::fstream::out);
   
    for(int i=0;i<vel;++i) {
        izlaz << std::to_string(rand() % 10000 + 1);
        if(i != vel-1) izlaz << " ";
    }
    
    izlaz.close();
}

template <typename T>
bool testSortiranja(T* niz, int vel) {
    for(int i=0;i<vel-1;++i)
        if(niz[i] > niz[i+1])
            return false;
    
    return true;
} 

template <typename T>
bool zapisi(std::string filename,T* niz, int &vel) {
    std::fstream izlaz(filename,std::fstream::in | std::fstream::out);
   
    for(int i=0;i<vel;++i) 
        izlaz << niz[i] << " ";
    
    izlaz.close();
    return true;
}

//endregion

void interakcija() {
    while(true) {
        int unos;
        time_t begin,end;
        
        int vel = 100;
        int *niz;
        
        generisi("primjerulaza.txt",vel);
        ucitaj("primjerulaza.txt",niz,vel);

        std::cout << "1. Bubble Sort" << std::endl;
        std::cout << "2. Selection Sort" << std::endl;
        std::cout << "3. Quick Sort" << std::endl;
        std::cout << "4. Merge Sort" << std::endl;
        std::cout << "Unesite vas izbor: ";
        
        std::cin >> unos;
        std::cout << std::endl;
        
        begin = clock();

        switch(unos) {
            case 1:
                std::cout << "Bubble sort: ";
                bubble_sort(niz,vel);
                for(int i=0;i<vel;++i) std::cout << niz[i] << " ";
                break;
            case 2:
                std::cout << "Selection sort: ";
                selection_sort(niz,vel);
                for(int i=0;i<vel;++i) std::cout << niz[i] << " ";
                break;
            case 3:
                std::cout << "Quick sort: ";
                quick_sort(niz,vel);
                for(int i=0;i<vel;++i) std::cout << niz[i] << " ";
                break;
            case 4:
                std::cout << "Merge sort: ";
                merge_sort(niz,vel);
                for(int i=0;i<vel;++i) std::cout << niz[i] << " ";
                break;
            default:
                std::cout << "Pogresan odabir";
                break;
        }
        
        end = clock();
        
        double vrijeme = (end - begin) / (CLOCKS_PER_SEC / 1000); 
        
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << (testSortiranja(niz,vel) ? "Niz uspješno sortiran!" : "Niz nije sortiran!") << std::endl;
        std::cout << (zapisi("izlaz.txt",niz,vel) ? "Niz uspješno zapisan u datoteku!" : "Niz nije zapisan u datoteku!") << std::endl;
        std::cout << "Vrijeme izvršenja: " << vrijeme << "ms." << std::endl;
        std::cout << std::endl;
        
        delete[] niz;
    }
    
}

int main() {
    srand (time(NULL));
    
    // interakcija();
    
    int* niz;
    int vel = 0;
    
    generisi("autotest.txt", 3000000);
    ucitaj("autotest.txt", niz, vel);
    merge_sort(niz, vel);
    
    for(int i(0); i<vel-1; i++)
        if(niz[i] > niz[i+1]) {
            cout << "NOT ";
            break;    
        }
    
    std::cout << "Oke";
    
    delete[] niz;

    return 0;
}
