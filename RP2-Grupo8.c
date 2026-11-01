#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

/*
GRUPO 8
VEREDA, JUAN FRANCISCO - GIVA, JULIAN

*******************************************************************************************************
|---------------------------------||-------------------||-------------------||-------------------|
|         |         ALTA          ||       BAJA        ||     EVOCAR EX     ||    EVOCAR FRA     |
|         |   Max     |   Med     ||   Max   |  Med    ||   Max   |  Med    ||   Max   |  Med    |
|---------|-----------|-----------||---------|---------||---------|---------||---------|---------|
|Arbol    |    0.5    |    0.5    ||   1.5   |  1.0    ||  12.0   |  6.4    ||  11.0   |   5.9   |
|------------------------------------------------------------------------------------------------|
|LSO      |   91.0    |   23.0    ||  91.0   |  22.4   || 100.0   |  45.1   ||  71.0   |  24.2   |
|------------------------------------------------------------------------------------------------|
|LI       |    46.0   |    12.0   ||  45.5   |  11.2   ||   14.0   |  11.2  ||   14.0   |  10.3  |
|---------|-----------|-----------||---------|---------||---------|---------||-------------------|
*******************************************************************************************************

ANALISIS:
Podemos ver que la mejor estructura en terminos generales es el arbol, aunque en la evocacion hay una mejor opcion.
A este le sigue la lista invertida, donde la triseccion hace un gran trabajo bajando los costos.
Finalmente tenemos la lista secuencial ordenada, la cual por su manera de trabajar es la mas costosa en todo momento.
1. ABB, 2. LI, 3. LSO


*******************************************************************************
*/

#define MAX 130
int elementoscont = 0; ///// LSO

//--------------------------------------------
float costoarbol=0.0;
float costoil=0.0;
float costolso=0.0;
float contaltaA=0.0, contbajaA=0.0, contevocarexA=0.0, contevocarfraA=0.0;
float contaltaLSO=0.0, contbajaLSO=0.0, contevocarexLSO=0.0, contevocarfraLSO=0.0;
float contaltaIL=0.0, contbajaIL=0.0, contevocarexIL=0.0, contevocarfraIL=0.0;
float altamaxA;
float altamedA;
float bajamaxA;
float bajamedA;

float altamaxLSO;
float altamedLSO;
float bajamaxLSO;
float bajamedLSO;

float evocarfracLSO;
float evocarfracmedLSO;
float evocarexitoLSO;
float evocarexitomedLSO;
float evocarfracIL;
float evocarfracmedIL;
float evocarexitoIL;
float evocarexitomedIL;
float evocarfracA;
float evocarfracmedA;
float evocarexitoA;
float evocarexitomedA;

float altamaxIL;
float altamedIL;
float bajamaxIL;
float bajamedIL;


float altamaxA=0.0, bajamaxA=0.0, evocarfracA=0.0, evocarexitoA=0.0;
float altamedA=0.0, bajamedA=0.0, evocarfracmedA=0.0, evocarexitomedA=0.0;
float altamaxLSO=0.0, bajamaxLSO=0.0, evocarfracLSO=0.0, evocarexitoLSO=0.0;
float altamedLSO=0.0, bajamedLSO=0.0, evocarfracmedLSO=0.0, evocarexitomedLSO=0.0;
float altamaxIL=0.0, bajamaxIL=0.0, evocarfracIL=0.0, evocarexitoIL=0.0;
float altamedIL=0.0, bajamedIL=0.0, evocarfracmedIL=0.0, evocarexitomedIL=0.0;


//---------------------------------------------







typedef struct{
    char codigo[8];
    char nombre[80];
    char mail[23];
    int nota;
    char condicion[9];
} alumno;

///////LSO//////////////
typedef struct{
    alumno alumnos[MAX];
} lista;

/////// abb //////////////
typedef struct Nodo {
    alumno ab;
    struct Nodo *izquierdo;
    struct Nodo *derecho;
} Nodo;

typedef Nodo* Arbol;

typedef struct Pack {
    struct Nodo *actual;
    struct Nodo *padre;
} Pack;

typedef struct {
    Nodo* nodo;
    struct Pila* siguiente;
}Pila;
/////////////////////////
/////// IL //////////
typedef struct{
    alumno *al[MAX];
    int ult;
}InvertedList;
////////////////////

