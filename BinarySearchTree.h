/**
-----------------------------------------------------------------------------------
Laboratoire : 09
\file       BinarySearchTree.h
\author     Loïc Dessaules, Doran Kayoumi, Gabrielle Thurnherr
\date       04/06/2019
\brief      Classe représentant un arbre binaire de recherche, avec toutes les méthodes associées
Compilateur MinGW-gcc 6.3.0

Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.
**/

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>
#include <cassert>
#include <stdexcept>

using namespace std;

template<typename T>
class BinarySearchTree {
public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;

private:
    /**
     *  @brief Noeud de l'arbre.
     *
     * contient une cle et les liens vers les sous-arbres droit et gauche.
     */
    struct Node {
        const value_type key; // clé non modifiable
        Node *right;          // sous arbre avec des cles plus grandes
        Node *left;           // sous arbre avec des cles plus petites
        size_t nbElements;    // nombre de noeuds dans le sous arbre dont
        // ce noeud est la racine

        Node(const_reference key)  // seul constructeur disponible. key est obligatoire
                : key(key), right(nullptr), left(nullptr), nbElements(1) {
            cout << "(C" << key << ") ";
        }

        ~Node()               // destructeur
        {
            cout << "(D" << key << ") ";
        }

        Node() = delete;             // pas de construction par défaut
        Node(const Node &) = delete;  // pas de construction par copie
        Node(Node &&) = delete;       // pas de construction par déplacement
    };

    /**
     *  @brief  Racine de l'arbre. nullptr si l'arbre est vide
     */
    Node *_root;

public:

    /**
     *  @brief Constructeur par défaut. Construit un arbre vide
     *  @remark COmplexité : O(1)
     */
    BinarySearchTree() : _root(nullptr) {
        // Nothing to do...
    }

    /**
     *  @brief Constucteur de copie.
     *
     *  @param other le BinarySearchTree à copier
     *  @remark Complexité : O(n)
     */
    BinarySearchTree(const BinarySearchTree &other) : BinarySearchTree() {
        copy(other._root);
    }

    /**
     * @brief Copie les noeauds d'un arbre ds l'objet courrant
     *
     * @param N noeaud racine a copier
     * @remark Complexité : O(n)
     */
    void copy(const Node *N) {
        if (N) {
            insert(N->key);
            copy(N->left);
            copy(N->right);
        }
    }

    /**
     *  @brief Opérateur d'affectation par copie.
     *
     *  @param other le BinarySearchTree à copier
     *  @Remark Complexité : O(n)
     */
    BinarySearchTree &operator=(const BinarySearchTree &other) {
        // Si on essaye de faire une affectation entre 2 objet qui sont au meme emplacement mémoire -> retourne l'objet
        if (this->_root == other._root)
            return *this;
        // Appel le constructeur de copie dans un objet temp, et swap l'objet courant avec le temp
        BinarySearchTree tmpTree(other);
        swap(tmpTree);

        return *this;
    }

    /**
     *  @brief Echange le contenu avec un autre BST
     *
     *  @param other le BST avec lequel on echange le contenu
     *  @remark Complexité : O(1)
     */
    void swap(BinarySearchTree &other) noexcept {
        Node *root = this->_root;
        this->_root = other._root;
        other._root = root;
    }

    /**
     *  @brief constructeur de copie par déplacement
     *
     *  @param other le BST dont on vole le contenu
     *  @remark Complexité : O(n)
     */
    BinarySearchTree(BinarySearchTree &&other) noexcept : BinarySearchTree() {
        // Utilise l'opérateur d'affectation par copie créé aupréalable et met a null l'objet en parametre apres avoir été copié
        this->_root = other._root;
        other._root = nullptr;
    }

    /**
     *  @brief Opérateur d'affectation par déplacement.
     *
     *  @param other le BST dont on vole le contenu
     *  @remark Complexité : O(1)
     */
    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept {
        // Si on essaye de faire une affectation entre 2 objet qui sont au meme emplacement mémoire -> retourne l'objet
        if (this->_root == other._root)
            return *this;

        // On swap l'objet en param avec l'objet courant
        swap(other);

        return *this;
    }

