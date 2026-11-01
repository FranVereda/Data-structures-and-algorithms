/* GRUPO 8
    Giva Julian Agustin
    Vereda Juan Francisco
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*

        **************************************************************
        |---------||-------------------||-------------------|
        |         ||     EVOCAR EX     ||    EVOCAR FRA     |
        |         ||   Max   |  Med    ||   Max   |  Med    |
        |---------||---------|---------||---------|---------|
        |RAL      ||   81    |   16.5  ||   94    |  70.3   |
        |---------------------------------------------------|
        |RAC      ||   14    |   4.6   ||   15    |  11.6   |
        |---------------------------------------------------|
        |RS       ||   6     |   2.7   ||   4     |  1.8    |
        |---------||---------|---------||-------------------|
        **************************************************************

    Las estructuras de Rebalse Abierto Lineal y el Rebalse Abierto Cuadratico son similares en cuanto a estructura, pero si analizamos los costos
    podemos ver que existen diferencias. La diferencia esta en que el RAC va dando saltos cada vez mas grandes a la hora de buscar, lo que en muchos casos
    es bueno para la eficiencia de costos.
    Si tuvieramos que elegir una estructura de las tres, teniendo en cuenta los costos de las mismas, podemos afirmar que el
    Rebalse Separado es nuestra mejor opcion, ya que los costos son ampliamente menores comparados a las demas.

*/


#define MAXL 174 // MAXL ES M QUE ES IGUAL A TECHO DE (130/0.75)=173,3333 y REDONDEAMOS HACIA ARRIBA AL ENTERO MAS CERCANO QUE ES 174
#define MAXC 163 // MAXC ES M QUE ES IGUAL A TECHO DE (130/0.81)=160,493 y NOS VAMOS AL PRIMO MAS CERCANO (MAYOR) QUE ES 163
#define MAXS 85 // MAXC ES M QUE ES IGUAL A TECHO DE (130/1.53)=84,967 y REDONDEAMOS HACIA ARRIBA AL ENTERO MAS CERCANO QUE ES 85
typedef struct{
    char codigo[8];
    char nombre[80];
    char mail[23];
    int nota;
    char condicion[9];
} alumno;

typedef struct{
    alumno alumnos[131];
    int elementoscont;
} lista;