//COMPARAR
int comparar(char parametro[], char delista[]){
    int i=0;
    char aux1[120], aux2[120];
    //Correccion se colgaba por comparar afuera del arreglo
    while (*parametro && *delista && i<120) {
            aux1[i]=toupper(parametro[i]);
            aux2[i]=toupper(delista[i]);
        // Convertimos ambos caracteres a min�sculas para compararlos
       i++;
    }
    if (strcmp(aux1, aux2)!=0) {
        return 1; // Si no son iguales, retornamos 1
    }
    return (*parametro == *delista) ? 0 : 1;
}
/////////LSO/////////////

void localizar(lista a, char codigo[], int *pos, int *exito){
    *exito = 0;
    costolso = 0.0;
    int i = 0;
    while ((i < elementoscont) && (strcasecmp((a.alumnos[i].codigo),codigo)<0)){
        i++;
        costolso +=1.0;
    }
    *pos = i;

    if(i<elementoscont)     //salio por comparacion de codigo, ecnontró el elemento o es mayor
        costolso += 1;

    if (i < elementoscont && strcasecmp(a.alumnos[i].codigo,codigo)==0)
        *exito = 1;
    else
        *exito = 0;
}

void baja(lista *a,alumno al, int *exito){
    int pos = 0;
    int confirmacion = 0;
    if(elementoscont>0){     // ESTRUCTURA VACIA
            localizar(*a, al.codigo, &pos, exito);
            costolso = 0.0;
            if(*exito == 1){
                if(strcasecmp(al.codigo,a->alumnos[pos].codigo)==0 && strcasecmp(al.nombre,a->alumnos[pos].nombre)==0 && strcasecmp(al.mail,a->alumnos[pos].mail)==0 && (al.nota == a->alumnos[pos].nota) && strcasecmp(al.condicion,a->alumnos[pos].condicion)==0){
                confirmacion= 1;
            }
                if(confirmacion == 1){
                    if(pos != (MAX + 1)){
                        while (pos < elementoscont-1){
                            a->alumnos[pos] = a->alumnos[pos + 1];
                            pos++;
                            costolso +=1.0;
                        }
                        elementoscont--;
                    }
                    *exito = 1;
                }else{
                    *exito = 0;
                }
            }
    }else{
            *exito = 0; // SI ESTA VACIA, EXITO = 0
        }
}

void alta(lista *a, alumno x, int *exito){
    int pos=0, i=0;
    localizar(*a, x.codigo, &pos, exito);
    costolso = 0.0;
    if(*exito == 1){
        *exito = 0;
    }else{
        i = elementoscont;
            if (i < MAX){
                while (i > pos){
                    a->alumnos[i] = a->alumnos[i-1];
                    i--;
                    costolso +=1.0;
                }
                a->alumnos[pos] = x;
                a->alumnos[pos] = x;
                *exito = 1;
                elementoscont += 1;
            }else{
                *exito = 2;
            }
    }
}

void evocar(lista a, char codigo[], alumno *alum, int *exito){
    costolso = 0.0;
    int pos;
    if(elementoscont>0){     // ESTRUCTURA VACIA
        localizar(a, codigo, &pos, exito);

        if (*exito==1){
            *alum = a.alumnos[pos];
            *exito = 1;
        }
    }else{
        *exito = 0;   // SI ESTA VACIA, EXITO = 0
    }
}

/////////////////////////
/////////OP IL///////////
void initIL(InvertedList *IL){
    (*IL).ult=0;
}

void vaciarInvertedList(InvertedList *IL) {
    for (int i = 0; i <= IL->ult; i++) {
        free(IL->al[i]); // Liberar cada prestador en el array
        IL->al[i] = NULL;
    }
    IL->ult = 0;
}

//BUSQUEDA POR TRISECCION
// limite inferior inclusivo, limite superior inclusivo y segmento mas grande a la izquierda
void localizarIL(InvertedList IL, int *t, char codigo[], int *exito){
    int li = 0, ls = IL.ult-1;
    costoil = 0.0;
    *exito = 0;

    if (IL.ult!=0){
        *t = ceil((li+ls)/2.0);
        while ((li<=ls) && (strcasecmp((*IL.al[*t]).codigo,codigo)!=0)){
            if((strcasecmp((*IL.al[*t]).codigo,codigo))<0){ ////////// CORRECION REC -->  <0: string1 es menor que string2
                li = *t+1;
            }
            else{
                ls = *t - 1;
            }

            costoil += 2.0;


            *t = ceil((li+ls)/2.0);

        }
        if(*t >= 0 && *t <= MAX-1){
            if(strcasecmp((*IL.al[*t]).codigo,codigo)==0){
            costoil += 2.0;
            *exito = 1;
            return;
            }
        }

        else{
            *t = 0;
            *exito = 0;
        }
    }
    else{       //IL está vacía
        *exito = 0;
        *t = 0;
    }
}