    //
    // @brief Destructeur
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée deleteSubTree(Node*)
    // @remark Complexité : O(n)
    //
    ~BinarySearchTree() {
        if (_root != nullptr)
            deleteSubTree(_root);
    }

private:
    //
    // @brief Fonction détruisant (delete) un sous arbre
    //
    // @param r la racine du sous arbre à détruire.
    //          peut éventuellement valoir nullptr
    // @remark Complexité : O(n)
    //
    static void deleteSubTree(Node *r) noexcept {
        // On va d'abord dans tous les noeud de gauche (si pas une feuille)
        if (r->left != nullptr) {
            deleteSubTree(r->left);
        }
        // La on a fini pour le coté gauche, on va donc dans le coté droit (si pas une feuille)
        if (r->right != nullptr) {
            deleteSubTree(r->right);
        }

        // A ce moment on a une feuille, on peut donc supprimer le noeud
        r->~Node();
    }

public:
    //
    // @brief Insertion d'une cle dans l'arbre
    //
    // @param key la clé à insérer.
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée insert(Node*&,const_reference)
    // @remark Complexité moyenne : O(log(n))
    //
    void insert(const_reference key) {
        insert(_root, key);
    }

private:
    //
    // @brief Insertion d'une cle dans un sous-arbre
    //
    // @param r la racine du sous-arbre dans lequel
    //          insérer la cle.
    // @param key la clé à insérer.
    //
    // @return vrai si la cle est inseree. faux si elle etait deja presente.
    //
    // Si la cle est deja presente, cette fonction ne fait rien.
    // x peut éventuellement valoir nullptr en entrée.
    // la fonction peut modifier x, reçu par référence, si nécessaire
    //
    // @remark Complexité moyenne : O(log(n))
    //
    static bool insert(Node *&r, const_reference key) {
        // Cas triviale : On a atteint une feuille, on peut créer le nouveau noeud
        if (r == nullptr) {
            r = new Node{key};
            return true;
        }

        // On va d'abord dans le sous-arbre gauche (clé à ajouter plus petite que la clé du noeud)
        else if (key < r->key) {

            if (insert(r->left, key)) {
                // incrément du nb élément si on a bien inséré une nouvelle clé
                ++r->nbElements;
                return true;
            }

        }
        // Ensuite on va dans le sous-arbre droite (clé à ajouter plus grande que la clé du noeud)
        else if (key > r->key) {
            if (insert(r->right, key)) {
                // incrément du nb élément si on a bien inséré une nouvelle clé
                ++r->nbElements;
                return true;
            }
        }
        // Sinon la clé existe déjà !
        return false;
    }

public:
    //
    // @brief Recherche d'une cle.
    //
    // @param key la cle a rechercher
    //
    // @return vrai si la cle trouvee, faux sinon.
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée contains(Node*,const_reference)
    // @remark Complexité moyenne : O(log(n))
    //
    bool contains(const_reference key) const noexcept {
        return contains(_root, key);
    }

private:
    //
    // @brief Recherche d'une cle dans un sous-arbre
    //
    // @param key la cle a rechercher
    // @param r   la racine du sous-arbre
    //
    // @return vrai si la cle trouvee, faux sinon.
    // @remark Complexité moyenne : O(log(n)
    //
    static bool contains(Node *r, const_reference key) noexcept {
        // Cas trivial
        if (r == nullptr) {
            return false;
        }
        // si la clé cherchée est plus petite que la clé du noeud en cours, on va rechercher dans le ss-arbre gauche
        else if (key < r->key) {
            contains(r->left, key);
        }
        // si la clé cherchée est plus grande que la clé du noeud en cours, on va rechercher dans le ss-arbre droit
        else if (key > r->key) {
            contains(r->right, key);
        }
        // si pas nullptr, pas plus petite ou plus grande, on l'a trouvée
        else {
            return true;
        }
    }

public:
    //
    // @brief Recherche de la cle minimale.
    //
    // @return une const reference a la cle minimale
    //
    // @exception std::logic_error si necessaire
    //
    // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
    //
    // @remark Complexité moyenne : O(log(n))
    //
    const_reference min() const {
        if (_root == nullptr) {
            throw logic_error("Impossible to search the min key in an empty tree");
        }

        return min(_root);
    }

    //
    // @brief Supprime le plus petit element de l'arbre.
    //
    // @exception std::logic_error si necessaire
    //
    // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
    //
    // @remark Complexité moyenne : O(log(n))
    //
    void deleteMin() {
        if (_root == nullptr) {
            throw logic_error("Impossible to delete the min key in an empty tree");
        }

        deleteMin(_root);
    }


    //
    // @brief Supprime l'element de cle key de l'arbre.
    //
    // @param key l'element a supprimer
    //
    // si l'element n'est pas present, la fonction ne modifie pas
    // l'arbre mais retourne false. Si l'element est present, elle
    // retourne vrai
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée deleteElement(Node*&,const_reference)
    //
    bool deleteElement(const_reference key) noexcept {
        return deleteElement(_root, key);
    }

private:

