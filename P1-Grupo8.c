#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

void localizar(lista a, char codigo[], int *pos, int *exito){
    *exito = 0;
    int i = 0;
    while ((i < a.elementoscont) && (strcasecmp((a.alumnos[i].codigo),codigo)<0)){
        i++;
    }
    *pos = i;
    if (strcasecmp((a.alumnos[i].codigo),codigo)==0){
        *exito = 1;
    }else{
        *exito = 0;
    }
}

void baja(lista *a,int pos, int *exito){ // CORRECCION REC, NO LOC x2, paso la pos a eliminar
    if(a->elementoscont>0){     // ESTRUCTURA VACIA
            if(pos !=131){
                while (pos < a->elementoscont){ // mientras no llegue al final
                    a->alumnos[pos] = a->alumnos[pos + 1];
                    pos++;
                }
            }
            *exito = 1;
            a->elementoscont -= 1;
    }else{
            *exito = 0; // SI ESTA VACIA, EXITO = 0
        }
}

void alta(lista *a, alumno x, int *exito){
    int pos=0;
    localizar(*a, x.codigo, &pos, exito);
    if(*exito == 1){
        *exito = 0;
    }else{
        int i = a->elementoscont;
            if (i < 130){
                while (i >= pos){
                    a->alumnos[i + 1] = a->alumnos[i];
                    i--;
                }
                a->alumnos[pos] = x;
                *exito = 1;
                a->elementoscont += 1;
            }else{
                *exito = 2;
            }
    }
}

void evocar(lista a, char codigo[], alumno *alum,int *pos, int *exito){ // DEVUELVO POS PARA PASARLO A LA BAJA
    if(a.elementoscont>0){     // ESTRUCTURA VACIA
        *pos=0;
        localizar(a, codigo, pos, exito);
        if (*exito==1){
            *alum = a.alumnos[*pos];
        }
    }else{
        *exito = 0;   // SI ESTA VACIA, EXITO = 0
    }
}

void modificacion(lista *a,char codigo[], int *exito){
    if(a->elementoscont>0){         // CORRECION REC, ESTRUCTURA VACIA
        int pos, opmodificar=0;
        char modnombre[80];
        char modmail[23];
        int modnota;
        localizar(*a,codigo,&pos,exito);
        if (*exito == 1){
            while(opmodificar!=4){
                system("cls");
                printf("****************************************\n"
                "1- Modificar nombre\n"
                "2- Modificar mail\n"
                "3- Modificar nota\n"
                "4- Salir\n"
                "****************************************\n");
                scanf("%d", &opmodificar);
                system("cls");
                        switch(opmodificar){
                            case 1:
                                printf("****************************************\n"
                                        "Ingrese el nombre modificado\n"
                                        "****************************************\n");
                                scanf(" %[^\n]s", modnombre);
                                strcpy(a->alumnos[pos].nombre,modnombre);
                                break;
                            case 2:
                                printf("****************************************\n"
                                       "Ingrese el mail modificado\n"
                                       "****************************************\n");
                                scanf(" %[^\n]s", modmail);
                                strcpy(a->alumnos[pos].mail,modmail);
                                break;
                            case 3:
                                printf("****************************************\n"
                                       "Ingrese la nota modificado\n"
                                       "****************************************\n");
                                scanf(" %d", &modnota);
                                if(modnota>=0 && modnota<=10){
                                    a->alumnos[pos].nota = modnota;
                                    if(modnota == 7)
                                        strcpy(a->alumnos[pos].condicion,"Regular");
                                    if(modnota == 0)
                                        strcpy(a->alumnos[pos].condicion,"Ausente");
                                    if((modnota >= 8) && (modnota <= 10))
                                        strcpy(a->alumnos[pos].condicion,"Promociona");
                                    if(modnota >0 && modnota <7)
                                        strcpy(a->alumnos[pos].condicion,"Libre");
                                }
                                break;
                            default:
                                break;
                        }
            }
        }
    }else{
        *exito = 0; // SI ESTA VACIA, EXITO = 0
    }
}

