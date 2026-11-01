/* GRUPO 13
    INTEGRANTES:
    Vereda, Juan Francisco
    Rodriguez, Jerónimo Martin
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
-----------------------------------------------------------------------
|---------------------|--------- RS1 ---------|--------- RS2 ---------|
|Evocacion Exito max  |         7.00          |          7.00         |
|Evocacion Exito med  |         3.24          |          3.21         |
|---------------------------------------------------------------------|
|Evocacion Fracaso max|         7.00          |          6.00         |
|Evocacion Fracaso med|         2.63          |          2.58         |
|---------------------------------------------------------------------|
|       Alta max      |         1.00          |          1.00         |
|       Alta med      |         1.00          |          1.00         |
|---------------------------------------------------------------------|
|       Baja max      |         2.00          |          2.00         |
|       Baja med      |         1.13          |          1.10         |
-----------------------------------------------------------------------
Cantidad altas exitosas: 266
Cantidad bajas exitosas: 187
Cantidad elementos en RS1: 79
Cantidad elementos en RS2: 79
Cantidad evocaciones exitosas: 430
Cantidad evocaciones fracasadas: 344

Podemos notar que el rebalse numero dos es apenas mejor que el rebalse numero 1, esto se debe a la
politica de reemplazo en las bajas, que hace que el costo de evocacion y bajas sea ligeramente menor en el rebalse numero 2
al reacomodar los nodos de manera mas eficiente al momento de efectuar una baja.

*/




#define MAXS 23 //maximo de prestadores de los rebalses   210/9.47 = 22.17 aprox
#define factorDeCarga 9.47  //factor de carga del rebalse

int exito = 0;
int contador = 0;
int cantAltasRS1 = 0;
int cantAltasRS2 = 0;
int cantBajasRS1 = 0;
int cantBajasRS2 = 0;
int cantEvocExit = 0;
int cantEvocFrac = 0;
float costoEvocarRS1=0.0, costoAltaRS1=0.0, costoBajaRS1=0.0;
float evocarFracMedRS1=0.0, evocarFracMaxRS1 = 0.0, evocarFracMinRS1 = 0.0, evocarExitoMedRS1=0.0, evocarExitoMaxRS1 = 0.0, evocarExitoMinRS1 = 0.0;
float altaFracMedRS1=0.0, altaFracMaxRS1 = 0.0, altaFracMinRS1 = 0.0, altaExitoMedRS1=0.0, altaExitoMaxRS1 = 0.0, altaExitoMinRS1 = 0.0;
float bajaFracMedRS1=0.0, bajaFracMaxRS1 = 0.0, bajaFracMinRS1 = 0.0, bajaExitoMedRS1=0.0, bajaExitoMaxRS1 = 0.0, bajaExitoMinRS1 = 0.0;

float costoEvocarRS2=0.0, costoAltaRS2=0.0, costoBajaRS2=0.0;
float evocarFracMedRS2=0.0, evocarFracMaxRS2 = 0.0, evocarFracMinRS2 = 0.0, evocarExitoMedRS2=0.0, evocarExitoMaxRS2 = 0.0, evocarExitoMinRS2 = 0.0;
float altaFracMedRS2=0.0, altaFracMaxRS2 = 0.0, altaFracMinRS2 = 0.0, altaExitoMedRS2=0.0, altaExitoMaxRS2 = 0.0, altaExitoMinRS2 = 0.0;
float bajaFracMedRS2=0.0, bajaFracMaxRS2 = 0.0, bajaFracMinRS2 = 0.0, bajaExitoMedRS2=0.0, bajaExitoMaxRS2 = 0.0, bajaExitoMinRS2 = 0.0;

typedef struct Prestador{
    int DNI;
    char nombre[80];
    char servicio[120];
    char domicilio[80];
    char mail[50];
    char telefono[30];
}Prestador;