    //
    // @brief Recherche de la cle minimale.
    // @param r la racine du sous arbre
    // @return une const reference a la cle minimale
    // @remark Complexité moyenne : O(log(n))
    //
    static const_reference min(Node *r) {
        // La clé min est forcément le dernier noeud du sous-arbre gauche
        if (r->left != nullptr) {
            min(r->left);
        } else {
            return r->key;
        }
    }

    //
    // @brief Supprime le plus petit element de l'arbre.
    // @param r La racine du sous arbre
    // @remark Complexité moyenne : O(log(n))
    //
    static void deleteMin(Node *&r) {
        // Appel récursif tant qu'on a pas le noeud avec la clé min (dernier noeud a gauche)
        if (r->left != nullptr) {
            deleteMin(r->left);
            // Décrément du nbElement en remontant les appels récursif, pour la mise a jour
            --r->nbElements;
        }
        // Ici, le noeud est celui qui a la clé la plus petite
        else {
            // On détruit le noeud
            r->~Node();
            // Si le noeud a un enfant droit, celui-ci prend ca place
            if (r->right != nullptr) {
                r = r->right;
            }
            // Si le noeud avec la clé min est une feuille, on le met a nullptr
            else {
                r = nullptr;
            }
        }
    }

    //
    // @brief Supprime l'element de cle key du sous arbre.
    //
    // @param r la racine du sous arbre
    // @param key l'element a supprimer
    //
    // si l'element n'est pas present, la fonction ne modifie pas
    // l'arbre mais retourne false. Si l'element est present, elle
    // retourne vrai
    // @remark Complexité moyenne : O(log(n))
    //
    static bool deleteElement(Node *&r, const_reference key) noexcept {
        // Cas triviale, clé pas trouvée
        if (r == nullptr) {
            return false;
        }

        // Besoin de recherché dans le sous arbre de gauche
        if (key < r->key) {
            if (deleteElement(r->left, key)) {
                // Si on a supprimé un noeud, on va décrémenter le nbElement du noeud courant pour le mettre à jour
                --r->nbElements;
                return true;
            }
        }
        // Besoin de recherché dans le sous arbre de gauche
        else if (key > r->key) {
            if (deleteElement(r->right, key)) {
                // Si on a supprimé un noeud, on va décrémenter le nbElement du noeud courant pour le mettre à jour
                --r->nbElements;
                return true;
            }
        }
        // Clé trouvée
        else {
            // on va décrémenter le nbElement du noeud courant pour le mettre à jour
            --r->nbElements;
            // Cas simple, on a un des deux enfants null, on detruit le noeud courant et l'enfant prend la place du noeud courant
            if (r->left == nullptr) {
                r->~Node();
                r = r->right;
            } else if (r->right == nullptr) {
                r->~Node();
                r = r->left;
            }
            // Cas compliqué (on a les deux enfants) on utilise la technique de Hibbard
            else {
                // Appel une métode qui nous retourne un ptr référence sur le noeud le plus petit du ss-arbre droit
                // Call a method to fetch the minimum node of the right subtree
                Node *&minNode = chercherMinNode(r->right);
                // On swap le noeud courant et le noeud le plus petit du ss arbre droite
                swapNodes(r, minNode);
                // On supprime l'élément minimum du ss arbre droit (celui qu on vient d'etre swap, donc celui qu on veut)
                deleteMin(r->right);
            }
            return true;
        }
    }

    /**
     * @brief Permet de swap correctement deux noeuds
     * @param a Un des deux Node a échanger (passé en pointeur référence)
     * @param b Un des deux Node a échanger (passé en pointeur référence)
     * @remark Complexité : O(1)
     */
    static void swapNodes(Node *&a, Node *&b) {
        std::swap(a->left, b->left);
        std::swap(a->right, b->right);
        std::swap(a->nbElements, b->nbElements);
        std::swap(a, b);
    }

    /**
     * @brief Recherche et retourne le noeud qui contient la clé minimum dans l'arbre
     * @param r Le sous arbre dans lequel chercher
     * @return Le noeud qui contient la clé minimum dans l'arbre en tant que pointeur référence
     * @remark Complexité moyenne : O(log(n))
     */
    static Node *&chercherMinNode(Node *&r) {
        if (r->left != nullptr) {
            chercherMinNode(r->left);
        }
        else {
            return r;
        }
    }

public:
    //
    // @brief taille de l'arbre
    //
    // @return le nombre d'elements de l'arbre
    // @remark Complexité : O(1)
    //
    size_t size() const noexcept {
        return _root->nbElements;
    }