int Lecturadatos(lista *alumnos){
    FILE *fp;
    int contaux = 0;
    int exito = 0;
    alumno aux;
    if(( fp = fopen( "prueba.txt" , "r") )==NULL){
        return 0;
    }else {
    while (!( feof (fp))){
        fscanf( fp , " %[^\n] " ,aux.codigo );
        fscanf( fp , " %[^\n] " ,aux.nombre );
        fscanf( fp , " %[^\n] " ,aux.mail );
        fscanf(fp," %d",&aux.nota);
        fscanf( fp , " %[^\n] ",aux.condicion );

        if((aux.nota>= 0) && (aux.nota <=10)){
            alta(alumnos, aux, &exito);
        }else{
            exito = 0;
        }
        switch(exito){
            case 0:
                printf("El alumno con CODIGO %s no se ingreso \n", aux.codigo);
                break;
            case 1: break;
            case 2:
                if (contaux==0){
                    printf("La lista esta llena\n");
                    contaux+=1;
                }
                break;
            default:
                break;
        }
        if (exito==2){
            continue;
        }
    }

    fclose( fp );

    return 1;
    }
}

int main(){
    int opt=0,sel, i, x,pos=0;
    lista list;
    alumno aux;
    list.alumnos[0]= aux;
    list.elementoscont = 0;
    int listacarga=0;
    int exito = 0;
    int optaux=0;
    char codigoaux[7];
    while (opt != 7){
        printf("****************************************\n"
               "1- Ingresar nuevo alumno\n"
               "2- Eliminar alumno existente\n"
               "3- Modificar datos de un alumno\n"
               "4- Consultar informacion de un alumno\n"
               "5- Cargar alumnos desde el archivo\n"
               "6- Mostrar todos los alumnos\n"
               "7- Salir\n"
               "****************************************\n");
        scanf("%d", &opt);
        system("cls");
        switch (opt){
        case 1:
            exito=0;
            alumno a;
            printf("Ingrese el codigo: ");
            scanf(" %[^\n]s", a.codigo);

            printf("Ingrese el nombre: ");
            scanf(" %[^\n]s", a.nombre);
            printf("Ingrese el mail: ");
            scanf(" %[^\n]s", a.mail);
            do{
                printf("Ingrese la nota: ");
                scanf(" %d", &a.nota);
            }while(a.nota <=-1 || a.nota >= 11 );

            if(a.nota == 7)
                strcpy(a.condicion, "Regular");
            if((a.nota >=8) && (a.nota <= 10))
                strcpy(a.condicion, "Promociona");
            if((a.nota >=1) && (a.nota <= 6))
                strcpy(a.condicion, "Libre");
            if(a.nota == 0)
                strcpy(a.condicion, "Ausente");
            system("cls");

            alta(&list, a, &exito);


            switch (exito){
            case 0:
                printf("****************************************\n"
                       "Ingreso fallido\n"
                       "****************************************\n");
                break;
            case 1:
                printf("****************************************\n"
                       "El ingreso ha sido exitoso\n"
                       "****************************************\n");
                break;
            case 2:
                printf("****************************************\n"
                       "La lista esta completa\n"
                       "****************************************\n");
                break;
            default:
                break;
            }
            printf("Presione enter para continuar...\n");
            fflush(stdin);
            getchar();
            system("cls");
            break;
        case 2:
            pos=0;
            exito=0;
            optaux=0;
            if(list.elementoscont>0){
                alumno alum;
                    printf("****************************************\n"
                           "Ingrese el CODIGO del alumno a eliminar: \n"
                           "****************************************\n");
                    scanf(" %[^\n]s", codigoaux);
                evocar(list, codigoaux, &alum,&pos, &exito);

                if(exito==1){
                    printf("****************************************\n");
                    printf("CODIGO: %s \n" ,alum.codigo);
                    printf("Nombre: %s \n" ,alum.nombre);
                    printf("Mail: %s \n" ,alum.mail);
                    printf("Nota: %d \n" ,alum.nota);
                    printf("Condicion: %s \n" ,alum.condicion);
                    printf("****************************************\n");
                    printf("Desea eliminar el alumno %d?\n1-Si\n2-No\n",pos+1);
                    printf("****************************************\n");
                    scanf(" %d", &optaux);
                    if(optaux==1){
                        baja(&list, pos, &exito);
                        if(exito==1){
                            printf("****************************************\n"
                                   "La baja ha sido exitosa\n"
                                   "****************************************\n");
                        }else{
                            printf("****************************************\n"
                                   "No se pudo dar de baja el alumno\n"
                                   "****************************************\n");
                        }
                    }
                }else{
                    printf("****************************************\n"
                            "El alumno con ese CODIGO no esta en la lista\n"
                            "****************************************\n");
                }
                printf("Presione enter para continuar...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }else{
                printf("****************************************\n"
                       "La lista esta vacia.\n"
                       "****************************************\n");
                        printf("Presione enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
            }
            break;
        case 3:
            exito=0;
            if(list.elementoscont>0){
                    printf("****************************************\n"
                           "Ingresar CODIGO del alumno a modificar\n"
                           "****************************************\n");
                    scanf(" %[^\n]s", codigoaux);
                 modificacion(&list,codigoaux,&exito);
                if(exito==1){
                    printf("****************************************\n"
                           "La modificacion fue exitosa\n"
                           "****************************************\n");
                }else{
                    printf("****************************************\n"
                           "El alumno con ese CODIGO no esta en la lista\n"
                           "****************************************\n");
                }
                printf("Presione enter para continuar...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }else{
                printf("****************************************\n"
                       "La lista esta vacia.\n"
                       "****************************************\n");
                       printf("Presione enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        system("cls");
            }
            break;
        case 4:
            pos=0;
            exito=0;
            alumno al;
            if(list.elementoscont>0){
                printf("****************************************\n"
                           "Ingresar CODIGO del alumno\n"
                           "****************************************\n");
                scanf(" %[^\n]s", codigoaux);
                evocar(list, codigoaux, &al, &pos,&exito);
                if(exito==1){
                    printf("****************************************\n");
                    printf("CODIGO: %s \n" ,al.codigo);
                    printf("Nombre: %s \n" ,al.nombre);
                    printf("Mail: %s \n" ,al.mail);
                    printf("Nota: %d \n" ,al.nota);
                    printf("Condicion: %s \n" ,al.condicion);
                    printf("****************************************\n");
                }else{
                    printf("****************************************\n"
                    "El alumno con ese CODIGO no esta en la lista\n"
                    "****************************************\n");
                }
                printf("Presione enter para continuar...\n");
                fflush(stdin);
                getchar();
                system("cls");
            }else{
                printf("****************************************\n"
                       "La lista esta vacia.\n"
                       "****************************************\n");
                       printf("Presione enter para continuar...\n");
                    fflush(stdin);
                    getchar();
                    system("cls");
            }
            break;
        case 5:
            if(listacarga==0){
                if(Lecturadatos(&list)==1){
                    printf("****************************************\n"
                       "La lista fue cargada con exito\n"
                       "****************************************\n");
                }else{
                    printf("****************************************\n"
                       "La lista no pudo ser cargada\n"
                       "****************************************\n");
                }
                listacarga=1;
            }else{
                printf("****************************************\n"
                       "La lista ya fue cargada\n"
                       "****************************************\n");
            }
            printf("Presione enter para continuar...\n");
            fflush(stdin);
            getchar();
            system("cls");
            break;
        case 6:
            x=0;
            if(list.elementoscont>0){
                        for(x=0; x< list.elementoscont; x++){
                            printf("****************************************\n");
                            printf("Codigo: %s \n" ,list.alumnos[x].codigo);
                            printf("Nombre: %s \n" ,list.alumnos[x].nombre);
                            printf("Mail: %s \n" ,list.alumnos[x].mail);
                            printf("Nota: %d \n" ,list.alumnos[x].nota);
                            printf("Condicion: %s \n" ,list.alumnos[x].condicion);
                            printf("****************************************\n");
                            if((x+1)% 5 == 0 && (x+1)<list.elementoscont){
                                printf("ALUMNOS %d/%d\n"
                                    "****************************************\n",(x+1),list.elementoscont);
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
        default:
            break;
        }
    }
    return 0;

}