void altaIL(InvertedList* IL,alumno a,int *exito){
    int t,j;

    if(IL->ult==(MAX)){ // ESTA LLENA
        *exito = 0;
    }
    else{
        localizarIL(*IL,&t,a.codigo,exito);
        costoil=0.0;

        if(*exito==1){  // YA ESTA EN LA LISTA
            *exito = 0;
        }
        else{
            alumno *nuevo=(alumno*)malloc(sizeof(alumno));
            if(nuevo==NULL){
                *exito = 0;
            }
            else{
                *nuevo=a;

                for(j=(*IL).ult;j>t;j--){
                    (*IL).al[j]=(*IL).al[j-1];
                    costoil += 0.5;
                }
                (*IL).al[t]=nuevo;
                (*IL).ult++;
                costoil += 0.5;   /// CORRECCION REC, CUENTA POR ASIGNACION
                *exito = 1;
            }
        }
    }
}

void bajaIL(InvertedList* IL,alumno a,int *exito){
    int t;
    localizarIL(*IL,&t,a.codigo,exito);
    costoil = 0.0;

        if(*exito==1){
            if(strcasecmp(a.codigo,(*IL).al[t]->codigo)==0 &&
                strcasecmp(a.nombre,(*IL).al[t]->nombre)==0 && strcasecmp(a.mail,(*IL).al[t]->mail)==0 &&
                (a.nota == (*IL).al[t]->nota) && strcasecmp(a.condicion,(*IL).al[t]->condicion)==0){
                free((*IL).al[t]);
                IL->ult--;
                for(int i = t; i<(*IL).ult;i++){
                    costoil += 0.5;
                    IL->al[i] = IL->al[i+1];
                }

                IL->al[IL->ult] = NULL;
                *exito = 1;
            }
            else        ///no coinciden todos los campos de la nupla
                *exito = 0;
        }
        else            ///no se encuentra en la lista
            *exito = 0;

}

void evocarIL2(InvertedList IL, char codigo[], alumno *a, int *exito) {
    int pos; // Posición donde se encuentra  el prestador
    *exito = 0; // Inicializamos como no encontrado
    costoil = 0.0;
    localizarIL(IL, &pos, codigo, exito);
    if (*exito == 1) {
        *exito = 1;
        *a = (*IL.al[pos]);
        return;
    }
}


////////////////////////////////////////////////////////////////////////////////
//////////OP ABB///////////////
//poner un nodo en la pila
void push(Pila** pilav, Nodo* nodo) {
    Pila* nuevoElemento = (Pila*)malloc(sizeof(Pila));
    if(nuevoElemento==NULL){
        return;
    }
    nuevoElemento->nodo = nodo;
    nuevoElemento->siguiente = *pilav;
    *pilav = nuevoElemento;
}

//sacar un nodo de la pila
Nodo* pop(Pila** pilav) {
    Pila* temp = *pilav;
    Nodo* nodo = temp->nodo;
    *pilav = temp->siguiente;
    free(temp);
    return nodo;
}

void preorden(Nodo* arbol) {
    Pila* pila = NULL;
    int contadorPila=0;
    // Si el �rbol no est� vac�o, comenzar con la ra�z
    if (arbol != NULL){
        push(&pila, arbol);
    }
    int z=0;

    printf("****************************************\n");
    printf("****** Arbol Binario de Busqueda *******\n");

    while (pila != NULL) {
        // Obtener el nodo actual y procesarlo
        Nodo* nodoActual = pop(&pila);
        printf("****************************************\n");
        printf("CODIGO: %s \n" ,nodoActual->ab.codigo);
        printf("Nombre: %s \n" ,nodoActual->ab.nombre);
        printf("Mail: %s \n" ,nodoActual->ab.mail);
        printf("Nota: %d \n" ,nodoActual->ab.nota);
        printf("Condicion: %s \n" ,nodoActual->ab.condicion);
        if(nodoActual->izquierdo!=NULL){
            printf("CODIGO hijo izquierdo: %s \n" ,nodoActual->izquierdo->ab.codigo);
        }
        if(nodoActual->derecho!=NULL){
            printf("CODIGO hijo derecho: %s \n" ,nodoActual->derecho->ab.codigo);
        }
        printf("****************************************\n");
        if((z+1)%5==0){
            printf("Presione Enter para mostrar mas...\n");
            fflush(stdin);
            getchar();
            system("cls");
        }
        // Primero empujamos el hijo derecho, luego el izquierdo
        if (nodoActual->derecho != NULL) push(&pila, nodoActual->derecho);
        if (nodoActual->izquierdo != NULL) push(&pila, nodoActual->izquierdo);
        z++;
    }

}

