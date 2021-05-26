#include<iostream>
using namespace std;
class BArboreEnti
{
private:
    int *keys; // array de chei
    int B; // valoare minima pt nr de chei
    int n; // nr de chei curent
    bool efrunza; // false -> cand nu este frunza; true -> cand este frunza
    BArboreEnti **C; // un array de pointeri copii

public:
    BArboreEnti(int _B, bool _efrunza)// constructor ( "_" permite modificarea dintr-un byte in caractere de 2 bytes)
    {
        B = _B; // copiem valoarea minima pt nr de chei
        efrunza = _efrunza; // copiem proprietatea true/false pentru frunza
        n = 0; // nr initial de chei va fi 0
        keys = new int[2*B - 1]; // conform formulei, redam memorie cat pentru maximul numarului de chei posibile
        C = new BArboreEnti *[2*B]; // analog pentru copiii-pointer
    }

     BArboreEnti *cautare(int y) // functie pentru cautarea cheii y intr-un sub-arbore
    {
        int i = 0;

        while (i < n && y > keys[i]) // cautam prima cheie >= y
            i++;

        if(keys[i] == y )
            return this; // daca cheia e egala cu y, returnam nodul curent

        if(efrunza == true)
        {
            return  NULL; // alfel, daca cheia nu este gasita, y e frunza si returnam NULL
        }
        return C[i] -> cautare(y); // cautam, in continuare, copilul potrivit
    }

    void traversare() // functie pentru traversarea nodurilor intr-un sub-arbore
    {
        int i;
        // n chei; n + 1 copii; traversam cele n chei si primii n copii
        for(i=0; i <= n - 1; i++)
        {
            if(efrunza == false) // daca nodul curent nu este frunza, traversam sub-arborele cu radacina C[i],
                C[i] -> traversare();
            cout << ' ' << keys[i]; // abia apoi afisam cheia indexului i
        }

        if (efrunza == false) // este necesara o parcurgere suplimentara pentru sub-arborele care are ca radacina ultimul copil
            C[i] -> traversare();

    }

    //---------inserare--------------------------------//
    void inserare_la_indemana(int y); // functie care permite inserarea unei chei, asumandu-ne ca are spatiu

    void splitC(int i, BArboreEnti *y); // functie care face split pt copilul y, asumandu-ne ca spatiul pentru copii este full

    //------------------------------------------------//


    //---------stergere-------------------------------//
    int gasesteCheie(int y); // gaseste index-ul primei chei >= y
    void sterge(int y); // sterge cheia y din sub-arbore
    void stergefrunza(int index); // sterge cheia de pe poz index, unde nodul e frunza
    void stergenod(int index); // sterge cheia de pe poz index, unde nodul nu e frunza

    int succesor(int index);
    int predecesor(int index);

    void umple(int index); // va umple pozitia index in array-ul C (in conditia in care sunt B-1 chei)

    void imprumutpred(int index); // va imprumuta cheia de la predecesor si o va muta pe pozitia index
    void imprumutsucc(int index); // va imprumura cheia de la succesor si o va muta pe pozitia index

    void merge(int index); //va face merge intre copilul de pe pozitia index si cel de pe pozitia index+1

    //------------------------------------------------//



    //------succesor + predecesor--------------------//
    int pt_succ(int y, bool &ok, int& succ)
    {

        int k=0;
        while(k < n && this-> keys[k] < y) // cat timp k < nr de val din nodul curent
        {
            k++;
        }
        if(this->C[k]->keys[(this->C[k]->n)-1] == y) // varif in copil ultima poz == y
        {
            if(k == n)
            {
                ok = false;
                return y;
            }
            else
            {
                ok = true;
                succ = keys[k];
                return (this->keys[k]);
            }
        }
        else
        {
            C[k] -> pt_succ(y, ok, succ); // daca e nod intern reapelam functia
        }
        if(ok == true)
            return succ;
        if(ok == false  && k != n)
        {
            ok = true; // a fost apelata fct dintre valori
            return (this->keys[k]);
        }
        else if(ok == false and k == n)
            return y;

    }

    int pt_pred(int y, bool &ok, int& prd){
        int k = 0;
        while(k < n and this->keys[k] < y){ // cat timp k < nr de val din nodul curent
            k++;
        }
        if(this->C[k]->keys[0] == y){ // verif in copil prima poz == y
            if(k == 0) {ok = false; return y;}
            else { ok = true; prd = keys[k-1]; return (this->keys[k-1]);}
        }
        else C[k]->pt_pred(y,ok,prd); // daca este nod intern reapelam fct
        if(ok == true) return prd;
        if(ok == false and k > 0){
            ok = true; // a fost apelata fct dintre valori
            return (this->keys[k-1]);
        }
        else if(ok == false and k == 0)
            return y;
    }