    //
    // @brief cle en position n
    //
    // @return une reference a la cle en position n par ordre croissant des
    // elements
    //
    // @exception std::logic_error si nécessaire
    //
    // ajoutez le code de gestion des exceptions, puis mettez en oeuvre
    // la fonction recursive nth_element(Node*, n)
    // @remark Complexité : O(log(n))
    //
    const_reference nth_element(size_t n) const {

        // gestion des exceptions
        if (_root == nullptr)
            throw std::logic_error("Il n'y a aucun éléments dans l'arbre!");
        if (n > size())
            throw std::logic_error("Index trop grand");

        return nth_element(_root, n);
    }

private:

    // @brief cle en position n dans un sous arbre
    //
    // @param r la racine du sous arbre. ne peut pas etre nullptr
    // @param n la position n
    //
    // @return une reference a la cle en position n par ordre croissant des
    // elements
    //
    static const_reference nth_element(Node *r, size_t n) noexcept {
        size_t leftCount = r->left != nullptr ? r->left->nbElements : 0;

        // si la position est égale au nombre d'éléments a gauche, on a trouvé notre Node
        if (n == leftCount)
            return r->key;

        // check pour savoir si l'on doit chercher la valeur a gauche ou a droite de l'arbre
        // si la position est plus petite que le nbre d'éléments a gauche, il faut aller a gauche
        if (n < leftCount)
            return nth_element(r->left, n);

        // si la position est plus grande que le nbre d'éléments a gauche, il faut aller a droite
        //  et l'on soustrait le nbre d'élément a gauche à la position
        if (n > leftCount)
            return nth_element(r->right, n - leftCount - 1);

    }

public:
    //
    // @brief position d'une cle dans l'ordre croissant des elements de l'arbre
    //
    // @param key la cle dont on cherche le rang
    //
    // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée rank(Node*,const_reference)
    // @remark Complexité moyenne : O(log(n))
    //
    size_t rank(const_reference key) const noexcept {
        return rank(_root, key);
    }

private:
    //
    // @brief position d'une cle dans l'ordre croissant des elements du sous-arbre
    //
    // @param key la cle dont on cherche le rang
    // @param r la racine du sous arbre
    //
    // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
    // @remark Complexité moyenne : O(log(n))
    //
    static size_t rank(Node *r, const_reference key) noexcept {

        if (r == nullptr or !contains(r, key))
            return size_t(-1);

        size_t leftCount = r->left != nullptr ? r->left->nbElements : 0;
        
        if (key < r->key)
            return rank(r->left, key);

        if (key > r->key)
            return rank(r->right, key) + leftCount + 1;

        if (key == r->key)
            return leftCount;

        return size_t(-1);
    }

public:
    //
    // @brief linearise l'arbre
    //
    // transforme l'arbre en une liste simplement chainee composee des memes
    // noeuds que precedemment, mais dont les pointeurs left sont tous egaux
    // a nullptr. Cette liste doit toujours respecter les conditions d'un
    // arbre binaire de recherche
    //
    // Ne pas modifier cette fonction qui sert essentiellement a tester la
    // fonction recursive linearize(Node*, Node*&, size_t&) utilisée par
    // la methode publique arborize
    //
    // @remark Complexité : O(n)
    //
    void linearize() noexcept {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        _root = list;
    }

private:
    //
    // @brief linearise une sous arbre tree en une liste
    //
    // @param tree pointe vers la racine de l'arbre a lineariser
    // @param list reference a la tete de la liste a creer. sera modifiee
    //             par la fonction pour que list pointe vers le plus petit
    //             element de tree
    // @param cnt  calcule au fure et a mesure le nombre d'elements de la liste
    //             cree. l'effet de la fonction doit etre d'ajouter le nombre
    //             d'elements du sous-arbre de racine tree. Cependant, vous
    //             avez uniquement le droit d'utiliser l'opérateur ++.
    // @remark Complexité : O(n)
    //
    static void linearize(Node *tree, Node *&list, size_t &cnt) noexcept {
        if (tree == nullptr) {
            return;
        }
        linearize(tree->right, list, cnt);
        // Stock dans l'enfant droit, le noeud de la list et la list contient le noeud courant
        tree->right = list;
        list = tree;
        // Mise à jour du count et du nb des éléments du noeud courant
        ++cnt;
        tree->nbElements = cnt;

        // à chaque enfant gauche, on ba donné nullptr car on veut un arbre sous forme de liste
        linearize(tree->left, list, cnt);
        tree->left = nullptr;
    }

public:
    //
    // @brief equilibre l'arbre
    //
    // applique l'algorithme d'equilibrage de l'arbre par linearisation et
    // arborisation
    //
    // Ne pas modifier cette fonction.
    //
    void balance() noexcept {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        arborize(_root, list, cnt);
    }

private:
    //
    // @brief arborise les cnt premiers elements d'une liste en un arbre
    //
    // @param tree reference dans laquelle il faut ecrire la racine de l'arbre
    //             arborise par la fonction
    // @param list IN - reference a la tete de la liste a parcourir. La liste
    //                  est composee de Node dont le pointer left est nullptr
    //             OUT - debut de la suite de la liste dont on a utilise cnt
    //                   elements
    // @param cnt  nombre d'elements de la liste que l'on doit utiliser pour
    //             arboriser le sous arbre
    // @Remark Complexité : O(n)
    //
    static void arborize(Node *&tree, Node *&list, size_t cnt) noexcept {
        if(cnt == 0){
            tree = nullptr;
            return;
        }
        // On va d'abord arboriser les sous arbres de gauche, ensuite droite
        Node* rg = nullptr;
        arborize(rg, list, (cnt-1)/2);
        tree = list;
        tree->left = rg;
        list = list->right;
        tree->nbElements = cnt;
        arborize(tree->right, list, cnt/2);
    }

public:
    //
    // @brief Parcours pre-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    //
    // @remark Complexité : O(n)
    //
    template<typename Fn>
    void visitPre(Fn f) {
        visitPre(f, _root);
    }