void vaciarArbol(Arbol *arbol) {
    if (*arbol != NULL) {
        vaciarArbol(&((*arbol)->izquierdo));
        vaciarArbol(&((*arbol)->derecho));
        free(*arbol);
        *arbol = NULL; // Reinicia la raíz
    }
}

Pack localizararbol(Nodo* raiz, char codigo[], int *exito, Nodo* padre) {
    Nodo* pos;
    Pack datos;
    pos = raiz;
    costoarbol = 0.0;
    while(pos!=NULL && strcasecmp(pos->ab.codigo,codigo)!=0){
        padre = pos;
        if (strcasecmp(codigo,pos->ab.codigo)<0) {
            pos = pos->izquierdo;
            costoarbol += 1.0;
        } else {
            pos = pos->derecho;
            costoarbol += 1.0;
        }
    }
    datos.padre=padre;
    datos.actual=pos;
    if(pos==NULL){
        *exito=0;
        return datos;// Nodo no encontrado y devuelve el padre de pos
    }else{
        costoarbol += 1.0; // CORRECCION REC, CUENTA LA ULTIMA COMPARACION
        *exito=1;
        return datos;//Nodo encontrado y devuelve la pos donde esta
    }

}

void altaarbol(Arbol* arbol, alumno alum, int *exito){
    Nodo* pos;
    Nodo* aux;
    Nodo* padre = NULL;
    Pack datos;
    datos = localizararbol(*arbol, alum.codigo, exito, padre);
    costoarbol = 0.0;
    pos=datos.padre;
    if(*exito==1){
        *exito=0;
        return;
    }
    if(*exito==0){
        if(*arbol==NULL){
            *arbol=(Nodo*)malloc(sizeof(Nodo));
            if (*arbol == NULL) {  // Verificar si malloc fall�
                *exito = 0;
                return;
            }
            (*arbol)->ab=alum;
            costoarbol += 0.5;
            (*arbol)->izquierdo = (*arbol)->derecho = NULL;
            *exito = 1;
        }
        else if(strcasecmp(alum.codigo, pos->ab.codigo)<0) {
            aux = (Nodo*)malloc(sizeof(Nodo));
            if (aux==NULL) {  // Verificar si malloc fall�
                *exito = 0;
            }else{//si no falla se sigue
                pos->izquierdo = aux;
                costoarbol += 0.5;
                aux->ab = alum;
                aux->izquierdo = NULL;
                aux->derecho = NULL;
                *exito = 1;
            }
        }else if(strcasecmp(alum.codigo, pos->ab.codigo)>0) {
            aux = (Nodo*)malloc(sizeof(Nodo));
            if (aux == NULL) {  // Verificar si malloc fall�
                *exito = 0;
            }else{//si no falla se sigue
                pos->derecho = aux;
                costoarbol += 0.5;
                aux->ab = alum;
                aux->izquierdo =NULL;
                aux->derecho = NULL;
                *exito = 1;
            }
        }
    }
}