    //------------------------------------------------//

    friend class BArbore;
};

class BArbore
{
private:
    BArboreEnti *radacina; // variabila BArboreEnti de tip pointer catre nod-ul radacina
    int B; // valoare minima pt nr de chei
public:

    BArbore(int _B) // constructor de initializare
    {
        radacina = NULL;
        B = _B;
    }

    void traversare()
    {
        if(radacina != NULL)
            radacina -> traversare();
    }

    BArboreEnti* cautare(int y) // functie pentru cautarea cheii y in BArbore
    {

        if(radacina != NULL)
            return radacina ->cautare(y);
        else
            return NULL;

    }

    //----------INSERARE------------//
    void insert(int y)
    {
        if (radacina == NULL) // daca arborele este gol
        {
            radacina = new BArboreEnti(B, true); // facem loc in memorie pt radacina
            radacina -> keys[0] = y; // inseram cheia
            radacina -> n = 1; // crestem cu 1 numarul de chei din radacina
        }
        else // daca radacina != NULL => arborele nu este gol, deci avem radacina
        {
            if(radacina->n == 2*B - 1) // daca radacina este full, crestem nivelul
            {
                BArboreEnti *radacina2 = new BArboreEnti(B, false); // facem spatiu pentru o noua radacina
                radacina2->C[0] = radacina; // facem vechea radacina sa fie copilul noii radacini
                radacina2->splitC(0, radacina); // facem split si mutam una din chei in noua radacina

            int i=0;
            if(radacina2->keys[0] < y) // dintre cei 2 copii ai noii radacini decidem care va avea o noua cheie
                i++;
            radacina2->C[i]->inserare_la_indemana(y);

            radacina=radacina2; // setam vechea radacina cu valoarea noii radacini
            }
            else // daca radacina nu este full, inseram pur si simplu
                radacina->inserare_la_indemana(y);
        }
    }
    //---------------------------------------------//


    //--------------STERGERE-----------------------//
    void sterge(int y)
    {

            if(!radacina)
                {
                    cout << "Arbore gol";
                    return;
                }
            radacina->sterge(y);

            if(radacina->n == 0) // daca radacina are 0 chei, facem primul ei copil noua radacina
            {
                BArboreEnti *tm = radacina;
                if(radacina->efrunza)
                    radacina = NULL;
                else
                    radacina = radacina -> C[0]; // daca nu, o setam ca NULL
                delete tm;
            }
            return;
    }
    int get_succesor(int y)
    {
        if(radacina == NULL)
           {
               cout<<"B-Arbore gol";
               return y;
           }
        else {
            BArboreEnti *p = radacina->cautare(y); // daca gasim valoarea
            if(p == NULL)
            {
                cout<<"Nodul "<<y<<" nu se afla in arbore.";
                return y;
            }
            else
            {
                int i = 0;
                while(i<p->n and p->keys[i] != y) // cautam pozitia valorii din nodul curent
                    i++;
                if(p->efrunza == false)
                {
                    int succ = p->succesor(i);
                    return succ;
                }
                else
                {

                    if(i != p->n-1) // daca i < indexul valorii cea mai din dreapta val
                    {

                        return (p->keys[i+1]); // returnam urmatoarea cheie din nod
                    }

                    else
                    {   // daca i e pe poz cea mai din dreapta cautam in parinte

                        if(radacina->efrunza == true)
                            return y;
                        else
                        {
                            bool ok;
                            int succ = radacina->pt_succ(y, ok, succ);
                            if(ok == false)
                            {
                                cout<< "Valoarea "<< y <<" este maximul din arbore, deci nu avem succesor.";

                            }
                            else
                                return succ;
                            return y;
                        }
                }
            }
        }

    }
    }
    int get_predecesor(int y){
        if(radacina == NULL){
            cout<<"B-Arbore gol.";
            return y;
        }
        else{
            BArboreEnti *p = radacina->cautare(y); //cautam nodul in arbore
            if(p == NULL){
                cout<<"Nodul "<< y <<" nu se afla in B-Arbore.";
                return y;
            }
            else{
                int i = 0;
                while(i < p->n and p->keys[i] != y) i++;//cautam poz valorii din nodul curent

                if(p->efrunza == false){ // daca nu este frunza
                    int prd = p->predecesor(i);
                    return prd;
                }
                else{
                    if(i > 0) return (p->keys[i-1]); // daca e frunza si  si nu se afla pe prima poz din nod, pred e imd urmatorul
                    else{
                        if(radacina->efrunza == true) return y; // daca e pe prima poz de pe frunza
                        else{
                            bool ok;
                            int prd = radacina->pt_pred(y, ok, prd);
                            if(ok == false){
                                cout<<"Valoarea "<< y <<" e minimul din arbore, deci nu avem predecesor.";}
                            else return prd;
                            return y;
                        }
                    }
                }
            }
        }
    }


};