    //
    // @brief Parcours symétrique de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    // @remark Complexité : O(n)
    template<typename Fn>
    void visitSym(Fn f) {
        visitSym(f, _root);
    }

    //
    // @brief Parcours post-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    //
    // @remark Complexité : O(n)
    template<typename Fn>
    void visitPost(Fn f) {
        visitPost(f, _root);
    }

private:
    //
    // @brief Parcours pre-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    // @param r La racine du sous arbre
    // @remark Complexité : O(n)
    //
    template<typename Fn>
    static void visitPre(Fn f, Node *r) {
        if (r != nullptr) {
            f(r->key);
            visitPre(f, r->left);
            visitPre(f, r->right);
        }
    }

    //
    // @brief Parcours symétrique de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    // @param r La racine du sous arbre
    // @remark Complexité : O(n)
    //
    template<typename Fn>
    void visitSym(Fn f, Node *r) {
        if (r != nullptr) {
            visitSym(f, r->left);
            f(r->key);
            visitSym(f, r->right);
        }
    }

    //
    // @brief Parcours post-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    // @param r La racine du sous arbre
    // @remark Complexité : O(n)
    //
    template<typename Fn>
    void visitPost(Fn f, Node *r) {
        if (r != nullptr) {
            visitPost(f, r->left);
            visitPost(f, r->right);
            f(r->key);
        }
    }


public:
    //
    // Les fonctions suivantes sont fournies pour permettre de tester votre classe
    // Merci de ne rien modifier au dela de cette ligne
    //
    void display() const {
        stringstream ss1, ss2, ss3;
        displayKeys(ss1);
        displayKeys(ss3);
        displayCounts(ss2);
        string l1, l2;

        size_t W = 11;
        while (getline(ss3, l1))
            if (l1.length() > W) W = l1.length();

        displayKeys(ss1);

        cout << "\n";
        cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
        cout << "| " << left << setw(W) << "key" << "| " << setw(W) << "nbElements" << "|" << endl;
        cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
        while (getline(ss1, l1) and getline(ss2, l2)) {
            cout << "| " << left << setw(W) << l1 << "| " << setw(W) << l2 << "|" << endl;
        }
        cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;

    }

    void displayKeys(ostream &os = cout) const {
        display([](Node *n) -> const_reference { return n->key; }, os);
    }

    void displayCounts(ostream &os = cout) const {
        display([](Node *n) -> size_t { return n->nbElements; }, os);
    }

    //
    // utilise un parcours de l'arbre en largeur avec
    // un noeud sentinelle newLevel qui traque les
    // changements de niveaux
    //
    template<typename Fn>
    void display(Fn func, ostream &os = cout) const {
        Node *newLevel = (Node *) -1;
        // addresse non nulle dont on est sur qu'elle ne contient pas
        // vraiment un Node. Utilisée comme sentinelle.

        queue<Node *> Q;
        Q.push(_root);
        Q.push(newLevel);

        while (!Q.empty()) {
            Node *cur = Q.front();
            Q.pop();

            if (cur == newLevel) {
                os << endl;
                if (!Q.empty())
                    Q.push(newLevel);
            } else if (cur == nullptr) {
                os << "- ";
            } else {
                os << func(cur) << " ";
                Q.push(cur->left);
                Q.push(cur->right);
            }
        }
    }
};