void bajaarbol(Arbol *arbol, alumno alum, int *exito){
    int confirmacion = 0;
    Nodo* pos;
    Nodo* padre= NULL;
    Pack datos;
    datos=localizararbol(*arbol, alum.codigo, exito, padre);
    costoarbol = 0.0;
    pos=datos.actual;
    padre=datos.padre;

    // Buscar el nodo a eliminar y su padre
    if(*exito==0){
        return; // Nodo no encontrado
    }else{
        if(strcasecmp(alum.codigo,pos->ab.codigo)==0 && strcasecmp(alum.nombre,pos->ab.nombre)==0 && strcasecmp(alum.mail,pos->ab.mail)==0 && (alum.nota == pos->ab.nota) && strcasecmp(alum.condicion,pos->ab.condicion)==0){
            confirmacion= 1;
        }

        if(confirmacion == 1){
                if(pos->izquierdo==NULL&&pos->derecho==NULL){ // es una hoja
                    if(padre == NULL) { // es la ra�z
                        *arbol = NULL;
                        costoarbol += 0.5;
                        ///+0.5 CORREC
                    }else
                        if (padre->izquierdo == pos) {
                            padre->izquierdo = NULL;
                            costoarbol += 0.5;
                        }else {
                            padre->derecho = NULL;
                            costoarbol += 0.5;
                        }
                free(pos);
            }else if(pos->izquierdo==NULL){// solo tiene hijo derecho
                if(padre==NULL){ // es la ra�z
                    *arbol=pos->derecho;
                    costoarbol += 0.5;
                }else if (padre->izquierdo == pos) {
                    padre->izquierdo = pos->derecho;
                    costoarbol += 0.5;
                }else {
                    padre->derecho = pos->derecho;
                    costoarbol += 0.5;
                }
                free(pos);
            }else if (pos->derecho == NULL) { // solo tiene hijo izquierdo
                if (padre == NULL) { // es la ra�z
                    *arbol = pos->izquierdo;
                    costoarbol += 0.5;

                } else if (padre->izquierdo == pos) {
                    padre->izquierdo = pos->izquierdo;
                    costoarbol += 0.5;
                } else {
                    padre->derecho = pos->izquierdo;
                    costoarbol += 0.5;
                }
                free(pos);
            }else{ // tiene ambos hijos     // Mayor de los menores

                Nodo* mayorDeMenores = pos->izquierdo;
                Nodo* padreMayorDeMenores = pos;
                while (mayorDeMenores->derecho != NULL) { ///////// CORRECCION REC: DERECHO
                    padreMayorDeMenores = mayorDeMenores;
                    mayorDeMenores = mayorDeMenores->derecho; /////  CORRECCION REC: DERECHO
                }

                pos->ab = mayorDeMenores->ab;
                costoarbol += 1;      ///esto estaba en +1 costo, modificacion de punteros = 0.5

                if (padreMayorDeMenores->izquierdo == mayorDeMenores) {         ///si el hijo izq del que elimino no tiene hijos:
                    padreMayorDeMenores->izquierdo = mayorDeMenores->izquierdo;
                    costoarbol += 0.5;
                }
                else {
                    padreMayorDeMenores->derecho = mayorDeMenores->izquierdo; ///si entró al bucle
                    costoarbol += 0.5;
                }

                free(mayorDeMenores);
            }

            *exito = 1;
        }else{
            *exito = 0;
        }
    }
}

void evocarArbol(Arbol* arbol, char codigo[], alumno *al, int *exito){
    costoarbol = 0.0;
    Nodo* pos;
    Nodo* padre;
    Pack datos;
    datos = localizararbol(*arbol, codigo, exito, padre);
    pos=datos.actual;
    if (*exito==1){
        *al = pos->ab;
    }
}