//------------inserare----------------//
void BArboreEnti::inserare_la_indemana(int y)
{
    int i = n - 1; // i va contine index-ul elementului cel mai din dreapta (elementului cel mai mare)

    if(efrunza == true) // daca nodul pe care ne aflam este frunza
    {
        while(i >=0 && keys[i] > y) // gasim locatia unde dorim ca noua cheie sa fie inserata
        {
            keys[i+1] = keys[i]; // mutam totate chile mai mari cu un loc mai sus
            i--;
        }

        keys[i+1] = y; // inseram noua cheie in locul corespunzator
        n++; // incrementam n-ul
    }
    else
    {
        while(i >= 0 && keys[i] > y) // cautam copilul care va primi o noua cheie
            i--;

        if(C[i+1] ->n == 2*B-1) // verif daca copilul gasit mai are sau nu loc
        {
            splitC(i+1, C[i+1]); // daca e full, facem split

            if(keys[i+1] < y) // mutam mijlocul cheii cu un loc mai sus, C[i] se imparte in 2; alegem care va avea noua cheie
                i++;

        }
        C[i+1]->inserare_la_indemana(y);
    }

}

void BArboreEnti::splitC(int i, BArboreEnti *w)
{
    BArboreEnti *nodnou = new BArboreEnti(w->B, w->efrunza); // un nou nod
    nodnou->n = B - 1; // nodnou va retine B-1 chei ale lui w

    for(int j=0; j< B-1; j++)
        nodnou->keys[j] = w-> keys[j+B]; // copiem ultimele B-1 chei ale lui w in nodnou

    if(w->efrunza == false)
    {
        for(int j=0; j < B; j++)
            nodnou->C[j] = w->C[j+B]; // copiem ultimii B copii ai lui w in nodnou
    }
    w->n=B-1; // decrementam nr de chei din w

    for(int j=n; j>= i+1; j--)
        C[j+1] = C[j]; // cream spatiu pt un nou copil

    C[i+1] = nodnou; // legam copilul de nodnou

    for(int j = n-1; j >=i; j--) // o cheie a lui w se va muta in nod
        keys[j+1] = keys[j]; // gasim locul unei noi chei si mutam toate cheile mai mari cu un loc mai sus

    keys[i] = w->keys[B-1]; // copiem cheia din mijlocul lui w in acest nod
    n++; // marim nr de chei

}
//------------------------//


//------------stergere------------------//
int BArboreEnti::gasesteCheie(int y)
{
    int index=0;
    while(index <= n - 1 && keys[index] < y)
        ++index;
    return index;
}

void BArboreEnti:: sterge(int y)
{
    int index = gasesteCheie(y);

    if(index < n && keys[index] == y)
    {
        if(efrunza)
            stergefrunza(index); // daca nodul gasit e frunza folosim functia pt stergere de frunze
        else
            stergenod(index); // daca nu, folosim functia de stergere de nod
    }
    else
    {
        if(efrunza) // daca nodul in care am ajuns este frunza, inseamna ca arborele nu contine cheia pe care o cautam
        {
            cout << "Cheia " << y << " nu se regaseste in B-Arborele nostru.";
            return;
        }

        bool ok = ( (index==n)? true : false); // daca cheia e prezenta in subarborele care are ca radacina ultimul copil al nodului curent => true; altfel => false
        if(C[index]->n < B) // daca locul unde ar trebui sa fie cheia noasra permite inserarea acesteia, atunci folosim fct umple
            umple(index);

        if(ok && index > n) // daca ultimului copil i-a fost facut merge-ul cu predecesorul sau => operam C[index-1]
            C[index - 1]->sterge(y);
        else
            C[index]->sterge(y); // daca nu, C[index]


    }
    return;
}
void BArboreEnti::stergefrunza(int index)
{
    for(int i = index + 1; i < n; ++i) // mutam toate cheile de dupa poizitia index-ului in spate cu 1
        keys[i - 1] = keys[i];

    n--; // decrementam n-ul

    return;
}