int hashing1 (int dni, int M) {
    char x[10];
    int longitud, i;
    int contador = 0;
    sprintf(x, "%d", dni);
    longitud = strlen(x);
    for (i = 0; i < longitud; i++)
        contador += ((int)x[i]) * (i + 1);
    return (contador % M);
}

int hashing2 (int dni, int M) {
    return (dni % M);
}

////////////////////////////////////////////////

int comparar(char parametro[], char delista[]){
    int i=0;
    char aux1[80], aux2[80];
    while (*parametro && *delista && i<80) {
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

/////////////////// ABB ////////////////////////
typedef struct Nodo{
    Prestador pr;
    struct Nodo *izq;
    struct Nodo *der;
}Nodo;


typedef struct Pack{         //esto sirve para cuando tenemos que obtener padres de algun dato en el arbol
    struct Nodo *actual;
    struct Nodo *padre;
}Pack;

/////////////// REBALSE SEPARADO ///////////////

typedef struct Rebalse{
    Nodo *raiz[MAXS];
    int cantidad;
}Rebalse;

void init_RS(Rebalse *rs){
    int i;
    for(i=0;i<MAXS;i++){
        rs->raiz[i] = NULL;
    }
    (*rs).cantidad=0;
}

void vaciarArbol(Nodo **arbol) {
    if (*arbol != NULL) {
        vaciarArbol(&((*arbol)->izq));
        vaciarArbol(&((*arbol)->der));
        free(*arbol);
        *arbol = NULL;
    }
}

void vaciarRebalse(Rebalse *rs){
    int i;
    for(i=0;i<MAXS;i++){
        vaciarArbol(&(rs->raiz[i]));
        rs->raiz[i] = NULL;
    }
    rs->cantidad = 0;
}


Pack localizarRS1(Rebalse *rs,int dni){
    Pack datos;
    datos.padre=NULL;
    datos.actual=NULL;
    exito = 0;
int hash = hashing1(dni, MAXS);
    Nodo* raiz = rs->raiz[hash];
    Nodo* pos = NULL;
    Nodo* padre = NULL;
    pos = raiz;
    costoEvocarRS1 = 1.0;

    while(pos!=NULL && dni != pos->pr.DNI){
        costoEvocarRS1 += 1.0;
        padre = pos;
        if (dni < pos->pr.DNI) {
            pos = pos->izq;
        }
        else{
            pos = pos->der;
        }
    }
    datos.padre=padre;
    datos.actual=pos;
    if(pos!=NULL){
        costoEvocarRS1 += 1.0;
        exito=1;
        return datos;// Nodo no encontrado y devuelve el padre de pos
    }
    return datos;
}

Pack localizarRS2(Rebalse *rs,int dni){
    Pack datos;
    datos.padre=NULL;
    datos.actual=NULL;
    exito = 0;
 int hash = hashing2(dni, MAXS);
    Nodo* raiz = rs->raiz[hash];
    Nodo* pos = NULL;
    Nodo* padre = NULL;
    pos = raiz;
    costoEvocarRS2 = 1.0;

    while(pos!=NULL && pos->pr.DNI != dni){
        costoEvocarRS2 += 1.0;
        padre = pos;
        if (dni < pos->pr.DNI) {
            pos = pos->izq;
        }
        else {
            pos = pos->der;
        }
    }
    datos.padre=padre;
    datos.actual=pos;
    if(pos!=NULL){
        costoEvocarRS2 += 1.0;
        exito=1;
        return datos;// Nodo encontrado y devuelve el padre de pos
    }
    return datos;
}

void altaRS1(Rebalse *rs, Prestador pr){
    costoAltaRS1 = 0.0;
 int hash = hashing1(pr.DNI, MAXS);
    Pack datos = localizarRS1(&(*rs), pr.DNI);

    if (exito == 1) {
        exito = 0;  // ya estaba cargado
        return;
    } else {
        // Si no existe, creamos el nuevo nodo
        Nodo* nuevo_nodo = (Nodo*) malloc(sizeof(Nodo));
        if (nuevo_nodo == NULL) {
            exito = 0; // error memoria
            return;
        }

        nuevo_nodo->pr = pr;

        nuevo_nodo->der = NULL;


        nuevo_nodo->izq = NULL;


        // Enlazamos el nodo al árbol
        if (datos.padre == NULL) {
            // El arbol de esta cabecera estaba vacio
            rs->raiz[hash] = nuevo_nodo;
            costoAltaRS1 += 1.0; // 1 punto por modificar puntero de la raiz
        } else {
            // Lo colgamos del padre correspondiente
            if (pr.DNI < datos.padre->pr.DNI) {
                datos.padre->izq = nuevo_nodo;
                costoAltaRS1 += 1.0; // 1 punto por modificar puntero
            } else {
                datos.padre->der = nuevo_nodo;
                costoAltaRS1 += 1.0; // 1 punto por modificar puntero
            }
        }

        rs->cantidad += 1;
        exito = 1;
    }
}

void altaRS2(Rebalse *rs, Prestador pr){
    costoAltaRS2 = 0.0;
    int hash;
hash = hashing2(pr.DNI, MAXS);
    Pack datos = localizarRS2(&(*rs), pr.DNI);

    if (exito == 1) {
        exito = 0;  // ya estaba cargado
    } else {
        // Si no existe, creamos el nuevo nodo
        Nodo* nuevo_nodo = (Nodo*) malloc(sizeof(Nodo));
        if (nuevo_nodo == NULL) {
            exito = 0; // error memoria
            return;
        }

        nuevo_nodo->pr = pr;

        nuevo_nodo->der = NULL;


        nuevo_nodo->izq = NULL;



        if (datos.padre == NULL) {
            rs->raiz[hash] = nuevo_nodo;
            costoAltaRS2 += 1.0; // 1 punto por modificar puntero
        } else {
            if (pr.DNI < datos.padre->pr.DNI) {
                datos.padre->izq = nuevo_nodo;
                costoAltaRS2 += 1.0; // 1 punto por modificar puntero
            } else {
                datos.padre->der = nuevo_nodo;
                costoAltaRS2 += 1.0; // 1 punto por modificar puntero
            }
        }

        rs->cantidad += 1;
        exito = 1;
    }
}

void bajaRS1(Rebalse *rs, Prestador p){
    costoBajaRS1 = 0.0;
    Pack datos = localizarRS1(&(*rs), p.DNI);
    Nodo* pos = datos.actual;
    Nodo* padre = datos.padre;

    if (rs->cantidad == 0 || exito == 0) {
        exito = 0; // No hay elementos o no se encontró
        return;
    }

    // Si lo encontró, comprobamos que toda la nupla sea exacta
    if (comparar(pos->pr.mail, p.mail) == 0 &&
        comparar(pos->pr.domicilio, p.domicilio) == 0 &&
        pos->pr.DNI == p.DNI &&
        comparar(pos->pr.nombre, p.nombre) == 0 &&
        comparar(pos->pr.servicio, p.servicio) == 0 &&
        comparar(pos->pr.telefono, p.telefono) == 0) {

        // CASO A, B y C: El nodo tiene 0 o 1 hijo
        if (pos->izq == NULL || pos->der == NULL) {
            Nodo* hijo = (pos->izq != NULL) ? pos->izq : pos->der; // Buscamos cual es el hijo que existe (o NULL)

            if (padre == NULL) { // Era la raíz de la cabecera
               int hash = hashing1(p.DNI, MAXS);
                rs->raiz[hash] = hijo;
                costoBajaRS1 += 1.0;
            } else if (padre->izq == pos) { // Era hijo izquierdo
                padre->izq = hijo;
                costoBajaRS1 += 1.0;
            } else { // Era hijo derecho
                padre->der = hijo;
                costoBajaRS1 += 1.0;
            }
            free(pos);
        }
        // CASO D: El nodo tiene 2 hijos. RS1 usa MENOR DE LOS MAYORES
        else {
            Nodo* padre_reemplazo = pos;
            Nodo* reemplazo = pos->der; // Vamos una vez a la derecha (los mayores)

            while (reemplazo->izq != NULL) { // Y todo a la izquierda (el menor)
                padre_reemplazo = reemplazo;
                reemplazo = reemplazo->izq;
            }

            // Copia de datos
            pos->pr = reemplazo->pr;
            costoBajaRS1 += 1.0; // 1 punto por copia de datos

            // Enganchamos el hijo del reemplazo (que como es el menor, solo puede tener hijo der o null)
            if (padre_reemplazo == pos) {
                padre_reemplazo->der = reemplazo->der;
            } else {
                padre_reemplazo->izq = reemplazo->der;
            }
            costoBajaRS1 += 1.0; // 1 punto por modificación de puntero

            free(reemplazo);
        }

        rs->cantidad -= 1;
        exito = 1;
    } else {
        exito = 0; // Datos no coinciden
    }
}

void bajaRS2(Rebalse *rs, Prestador p){
    costoBajaRS2 = 0.0;
    Pack datos = localizarRS2(&(*rs), p.DNI);
    Nodo* pos = datos.actual;
    Nodo* padre = datos.padre;

    if (rs->cantidad == 0 || exito == 0) {
        exito = 0;
        return;
    }

    if (comparar(pos->pr.mail, p.mail) == 0 &&
        comparar(pos->pr.domicilio, p.domicilio) == 0 &&
        pos->pr.DNI == p.DNI &&
        comparar(pos->pr.nombre, p.nombre) == 0 &&
        comparar(pos->pr.servicio, p.servicio) == 0 &&
        comparar(pos->pr.telefono, p.telefono) == 0) {

        // CASO A, B y C: El nodo tiene 0 o 1 hijo
        if (pos->izq == NULL || pos->der == NULL) {
            Nodo* hijo = (pos->izq != NULL) ? pos->izq : pos->der;

            if (padre == NULL) {
               int hash = hashing2(p.DNI, MAXS);
                rs->raiz[hash] = hijo;
                costoBajaRS2 += 1.0;
            } else if (padre->izq == pos) {
                padre->izq = hijo;
                costoBajaRS2 += 1.0;
            } else {
                padre->der = hijo;
                costoBajaRS2 += 1.0;
            }
            free(pos);
        }
        // CASO D: El nodo tiene 2 hijos. RS2 usa MAYOR DE LOS MENORES
        else {
            Nodo* padre_reemplazo = pos;
            Nodo* reemplazo = pos->izq; // Vamos una vez a la izquierda (los menores)

            while (reemplazo->der != NULL) { // Y todo a la derecha (el mayor)
                padre_reemplazo = reemplazo;
                reemplazo = reemplazo->der;
            }

            // Copia de datos
            pos->pr = reemplazo->pr;
            costoBajaRS2 += 1.0; // 1 punto por copia de datos

            // Enganchamos el hijo del reemplazo (que solo puede tener hijo izq o null)
            if (padre_reemplazo == pos) {
                padre_reemplazo->izq = reemplazo->izq;
            } else {
                padre_reemplazo->der = reemplazo->izq;
            }
            costoBajaRS2 += 1.0; // 1 punto por modificación de puntero

            free(reemplazo);
        }

        rs->cantidad -= 1;
        exito = 1;
    } else {
        exito = 0;
    }
}
void evocarRS1(Rebalse *rs, int codigo, Prestador *pr){
    costoEvocarRS1 =0.0;
    exito = 0;
    Pack datos=localizarRS1(&(*rs),codigo);
    if(exito == 1){
        *pr = datos.actual->pr;
        exito =1;
    }
}

void evocarRS2(Rebalse *rs, int codigo, Prestador *pr){
    costoEvocarRS2 =0.0;
    exito = 0;
    Pack datos=localizarRS2(&(*rs),codigo);
    if(exito == 1){
        *pr = datos.actual->pr;
        exito =1;
    }
}

void inorden(Nodo *raiz){
    if(raiz != NULL){
        inorden(raiz->izq);
        printf("\n\nPrestador-------------------\n");
        printf("Nombre: %s\n", raiz->pr.nombre);
        printf("DNI: %d\n", raiz->pr.DNI);
        printf("Mail: %s\n", raiz->pr.mail);
        printf("Domicilio: %s\n", raiz->pr.domicilio);
        printf("Telefono: %s\n", raiz->pr.telefono);
        printf("Servicio: %s\n", raiz->pr.servicio);
        printf("-----------------------------\n");

        if(raiz->izq != NULL){
            printf("DNI Nodo Izquierdo: %d\n", raiz->izq->pr.DNI);
        } else {
            printf("No tiene nodo izquierdo\n");
        }

        if(raiz->der != NULL){

            printf("DNI Nodo Derecho: %d\n", raiz->der->pr.DNI);
        } else {
            printf("No tiene nodo derecho\n");
        }

        contador += 1;
        printf("-----------------------------\n");
        if ((contador + 1) % 3 == 0) {
            printf("\n");
            printf("Presione Enter para mostrar mas...\n");
            fflush(stdin);
            getchar();
            system("cls");
            contador = 0;
        }
        inorden(raiz->der);
    }
}

void mostrarRS1(Rebalse rs){
    int i = 0;

    for(i=0; i< MAXS ; i++){
        if(rs.raiz[i] != NULL){
            system("cls");
            printf("Rebalse numero 1\n");
            printf("********************************************\n");
            printf("------------- Cabecera N_%d ----------------\n", i);
            inorden(rs.raiz[i]);
        }
    }
    printf("Presione Enter para continuar\n");
    fflush(stdin);
    getchar();
    system("cls");
    i = 0;
    contador = 0;
    system("cls");
    printf("---------- Cabeceras vacias ----------\n");
    for(i = 0; i < MAXS; i++){
        if(rs.raiz[i] == NULL){
            printf("Cabecera: %d\n", i);
            contador++;
            if ((contador) % 8 == 0) { printf("\n");
                printf("Presione Enter para mostrar mas...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }
        }
    }
}

void mostrarRS2(Rebalse rs){
    int i = 0;

    for(i=0; i< MAXS ; i++){
        if(rs.raiz[i] != NULL){
            system("cls");
            printf("Rebalse numero 1\n");
            printf("********************************************\n");
            printf("------------- Cabecera N_%d ----------------\n", i);
            inorden(rs.raiz[i]);
        }
    }
    printf("Presione Enter para continuar\n");
    fflush(stdin);
    getchar();
    system("cls");
    i = 0;
    contador = 0;
    printf("---------- Cabeceras vacias ----------\n");
    for(i = 0; i < MAXS; i++){
        if(rs.raiz[i] == NULL){
            printf("Cabecera: %d\n", i);
            contador++;
            if ((contador) % 8 == 0) { printf("\n");
                printf("Presione Enter para mostrar mas...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }
        }
    }
}


//////////////////////////////////////////////////


int Lecturadatos(Rebalse *rs1, Rebalse *rs2){
    FILE *fp;
    Prestador aux;
    Prestador prest_evocado; // <-- Variable para guardar el prestador cuando lo buscamos
    int selector = 0;

    if((fp = fopen("Operaciones-Prestadores.txt", "r")) == NULL){
        return 0;
    } else {
        while (!(feof(fp))){
            fscanf(fp, " %d", &selector);

            switch(selector){
                case 1:
                    fscanf(fp, " %d", &aux.DNI);
                    fscanf(fp, " %[^\n]", aux.nombre);
                    fscanf(fp, " %[^\n]", aux.servicio);
                    fscanf(fp, " %[^\n]", aux.domicilio);
                    fscanf(fp, " %[^\n]", aux.mail);
                    fscanf(fp, " %[^\n]", aux.telefono);

                    aux.nombre[strcspn(aux.nombre, "\r\n")] = 0;
                    aux.servicio[strcspn(aux.servicio, "\r\n")] = 0;
                    aux.domicilio[strcspn(aux.domicilio, "\r\n")] = 0;
                    aux.mail[strcspn(aux.mail, "\r\n")] = 0;
                    aux.telefono[strcspn(aux.telefono, "\r\n")] = 0;


                    altaRS1(rs1, aux);
                    if(exito == 1){
                        cantAltasRS1 += 1;
                        altaExitoMedRS1 += costoAltaRS1;
                        if(costoAltaRS1 > altaExitoMaxRS1)
                            altaExitoMaxRS1 = costoAltaRS1;
                    }
                    else{
                        altaFracMedRS1 += costoAltaRS1;
                        if(costoAltaRS1 > altaFracMaxRS1)
                            altaFracMaxRS1 = costoAltaRS1;
                    }

                    altaRS2(rs2, aux);

                    if(exito == 1){
                        cantAltasRS2 += 1;
                        altaExitoMedRS2 += costoAltaRS2;
                        if(costoAltaRS2 > altaExitoMaxRS2)
                            altaExitoMaxRS2 = costoAltaRS2;
                    }
                    else{
                        altaFracMedRS2 += costoAltaRS2;
                        if(costoAltaRS2 > altaFracMaxRS2)
                            altaFracMaxRS2 = costoAltaRS2;
                    }

                    break;

                case 2:

                    fscanf(fp, " %d", &aux.DNI);
                    fscanf(fp, " %[^\n]", aux.nombre);
                    fscanf(fp, " %[^\n]", aux.servicio);
                    fscanf(fp, " %[^\n]", aux.domicilio);
                    fscanf(fp, " %[^\n]", aux.mail);
                    fscanf(fp, " %[^\n]", aux.telefono);

                    aux.nombre[strcspn(aux.nombre, "\r\n")] = 0;
                    aux.servicio[strcspn(aux.servicio, "\r\n")] = 0;
                    aux.domicilio[strcspn(aux.domicilio, "\r\n")] = 0;
                    aux.mail[strcspn(aux.mail, "\r\n")] = 0;
                    aux.telefono[strcspn(aux.telefono, "\r\n")] = 0;


                    bajaRS1(rs1, aux);
                    if(exito == 1){
                        cantBajasRS1 += 1;
                        bajaExitoMedRS1 += costoBajaRS1;
                        if(costoBajaRS1 > bajaExitoMaxRS1)
                            bajaExitoMaxRS1 = costoBajaRS1;
                    }
                    else{
                        bajaFracMedRS1 += costoBajaRS1;
                        if(costoBajaRS1 > bajaFracMaxRS1)
                            bajaFracMaxRS1 = costoBajaRS1;
                    }
                    bajaRS2(rs2, aux);
                    if(exito == 1){
                        cantBajasRS2 += 1;
                        bajaExitoMedRS2 += costoBajaRS2;
                        if(costoBajaRS2 > bajaExitoMaxRS2)
                            bajaExitoMaxRS2 = costoBajaRS2;
                    }
                    else{
                        bajaFracMedRS2 += costoBajaRS2;
                        if(costoBajaRS2 > bajaFracMaxRS2)
                            bajaFracMaxRS2 = costoBajaRS2;
                    }



                    break;

                case 3:

                    fscanf(fp, " %d", &aux.DNI);


                    evocarRS1(rs1, aux.DNI, &prest_evocado);
                    if(exito == 1){
                        if(evocarExitoMaxRS1 < costoEvocarRS1){
                            evocarExitoMaxRS1 = costoEvocarRS1;
                        }
                        evocarExitoMedRS1 += costoEvocarRS1;
                        cantEvocExit += 1;

                    } else {
                        if(evocarFracMaxRS1 < costoEvocarRS1){
                            evocarFracMaxRS1 = costoEvocarRS1;
                        }
                        evocarFracMedRS1 += costoEvocarRS1;
                        cantEvocFrac += 1;

                    }


                    evocarRS2(rs2, aux.DNI, &prest_evocado);
                    if(exito == 1){
                        if(evocarExitoMaxRS2 < costoEvocarRS2){
                            evocarExitoMaxRS2 = costoEvocarRS2;
                        }
                        evocarExitoMedRS2 += costoEvocarRS2;
                        cantEvocExit += 1;

                    } else {
                        if(evocarFracMaxRS2 < costoEvocarRS2){
                            evocarFracMaxRS2 = costoEvocarRS2;
                        }
                        evocarFracMedRS2 += costoEvocarRS2;
                        cantEvocFrac += 1;

                    }

                    break;


                default:
                    break;
            }
        }


        fclose(fp);
        return 1;
    }
}

int main(){
    int opt=0, sel;
  Rebalse rs1;
Rebalse rs2;
init_RS(&rs1);
init_RS(&rs2);
    int menu=0;

    while (opt != 3){
        printf("****************************************\n");
        printf("[1] Mostrar estructuras\n");
        printf("[2] Carga y comparacion de estructuras\n");
        printf("[3] Salir\n");
        printf("****************************************\n");
        scanf(" %d", &opt);
        system("cls");

        switch (opt){
        case 1:
            sel=0;
            while(sel!=3){
                printf("****************************************\n");
                printf("[1] Rebalse Separado 1 (RS1)\n");
                printf("[2] Rebalse Separado 2 (RS2)\n");
                printf("[3] Salir\n");
                printf("****************************************\n");
                scanf(" %d", &sel);
                system("cls");

                switch(sel){
                case 1:
                    if(rs1.cantidad > 0){
                        mostrarRS1(rs1);
                    }else{
                        printf("****************************************\n");
                        printf("El RS1 esta vacio\n");
                        printf("****************************************\n");
                        printf("Presione Enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
                    }
                    break;
                case 2:
                    if(rs2.cantidad > 0){
                        mostrarRS2(rs2);
                    }else{
                        printf("****************************************\n");
                        printf("El RS2 esta vacio\n");
                        printf("****************************************\n");
                        printf("Presione Enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case 2:

            exito = 0;
            contador = 0;
            cantAltasRS1 = 0;
            cantAltasRS2 = 0;
            cantBajasRS1 = 0;
            cantBajasRS2 = 0;
            cantEvocExit = 0;
            cantEvocFrac = 0;
            costoEvocarRS1=0.0; costoAltaRS1=0.0; costoBajaRS1=0.0;
            evocarFracMedRS1=0.0; evocarFracMaxRS1 = 0.0; evocarFracMinRS1 = 0.0; evocarExitoMedRS1=0.0; evocarExitoMaxRS1 = 0.0; evocarExitoMinRS1 = 0.0;
            altaFracMedRS1=0.0; altaFracMaxRS1 = 0.0; altaFracMinRS1 = 0.0; altaExitoMedRS1=0.0; altaExitoMaxRS1 = 0.0; altaExitoMinRS1 = 0.0;
            bajaFracMedRS1=0.0; bajaFracMaxRS1 = 0.0; bajaFracMinRS1 = 0.0; bajaExitoMedRS1=0.0; bajaExitoMaxRS1 = 0.0; bajaExitoMinRS1 = 0.0;

            costoEvocarRS2=0.0; costoAltaRS2=0.0; costoBajaRS2=0.0;
            evocarFracMedRS2=0.0; evocarFracMaxRS2 = 0.0; evocarFracMinRS2 = 0.0; evocarExitoMedRS2=0.0; evocarExitoMaxRS2 = 0.0; evocarExitoMinRS2 = 0.0;
            altaFracMedRS2=0.0; altaFracMaxRS2 = 0.0; altaFracMinRS2 = 0.0; altaExitoMedRS2=0.0; altaExitoMaxRS2 = 0.0; altaExitoMinRS2 = 0.0;
            bajaFracMedRS2=0.0; bajaFracMaxRS2 = 0.0; bajaFracMinRS2 = 0.0; bajaExitoMedRS2=0.0; bajaExitoMaxRS2 = 0.0; bajaExitoMinRS2 = 0.0;

            vaciarRebalse(&rs1);
            vaciarRebalse(&rs2);

            menu = Lecturadatos(&rs1, &rs2);

                if (menu == 0) {
                    printf("Error al abrir el archivo.\n");
                } else {
                    printf("Datos procesados correctamente.\n");

                }

                float promEvocExRS1 = (cantEvocExit > 0) ? evocarExitoMedRS1 / (cantEvocExit/2) : 0;
                float promEvocFraRS1 = (cantEvocFrac > 0) ? evocarFracMedRS1 / (cantEvocFrac/2) : 0;
                float promAltaRS1 = (cantAltasRS1 > 0) ? altaExitoMedRS1 / cantAltasRS1 : 0;
                float promBajaRS1 = (cantBajasRS1 > 0) ? bajaExitoMedRS1 / cantBajasRS1 : 0;

                float promEvocExRS2 = (cantEvocExit > 0) ? evocarExitoMedRS2 / (cantEvocExit/2) : 0;
                float promEvocFraRS2 = (cantEvocFrac > 0) ? evocarFracMedRS2 / (cantEvocFrac/2) : 0;
                float promAltaRS2 = (cantAltasRS2 > 0) ? altaExitoMedRS2 / cantAltasRS2 : 0;
                float promBajaRS2 = (cantBajasRS2 > 0) ? bajaExitoMedRS2 / cantBajasRS2 : 0;

                printf("-----------------------------------------------------------------------\n");
                printf("|---------------------|--------- RS1 ---------|--------- RS2 ---------|\n");
                printf("|Evocacion Exito max  |         %.2f          |          %.2f         |\n", evocarExitoMaxRS1, evocarExitoMaxRS2);
                printf("|Evocacion Exito med  |         %.2f          |          %.2f         |\n", promEvocExRS1, promEvocExRS2);
                printf("|---------------------------------------------------------------------|\n");
                printf("|Evocacion Fracaso max|         %.2f          |          %.2f         |\n", evocarFracMaxRS1, evocarFracMaxRS2);
                printf("|Evocacion Fracaso med|         %.2f          |          %.2f         |\n",promEvocFraRS1, promEvocFraRS2);
                printf("|---------------------------------------------------------------------|\n");
                printf("|       Alta max      |         %.2f          |          %.2f         |\n", altaExitoMaxRS1, altaExitoMaxRS2);
                printf("|       Alta med      |         %.2f          |          %.2f         |\n", promAltaRS1, promAltaRS2);
                printf("|---------------------------------------------------------------------|\n");
                printf("|       Baja max      |         %.2f          |          %.2f         |\n", bajaExitoMaxRS1, bajaExitoMaxRS2);
                printf("|       Baja med      |         %.2f          |          %.2f         |\n", promBajaRS1, promBajaRS2);
                printf("-----------------------------------------------------------------------\n");

                printf("Cantidad altas exitosas: %d\n", cantAltasRS1);
                printf("Cantidad bajas exitosas: %d\n", cantBajasRS1);
                printf("Cantidad elementos en RS1: %d\n", rs1.cantidad);
                printf("Cantidad elementos en RS2: %d\n", rs2.cantidad);
                printf("Cantidad evocaciones exitosas: %d\n", cantEvocExit/2);
                printf("Cantidad evocaciones fracasadas: %d\n", cantEvocFrac/2);

                menu = 0;

                printf("\nPresione enter para continuar...\n");
                fflush(stdin);
                getchar();
                system("cls");
                break;
        default:
            break;
        }
    }

    return 0;
}