int Lecturadatos(lista *lso,InvertedList *IL,Arbol *arbol){
    FILE *fp;
    alumno aux;
    int exito=0;
    int selector=0;
    int testigo = 0;
    alumno alum;

    if((fp = fopen("operaciones-copia.txt", "r"))==NULL){
        return 0;
    }else {
    while (!( feof (fp))){
        fscanf(fp," %d",&selector);
        switch(selector){
            case 1:                                                     //ALTA

                fscanf( fp , " %[^\n] " ,aux.codigo );
                fscanf( fp , " %[^\n] " ,aux.nombre );
                fscanf( fp , " %[^\n] " ,aux.mail );
                fscanf(fp," %d",&aux.nota);
                fscanf( fp , " %[^\n] ",aux.condicion );

                costolso = 0.0;
                alta(lso,aux, &exito);       //alta LSO
                if(exito==1){
                    contaltaLSO++;
                    altamedLSO += costolso;
                    if(costolso > altamaxLSO)
                        altamaxLSO = costolso;
                }
                costolso = 0.0;

                costoil = 0.0;
                altaIL(IL, aux, &exito);       //alta IL
                if(exito == 1){
                    contaltaIL++;
                    altamedIL += costoil;
                    if(costoil>altamaxIL)
                        altamaxIL = costoil;
                }
                costoil = 0.0;

                costoarbol = 0.0;
                altaarbol(arbol,aux, &exito);       //alta A
                if(exito == 1){
                    contaltaA++;
                    altamedA += costoarbol;
                    if(costoarbol > altamaxA)
                        altamaxA = costoarbol;
                }
                costoarbol = 0.0;

                break;

            case 2:                                                     //BAJA

                fscanf( fp , " %[^\n] " ,aux.codigo );
                fscanf( fp , " %[^\n] " ,aux.nombre );
                fscanf( fp , " %[^\n] " ,aux.mail );
                fscanf(fp," %d",&aux.nota);
                fscanf( fp , " %[^\n] ",aux.condicion );

                costolso = 0.0;
                baja(lso,aux,&exito);               ///costos baja lso
                if(exito == 1){
                    contbajaLSO++;
                    bajamedLSO += costolso;

                    if(costolso > bajamaxLSO)
                        bajamaxLSO = costolso;
                }

                costolso = 0.0;


                costoil = 0.0;
                bajaIL(IL, aux,&exito);               ///costos baja IL
                if(exito==1){
                    contbajaIL+=1.0;
                    bajamedIL += costoil;
                    if(costoil > bajamaxIL)
                        bajamaxIL = costoil;
                }
                costoil = 0.0;

                costoarbol = 0.0;
                bajaarbol(arbol, aux, &exito);               ///costos baja arbol
                if(exito==1){
                    contbajaA+=1.0;
                    bajamedA += costoarbol;
                    if(costoarbol > bajamaxA)
                        bajamaxA=costoarbol;
                }
                costoarbol = 0.0;
                break;

            case 3:                                                     //EVOCAR

                fscanf(fp," %[^\n]",&aux.codigo);

                costolso = 0.0;
                evocar(*lso, aux.codigo, &alum, &exito);            ///costos evocacion de lso
                if(exito==1){
                    contevocarexLSO+=1.0;
                    evocarexitomedLSO+=costolso;
                    if(evocarexitoLSO<costolso)
                        evocarexitoLSO=costolso;
                }
                else{
                    if(evocarfracLSO<costolso){
                        evocarfracLSO=costolso;
                    }
                    evocarfracmedLSO+=costolso;
                    contevocarfraLSO+=1.0;
                }
                costolso = 0.0;


                costoil = 0.0;
                evocarIL2(*IL, aux.codigo, &alum, &exito);          ///costos evocacion de invertedList
                if(exito==1){
                    contevocarexIL+=1.0;
                    evocarexitomedIL+=costoil;
                    if(evocarexitoIL<costoil)
                        evocarexitoIL=costoil;
                }
                else{
                    if(evocarfracIL<costoil){
                            evocarfracIL=costoil;
                        }
                    evocarfracmedIL+=costoil;
                    contevocarfraIL+=1.0;
                }
                costoil = 0.0;

                costoarbol = 0.0;
                evocarArbol(arbol, aux.codigo, &alum, &exito);              ///Costos evocacion de Arbol
                if(exito==1){
                    contevocarexA+=1.0;
                    evocarexitomedA+=costoarbol;
                    if(evocarexitoA<costoarbol)
                        evocarexitoA=costoarbol;
                }
                else{
                    if(evocarfracA<costoarbol){
                            evocarfracA=costoarbol;
                        }
                    evocarfracmedA+=costoarbol;
                    contevocarfraA+=1.0;
                }
                break;
            default:
                break;
        }
    }

    ////////////////////////////////////////

    if(contaltaA!=0.0)
        altamedA= (altamedA/contaltaA);

    if(contbajaA!=0.0)
        bajamedA=(bajamedA/contbajaA);

    if(contevocarexA!=0.0)
        evocarexitomedA=(evocarexitomedA/contevocarexA);

    if(contevocarfraA!=0.0)
        evocarfracmedA=(evocarfracmedA/contevocarfraA);

    ////////////////////////////////////////
    if(contaltaLSO!=0.0)
        altamedLSO=(altamedLSO/contaltaLSO);

    if(contbajaLSO!=0.0)
        bajamedLSO=(bajamedLSO/contbajaLSO);

    if(contevocarexLSO!=0.0)
        evocarexitomedLSO=(evocarexitomedLSO/contevocarexLSO);

    if(contevocarfraLSO!=0.0)
        evocarfracmedLSO=(evocarfracmedLSO/contevocarfraLSO);

    ////////////////////////////////////////
    if(contaltaIL!=0.0)
        altamedIL=(altamedIL/contaltaIL);

    if(contbajaIL!=0.0)
        bajamedIL=(bajamedIL/contbajaIL);

    if(contevocarexIL!=0.0)
        evocarexitomedIL=(evocarexitomedIL/contevocarexIL);

    if(contevocarfraIL!=0.0)
        evocarfracmedIL=(evocarfracmedIL/contevocarfraIL);

    ////////////////////////////////////////

         fclose( fp );

    return 1;
    }
}