void BArboreEnti::stergenod(int index)
{
    int y;
    y = keys[index];

    if(C[index]->n >= B) // daca copilul are cel putin B chei, gasim predecesorul sau; inlocuim y cu predecesorul
    {
        int predec;
        predec=predecesor(index);
        keys[index] = predec;
        C[index]-> sterge(predec); // stergem predecesorul
    }
    else if(C[index + 1]->n >= B) // daca copilul are mai putin de B chei, verificam daca C[index+ 1] are cel putin B chei
    {
        int succes;
        succes=succesor(index); // identif succesorul
        keys[index] = succes;
        C[index + 1]-> sterge(succes); // stergem succesorul
    }
    else // cand C[index] si C[index +1] au ambii <B chei,
    {
        merge(index); // facem merge cu y si cu C[index + 1] in C[index]
        C[index]->sterge(y); // C[index] are 2*B-1 chei(cf form) si putem sa stergem y din C[index]
    }
    return;

}
int BArboreEnti::succesor(int index)
{
    BArboreEnti *curent = C[index + 1];
    while(curent->efrunza == false)// cat timp nodul curent nu este frunza
        curent = curent->C[0];// continuam parcurgerea arborelui spre stangfa pana gasim o frunza
    return curent-> keys[0]; // prima cheie dintre frunze
}
int BArboreEnti::predecesor(int index)
{
    BArboreEnti *curent = C[index];
    while(curent->efrunza == false) // cat timp nodul curent nu este frunza
        curent = curent -> C[curent->n]; // continuam parcurgerea arborelui spre dreapta pana gasim o frunza
    return curent->keys[curent->n-1]; // ultima cheie dintre frunze
}
void BArboreEnti::umple(int index)
{
    if(index!=0 && C[index - 1]-> n>=B) // daca C[index-1] are mai mult de B-1 chei
        imprumutpred(index); //, imprumutam o cheie de la acel copil
    else if(index!=n && C[index + 1]->n>=B) // altfel, daca C[index+1] are mai mult de B-1 chei
        imprumutsucc(index); //, imprumutam o cheie de la acel copil
    else
    {
        if(index != n)
            merge(index); //altfel, daca C[index] este ultimul copil, facem merge cu predecesor
        else
            merge(index - 1); // daca C[index] nu este ultimul copil, facem merge cu succesor
    }
    return;

}

