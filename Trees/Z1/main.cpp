#include <iostream>

#include <stdexcept>
#include <iomanip>

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

//region AVL Tree

//region AVL Tree Node

template <typename TK, typename TV>
class AVLNode {
public:
    TK key;
    TV value;
    AVLNode *left,*right,*parent;
    int balans;
    
    AVLNode() { left = right = nullptr; }
    AVLNode(const TK& k, const TV& v, AVLNode* p = nullptr, AVLNode* l  = nullptr, AVLNode* r = nullptr,int bal = 0) {
        key=k;
        value=v;
        parent=p;
        left=l;
        right=r;
        balans = bal;
    }
};

//endregion

//region Definicija klase

template <typename TK, typename TV>
class AVLStabloMapa : public Mapa<TK,TV> {

    AVLNode<TK,TV>* tekuci;
    
    int duzina;
    AVLNode<TK,TV>* pronadji(AVLNode<TK,TV>* trenutni, const TK& kljuc) const;
    void brisi(const TK& kljuc,AVLNode<TK,TV> *&r);
    void brisiSve(AVLNode<TK,TV> *r);
    AVLNode<TK,TV>* nextMinNode(AVLNode<TK,TV>* pocetak);
    AVLNode<TK,TV>* kloniraj(AVLNode<TK,TV>* trenutna) const;
    void azurirajBalans(AVLNode<TK,TV>* novi);
    void umetni(AVLNode<TK,TV>* &trenutni,  AVLNode<TK,TV>* novi, AVLNode<TK,TV>* rod = nullptr);
    void rebalansiraj(AVLNode<TK,TV>* trenutni);
    
    // Dodano za zadaću 3
    void rotirajLijevo(AVLNode<TK,TV>* novi);
    void rotirajDesno(AVLNode<TK,TV>* novi);
    void rotirajLijevoDesno(AVLNode<TK,TV>* novi);
    void rotirajDesnoLijevo(AVLNode<TK,TV>* novi);
    
public:
    AVLNode<TK,TV>* root;
    AVLStabloMapa() { root = nullptr; tekuci = nullptr; duzina = 0; }
    AVLStabloMapa(const AVLStabloMapa &nova);
    AVLStabloMapa operator=(const AVLStabloMapa &nova);
    ~AVLStabloMapa() { brisiSve(root); }
    TV operator[] (const TK& kljuc) const;
    TV& operator[] (const TK& kljuc);
    int brojElemenata() const { return duzina; }
    void obrisi(const TK& kljuc);
    void obrisi();
};

//endregion

//region Implementacija metoda

template <typename TK, typename TV>
AVLNode<TK,TV>* AVLStabloMapa<TK,TV>::kloniraj(AVLNode<TK,TV>* trenutna) const {
  if (trenutna) {
    AVLNode<TK,TV>* nova = new AVLNode<TK,TV>(trenutna -> key,trenutna -> value,nullptr,nullptr,nullptr,trenutna -> balans);
    
    nova -> left = kloniraj(trenutna -> left);
    nova -> right = kloniraj(trenutna -> right);
    
    return nova;
  }
  
  return nullptr;
}

template <typename TK, typename TV>
AVLStabloMapa<TK,TV>::AVLStabloMapa(const AVLStabloMapa &nova) {
    if(&nova != this)  {
        root = kloniraj(nova.root);
        duzina = nova.duzina;
    }
}

template <typename TK, typename TV>
AVLStabloMapa<TK,TV> AVLStabloMapa<TK,TV>::operator=(const AVLStabloMapa &nova) {
    if(&nova != this)  {
        delete root;
        root = kloniraj(nova.root);
        duzina = nova.duzina;
    }
    
    return *this;
}
    
template <typename TK, typename TV>
AVLNode<TK,TV>* AVLStabloMapa<TK,TV>::pronadji(AVLNode<TK,TV>* trenutni, const TK& kljuc) const {
    if(trenutni == nullptr || kljuc == trenutni -> key)
        return trenutni;
    
    if(kljuc < trenutni -> key)
        return pronadji(trenutni -> left, kljuc);
    else
        return pronadji(trenutni -> right, kljuc);
}