void vaciarEstructuras(Arbol *ar, lista *lso, InvertedList *il){
    int h, aux;

    //LSO - vaciar
    aux = elementoscont;
    if(elementoscont != 0){
        for(h=0;h<aux;h++){
            strcpy(lso->alumnos[h].codigo, "");
            strcpy(lso->alumnos[h].condicion, "");
            strcpy(lso->alumnos[h].mail, "");
            strcpy(lso->alumnos[h].nombre, "");
            lso->alumnos[h].nota = -1;
        }
        elementoscont = 0;      ///se elimino el +1 en el for del aux y se elimino el elementoscont -1;
    }

    vaciarArbol(ar);
    vaciarInvertedList(il);






    costoarbol=0.0;
    costoil=0.0;
    costolso=0.0;
    contaltaA=0.0; contbajaA=0.0; contevocarexA=0.0; contevocarfraA=0.0;
    contaltaLSO=0.0; contbajaLSO=0.0; contevocarexLSO=0.0; contevocarfraLSO=0.0;
    contaltaIL=0.0; contbajaIL=0.0; contevocarexIL=0.0; contevocarfraIL=0.0;
    altamaxA = 0.0;
    altamedA = 0.0;
    bajamaxA = 0.0;
    bajamedA = 0.0;

    altamaxLSO = 0.0;
    altamedLSO = 0.0;
    bajamaxLSO = 0.0;
    bajamedLSO = 0.0;

    evocarfracLSO = 0.0;
    evocarfracmedLSO = 0.0;
    evocarexitoLSO = 0.0;
    evocarexitomedLSO = 0.0;
    evocarfracIL = 0.0;
    evocarfracmedIL = 0.0;
    evocarexitoIL = 0.0;
    evocarexitomedIL = 0.0;
    evocarfracA = 0.0;
    evocarfracmedA = 0.0;
    evocarexitoA = 0.0;
    evocarexitomedA = 0.0;

    altamaxIL = 0.0;
    altamedIL = 0.0;
    bajamaxIL = 0.0;
    bajamedIL = 0.0;


    altamaxA=0.0; bajamaxA=0.0; evocarfracA=0.0; evocarexitoA=0.0;
    altamedA=0.0; bajamedA=0.0; evocarfracmedA=0.0; evocarexitomedA=0.0;
    altamaxLSO=0.0; bajamaxLSO=0.0; evocarfracLSO=0.0; evocarexitoLSO=0.0;
    altamedLSO=0.0; bajamedLSO=0.0; evocarfracmedLSO=0.0; evocarexitomedLSO=0.0;
    altamaxIL=0.0; bajamaxIL=0.0; evocarfracIL=0.0; evocarexitoIL=0.0;
    altamedIL=0.0; bajamedIL=0.0; evocarfracmedIL=0.0; evocarexitomedIL=0.0;
}