void BArboreEnti:: imprumutsucc(int index)
{
    BArboreEnti *copil = C[index];
    BArboreEnti *sibling = C[index + 1];

    // se insereaza keys[index] ca ultima cheie in C[index]
    copil->keys[(copil-> n)] = keys[index];

    if(!copil->efrunza) // daca copil nu e frunza
        copil->C[(copil->n) + 1] = sibling->C[0]; //inseram primul copil al rudei ca ultim copil pentru C[index]

    keys[index] = sibling->keys[0]; // prima cheie din sibling se insereaza in keys[index]


    for(int i=1; i < sibling->n; ++i) // mutam toate cheile din sibling cu un loc mai joc
        sibling->keys[i - 1] = sibling->keys[i];

    if(!sibling->efrunza) // daca sibling nu e frunza
    {
        for(int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling -> C[i]; // mutam toti pointerii sai cu un loc mai jos
    }

    copil->n += 1;
    sibling->n -= 1;
    return;
}


void BArboreEnti:: imprumutpred(int index)
{
    BArboreEnti *copil = C[index];
    BArboreEnti *sibling = C[index - 1];

    // ultima cheie din C[index - 1] urca la parinte
    // key[index -1] de la parinte se insereaza ca prima cheie in C[index]
    // copil castiga o cheie, iar ruda sa pierde una

    for(int i=copil->n-1; i>=0; --i) // mutam toate cheile in C[index] cu un loc mai in sus
        copil->keys[i + 1] = copil->keys[i];

    if(!copil->efrunza) // daca copil nu e frunza
    {
        for(int i = copil->n; i >= 0; --i)
            copil->C[i + 1] = copil->C[i]; // mutam toti pointerii sai cu un loc mai sus
    }

    copil->keys[0] = keys[index - 1]; // punem prima cheie a copilului sa fie egala cu keys[index-1] a nodului curent


    if(!copil->efrunza)
        copil->C[0] = sibling->C[sibling->n]; // punem ultimul copil al rudei ca primul copil al lui C[index]

    keys[index - 1] = sibling->keys[sibling-> n - 1]; // mutam cheile din sibling in parinte, dar se reduce nr de chei din sibling
    copil->n += 1;
    sibling->n -= 1;
    return;
}

void BArboreEnti::merge(int index)
{
    BArboreEnti *copil = C[index];
    BArboreEnti *sibling = C[index + 1];

    copil->keys[B - 1] = keys[index]; // luam o cheie din nodul curent si o inseram pe a B-1 pozitie a C[index]

    for(int i = 0; i<sibling->n; ++i) // copiem toate cheile din C[index + 1] la sf lui C[index]
        copil->keys[i + B] = sibling->keys[i];

    if(!copil->efrunza) // daca copil nu e frunza
    {
        for(int i = 0; i <= sibling->n; ++i)
            copil->C[i + B] = sibling->C[i]; // copiem copiii pointer din C[index + 1] in C[index]
    }

    for(int i = index + 1; i < n; ++i)
        keys[i - 1] = keys[i]; // mutam toate cheile de dupa index in nodul curent cu un pas mai inainte pt a umple spatiul creat de mutarea anterioara a cheilor

    for(int i = index + 2; i <= n; ++i)
        C[i - 1] = C[i]; // mutam toti copiii pointeri dupa index+1 in nodul curent cu un pas mai inainte

    copil->n += sibling->n+1; // rearanjam nr de chei
    n--;

    delete(sibling); // dezalocam memoria ocupata de sibling
    return;
}
//------------------------------------------------//



int main()
{
    int optiune;
    cout<<"Optiuni disponibile:"<<endl;
    cout<<"1 - Inserarea in B-Arbore. "<<endl;
    cout<<"2 - Stergerea din B-Arbore. "<<endl;
    cout<<"3 - Cautarea unui anumit element in B-Arbore. "<<endl;
    cout<<"4 - Afisarea elementelor din B-Arbore sortat.   "<<endl;
    cout<<"5 - Afisarea succesorului unui nod dat. "<<endl;
    cout<<"6 - Afisarea predecesorului unui nod dat. "<<endl;
    cout<<"0 - Exit"<<endl;
    cout<<"Optiune: ";
    cin>>optiune;
    while(optiune){
        switch(optiune){
            case 1: {
                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);
                    cout << "Traseul este: ";
                    B.traversare();
                    break;

            }
            case 2: {

                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);
                        cout << "Traseul este: "<<endl;
                        B.traversare();
                        cout << endl;

                        B.sterge(5);
                        cout << "Traseul, dupa eliminarea lui 5, este: "<<endl;
                        B.traversare();
                        cout << endl;

                        B.sterge(12);
                        cout << "Traseul, dupa eliminarea lui 12, este: "<<endl;
                        B.traversare();
                        cout << endl;
                        break;
                    }
                    case 3: {
                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);

                        int y = 18;
                        (B.cautare(y) != NULL)? cout << "Nodul "<< y <<" a fost gasit in B-Arbore." : cout << "Nodul "<< y << " nu este prezent in B-Arbore.";
                        break;
                    }
                    case 4: {
                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);
                        cout<<"Elementele B-Arborelui au fost sortate astfel:"<< endl;
                        B.traversare();
                        break;
                    }
                    case 5: {
                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);
                        int y = 11;
                        int succe = B.get_succesor(y);
                        if(succe != y)
                            cout << succe;
                        break;
                    }
                    case 6: {
                        BArbore B(3);
                        B.insert(1);
                        B.insert(3);
                        B.insert(7);
                        B.insert(10);
                        B.insert(11);
                        B.insert(20);
                        B.insert(19);
                        B.insert(30);
                        B.insert(15);
                        B.insert(5);
                        int y = 15;
                        int prede = B.get_predecesor(y);
                        if(prede != y)
                            cout << prede;
                        break;
                    }


        }
        cout<<endl<<endl<<"Optiuni disponibile:"<<endl;
        cout<<"1 - Inserarea in B-Arbore. "<<endl;
        cout<<"2 - Stergerea din B-Arbore. "<<endl;
        cout<<"3 - Cautarea unui anumit element in B-Arbore. "<<endl;
        cout<<"4 - Afisarea elementelor din B-Arbore sortat. "<<endl;
        cout<<"5 - Afisarea succesorului unui nod dat. "<<endl;
        cout<<"6 - Afisarea predecesorului unui nod dat. "<<endl;
        cout<<"0 - Exit"<<endl;
        cout<<"Optiune: ";
        cin>>optiune;
    }


    return 0;
}