template <typename TK, typename TV>
TV AVLStabloMapa<TK,TV>::operator[] (const TK& kljuc) const {
    AVLNode<TK,TV>* trazeni = pronadji(root,kljuc);
  
    if(trazeni == nullptr)
        return TV();
    
    return trazeni -> value;
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::rotirajLijevo(AVLNode<TK,TV>* trenutni) {
    // std::cout << "left" << std::endl;
    
    AVLNode<TK,TV>* temp = trenutni -> right;
    trenutni -> right = temp -> left;
    
    if(temp -> left)
        temp -> left -> parent = trenutni;
    
    temp -> parent = trenutni -> parent;
    
    if(trenutni -> parent == nullptr)
        root = temp;
    else if(trenutni == trenutni -> parent -> left)
        trenutni -> parent -> left = temp;
    else 
        trenutni -> parent -> right = temp;
    
    temp -> left = trenutni;
    trenutni -> parent = temp;
    
    trenutni -> balans = trenutni -> balans - 1 - max(0,temp -> balans);
    temp -> balans = temp -> balans - 1 + min(0, trenutni -> balans);
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::rotirajDesno(AVLNode<TK,TV>* trenutni) {
    
    AVLNode<TK,TV>* temp = trenutni -> left;
    trenutni -> left = temp -> right;
    
    if(temp -> right != nullptr)
        temp -> right -> parent = trenutni;
    
    temp -> parent = trenutni -> parent;
    
    if(trenutni -> parent == nullptr)
        root = temp;
    else if(trenutni == trenutni -> parent -> right)
        trenutni -> parent -> right = temp;
    else 
        trenutni -> parent -> left = temp;
    
    temp -> right = trenutni;
    trenutni -> parent = temp;
    
    trenutni -> balans = trenutni -> balans + 1 - min(0, temp -> balans);
    temp -> balans = temp -> balans + 1 + max(0, trenutni -> balans);
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::rotirajLijevoDesno(AVLNode<TK,TV>* trenutni) {
    rotirajLijevo(trenutni -> left);
    rotirajDesno(trenutni);
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::rotirajDesnoLijevo(AVLNode<TK,TV>* trenutni) {
    rotirajDesno(trenutni -> right);
    rotirajLijevo(trenutni);
}

bool trebaRotiratiLijevoDesno(int prvo,int drugo) {
    return (prvo == -1 && (drugo == 1 || drugo == 0 || drugo == -1)) || (drugo == -1 && (prvo == 1 || prvo || prvo == 0 || prvo == -1));
}

bool trebaRotiratiDesnoLijevo(int prvo,int drugo) {
    return (prvo == 1 && (drugo == 1 || drugo == 0 || drugo == -1)) || (drugo == 1 && (prvo == 1 || prvo || prvo == 0 || prvo == -1));
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::rebalansiraj(AVLNode<TK,TV> *trenutni) {
    
    if(trenutni -> balans > 0) {
        if(trenutni -> right -> balans < 0)
            rotirajDesnoLijevo(trenutni);
        else
            rotirajLijevo(trenutni);
    } else if(trenutni -> balans < 0) {
        if(trenutni -> left -> balans > 0)
            rotirajLijevoDesno(trenutni);
        else
            rotirajDesno(trenutni);
    } 
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::azurirajBalans(AVLNode<TK,TV>* trenutni) {
    AVLNode<TK,TV>* parent = trenutni -> parent;
        
    if(trenutni -> balans < -1 || trenutni -> balans > 1) {
        rebalansiraj(trenutni);
        return;
    }
       
    if(parent) {
        if(parent -> left == trenutni)
            parent -> balans -= 1;
        else if(parent -> right == trenutni)
            parent -> balans += 1; //obrnut
            
        if(parent -> balans != 0)
            azurirajBalans(parent);
    }
    
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::umetni(AVLNode<TK,TV>* &trenutni, AVLNode<TK,TV>* novi, AVLNode<TK,TV>* rod) {
    AVLNode<TK,TV>* p = trenutni;
    
    if(trenutni == nullptr) {
        novi -> parent = rod;
        trenutni = novi;
    } else {
        if(novi -> key < p -> key) 
            umetni(p -> left, novi, p);
        else
            umetni(p -> right, novi, p);
    }

}

template <typename TK, typename TV>
TV& AVLStabloMapa<TK,TV>::operator[] (const TK& kljuc) {
    AVLNode<TK,TV>* trazeni = pronadji(root,kljuc);
    
    if(trazeni == nullptr) {
        AVLNode<TK,TV>* novi = new AVLNode<TK,TV>(kljuc,TV());
       
        if(duzina == 0) 
            root = novi;       
        else {
            umetni(root,novi);
            azurirajBalans(novi);
        }
        
        ++duzina;
        
        return novi -> value;
    }
    
    return trazeni -> value;
}

template <typename TK, typename TV>
AVLNode<TK,TV>* AVLStabloMapa<TK,TV>::nextMinNode(AVLNode<TK,TV>* pocetak) {
  
    while(pocetak -> left != nullptr) 
        pocetak = pocetak -> left;

    return pocetak;
}

template <typename TK, typename TV>
void AVLStabloMapa<TK, TV>::brisi(const TK& kljuc, AVLNode<TK, TV>* &cvor) {
	AVLNode<TK,TV>* p(pronadji(root,kljuc)), *rp(nullptr), *sr(nullptr);

	bool lijevo = false;

	if (p == nullptr)
		return;
	if (p->left == nullptr) {
		rp = p->right;
		sr = rp;
		if(p->right == nullptr)
			sr = p->parent;
	}
	else if (p->right == nullptr) {
		rp = p->left;
		sr = rp;
		lijevo = true;
	}
	else {
		rp = p->left;
		while (rp->right != nullptr)
			rp = rp->right;
		if (rp->parent != p) {

			sr = rp->parent;
			rp->parent->right = rp->left;
			if(rp->left)
				rp->left->parent = rp->parent->right;
			rp->left = p->left;
			if(rp->left)
				rp->left->parent = rp;
		} else
			sr = rp;
		rp->right = p->right;
		rp->right->parent = rp;
	}
	if (p->parent == nullptr) {
		root = rp;
	}
	else if (p == p->parent->left)
		p->parent->left = rp;
	else
		p->parent->right = rp;
	if(rp) {
		rp->parent = p->parent;
		rp->balans = p->balans;
	}
	
	duzina--;
	
	delete p;
	
	p = nullptr;

	if(sr) {
		if(sr->left || sr->right)
			sr->balans += (sr->left ? 1 : -1);
		else 
			azurirajBalans(sr);
	}
}


template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::obrisi(const TK& kljuc) {
    brisi(kljuc,root);
    --duzina;
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::brisiSve(AVLNode<TK,TV> *r) {
    if (r) {
        brisiSve(r->left);
        brisiSve(r->right);
        delete r;
    }
}

template <typename TK, typename TV>
void AVLStabloMapa<TK,TV>::obrisi() {
    brisiSve(root);
    duzina = 0;
    root = nullptr;
}

//endregion

//endregion


int main() {
    
   // Malo jaci test performansi AVLStabloMape gdje se vidi 
// da li je zaista O(log n)
AVLStabloMapa <int, int> m;
int vel(2);
for (int i(0); i<vel; i++)
    m[rand()%(2*vel)-vel] = i;
for (int i(-vel); i<vel; i++)
    m[i] = i;
for (int i(-vel); i<vel; i++) {
    // std::cout << m[i] << "->" << i << std::endl;
    if (m[i] != i) { cout << i << " NOT "; break; }
    m.obrisi(i);
}
cout << "OK";
    
    return 0;
}