int main(){
    int opc, x, carga=0;
    int h, auxx, exiito = 0;


    lista lso;

    InvertedList IL;
    initIL(&IL);

    Arbol arbol = NULL;
    //vaciarArbol(&arbol);

int menu=0;
    while(opc!=5){
        printf("****************************************\n"
               "[1] Cargar estructuras y comparacion\n"
               "[2] Mostrar LSO\n"
               "[3] Mostrar LI \n"
               "[4] Mostrar ABB \n"
               "[5] Salir\n"
              "****************************************\n");
              scanf(" %d", &opc);
        switch(opc){
            case 1:
                vaciarEstructuras(&arbol, &lso, &IL);
                opc = 0;
                    menu = Lecturadatos(&lso, &IL, &arbol);
                if (menu == 0) {
                    printf("Error al abrir el archivo.\n");
                } else {
                    system("cls");
                    printf("Datos procesados correctamente.\n");
                }

                printf("*******************************************************************************************************\n");
                printf("|---------------------------------||-------------------||-------------------||-------------------|\n");
                printf("|         |         ALTA          ||       BAJA        ||     EVOCAR EX     ||    EVOCAR FRA     |\n");
                printf("|         |   Max     |   Med     ||   Max   |  Med    ||   Max   |  Med    ||   Max   |  Med    |\n");
                printf("|---------|-----------|-----------||---------|---------||---------|---------||---------|---------|\n");
                printf("|Arbol    |    %.1f    |    %.1f    ||   %.1f   |  %.1f    ||  %.1f   |  %.1f    ||  %.1f   |   %.1f   |\n", altamaxA, altamedA, bajamaxA, bajamedA, evocarexitoA, evocarexitomedA, evocarfracA, evocarfracmedA);
                printf("|------------------------------------------------------------------------------------------------|\n");
                printf("|LSO      |   %.1f    |   %.1f    ||  %.1f   |  %.1f   || %.1f   |  %.1f   ||  %.1f   |  %.1f   |\n", altamaxLSO, altamedLSO, bajamaxLSO, bajamedLSO, evocarexitoLSO, evocarexitomedLSO, evocarfracLSO, evocarfracmedLSO);
                printf("|------------------------------------------------------------------------------------------------|\n");
                printf("|LI       |    %.1f   |    %.1f   ||  %.1f   |  %.1f   ||   %.1f  |  %.1f   ||   %.1f   |  %.1f  |\n", altamaxIL, altamedIL, bajamaxIL, bajamedIL, evocarexitoIL, evocarexitomedIL, evocarfracIL, evocarfracmedIL);
                printf("|---------|-----------|-----------||---------|---------||---------|---------||-------------------|\n");
                printf("*******************************************************************************************************\n");
                printf("Presione enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");

                break;
            case 2:
                x=0;
                if(elementoscont>0){
                        for(x=0; x< elementoscont; x++){
                            printf("****************************************\n");
                            printf("Codigo: %s \n" ,lso.alumnos[x].codigo);
                            printf("Nombre: %s \n" ,lso.alumnos[x].nombre);
                            printf("Mail: %s \n" ,lso.alumnos[x].mail);
                            printf("Nota: %d \n" ,lso.alumnos[x].nota);
                            printf("Condicion: %s \n" ,lso.alumnos[x].condicion);
                            printf("****************************************\n");
                            if((x+1)% 5 == 0 && (x+1)<elementoscont){
                                printf("ALUMNOS %d/%d\n"
                                    "****************************************\n",(x+1),elementoscont);
                                printf("Presione Enter para mostrar mas...\n");
                                fflush(stdin);
                                getchar();
                                system("cls");
                            }
                        }
                }else{
                        printf("****************************************\n"
                            "No hay elementos en la lista\n"
                            "****************************************\n");
                    }
                    printf("Presione enter para continuar...\n");
                    fflush(stdin);
                    getchar();
                    system("cls");

                break;
            case 3:
                if(IL.ult>0){
                    printf("\n=== LISTA INVERTIDA DE PRESTADORES ===\n");
                    printf("Cantidad de prestadores: %d\n", IL.ult);
                    printf("------------------------------------\n");
                    for (int i = 0; i < IL.ult; i++) {
                        printf("[%d] CODIGO: %s\n", i+1, IL.al[i]->codigo);
                        printf("    Nombre: %s\n", IL.al[i]->nombre);
                        printf("    Email: %s\n", IL.al[i]->mail);
                        printf("    Nota: %d\n", IL.al[i]->nota);
                        printf("    Condicion: %s\n", IL.al[i]->condicion);

                        if ((i + 1) % 5 == 0 && i != IL.ult) {
                            printf("\n--- Presione Enter para continuar ---\n");
                            fflush(stdin);
                            getchar();
                            system("cls");
                        }

                    }
                }else{
                    printf("----- Lista Invertida vacia -----\n");
                    printf("Presione enter para continuar...\n");
                    fflush(stdin);
                    getchar();
                    system("cls");
                }

                break;
            case 4:
                if (arbol == NULL) {
                    printf("------------------------------------\n");
                    printf("El arbol esta vacio.\n");
                    printf("------------------------------------\n");
                    printf("Presione Enter para mostrar mas...\n");
                    fflush(stdin);
                    getchar();
                    system("cls");

                }else{
                    system("cls");
                    preorden(arbol);
                }
                break;
            default:
                break;

        }
    }

    return 0;
}