int comparar(char parametro[], char delista[]){
    int i=0;
    char aux1[120], aux2[120];
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

int hashing(char* x , int M) {
    int longitud, i;
    int contador=0;
    longitud = strlen(x);
    for(i=0; i< longitud;i++){

        char codmayus = toupper(x[i]);  // CORRECCION REC: CODIGO EN MAYUSCULA PARA CALCULO HASH

        contador+=((int)codmayus) * (i+1);
    }
    return(contador%M);
}

/////////////// REBALSE LINEAL ///////////////
typedef struct{
    char estado; //*(virgen) +(libre) -(ocupada)
    alumno a;
}celda;

typedef struct{
    celda arr[MAXL];
    int cantidad;
}RAL;

void init_RAL(RAL *rebalse){
    int i;
    for(i=0;i<MAXL;i++){
        (*rebalse).arr[i].estado='*';
    }
    (*rebalse).cantidad=0;
}

void LocalizarRAL(RAL rebalse,char* codigo,int *pos,int *exito,int *costo){
    int i=hashing(codigo,MAXL);
    int j=i;
    int bucle = 0;
    int libre=0;
    *exito = 0;
    *costo=0;
    // condicion while
    while((bucle == 0) && (rebalse.arr[i].estado!='*')){
        if((comparar(codigo,rebalse.arr[i].a.codigo)==0) && (rebalse.arr[i].estado=='-')){
                *pos=i;
                *exito = 1;
                *costo+=1;
                return;
        }else{
            if((rebalse.arr[i].estado=='+') && (libre==0)){
                libre++;
                *pos=i;
            }
        }
        *costo+=1;
        i=(i+1)% MAXL;
        if(i==j){
            bucle = 1;
        }
    }//fin while
    if((bucle==1)){
        if(libre==1){
            *exito=0;// No esta el elemento devuelvo la libre
        }else{
            *exito = 2;
        }

    }else{
        // ELSE + COSTO
        *costo +=1;
        if(libre==1){
            *exito=0;// No esta el elemento devuelvo la libre
        }else{
            *exito = 0;
            *pos = i;
        }
    }
}

void AltaRAL(RAL *rebalse, alumno al,int *exito,int *costo){
    int pos;

    //CONTROL CANTIDAD
    if(rebalse->cantidad < MAXL){
        LocalizarRAL(*rebalse, al.codigo,&pos,exito, costo);
        *costo = 0;
        //exito = 2 no hay espacio en el rebalse
        if(*exito==1){
            *exito = 0;//fracaso el alta ya que la localiza es exitosa
        }else if(*exito==0){
            (*rebalse).arr[pos].a=al;
            (*rebalse).arr[pos].estado='-';
            (*rebalse).cantidad+=1;
            *exito = 1;
            //alta exitosa
        }
    }else{
        *exito = 0;
    }
}

void BajaRAL(RAL *rebalse,alumno al,int *exito,int *costo){
    int pos;
    LocalizarRAL(*rebalse, al.codigo, &pos, exito, costo);
    *costo=0;
    if((*exito == 1) && (comparar((*rebalse).arr[pos].a.codigo,al.codigo)==0 && comparar((*rebalse).arr[pos].a.nombre,al.nombre)==0 && comparar((*rebalse).arr[pos].a.mail,al.mail)==0 && (*rebalse).arr[pos].a.nota==al.nota && comparar((*rebalse).arr[pos].a.condicion,al.condicion)==0)){
        (*rebalse).arr[pos].estado='+';//celda libre
        (*rebalse).cantidad-=1;
        *exito = 1;//baja exitosa
    }else{
        *exito = 0;
    }
}

void MostrarRAL(RAL rebalse){
    int i;
    for(i=0;i<MAXL;i++){
        if(rebalse.arr[i].estado=='-'){
            //muestro los datos
            //AGREGADO NRO CELDA
            printf("\n---------------------------------\nCelda con clave %d\n---------------------------------\n",i);
            printf("******************************\n");
            printf("CODIGO: %s\n",rebalse.arr[i].a.codigo);
            printf("Nombre: %s\n",rebalse.arr[i].a.nombre);
            printf("Mail: %s\n",rebalse.arr[i].a.mail);
            printf("Nota: %d\n",rebalse.arr[i].a.nota);
            printf("Condicion: %s\n",rebalse.arr[i].a.condicion);
            printf("********************************\n");
            }
        if(rebalse.arr[i].estado=='*'){
            printf("\n---------------------------------\nLa celda con clave %d es virgen(*)\n---------------------------------\n",i);}

        if(rebalse.arr[i].estado=='+'){
            printf("\n---------------------------------\nLa celda con clave %d es libre(+)\n---------------------------------\n",i);}
        if((i+1)%5==0){
                printf("++++++++++++++++++++++++++++++++++++++\n");
                printf("Presione Enter para mostrar mas...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }
    }
}

void evocarRAL(RAL rebalse,char* codigo,alumno *al,int *exito,int *costo){
    int pos=0;
    *costo=0;
    LocalizarRAL(rebalse, codigo, &pos, exito, costo);
    if(*exito == 1){
        *al=rebalse.arr[pos].a;
        *exito = 1;
    }
}

//////////////////////////////////////////////////

/////////////// REBALSE CUADRATICO ///////////////

typedef struct{
    char estado; //*(virgen) +(libre) -(ocupada)
    alumno a;
}celdaRAC;

typedef struct{
    celdaRAC arr[MAXC];
    int cantidad;
}RAC;

void init_RAC(RAC *rebalse){
    int i;
    for(i=0;i<MAXC;i++){
        (*rebalse).arr[i].estado='*';
    }
    (*rebalse).cantidad=0;
}

void LocalizarRAC(RAC rebalse,char* codigo,int *pos,int *exito,int *costo){
    int i=hashing(codigo,MAXC);
    int j=0;
    int libre=0;
    *exito = 0;
    *costo = 0;
    while(j<MAXC && (rebalse.arr[i].estado!='*')){
        if((comparar(codigo,rebalse.arr[i].a.codigo)==0) && (rebalse.arr[i].estado=='-')){
                *pos=i;
                *exito=1;
                *costo+=1;
                return;
        }else{
            if((rebalse.arr[i].estado=='+')&& (libre==0)){
                libre++;
                *pos=i;
            }
            *costo+=1;
        }

        j++;
        i=(i+j)%MAXC;
    }
    if((j==MAXC)){
        if(libre==1){
            *exito=0;// No esta el elemento devuelvo la libre
        }else{
            *exito = 2;
        }
    }else{
        //ELSE + COSTO
        *costo +=1;
        if(libre==1){
            *exito=0;// No esta el elemento devuelvo la libre
        }else{
            *exito = 0;
            *pos = i;
        }
    }

}

void AltaRAC(RAC *rebalse, alumno al,int *exito,int *costo){
    int pos;
    // CONTROL CANTIDAD
    if(rebalse->cantidad < MAXC){
        LocalizarRAC(*rebalse,al.codigo,&pos,exito, costo);
    *costo = 0;
    //exito = 2 no hay espacio en el rebalse
    if(*exito==1){
        *exito = 0;//fracaso el alta ya que la localizar es exitosa
    }else if(*exito==0){
        (*rebalse).arr[pos].a=al;
        (*rebalse).arr[pos].estado='-';
        (*rebalse).cantidad+=1;
        *costo=1;
        *exito = 1;//alta exitosa
    }
    }else{
        *exito = 0;
    }

}

void BajaRAC(RAC *rebalse,alumno al,int *exito,int *costo){
    int pos;
    LocalizarRAC(*rebalse, al.codigo, &pos, exito, costo);
    *costo=0;
    if(*exito == 1){

        if((comparar((*rebalse).arr[pos].a.codigo,al.codigo)==0 && comparar((*rebalse).arr[pos].a.nombre,al.nombre)==0 && comparar((*rebalse).arr[pos].a.mail,al.mail)==0 && (*rebalse).arr[pos].a.nota==al.nota && comparar((*rebalse).arr[pos].a.condicion,al.condicion)==0)){
            (*rebalse).arr[pos].estado='+'; //celda libre
            *exito = 1;//baja exitosa
            (*rebalse).cantidad-=1;
        }else{
            *exito = 0;
        }
    }
}

void MostrarRAC(RAC rebalse){
int i;
for(i=0;i<MAXC;i++){
    if(rebalse.arr[i].estado=='-'){
        //muestro los datos
        printf("\n---------------------------------\nCelda con clave %d\n---------------------------------\n",i);
        printf("******************************\n");
        printf("CODIGO: %s\n",rebalse.arr[i].a.codigo);
        printf("Nombre: %s\n",rebalse.arr[i].a.nombre);
        printf("Mail: %s\n",rebalse.arr[i].a.mail);
        printf("Nota: %d\n",rebalse.arr[i].a.nota);
        printf("Condicion: %s\n",rebalse.arr[i].a.condicion);
        printf("********************************\n");
    }
    if(rebalse.arr[i].estado=='*'){
        printf("\n---------------------------------\nLa celda con clave %d es virgen(*)\n---------------------------------\n",i);}
    if(rebalse.arr[i].estado=='+'){
        printf("\n---------------------------------\nLa celda con clave %d es libre(+)\n---------------------------------\n",i);}

    if((i+1)%5==0){
            printf("++++++++++++++++++++++++++++++++++++++\n");
            printf("Presione Enter para mostrar mas...\n");
            fflush(stdin);
            getchar();
            system("cls");
        }

    }
}

void evocarRAC(RAC rebalse,char* codigo,alumno *al, int *exito,int *costo){
    int pos=0;
    *costo = 0;
    LocalizarRAC(rebalse,codigo,&pos,exito,costo);
    if(*exito==1){
        *al=rebalse.arr[pos].a;
        *exito = 1;
    }
}

//////////////////////////////////////////////////

/////////////// REBALSE SEPARADO ///////////////

typedef struct Balde{
    alumno a;
    struct Balde *next;
}Balde;

typedef struct{
    Balde *nodos[MAXS];
    int cantidad;
}RS;

typedef struct Pack {
    struct Balde *actual;
    struct Balde *padre;
} Pack;

void init_RS(RS *rs){
    int i;
    for(i=0;i<MAXS;i++){
        rs->nodos[i] = NULL;
    }
    (*rs).cantidad=0;
}

void vaciarRS(RS *rs) {
    for (int i = 0; i < MAXS; i++) {
        // El inicio de la lista es el puntero en el arreglo
        Balde *actual = rs->nodos[i];
        Balde *temp;

        // Recorre la lista vinculada y libera la memoria de cada nodo
        while (actual != NULL) {
            temp = actual;
            actual = actual->next;
            free(temp);
        }

        // reseteamos el puntero a NULL
        rs->nodos[i] = NULL;
    }
    (*rs).cantidad = 0;
}

Pack localizarRS(RS *rs,char* codigo,int *pos,int *exito,int *costo){
    Pack datos;
    datos.padre=NULL;
    datos.actual=NULL;
    *exito = 0;
    *costo = 0;
    int m=hashing(codigo,MAXS);
    *pos=m;

    // consulta a cabecera
    *costo += 1;

    Balde *puntero_actual = rs->nodos[m];

    if(puntero_actual==NULL){  // caso vacio
        *exito = 0;
       // *costo+=1;
        return datos;
    }else{
        while (puntero_actual != NULL) {
            *costo += 1; // costo por cada nodo que comparamos.

            if (comparar(puntero_actual->a.codigo, codigo) == 0) {
                *exito = 1;
                datos.actual = puntero_actual;

                return datos;
            }
               // Si no es el que buscamos, avanza
            datos.padre = puntero_actual;
            puntero_actual = puntero_actual->next;
        }

        //en caso de no encontrarlo, devolvemos el paquete
        return datos;
    }
}

void altaRS(RS *rs,alumno al,int *exito,int *costo){
    int pos=0;
    Pack datos = localizarRS(&(*rs),al.codigo,&pos,exito,costo);
     if (*exito == 1) {
        *exito = 0;  // ya estaba cargado
    } else {

        // Si no existe, creamos el nuevo nodo para el alumno.
        Balde* nuevo_nodo = (Balde*) malloc(sizeof(Balde));
        if (nuevo_nodo == NULL) {
            *exito = 0; // error memoria
            return;
        }

        nuevo_nodo->a = al; // le asignamos la informacion del alumno

        // el next del nuevo nodo apunta a lo que antes era el primer nodo
        nuevo_nodo->next = rs->nodos[pos];

        //el nuevo nodo es la cabeza de la lista ahora
        rs->nodos[pos] = nuevo_nodo;

        rs->cantidad += 1;
        *exito = 1;
    }
}

void bajaRS(RS *rs,alumno al,int *exito,int *costo){
    int pos=0;

    Pack datos=localizarRS(&(*rs),al.codigo,&pos,exito,costo);

    Balde *actual = datos.actual;
    Balde *padre = datos.padre;
    Balde *aux = NULL;

    if(rs->cantidad == 0){
        *exito = 0; // no hay elm en el rs
    }else if(*exito == 1){

        if (comparar(datos.actual->a.nombre, al.nombre)==0 && comparar(datos.actual->a.mail, al.mail)==0 && datos.actual->a.nota==al.nota && comparar(datos.actual->a.condicion, al.condicion)==0) {
                // si todo coincide, seguimos adelante

            // CASO A: El nodo a borrar es la CABEZA de la lista (no hay padre).
            if (datos.padre == NULL) {
                rs->nodos[pos] = datos.actual->next;
            }
            // CASO B: El nodo a borrar está en el MEDIO o al FINAL.
            else {
                // El 'next' del padre ahora apunta al nodo siguiente al que borramos.
                datos.padre->next = datos.actual->next;
            }

            // liberamos la memoria del nodo a eliminar
            free(datos.actual);

            rs->cantidad-=1;
            *exito = 1;
        }else{
            *exito = 0;
            // todos los datos del alumno no son iguales, no hay baja
        }
    }
}


void evocarRS(RS *rs, char* codigo, alumno *al, int *exito, int *costo){
    int pos=0;
    *costo =0;
    Pack datos=localizarRS(&(*rs),codigo,&pos,exito,costo);
    if(*exito == 1){
        *al=datos.actual->a;
        *exito =1;
    }
}

void mostrarRS(RS *rs){
    for (int i = 0; i < MAXS; i++) {
        printf("********************************\n");
        printf("Lista %d:\n", i);

        Balde *actual = rs->nodos[i];
        if (actual == NULL) {
            printf("---------------------------------\n");
            printf("Lista vacia\n");
            printf("---------------------------------\n");
        }else {
            while (actual != NULL) {
                printf("********************************\n");
                printf("CODIGO: %s\nNombre: %s\nMail: %s\nNota: %d\nCondicion: %s\n",
                       actual->a.codigo, actual->a.nombre, actual->a.mail, actual->a.nota, actual->a.condicion);
                printf("********************************\n");

                actual = actual->next;
            }
        }
        if ((i + 1) % 4 == 0 && (i + 1) < MAXS) {
            printf("++++++++++++++++++++++++++++++++++++++\n");
            printf("Presione Enter para mostrar mas...\n");
            fflush(stdin);
            getchar();
            system("cls");
        }
    }
}

//////////////////////////////////////////////////


int Lecturadatos(RAL *ral, RAC *rac,RS *rs, int *evocarfracRS, float *evocarfracmedRS, int *evocarexitoRS, float *evocarexitomedRS, int *evocarfracRAC, float *evocarfracmedRAC, int *evocarexitoRAC, float *evocarexitomedRAC, int *evocarfracRAL, float *evocarfracmedRAL, int *evocarexitoRAL, float *evocarexitomedRAL){
    FILE *fp;
    alumno aux;
    int contaux=0, exito=0;
    int selector=0;
    int costoRS=0;
    int costoRAL=0;
    int costoRAC=0;
    int contevocarexRS=0, contevocarfraRS=0;
    int contevocarexRAC=0, contevocarfraRAC=0;
    int contevocarexRAL=0, contevocarfraRAL=0;
    if(( fp = fopen( "Operaciones-Alumnos.txt" , "r") )==NULL){
        return 0;
    }else {
    while (!( feof(fp))){
        fscanf(fp," %d",&selector);
        switch(selector){
            case 1:
                fscanf( fp, " %[^\n] " ,aux.codigo);
                fscanf( fp, " %[^\n] " ,aux.nombre);
                fscanf( fp, " %[^\n] " ,aux.mail);
                fscanf( fp," %d",&aux.nota);
                fscanf( fp, " %[^\n] " ,aux.condicion);

                    AltaRAL(&(*ral), aux, &exito, &costoRAL);

                    AltaRAC(&(*rac), aux, &exito, &costoRAC);

                    altaRS(&(*rs),aux, &exito, &costoRS);


                break;
            case 2:
                fscanf( fp, " %[^\n] " ,aux.codigo);
                fscanf( fp, " %[^\n] " ,aux.nombre);
                fscanf( fp, " %[^\n] " ,aux.mail);
                fscanf( fp," %d",&aux.nota);
                fscanf( fp, " %[^\n] " ,aux.condicion);

                BajaRAL(ral, aux, &exito, &costoRAL);

                BajaRAC(rac, aux, &exito, &costoRAC);

                bajaRS(rs, aux, &exito, &costoRS);

                break;
            case 3:
                fscanf( fp, " %[^\n] " ,aux.codigo);
                alumno alum;
                evocarRAL(*ral, aux.codigo, &alum, &exito, &costoRAL);
                if(exito==1){
                    if(*evocarexitoRAL<costoRAL){
                        *evocarexitoRAL=costoRAL;
                    }
                    *evocarexitomedRAL+=costoRAL;
                    contevocarexRAL+=1;
                }else{
                    if(*evocarfracRAL<costoRAL){
                        *evocarfracRAL=costoRAL;
                    }
                    *evocarfracmedRAL+=costoRAL;
                    contevocarfraRAL+=1;
                }

                evocarRAC(*rac, aux.codigo, &alum, &exito, &costoRAC);
                if(exito==1){
                    if(*evocarexitoRAC<costoRAC){
                        *evocarexitoRAC=costoRAC;
                    }
                    *evocarexitomedRAC+=costoRAC;
                    contevocarexRAC+=1;
                }else{
                    if(*evocarfracRAC<costoRAC){
                        *evocarfracRAC=costoRAC;
                    }
                    *evocarfracmedRAC+=costoRAC;
                    contevocarfraRAC+=1;
                }
                evocarRS(&(*rs), aux.codigo, &alum, &exito, &costoRS);
                if(exito==1){
                    if(*evocarexitoRS<costoRS){
                        *evocarexitoRS=costoRS;
                    }
                    *evocarexitomedRS+=costoRS;
                    contevocarexRS+=1;
                }else{
                    if(*evocarfracRS<costoRS){
                        *evocarfracRS=costoRS;
                    }
                    *evocarfracmedRS+=costoRS;
                    contevocarfraRS+=1;
                }
                break;
            default:
                break;
        }
    }

    if(contevocarexRS!=0){
        *evocarexitomedRS=*evocarexitomedRS/contevocarexRS;
    }

    if(contevocarfraRS!=0){
        *evocarfracmedRS=*evocarfracmedRS/contevocarfraRS;
    }

    if(contevocarexRAC!=0){
        *evocarexitomedRAC=*evocarexitomedRAC/contevocarexRAC;
    }

    if(contevocarfraRAC!=0){
        *evocarfracmedRAC=*evocarfracmedRAC/contevocarfraRAC;
    }

    if(contevocarexRAL!=0){
        *evocarexitomedRAL=*evocarexitomedRAL/contevocarexRAL;
    }

    if(contevocarfraRAL!=0){
        *evocarfracmedRAL=*evocarfracmedRAL/contevocarfraRAL;
    }

    fclose( fp );

    return 1;
    }
}


int main(){
    int opt=0,sel, i, x;
    int evocarfracRS=0, evocarexitoRS=0, evocarfracRAC=0, evocarexitoRAC=0, evocarfracRAL=0, evocarexitoRAL=0;
    float evocarfracmedRS=0.0, evocarexitomedRS=0.0, evocarfracmedRAC=0.0, evocarexitomedRAC=0.0, evocarfracmedRAL=0.0, evocarexitomedRAL=0.0;
    int carga=0;
    RAL ral;
    RAC rac;
    RS rs;
    init_RAL(&ral);
    init_RAC(&rac);
    init_RS(&rs);
    ral.cantidad = 0;
    rac.cantidad = 0;

    int menu=0;

    while (opt != 3){
        printf("****************************************\n"
               "[1] Mostrar estructuras\n"
               "[2] Carga y comparacion de estructuras\n"
               "[3] Salir\n"
               "****************************************\n");
        scanf(" %d", &opt);
        system("cls");
        switch (opt){
        case 1:
            sel=0;
            while(sel!=4){
                printf("****************************************\n"
                    "[1] Rebalse Abierto Lineal\n"
                    "[2] Rebalse Abierto Cuadratico\n"
                    "[3] Rebalse Separado\n"
                    "[4] Salir\n"
                    "****************************************\n");
                scanf(" %d", &sel);
                system("cls");
                switch(sel){
                case 1:

                    MostrarRAL(ral); //CORRECION REC: MUESTRA ESTRUCTURA VACIA

                    if (ral.cantidad==0) {
                        printf("****************************************\n"
                               "El RAL esta vacio.\n"
                               "****************************************\n");
                    }
                    break;
                case 2:

                    MostrarRAC(rac);//CORRECION REC: MUESTRA ESTRUCTURA VACIA

                    if (rac.cantidad==0) {
                        printf("****************************************\n"
                               "El RAC esta vacio.\n"
                               "****************************************\n");
                    }
                    break;
                case 3:

                    mostrarRS(&rs);//CORRECION REC: MUESTRA ESTRUCTURA VACIA

                    if(rs.cantidad>0){
                        printf("****************************************\n"
                            "El RS esta vacio\n"
                            "****************************************\n");
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case 2:
            if(carga==0){
                init_RAL(&ral);
                init_RAC(&rac);
                vaciarRS(&rs);

                menu = Lecturadatos(&ral, &rac, &rs, &evocarfracRS, &evocarfracmedRS, &evocarexitoRS, &evocarexitomedRS, &evocarfracRAC, &evocarfracmedRAC, &evocarexitoRAC, &evocarexitomedRAC, &evocarfracRAL, &evocarfracmedRAL, &evocarexitoRAL, &evocarexitomedRAL);
                if (menu == 0) {
                    printf("Error al abrir el archivo.\n");
                } else {
                    printf("Datos procesados correctamente.\n");
                    carga=1;
                }

                printf("**************************************************************\n");
                printf("|---------||-------------------||-------------------|\n");
                printf("|         ||     EVOCAR EX     ||    EVOCAR FRA     |\n");
                printf("|         ||   Max   |  Med    ||   Max   |  Med    |\n");
                printf("|---------||---------|---------||---------|---------|\n");
                printf("|RAL      ||   %d    |   %.1f  ||   %d    |  %.1f   |\n", evocarexitoRAL, evocarexitomedRAL, evocarfracRAL, evocarfracmedRAL);
                printf("|---------------------------------------------------|\n");
                printf("|RAC      ||   %d    |   %.1f   ||   %d    |  %.1f   |\n", evocarexitoRAC, evocarexitomedRAC, evocarfracRAC, evocarfracmedRAC);
                printf("|---------------------------------------------------|\n");
                printf("|RS       ||   %d     |   %.1f   ||   %d     |  %.1f    |\n", evocarexitoRS, evocarexitomedRS, evocarfracRS, evocarfracmedRS);
                printf("|---------||---------|---------||-------------------|\n");
                printf("**************************************************************\n");
                printf("Presione enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
            }else{
                printf("Estructuras cargadas anteriormente.\n");
                printf("\n");

                printf("**************************************************************\n");
                printf("|---------||-------------------||-------------------|\n");
                printf("|         ||     EVOCAR EX     ||    EVOCAR FRA     |\n");
                printf("|         ||   Max   |  Med    ||   Max   |  Med    |\n");
                printf("|---------||---------|---------||---------|---------|\n");
                printf("|RAL      ||   %d    |   %.1f  ||   %d    |  %.1f   |\n", evocarexitoRAL, evocarexitomedRAL, evocarfracRAL, evocarfracmedRAL);
                printf("|---------------------------------------------------|\n");
                printf("|RAC      ||   %d    |   %.1f   ||   %d    |  %.1f   |\n", evocarexitoRAC, evocarexitomedRAC, evocarfracRAC, evocarfracmedRAC);
                printf("|---------------------------------------------------|\n");
                printf("|RS       ||   %d     |   %.1f   ||   %d     |  %.1f    |\n", evocarexitoRS, evocarexitomedRS, evocarfracRS, evocarfracmedRS);
                printf("|---------||---------|---------||-------------------|\n");
                printf("**************************************************************\n");
                printf("Presione enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
            }

            break;
        default:
            break;
        }
    }

    return 0;
}

