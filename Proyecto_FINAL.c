
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h> /* setlocale */
#include <time.h> 

#define T 30

char z;
int c_elementos = 0, CANT = 0;			//contador de elementos ingrsados al inventario.
FILE *f = NULL;							// Puntero gloval para la apertura de los archivos.

char cadena[60];						//VARIBALE GLOBAL UTILIZADA PARA INGRESAR DATOS 

struct turno
{
	int n_turno;						//Contador de cantidad de turnos en el día
	char nombre[60];
};

struct dia
{
	struct turno t_MoT[2][9];			//en la primer fila tengo el turno mañana y en la segunda fila el turno tarde
}c_mes[31];

struct pila_turnos
{
	int fecha;
	int manianatarde;
	int inicio;
	int fin;
	char nombre[60];
	struct pila_turnos *siguiente;
};
	
float intervalo_horas_maniana[]={7.0,7.3,8.0,8.3,9.0,9.3,10.0,10.3,11.0,11.3,12};
float intervalo_horas_tarde[]={2.0,2.3,3.0,3.3,4.0,4.3,5.0,5.3,6.0,6.3,7};

struct pila_turnos *pila=NULL;

struct cliente
{
	char c_nombre[60];
	char c_sexo;
	int c_edad;
	char c_num_tel[15]; 
 	struct cliente *anterior;
	struct cliente *siguiente;
	
};

struct cliente *ultimo=NULL;
struct cliente *primero=NULL;

int v[T] = {15,7,23,3,11,19,27,1,5,9,13,17,21,25,29,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};

struct guia 							// Vector guia para el amnejo del Arbol del inventario.
{
	char n_p[60];						//estrctura que relaciona los nombres de los productos,
	int c_p;       						// y el codigo de producro que le corresponde para obtener un arbol equilubrado.
};

struct guia arreglo [T];				//arreglo que contiene todos los nombres cargados y codigos asignados (vector guia).

struct guia *pguia = arreglo;			//puntero parael manejo del vector guia.
struct guia *auxiliar = arreglo;

struct inventario 
{										//estructura de cada elemento del arbol que contiene:
	char nom_producto [60];			//Nombre del Producto.
	int cod_producto;					//Codigo Interno.
	int cantidad;						//Cantidad Disponible.
	struct inventario *h_izq;			//Direccion de Memoria del Elemento " Hijo Izquierdo ".
	struct inventario *h_der;			//Direccion de Memoria del Elemento " Hijo Derecho ".
};

struct inventario *aux = NULL;
struct inventario *arbol = NULL;

int local_time ()
{
	time_t rawtime;
	struct tm * timeinfo;
	
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	
	
	return (timeinfo->tm_mday);
}

void mostarcliente(struct cliente *actual)//Esta funcion mustra los distintos campos dentro de la estructura
{
	printf("\n\t  		 ==> MOSTRANDO INFORMACION DEL CLIENTE <==\n");
	printf(" --------------------------------------------------------------------------------------------\n");
	printf("     NOMBRE Y APELLIDO                       SEXO        EDAD            N° Telefono\n ");
	printf("---------------------------------------------------------------------------------------------\n");
	printf("  %-45s%c%12d    %20s\n",actual->c_nombre,actual->c_sexo,actual->c_edad,actual->c_num_tel);
}

struct cliente *busqueda(char *nombre)     //Funcion que devuelva puntero de la direccion de memoria donde se encuentra el cliente hallado
{
	struct cliente *actual=primero;
	int encontrado=-1;
	int a;
	
	
	if(primero!=NULL)
	{
		while((actual!=NULL)&&(encontrado!=1))
		{
			if((a=strcmp(actual->c_nombre,nombre))==0)
			{
				mostarcliente(*&actual);
				encontrado=1;
				return(actual);
			}
			else
			{
				encontrado=0;
			}
			actual=actual->siguiente;
		}
		if(encontrado==0)
		printf("\n\tEl cliente [%s] no ha sido encontrado en la agenda\n",nombre);
		
		return(NULL);
	}
	else
	{
		printf("\n\tERROR,no existen clientes registrados en la lista");
		return(NULL);
	}
	
}
void eliminarpila()                        //Elimina la pila complete (NO REALIZA NADA AL VECTOR)
{
	struct pila_turnos *aux=pila;

	
		while(aux!=NULL)
		{
			pila=aux->siguiente;
			free(aux);
			
			aux=pila;
		}
	
}
void eliminarlista()
{
	struct cliente *actual=primero;
	
		
	while(actual!=NULL)
	{
		primero=actual->siguiente;
		free(actual);
			
			
		actual=primero;
	}
}
void subeliminarturno(int fecha,int mot,int inicio, int fin )
{
	int i=0;
	
	for(i=inicio-1;i<fin;i++)
	{
		c_mes[fecha].t_MoT[mot][i].n_turno=0;
	}
	
}

void eliminarturno()                       //Funcion para eliminar turno
{
	struct cliente *bandera=(struct cliente*)malloc(sizeof(struct cliente));
	char op;
	
	do{
	fflush(stdin);
	printf("Ingrese nombre: ");
	
	fflush(stdin);
	gets(cadena);
	
	//TRANSFORMAMOS A MAYUSCULAS
	int largo2=strlen(cadena);
	for(int i=0;i<largo2;i++)
	{
	cadena[i]=toupper(cadena[i]);
	}
	
	
	
	bandera=busqueda(*&cadena);
	system("cls");
	
	if(bandera==NULL)                   //Nos fijamos si el cliente esta registrado
	{
		do{
			printf("\n");
			printf("\tTurno no encontrado\n\n");
			system("pause");
			system("cls");
			printf("\n\t\t Que desea realizar?	\n");
			printf("\t\t[1]. Reingresar un nombre	\n");
			printf("\t\t[S]. Salir al menu turnos	\n");
			printf("\n\t\tIngrese su opcion: [ ]\b\b");
			fflush(stdin);
			scanf("%c",&op);
			
			switch(op)
			{
			case 'S':
				return;
				break;
				
			case 's':
				return;
				break;
				
			case '1':
				puts("\n");
				break;	
				
			}
		}while(op!='1');
	
	}
	else
	{
	puts("\n\tCliente encontrado,");
	}
	
	}while(bandera==NULL);
	
	int a;
	
	if(bandera!=NULL)						// me fijo en la pila si cliente tiene turno
	{
		struct pila_turnos *actual=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));
		struct pila_turnos *anterior=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));
		
		
		
		if(actual==NULL || anterior==NULL){
			puts("\nError: No se pudo asignar memoria.");
			return;
			
		}else{
			anterior=NULL;
			actual=pila;
			
			if(pila!=NULL){
				printf("\n\tBuscando turnos del cliente:");
				while(actual!=NULL)//RECORRO pila
				{
					if((a=strcmp(actual->nombre,cadena))==0)//caso que se halla encontrado 
					{
						printf("\n\tSe encontro un turno, mostrando informacion: \n ");
						
						if(actual->manianatarde==0)
						printf("\n\tDia %d, Turno Mañana, desde %1.2fhs hasta %1.2fhs \n\n",actual->fecha+1,intervalo_horas_maniana[actual->inicio],intervalo_horas_maniana[actual->fin]);
						else
						printf("\n\tDia %d, Turno Tarde, desde %1.2fhs hasta %1.2fhs \n\n",actual->fecha+1,intervalo_horas_tarde[actual->inicio],intervalo_horas_tarde[actual->fin]);
						
						do{
						printf("\n\t\t Que desea realizar?	\n");
						printf("\t\t[1]. Eliminar	\n");
						printf("\t\t[2]. No eliminar	\n");
						printf("\n\t\tIngrese su opcion: [ ]\b\b");
						fflush(stdin);
						scanf("%c",&op);
						
						
						switch(op)
						{
						case '1':
						subeliminarturno(actual->fecha,actual->manianatarde,actual->inicio,actual->fin);
						if(actual==pila)
						{
							pila=actual->siguiente;
							free(actual);
						}else{
							anterior->siguiente=actual->siguiente;
							free(actual);
						}
						puts("\n\tSe ha eliminado el turno correctamente");
						system("paue");
						system("cls");
						break;
						
						case '2':
						puts("\n\tNO se ha eliminado el turno");
						system("pause");
						system("cls");
						break;
						
						}
						
						}while(op!='2'&& op!='1');
						
						
					}
					anterior=actual;
					actual=actual->siguiente;
					
				}
				
			}else{//no se ecuentra turnos
				puts("\nError, no hay turnos registrados");
			}
		}
		
	}
	
}


void buscarturnoporfecha(int fecha,int mot,int intervalofinal)
{
	struct pila_turnos *actual=pila;
	
	while(actual!=NULL)
	{
	if(actual->fecha==fecha)
    {
	if(actual->manianatarde==mot && actual->fin==intervalofinal)
	{
	printf("Cliente: ");
	puts(actual->nombre);
	printf("\n");
	}
	}
	actual=actual->siguiente;	
	}
	
	
}

void mostrarturnos(int dato)               //Funcion que recorre con un for y mustra las horas reservadas para ese dia
{
	int i,band=3;
	
	
	for(i=0;i<9;i++)
	{
		
		//printf("%1.2fhs a %1.2fhs:  ",intervalo_horas_maniana[i],intervalo_horas_maniana[i+1]);
		
		
		if(c_mes[dato].t_MoT[0][i].n_turno==-1)
		{
			if(band==1){
			//printf(" Resevado ");
            }
			else{
			printf("\tInicio: %1.2fhs   ",intervalo_horas_maniana[i]);
			band=1;}
        }			
			

		
		
		if(c_mes[dato].t_MoT[0][i].n_turno==1)
		{
			if(band==1){
			printf("Fin: %1.2fhs  ",intervalo_horas_maniana[i+1]);
			buscarturnoporfecha(dato,0,i+1);}
			else
			{
			printf("\tInicio: %1.2fhs   Fin: %1.2fhs  ",intervalo_horas_maniana[i],intervalo_horas_maniana[i+1]);
			buscarturnoporfecha(dato,0,i+1);
			}
			band=0;
			
			
			
		}
	    
		
		if(c_mes[dato].t_MoT[0][i].n_turno==0){
		//printf("Disponible");
		band=0;}
		//printf("\n");
		
	}
	

	for(i=0;i<9;i++)
	{
		
		//printf("%1.2fhs a %1.2fhs:  ",intervalo_horas_maniana[i],intervalo_horas_maniana[i+1]);
		
		
		if(c_mes[dato].t_MoT[1][i].n_turno==-1)
		{
			if(band==1){
				//printf(" Resevado ")
				}
			else{
				printf("\tInicio: %1.2fhs   ",intervalo_horas_tarde[i]);
				band=1;}
		}			
		
		
		
		
		if(c_mes[dato].t_MoT[1][i].n_turno==1)
		{
			if(band==1){
			printf("Fin: %1.2fhs  ",intervalo_horas_tarde[i+1]);
			buscarturnoporfecha(dato,1,i+1);}
			else
			{
				printf("\tInicio: %1.2fhs   Fin: %1.2fhs  ",intervalo_horas_tarde[i],intervalo_horas_tarde[i+1]);
				buscarturnoporfecha(dato,1,i+1);
			}
			band=0;
			
			
			
		}
		
		
		if(c_mes[dato].t_MoT[1][i].n_turno==0){
			//printf("Disponible");
			band=0;}
		//printf("\n");
	}
	
	
}

void sub_verTurnos(int dia)                //Funcion para poder verificar si hay turnos en el dia buscado o no
{
	int i;
	int c=0;
	
	
	int j;
	
	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)
		{
			if(c_mes[dia].t_MoT[j][i].n_turno!=0)
				c=1;
		}
	}
	if(c==0)//CASO QUE ESTE VACIO LOS TURNOS
	{
		printf("\n\t No hay turnos registrados en el dia [%d] \n",dia+1);
	}
	else
	{//CASO QUE SI HAYAN TURNOS PARA LA FECHA
		system("cls");
		printf("\n\tMostrando Turnos en el dia: [%d] \n\n",dia+1);
		mostrarturnos(dia);
	}
	
	
	
}


void mostrarturnoscliente()
{
	struct cliente *bandera=(struct cliente*)malloc(sizeof(struct cliente));
	char op;
	
	do{
		fflush(stdin);
		printf("Ingrese nombre: ");
		
		fflush(stdin);
		gets(cadena);
		
		//TRANSFORMAMOS A MAYUSCULAS
		int largo2=strlen(cadena);
		for(int i=0;i<largo2;i++)
		{
			cadena[i]=toupper(cadena[i]);
		}
		
		bandera=busqueda(*&cadena);
		
		if(bandera==NULL)                   //Nos fijamos si el cliente esta registrado
		{
			do{
				printf("\n");
				system("pause");
				system("cls");
				printf("\n\n\t\t Que desea realizar?	\n");
				printf("\t\t[1]. Reingresar un nombre	\n");
				printf("\t\t[S]. Salir al menu turnos	\n");
				printf("\n\t\tIngrese su opcion: [ ]\b\b");
				fflush(stdin);
				scanf("%c",&op);
				
				switch(op)
				{
				case 'S':
					return;
					break;
					
				case 's':
					return;
					break;
					
				case '1':
					puts("\n");
					break;	
					
				default:
					puts("Opcion invalida, reingrese");
					
				}
			}while(op!='1');
			
		}
		else
		{
			puts("\nCliente encontrado!");
		}
		
	}while(bandera==NULL);
	
	int a;
	if(bandera!=NULL)						// me fijo en la pila si cliente tiene turno
	{
		struct pila_turnos *actual=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));
		
		
		
		if(actual==NULL) {
			puts("\nError: No se pudo asignar memoria.");
			return;
			
		}else
		{
			actual=pila;
			
			if(pila!=NULL){
				printf("\n\tBuscando turnos del cliente\n");
				while(actual!=NULL)//RECORRO pila
				{
					if((a=strcmp(actual->nombre,cadena))==0)//caso que se halla encontrado 
					{
						
						if(actual->manianatarde==0)
							printf("\n\tDia %d, Turno Maniana, desde %1.2fhs hasta %1.2fhs \n",actual->fecha+1,intervalo_horas_maniana[actual->inicio],intervalo_horas_maniana[actual->fin]);
						else
							printf("\n\tDia %d, Turno Tarde, desde %1.2fhs hasta %1.2fhs \n",actual->fecha+1,intervalo_horas_tarde[actual->inicio],intervalo_horas_tarde[actual->fin]);
					}
					actual=actual->siguiente;
					
				}
				
			}else{//no se ecuentra turnos
				puts("\nERROR, No hay turnos registrados");
			}
		}
		
	}
}

void verTurnos()                           //Menu para especificar de que dia quiere ver los turnos
{
	
	char opc;
	int dia_hoy,dia_cualquiera;
	
	fflush(stdin);
	
	do{
		system("cls");
		printf("\n=================================================================");
		printf("\n			PROGRAMA V1.0								");
		printf("\n=================================================================");
		printf("\n\n\t\t MENU VER TURNOS	\n");
		printf("\n\t\t[1]. Ver turnos de hoy	\n");
		printf("\t\t[2]. Ver turnos de un dia en especifico	\n");
		printf("\t\t[3]. Ver turnos de un cliente en especifico	\n");
		printf("\t\t[s]. Salir al menu turnos \n");
		printf("\n\t\tIngrese su opcion: [ ]\b\b");
		fflush(stdin);
		scanf("%c",&opc);
		
		
		switch(opc){
		case '1':
			dia_hoy=local_time();//TURNOS DE HOY
			system("cls");
			sub_verTurnos(dia_hoy-1);
			printf("\n");
			system("pause");
			break;
			
		case '2':
			
			do{
				system("cls");
				printf("\n Ingrese el dia[ ]\b\b");
				fflush(stdin);
				fgets(cadena,30,stdin);
				dia_cualquiera=atoi(cadena);
				
				if(dia_cualquiera<=0 ||  dia_cualquiera>31 )
					printf("\n ERROR: este no es un dia valido, intente nuevamente...");
				
			} while(dia_cualquiera<=0 ||  dia_cualquiera>31);
			
			
			sub_verTurnos(dia_cualquiera-1);
			printf("\n");
			system("pause");
			break;
			
		case '3':
			system("cls");
			mostrarturnoscliente();
			printf("\n");
			system("pause");
			break;
			
		
		
	   case 's':
	break;
		   
	   case 'S':
	   break;

		
		}
	} while(opc!='s' && opc!='S');
	
	
	
}

int insertar()                             //Funcion para registrar un nuevo cliente
{
	system("cls");
	char op2,op3,op4;
	int asignado=0;
	
	//INGRESO DEL NUEVO NODO
	//CERRAR AL TERMINAR EL INGRESO
	struct cliente *nuevo=(struct cliente*)malloc(sizeof(struct cliente));                             //Creacion del nuevo nodo
	
	if(nuevo==NULL)
	{
		
		printf("\n\t\t   		 ==> NO SE PUDO CREAR NUEVO CLIENTE <==\n");
		printf("\n Volviendo al MENU CLIENTES.....\n");
		return(0);
	}
	else
	{                                                      
		//CERRAR AL TERMINAR LA CARGA
		
		//CARGA DE DATOS A LA ESTRUCTURA NUEVA
		
		//NOMBRE 
		
		do
		{
			do
			{
			printf("\n\tIngrese el nombre y apellido que desee: ");
			fflush(stdin);
			gets(cadena);
			system("cls");
			}while(cadena[0]=='\0');
		
			//TRANSFORMAMOS A MAYUSCULAS
			int largo2=strlen(cadena);
			for(int i=0;i<largo2;i++)
			{
				cadena[i]=toupper(cadena[i]);
			}
			
			
			if(primero!=NULL)     //PREGUNTAMOS SI LA LISTA CONTIENE ELEMENTOS Y SI NO POSEE NO REALIZA LA BUSQUEDA
			{
				struct cliente *aux;             //CREO PUNTERO AUXILIAR PARA VERIFICAR SI SE ENCUENTRA EL NOMBRE EN EL REGISTRO O NO
				aux=busqueda(*&cadena);
				
				if(aux!=NULL)       //Preguntamos si el puntero que regreso ya posee dicho cliente en la lista
				{
					printf("\nERROR, nombre ya registrado\n");
					printf("\n\t\t[1].Ingresar otro nombre");
					printf("\n\t\t[s]. para salir: \n");
					printf("\t\tingrese opcion[ ]\b\b");
					scanf("%c",&op4);
					
					if(op4=='s' || op4=='S')
						return(0);
					
				}
				else
				{
					printf("\n\tCreando cliente nuevo: [%s]\n",cadena);
					strcpy(nuevo->c_nombre,cadena);
					asignado=1;
				}
			}
			else
			{
				strcpy(nuevo->c_nombre,cadena);
				asignado=1;
			}
			
		}while(asignado!=1);
		
		
		
		
		//SEXO
		printf("\n\tEscriba el sexo del cliente [M/F]: ");
		do
		{
			fflush(stdin);
			scanf("%c",&nuevo->c_sexo);
			nuevo->c_sexo=toupper(nuevo->c_sexo);                   //Convertimos el sexo en mayuscula
			if(nuevo->c_sexo!='M' && nuevo->c_sexo!='F')
			{
				printf("\n\tError, Ingrese nuevamente: ");
			}
		}while(nuevo->c_sexo!='M' && nuevo->c_sexo!='F');
		
		
		//EDAD 
		printf("\n\tEscriba la edad del cliente: ");
		do{
			
			fflush(stdin);
			fgets(cadena,5,stdin);
			nuevo->c_edad=atoi(cadena);
			
			if(nuevo->c_edad<=0)
				printf("\n\tError,valor no permitido. Ingrese nuevamente");     //Verificamos la entrada Edad>0
			
		} while(nuevo->c_edad <= 0);
		
		//NUMERO DE CONTACTO
		printf("\n\tEscriba el numero de contacto o un '0' si no posee: ");    
		fflush(stdin);
		gets(nuevo->c_num_tel);
		
		//PREGUNTAMOS AL USUARIO SI LOS DATOS ESTAN BIEN INGRESADOS:
		
		//En esta parte del programa se modificara los datos si se ingreso alguno de manera erronea
		system("cls");
		mostarcliente(*&nuevo);
		printf("\n\n\tDESEA MODIFICAR ALGUN CAMPO ? [S/N])[ ]\b\b");
		
		do{
			fflush(stdin);
			scanf("%c",&op2);
			op2=toupper(op2);
			if(op2=='S'&&op2=='N')
			{
			printf("\nOPCION INCORRECTA, Ingrese nuevamente: ");    
			}
			
		}while(op2!='S'&&op2!='N');
		
		if(op2=='S')
		{
			asignado=0;
			do{
				
				printf("\n\t\t Que campo desea modificar? 	\n");
				printf("\n\t\t[1]. Nombre	\n");
				printf("\t\t[2]. Edad	\n");
				printf("\t\t[3]. Sexo	\n");
				printf("\t\t[4]. Numero de contacto	\n");
				printf("\t\t[S]. Para finalizar la correccion	\n");
				printf("\n\t\tIngrese su opcion: [ ]\b\b");
				fflush(stdin);
				scanf("%c",&op3);
				
				switch(op3)
				{
				case '1':
					
					do
					{
						printf("\n\t\tIngrese el nuevo nombre y apellido que desee: ");
						fflush(stdin);
						gets(cadena);
						
						//TRANSFORMAMOS A MAYUSCULAS
						int largo2=strlen(cadena);
						for(int i=0;i<largo2;i++)
						{
							cadena[i]=toupper(cadena[i]);
						}
						
						if(primero!=NULL)     //PREGUNTAMOS SI LA LISTA CONTIENE ELEMENTOS
						{
							struct cliente *aux;             //CREO PUNTERO AUXILIAR PARA VERIFICAR SI SE ENCUENTRA EL NOMBRE EN EL REGISTRO O NO
							aux=busqueda(*&cadena);
							if(aux!=NULL)       //Preguntamos si el puntero que regreso ya posee dicho cliente en la lista
							{
								printf("\nERROR, nombre ya registrado\n");
								printf("\n\t\t[1].Ingresar otro nombre");
								printf("\n\t\t[s]. para salir: ");
								printf("\t\tingrese opcion[ ]\b\b");
								scanf("%c",&op4);
								
								if(op4=='1')
								{system("cls");}
								
								if(op4=='s' || op4=='S')
									return(0);
							}
							else
							{
								strcpy(nuevo->c_nombre,cadena);
								asignado=1;
							}
						}
						else
						{
							strcpy(nuevo->c_nombre,cadena);
							asignado=1;
						}
						
					}while(asignado!=1);
					
					printf("\n");
					system("pause");
					system("cls");
					printf("\n\tCampo Modificado con exito.......\n");
					mostarcliente(*&nuevo);
					printf("\n");
					break;
					
				case '2':
					printf("\nEscriba la edad del cliente: ");
					do{
						fflush(stdin);
						fgets(cadena,5,stdin);
						nuevo->c_edad=atoi(cadena);
						
						if(nuevo->c_edad<=0)
							printf("\nError,valor no permitido. Ingrese nuevamente");     //Verificamos la entrada Edad>0
						
					} while(nuevo->c_edad <= 0);
					printf("\n");
					system("pause");
					system("cls");
					printf("\n\tCampo Modificado con exito.......\n");
					mostarcliente(*&nuevo);
					printf("\n");
					break;
					
				case '3':
					printf("\nEscriba el nuevo sexo del cliente [M / F]: ");
					do{
						fflush(stdin);
						scanf("%c",&nuevo->c_sexo);
						nuevo->c_sexo=toupper(nuevo->c_sexo);
						if(nuevo->c_sexo!='M' && nuevo->c_sexo!='F')
						{
							printf("\n\tError, Ingrese nuevamente: ");
						}
					}while(nuevo->c_sexo!='M' && nuevo->c_sexo!='F');
					system("cls");
					printf("\nCampo Modificado con exito.......");
					mostarcliente(*&nuevo);
					printf("\n");
					break;
					
				case '4':
					printf("\nEscriba el nuevo numero de contacto: ");
					fflush(stdin);
					gets(nuevo->c_num_tel);
					printf("\n");
					system("pause");
					system("cls");
					printf("\n\tCampo Modificado con exito.......\n");
					mostarcliente(*&nuevo);
					printf("\n");
					break;
					
				case 's':
					break;
				case 'S':
					break;
					
				default:
					printf("\nERROR, INGRESE NUEVAMENTE:  ");
					
				}
				
			}while(op3!='s'&&op3!='S');
		}
		
		strcpy(cadena,nuevo->c_nombre);     //Necesitamos que cadena sea el nombre para poder guardar la pila en la funcion crearturno
		
		//TRABAJAMOS CON LOS PUNTEROS PARA PODER INGRESAR CORRECTAMENTE AL cliente
		
		if(primero==NULL)
		{
			primero=nuevo;
			primero->siguiente=NULL;
			primero->anterior=NULL;
			ultimo=primero;
		}
		else
		{
			ultimo->siguiente=nuevo;
			nuevo->siguiente=NULL;
			nuevo->anterior=ultimo;
			ultimo=nuevo;
		}
		
		return(1);
		
	}
}
int crearTurno()                           //Funcion para crear un nuevo turno
{
		
		int opcion,i,MoT;
		int dia;
		int intervalo_inicial;
		int intervalo_final;
		int bandera;
		char op;
		int c=0,j;
		
		do
		{
			system("cls");
			c=0;
			bandera=1;
			printf("\n Ingrese el dia[ ]\b\b");
			fflush(stdin);
			fgets(cadena,30,stdin);
			opcion=atoi(cadena);
			
			if(opcion<=0 || opcion>=31)
			{
			printf("\n ERROR: opcion no valida, intente nuevamente...");
			bandera=0;
			}
			else
			{
			for(j=0;j<2;j++)
			{
				for(i=0;i<9;i++)                                     //PREGUNTA SI ESE DIA YA ESTA OCUPADO
				{
					if(c_mes[opcion-1].t_MoT[j][i].n_turno!=0)
					c++;
				}
			}
			
			if(c==18)
			{
			printf("Error, los turnos en ese dia estan completos");
			bandera=0;
			}
			
			}
			
			
			
			
		} while(bandera!=1);
		
		dia=opcion-1;
		
		if(c==0)
		{
			printf("\nNo hay turnos registrados en el dia [%d] \n",dia+1);
			printf("\n");
			system("pause");
		}
		
		do{
			
			opcion=0;
			c=0;
			system("cls");
			printf("\n=================================================================");
			printf("\n			PROGRAMA V1.0								");
			printf("\n=================================================================");
			printf("\n\n\t\tMENU CREAR TURNOS\n");
			printf("\n\t\t[1]. Maniana	\n");
			printf("\t\t[2]. Tarde	\n");
			printf("\nPara el dia [%d], seleccione en que momento del dia quiere crear un turno:[ ]\b\b",dia+1);
			fflush(stdin);
			fgets(cadena,30,stdin);
			opcion=atoi(cadena);
			
			for(i=0;i<9;i++)
			{
				if(c_mes[dia].t_MoT[opcion-1][i].n_turno!=0)        //PREGUNTAMOS SI ESE TURNO ESTA LLENO          
				{
					c++;
				}
			}
			
			if(c==9){
			puts("\nError, en ese turno del dia no quedan mas horas disponibles");
			opcion=3;
			printf("\n");
			system("pause");
			}
				
		} while(opcion<=0 || opcion>=3);
		
		MoT=opcion-1;
		
		do
		{
			do
			{
				bandera=0;	
				if(opcion==1)
				{
					system("cls");
					printf("\n\tMostrando turnos del dia: [%d]\n",dia+1);
					mostrarturnos(dia);
					printf("\n\tTurno Mañana (HORA INICIAL) \n");
					printf("\n\t\t1) 7:00hs");
					printf("\n\t\t2) 7:30hs");
					printf("\n\t\t3) 8:00hs");
					printf("\n\t\t4) 8:30hs");
					printf("\n\t\t5) 9:00hs");
					printf("\n\t\t6) 9.30hs");
					printf("\n\t\t7) 10:00hs");
					printf("\n\t\t8) 10.30hs");
					printf("\n\t\t9) 11:00hs");
				}
				else
				{
					system("cls");
					printf("\tMostrando turnos del dia: [%d]\n",dia+1);
					mostrarturnos(dia);
					printf("\tTurno Tarde (HORA INICIAL) \n");
					printf("\n\t\t1) 2:00hs");
					printf("\n\t\t2) 2:30hs");
					printf("\n\t\t3) 3:00hs");
					printf("\n\t\t4) 3:30hs");
					printf("\n\t\t5) 4:00hs");
					printf("\n\t\t6) 4.30hs");
					printf("\n\t\t7) 5:00hs");
					printf("\n\t\t8) 5.30hs");
					printf("\n\t\t9) 6:00hs");
				}
				
				
				printf("\n\n\tIngrese hora inicial del turno o 's' para salir:[ ]\b\b");
				fflush(stdin);
				fgets(cadena,60,stdin);
				if(cadena[0]=='s' || cadena[0]=='S')
				{
				return(0);
				}
				intervalo_inicial=atoi(cadena);
				
				if((intervalo_inicial<=0 || intervalo_inicial>9))
				{
					printf("\n ERROR: opcion de tiempo fuera de los limites, intente nuevamente\n");
					bandera=1;	
					printf("\n");
					system("pause");
				}
				else
				{
					if(c_mes[dia].t_MoT[MoT][intervalo_inicial-1].n_turno!=0)
					{
						bandera=1;	
						printf("\n Error, hora ya ocupada, verifique la hora ingresada con los turnos en la parte superior del programa\n\n");
						system("pause");
					}
				}
				
			}while(bandera!=0);
			
			
			do{
				bandera=0;
				
				if(opcion==1)
				{
					system("cls");
					printf("\tMostrando turnos del dia: [%d]\n",dia+1);
					mostrarturnos(dia);
					printf("\tTurno Mañana (HORA FINAL) \n");
					printf("\n\t\t1) 7:30hs");
					printf("\n\t\t2) 8:00hs ");
					printf("\n\t\t3) 8:30hs");
					printf("\n\t\t4) 9:00hs");
					printf("\n\t\t5) 9:30hs");
					printf("\n\t\t6) 10:00hs");
					printf("\n\t\t7) 10:30hs");
					printf("\n\t\t8) 11:00hs");
					printf("\n\t\t9) 11:30hs");
				}
				else
				{
					system("cls");
					printf("\tMostrando turnos del dia %d\n",dia+1);
					mostrarturnos(dia);
					printf("\tTurno Tarde (HORA FINAL) \n");
					printf("\n\t\t1) 2:30hs");
					printf("\n\t\t2) 3:00hs ");
					printf("\n\t\t3) 3:30hs");
					printf("\n\t\t4) 4:00hs");
					printf("\n\t\t5) 4:30hs");
					printf("\n\t\t6) 5:00hs");
					printf("\n\t\t7) 5:30hs");
					printf("\n\t\t8) 6:00hs");
					printf("\n\t\t9) 6:30hs");
					
				}
				
				
				printf("\n\tIngrese hora final del turno o 's' para salir[ ]\b\b");
				fflush(stdin);
				fgets(cadena,60,stdin);
				if(cadena[0]=='s' || cadena[0]=='S')
				{
				return(0);
				}
				intervalo_final=atoi(cadena);
				
				
				if(intervalo_final<=0||intervalo_final>9)
				{
					printf("\n ERROR: opcion de tiempo fuera de los limites, intente nuevamente\n\n");
					bandera=1;
					printf("\n");
					system("pause");
				}
				else
				{
					if(c_mes[dia].t_MoT[MoT][intervalo_final-1].n_turno!=0)
					{
						bandera=1;	
						printf("\n Error, hora ya ocupada, verifique la hora ingresada con los turnos en la parte superior del programa\n");
						printf("\n");
						system("pause");
					}
				}
				
			} while(bandera!=0);
			
			
			//NOS ASEGURAMOS QUE LOS TIEMPOS DENTRO DEL RANGO NO ESTEN OCUPADOS
			if(intervalo_final<intervalo_inicial)
			{
				printf("\n ERROR: Rango de horas no validos. Reingrese hora inicial y luego la final\n");
				bandera=1;
				printf("\n");
				system("pause");
			}
			else
			{
				for(i=intervalo_inicial-1;i<(intervalo_final);i++)            //VEERIFICA QUE NO HAYA TURNOS EN EL MEDIO
				{
					
					if(c_mes[dia].t_MoT[MoT][i].n_turno!=0)                  
					{
						bandera=1;
						puts("ERROR! Espacio no disponible\n");
					}
					
				}
			}
			
			
		} while(bandera!=0);
		
		
		
		//Ingresamos nombre del cliente para la reserva
		do
		{
		printf("\n\tIngrese el nombre del cliente para su reserva: ");
		fflush(stdin);
		gets(cadena);
		
		//TRANSFORMAMOS A MAYUSCULAS
		int largo2=strlen(cadena);
		for(int i=0;i<largo2;i++)
		{
			cadena[i]=toupper(cadena[i]);
		}
		
		
		struct cliente *aux;             //CREO PUNTERO AUXILIAR PARA VERIFICAR SI SE ENCUENTRA EL NOMBRE EN EL REGISTRO O NO
		system("cls");
		aux=busqueda(*&cadena);
			if(aux!=NULL)       //Preguntamos si el puntero que regreso ya posee dicho cliente en la lista
			{
			printf("\n\tCliente encontrado, guardando su reserva\n\n");
			bandera=1;
			}
			else
			{
				printf("\n\n\t");
				system("pause");
				system("cls");
				
				printf("\n\t\tQue desea realizar	\n");
				printf("\t\t[1]. Ingresar otro nombre \n");
				printf("\t\t[2]. Crear cliente nuevo	\n");
				printf("\t\t[S]. Salir al menu turnos	\n");
				printf("\n\t\tIngrese su opcion: [ ]\b\b");
				fflush(stdin);
				scanf("%c",&op);
				
				switch(op)
				{
				case '1':
				printf("\n");   //No hace nada, agrega un espacio para luego escribir devuelta el nombre
				break;
				
			    case '2':      // Llama a la funcion insertar para luego preguntar si la carga se realizo con exito o no
				bandera=insertar();    //Si la funcion insertar retorna 1 (creo nuevo cliente) si retorna 0 (no creo  nuevo cliente)
				break;	
				
				case 's':
				puts("\tVolviendo al menu agenda\n");
				return(0);
				break;
				
				case 'S':
				puts("\tVolviendo al menu agenda\n");
				return(0);
				break;
				
				default:
				puts("\tOpcion no valida, reingrese\n");
				break;
				
				}
			}
			
		}while(bandera!=1);	
		
		
			 
		//ASIGNACIONES DE RESERVA   1=reserva y final del turno -1=reserva 0=disponible
		
			 for(i=intervalo_inicial-1;i<(intervalo_final);i++)
			 {
				 if(c_mes[dia].t_MoT[MoT][i].n_turno==0)
				 {
					 c_mes[dia].t_MoT[MoT][i].n_turno=-1;
				 }
				 
				 if(i==intervalo_final-1) 
				 {
					 c_mes[dia].t_MoT[MoT][i].n_turno=1;
				 }	
			 }
			 
	    //Guardamos los datos requeridos en una pila_turnos
		struct pila_turnos *nuevo=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));
		if(nuevo==NULL)
		{
			printf("Error, no se pudo asignar memoria");
			return (0);
		}
		else
		{
		nuevo->fecha=dia;
		nuevo->manianatarde=MoT;
		nuevo->inicio=intervalo_inicial-1;
		nuevo->fin=intervalo_final;
		strcpy(nuevo->nombre,cadena);

		nuevo->siguiente=pila;
		pila=nuevo;
	
		return(1);
		
		}
}
		
void menuturnos()                          //Menu para decidir que se quiere hacer en base a los turnos
{

			char opc,op2;
			int a;
			
			do
			{
				system("cls");
				printf("\n=================================================================");
				printf("\n			PROGRAMA V1.0								");
				printf("\n=================================================================");
				printf("\n\n\t\tMENU TURNOS\n");
				printf("\n\t\t[1]. Crear un turno	\n");
				printf("\t\t[2]. MENU ver turnos	\n");
				printf("\t\t[3]. Eliminar turno	\n");
				printf("\t\t[4]. Eliminar TODOS los turnos registrados	\n");
				printf("\t\t[S]. Salir al menu principal	\n");
				printf("\n\t\tIngrese su opcion: [ ]\b\b");
				fflush(stdin);
			    scanf("%c",&opc);
				
				switch(opc)
				{
					
				case '1':
					a=crearTurno();
					if(a==1){
					printf("\n\tEl turno se guardo con exito");
					}else{
					printf("\n\tEl turno no ha sido guardado");
				}
					printf("\n");
					system("pause");
					break;
					
				case '2':
					verTurnos();
					break;
					
				case '3':
					eliminarturno();
					printf("\n");
					system("pause");
					break;
				
				case '4':
				do{
				system("cls");
				printf("\nEsta seguro de eliminar TODOS los clientes de forma permanente ? [S/N]: [ ]\b\b");
				scanf("%c",&op2);
				op2=toupper(op2);
				}while(op2!='S' && op2!='N');
					
				if(op2=='S')
				{
				eliminarpila();
				int i=0,j=0,c=0;
				for(i=0;i<31;i++)
				{
					for(j=0;j<2;j++)
					{
						for(c=0;c<9;c++)	
						{
							c_mes[i].t_MoT[j][c].n_turno=0;
						}
					}
				}
				printf("Se ha eliminado TODOS los turnos\n");
				system("pause");
				}
				
				
				break;
				
				
				}
				
			} while(opc!='s' && opc!='S');
			
}		



void mostrarlista()                        //Funcion para recorrer la lista y mostrar todos los clientes
{
	system("cls");
	fflush(stdin);
	int ac=1;
	
	printf("\n\t\t   		 ==> MOSTRANDO CLIENTES <==\n");
	printf(" --------------------------------------------------------------------------------------------\n");
	printf("     NOMBRE Y APELLIDO                       SEXO        EDAD            N° Telefono\n ");
	printf("---------------------------------------------------------------------------------------------\n");
	
	
	struct cliente *actual=(struct cliente*)malloc(sizeof(struct cliente));
	actual=primero;
	
	if(primero!=NULL)
	{
	while(actual!=NULL)
	{
	printf("  %-45s%c%12d    %20s\n",actual->c_nombre,actual->c_sexo,actual->c_edad,actual->c_num_tel);
	actual=actual->siguiente;
	ac++;
	}
	}
	else
	{
	printf("\nNo posee ningun cliente");
	}
	
}



void modificardatos()                      //Funcion para modificar los datos de un cliente especifico
{
	system("cls");
	char op3,op4;
	fflush(stdin);
	int asignado=0;
	
	struct cliente *actual=NULL;

	
    
	
	
	if(primero!=NULL)
	{
	printf("\n\tIngrese el nombre y apellido del cliente a modificar: ");
	fflush(stdin);
	gets(cadena);
	int largo2=strlen(cadena);
	
	printf("\n\tMostrando informacion: \n");
	
	for(int i=0;i<largo2;i++)
	{
	cadena[i]=toupper(cadena[i]);
	}
	actual=busqueda(*&cadena);
	
	if(actual!=NULL)
	{
	do{
		
		printf("\n\t\tQue campo desea modificar? 	\n");
		printf("\t\t[1]. Nombre	\n");
		printf("\t\t[2]. Edad	\n");
		printf("\t\t[3]. Sexo	\n");
		printf("\t\t[4]. Numero de contacto	\n");
		printf("\t\t[S]. Volver al MENU CLIENTES	\n");
		printf("\n\t\tIngrese su opcion: [ ]\b\b");
		fflush(stdin);
		scanf("%c",&op3);
		
		
		switch(op3)
		{
		case '1':
		do
		{
		printf("\n\tIngrese el nuevo nombre y apellido que desee modificar: ");
		fflush(stdin);
		gets(cadena);
				
		//TRANSFORMAMOS A MAYUSCULAS
		int largo2=strlen(cadena);
		for(int i=0;i<largo2;i++)
		{
		cadena[i]=toupper(cadena[i]);
		}
				
				
		if(primero!=NULL)     //PREGUNTAMOS SI LA LISTA CONTIENE ELEMENTOS
		{
		struct cliente *aux;             //CREO PUNTERO AUXILIAR PARA VERIFICAR SI SE ENCUENTRA EL NOMBRE EN EL REGISTRO O NO
		aux=busqueda(*&cadena);
					
		if(aux!=NULL)       //Preguntamos si el puntero que regreso ya posee dicho cliente en la lista
		{
		printf("\nERROR, nombre ya ingresado\n");
		printf("Eliga la opcion: \n\t1- Ingresar otro nombre o 's' para salir: ");
		scanf("%c",&op4);
						
		if(op4=='s' || op4=='S')
		{puts("No se pudo modificar el cliente\n");
		return;}
				
		}
		else
		{
		strcpy(actual->c_nombre,cadena);
		asignado=1;
		}
		}
		else
		{
		strcpy(actual->c_nombre,cadena);
		asignado=1;
		}
				
		}while(asignado!=1);
		printf("\n");
		system("pause");
		system("cls");
		printf("\n\tCampo Modificado con exito.......\n");
		mostarcliente(*&actual);
		printf("\n");
		break;
		

		case '2':
		printf("\n\tEscriba la edad del cliente: ");
		do{
		
		fflush(stdin);
		fgets(cadena,5,stdin);
		actual->c_edad=atoi(cadena);
		
		if(actual->c_edad<=0)
			printf("\n\tError,valor no permitido. Ingrese nuevamente");     //Verificamos la entrada Edad>0
		
		
		
		} while(actual->c_edad <= 0);
		printf("\n");
		system("pause");
		system("cls");
		printf("\n\tCampo Modificado con exito.......\n");
		mostarcliente(*&actual);
		printf("\n");
		break;
			
		case '3':
		printf("\nEscriba el nuevo sexo del cliente (M o F): ");
		do{
		fflush(stdin);
		scanf("%c",&actual->c_sexo);
		actual->c_sexo=toupper(actual->c_sexo);
		if(actual->c_sexo!='M' && actual->c_sexo!='F')
		{
		printf("\n\tError, Ingrese nuevamente: ");
		}
		}while(actual->c_sexo!='M' && actual->c_sexo!='F');
		printf("\n");
		system("pause");
		system("cls");
		printf("\n\tCampo Modificado con exito.......\n");
		mostarcliente(*&actual);
		break;

		case '4':
		printf("\n\tEscriba el nuevo numero de contacto: ");
		fflush(stdin);
		gets(actual->c_num_tel);
		int largo=strlen(actual->c_num_tel);
		for(int i=0;i<largo;i++)
		{
		actual->c_nombre[i]=toupper(actual->c_nombre[i]);
		}
		printf("\n");
		system("pause");
		system("cls");
		printf("\n\tCampo Modificado con exito.......\n");
		mostarcliente(*&actual);
		printf("\n");
		break;
		
		
		case 's':
		printf("\nVolviendo al MENU CLIENTE");
		break;
		
		case 'S':
		printf("\nVolviendo al MENU CLIENTE");
		break;
		
		}
	}while(op3!='s' && op3!='S');
	
	}
	else
	{
	printf("\n\tNo posee ningun cliente con ese nombre");
	}
	}
	else
	{
	puts("\nLa lista no ha sido creada\n");
	}
	
	
}

void eliminarcliente()                     //Funcion para eliminar un cliente especifico
{
	system("cls");
	struct cliente *actual=(struct cliente*)malloc(sizeof(struct cliente));    //Puntero auxiliar para no perder la lista
	actual=primero;
	
	char nombre[60];
	int a;
	
	int encontrado=0;
	
	struct cliente *atras=(struct cliente*)malloc(sizeof(struct cliente));     //Puntero auxiliar para poder modificar las posiciones de los punteros
	atras=NULL;
	
	printf("\n\tEscriba el nombre y apellido del cliente: ");
	fflush(stdin);
	gets(nombre);
	
	int largo=strlen(nombre);
	for(int i=0;i<largo;i++)
	{
	nombre[i]=toupper(nombre[i]);     //Funcion para poder pasar el string a mayusculas
	}
	
	if(primero!=NULL)        //Preguntamos si la lista esta vacia
	{
	while(actual!=NULL && encontrado!=1)    //Recorremos la lista
	{
	if((a=strcmp(actual->c_nombre,nombre))==0)      //Comparamos el nombre ingresado con el string de la lista
	{
	printf("\nCliente encontrado, mostrando informacion:\n");

	mostarcliente(*&actual);
	printf("\nCliente eliminado");
	encontrado=1;                                            //Aclaramos que el cliente fue encontrado
	
	if(actual==primero)    
	{
	primero=primero->siguiente;   
	free(actual);
	}
	else if (actual==ultimo)    //si el cliente es el ultimo en la lista
	{
	atras->siguiente=NULL;
	ultimo=atras;
	free(actual);
	}
	else     //demas de los casos
	{
    atras->siguiente=actual->siguiente;
	actual->siguiente->anterior=atras;
	free(actual);
	}
	
	
	}   //Ciere del while hasta 
	
	atras=actual;
	actual=actual->siguiente;
	
	}
	
	if(encontrado==0)                                  //Si el ciente no fue encontrado
	printf("\n El cliente [%s] no ha sido encontrado\n",nombre);
	
	}                  
	else
    printf("\nERROR,no hay clientes regitrados");          //Si es igual a NULL significa que no existen ningun nodo
	
	
	
}

void guardaragenda()                       //Funcion que se ejecuta sola al finalizar el programa y guarda los clientes registrados
{
	f=fopen("agenda.txt","w");
	if(f==NULL)
	{
		puts("\nError, no se pudo guardar agenda\n");
		exit(1);
	}
	
	struct cliente *actual=(struct cliente*)malloc(sizeof(struct cliente));
	actual=primero;
	
	while(actual!=NULL)
	{
	fputs(actual->c_nombre,f);
	fprintf(f,"-%c",actual->c_sexo);
	fprintf(f,"-%d-",actual->c_edad);
	fputs(actual->c_num_tel,f);
	fprintf(f,"-");
	fprintf(f,"\n");

	actual=actual->siguiente;
	}
	
	fclose(f);
	
}

void guardarturnos()
{
	f=fopen("turnos.txt","w");
	if(f==NULL)
	{
		puts("\nError, no se pudo guardar los turnos");
		exit(1);
	}
	
	struct pila_turnos *actual=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));
	actual=pila;
	
	while(actual!=NULL)
	{
		fputs(actual->nombre,f);
		fprintf(f,"-%d",actual->fecha);
		fprintf(f,"-%d",actual->manianatarde);
		fprintf(f,"-%d",actual->inicio);
		fprintf(f,"-%d-",actual->fin);
		fprintf(f,"\n");
		
		actual=actual->siguiente;
	}
	
	fclose(f);
	
	
}
void menuclientes()                       //Menu para decidir que se quiere hacer en base de los clientes
{
	char op,op2;
	int carga;
	char nombre[60];
	
	do
	{
	system("cls");
	printf("\n=================================================================");
	printf("\n			PROGRAMA V1.0								");
	printf("\n=================================================================");
	printf("\n\n\t\t MENU CLIENTE	\n");
	printf("\n\t\t[1]. Registrar nuevo cliente	\n");
	printf("\t\t[2]. Mostrar informacion de un cliente	\n");
	printf("\t\t[3]. Modificar datos de un cliente	\n");
	printf("\t\t[4]. Mostrar lista de clientes completa	\n");
	printf("\t\t[5]. Eliminar un cliente\n");
	printf("\t\t[6]. Eliminar la lista de los clientes completa	\n");
	printf("\t\t[S]. Salir al menu principal	\n");
	printf("\n\t\tIngrese su opcion: [ ]\b\b");
	fflush(stdin);
	scanf("%c",&op);
	
	switch(op)
	{
	case '1': 
	carga=insertar();
	if(carga==1){
	printf("\nCarga con exito");}
	else{
	printf("\nNo se ha realizado la carga");}
	puts("\n");
	system("pause");
	break;
	
	case '2':
	printf("\nEscriba el nombre y apellido del cliente: ");
	fflush(stdin);
	gets(nombre);
	int largo=strlen(nombre);
	for(int i=0;i<largo;i++)
	{
	nombre[i]=toupper(nombre[i]);
	}
	printf("\n ");
	system("cls");
	busqueda(nombre);
	printf("\n");
	system("pause");
	break; 
	
	case '3':
	modificardatos();
	puts("\n");
	system("pause");
	break;
	
	case '4':
	mostrarlista();
	puts("\n");
	system("pause");
	break;
	
	case '5':
	eliminarcliente();
	puts("\n");
	system("pause");
	break;
	
	case '6':
	printf("\nEsta seguro de eliminar TODOS los clientes de forma permanente? [S/N]: [ ]\b\b");
	do{
	scanf("%c",&op2);
	op2=toupper(op2);
	}while(op2!='S' && op2!='N');
	
	if(op2=='S'){
	eliminarlista();
	printf("\nSe ha eliminado TODOS los clientes\n\n\ns");
	system("pause");}
	
    break;
	
	case 's':
	break;
	
	case 'S':
	break;
	
	
	}
	
	}while((op!='s')&&(op!='S'));
}

void vaciar(char temp[])                  //FUNCION PARA PODER ELIMINAR LOS DATOS DE LA CADENA DE CARACTER AL LEER LA  AGENDA (lecturadeagenda)
{
	for (int w=0;w<60;w++)
	{
	temp[w]='\0';
	}
}


void lecturaturnos()                      //Lee del archivo de texto los turnos 
{
	char temp[80];
	int cont=-1,j=0;
	char aux;
	int i;
	
	f=fopen("turnos.txt","r");
	
	
	if(f==NULL)
	{
		puts("Error, no se pudo leer la agenda de turnos (No esta creada o posee un error en el ordenador)\n");
	}
	else
	{
		while(!feof(f))             //Funcion obtiene la cadena de cada renglon y va contando cuantos renglones tiene el archivo
		{
			fgets(temp,80,f);
			cont++;
		}
		printf("\nTurnos cargados: [%d]",cont);
		
		rewind(f);   //Pone el cursor al comienzo de la agenda 
		
		
	    for(i=0;i<cont;i++)
		{   //Bucle que crea y asigna los clientes a la memoria de la pc para luego poder utilizarse de manera normal
		 struct pila_turnos *nuevo=(struct pila_turnos*)malloc(sizeof(struct pila_turnos));                             //Creacion del nuevo nodo 
		 if(nuevo==NULL)
		 {
			 printf("\nNO SE PUDO CARGAR LOS turnos");
			 exit(6);
		 }
		 else
		 { 
			 aux='0';
			 
			 
			 
			 //NOMBRE
			 
			 //Vaciamos el contenido de temp para luego introducir el nombre
			 vaciar(temp);
			 
			 for(j=0;aux!='-';j++)
			 {
				 aux=fgetc(f);
				 
				 if(aux!='-')
				 {
					 temp[j]=aux;
				 }
				 
			 }
			 strcpy(nuevo->nombre,temp);   //ALMACENO EL NOMBRE EN LA ESTRUCTURA
			
			 //FECHA
			 int fecha=0;
			 aux='0';
			 
			 //vacio la cadena temp para poder usarla nuevamente
			 vaciar(temp);
			 
			 
			 for(j=0;aux!='-';j++)
			 {
				 aux=fgetc(f);
				 
				 if(aux!='-')
				 {
					 temp[j]=aux;
				 }
				 
			 }
			 fecha=atoi(temp);
			 nuevo->fecha=fecha;    //FUNCION PARA TRANSFORMAR LA CADENA EN ENTERO
			
			 //MoT
			 int mot=0;
			 aux='0';
			 
			 //vacio la cadena temp para poder usarla nuevamente
			 vaciar(temp);
			 
			 
			 for(j=0;aux!='-';j++)
			 {
				 aux=fgetc(f);
				 
				 if(aux!='-')
				 {
					 temp[j]=aux;
				 }
				 
			 }
			 mot=atoi(temp);
			 nuevo->manianatarde=mot;    //FUNCION PARA TRANSFORMAR LA CADENA EN ENTERO
			 
			 //INICIO
			 int inicio=0;
			 aux='0';
			 
			 //vacio la cadena temp para poder usarla nuevamente
			 vaciar(temp);
			 
			 
			 for(j=0;aux!='-';j++)
			 {
				 aux=fgetc(f);
				 
				 if(aux!='-')
				 {
					 temp[j]=aux;
				 }
				 
			 }
			 inicio=atoi(temp);
			 nuevo->inicio=inicio;    //FUNCION PARA TRANSFORMAR LA CADENA EN ENTERO
			 
			 
			 //FINAL
			 int fin=0;
			 aux='0';
			 
			 //vacio la cadena temp para poder usarla nuevamente
			 vaciar(temp);
			 
			 
			 for(j=0;aux!='-';j++)
			 {
				 aux=fgetc(f);
				 
				 if(aux!='-')
				 {
					 temp[j]=aux;
				 }
				 
			 }
			 fin=atoi(temp);
			 nuevo->fin=fin;    //FUNCION PARA TRANSFORMAR LA CADENA EN ENTERO
			 
			 fgetc(f); //LEEMOS EL ULTIMO '-'
				 
				 
			 for(j=inicio;j<(fin);j++)
			 {
				 if(c_mes[fecha].t_MoT[mot][j].n_turno==0)
				 {
					 c_mes[fecha].t_MoT[mot][j].n_turno=-1;
				 }
				 
				 if(j==fin-1) 
				 {
					 c_mes[fecha].t_MoT[mot][j].n_turno=1;
				 }	
				 
				 
			 }
			 //Asignamos los punteros a la pila
			 
			 nuevo->siguiente=pila;
			 pila=nuevo;
			 
			 
			 
		 }
		 
	 }
	 fclose(f);
	 
	} 
	
	
	
	
	
}

void lecturadeagenda()                    //Funcion para poder cargar los clientes registrados y guardados en un archivo de  texto
{
	char temp[80];
	int cont=-1,j=0;
	char aux;
	
	f=fopen("agenda.txt","r");
	
	if(f==NULL)
	{
	puts("Error, no se pudo leer la agenda (No esta creada o posee un error en el ordenador)\n");
	}
	else
	{
	
    while(!feof(f))             //Funcion obtiene la cadena de cada renglon y va contando cuantos renglones tiene el archivo
	{
	fgets(temp,80,f);
	cont++;
	}
	printf("\nClientes cargados: [%d]",cont);
	
	rewind(f);   //Pone el cursor al comienzo de la agenda 
	
	int i=0;
	
	for(i=0;i<cont;i++)     //Bucle que crea y asigna los clientes a la memoria de la pc para luego poder utilizarse de manera normal
	{
	struct cliente *nuevo=(struct cliente*)malloc(sizeof(struct cliente));                             //Creacion del nuevo nodo 

	if(nuevo==NULL)
	{
	printf("\nNO SE PUDO CARGAR LOS CLIENTES");
	exit(4);
	}
	else
	{ 
	
	aux='0';

	//NOMBRE
	
	//Vaciamos el contenido de temp para luego introducir el nombre
	vaciar(temp);
	
	for(j=0;aux!='-';j++)
	{
	aux=fgetc(f);
	
	if(aux!='-')
	{
	temp[j]=aux;
	}
	
	}
	strcpy(nuevo->c_nombre,temp);   //ALMACENO EL NOMBRE EN LA ESTRUCTURA
	
	//SEXO
	char aux2='\0';
	char sexo='\0';
	//Vaciamos el contenido de temp para luego introducir el nombre
	vaciar(temp);
	
	for(j=0;aux2!='-';j++)
	{
	aux2=fgetc(f);
		
	if(aux2!='-')
	{
	sexo=aux2;
	}
		
	}
	nuevo->c_sexo=sexo;
	
	//Edad
	int a=0;
	char aux3='\0';

	//vacio la cadena temp para poder usarla nuevamente
	vaciar(temp);
	
	for(j=0;aux3!='-';j++)
	{
	aux3=fgetc(f);
		
	if(aux3!='-')
	{
	temp[j]=aux3;
	}
		
	}
	a=atoi(temp);
	nuevo->c_edad=a;    //FUNCION PARA TRANSFORMAR LA CADENA EN ENTERO
	
	//Numero de contacto
	vaciar(temp);
	char aux4='\0'; 
	
	for(j=0;aux4!='-';j++)
	{
	aux4=fgetc(f);
	
	if(aux4!='-')
	{
	temp[j]=aux4;
	
	}
	
	}
	
	
	strcpy(nuevo->c_num_tel,temp);   //ALMACENO EL NOMBRE EN LA ESTRUCTURA
	
	fgetc(f); //LEEMOS EL ULTIMO '-'
	
	if(primero==NULL)
	{
	primero=nuevo;
	primero->siguiente=NULL;
	primero->anterior=NULL;
	ultimo=primero;
	}
	else
	{
	ultimo->siguiente=nuevo;
	nuevo->siguiente=NULL;
	nuevo->anterior=ultimo;
	ultimo=nuevo;
	}
	
	}
	
	}
	
	
	fclose(f);
    }
    printf("\n");
	
}
void Minimizar (char *Cadena)			  //Convierte todas las letras de una cadena en minusculas.
{
	int b = strlen(Cadena);
	for (int i=0;i<b;i++){
		Cadena[i]=tolower(Cadena[i]); //Combierte todos los caracteres en minusculas,para poder comparar luego.
	}			
}
struct inventario *CrearElemento (int cp)//Devuelve un puntero con la direccion de memoria del nuevo elemento con el codigo de producto asignado.
{
	char Nombre[60], opcion;
	int i = 0, band = 0;
	//reserva memoria para el nuevo elemento Nu_El.
	struct inventario *Nu_El = (struct inventario *) malloc(sizeof(struct inventario));
	//verifica que se haya podido reservar la memoria para elelemento.
	if(Nu_El!=NULL)
	{
		do{
			pguia = auxiliar;
			printf("\n\t Escriba el nombre del Producto que desea ingresar al inventario: ");
			fflush(stdin);
			gets(Nombre);
			Minimizar(Nombre);
			//Compara los nombres dentro del vector guia con el nombre cargado para ver que no existan repetidos.
			for(i=0;i<T;i++)
			{
				if(strcmp(Nombre,(pguia+i)->n_p)==0)
				{
					band++;
				    break;
				}
			}
			if (band!=0)
			{
				do{
				puts("\n Ese Nombre ya se encuentra cargado, revise la lista de productos o ingrese otro nombre.");
				printf("\n\n\tDesea ingresar otro Nombre o volver al Menu anterior [S/N])[ ]\b\b");
				fflush(stdin);
				scanf("%c",&opcion);
				opcion = tolower (opcion);
				if (opcion!='s'){
					free (Nu_El);
					return NULL;
				}
				}while(opcion!='s' && opcion!='n');
			}
			else
			{
				pguia = auxiliar;
				// si no hallo concidencias, asigna el nombre el Nuevo Elemento.
				strcpy(Nu_El->nom_producto,Nombre);
				// Y lo mismo con el vector guia.
				strcpy((pguia+CANT)->n_p,Nombre);  // ACA ESTABA EL ERROR
				
			}
		} while(band!=0);
		
		c_elementos++;
		Nu_El->cod_producto = cp;									
		printf("\n\t Ingrese la cantidad de dicho Producto para registrar en el inventario: ");
		
		do 
		{
			fflush(stdin);
			scanf("%d",&Nu_El->cantidad);
			if ((Nu_El->cantidad)<0)
			{
				printf(" Ingreso invalido, por favor repita: ");
			}
		}while((Nu_El->cantidad)<0);
		
		Nu_El->h_izq = NULL;
		Nu_El->h_der = NULL;
		
		
		do{
		system("cls");
		puts("\n El producto fue cargado exitosamente. ");
		printf("\n\t\t   		 ==> PRODUCTO CREADO <==\n");
		printf(" --------------------------------------------------------------------------------------------\n");
		printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
		printf("---------------------------------------------------------------------------------------------\n");
		printf(" %8d %28d  %35s\n",Nu_El->cod_producto,Nu_El->cantidad,Nu_El->nom_producto);
		printf("\n\n\tDESEA MODIFICAR ALGUN CAMPO ? [S/N])[ ]\b\b");
		fflush(stdin);
		scanf("%c",&opcion);
		opcion = tolower (opcion);
		}while(opcion!='s' && opcion!='n');
		
		if(opcion=='s')
		{	
			do {
				system("cls");
				printf(" --------------------------------------------------------------------------------------------\n");
				printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
				printf("---------------------------------------------------------------------------------------------\n");
				printf(" %8d %28d  %35s\n",Nu_El->cod_producto,Nu_El->cantidad,Nu_El->nom_producto);
				printf("\n\t\t Que campo desea modificar? 	\n");
				printf("\n\t\t[1]. Nombre	\n");
				printf("\t\t[2]. Cantidad	\n");
				printf("\t\t[S]. Para finalizar la correccion	\n");
				printf("\n\n\tIngrese su opcion: [ ]\b\b");
				fflush(stdin);
				scanf("%c",&opcion);
				opcion = tolower (opcion);
				
				switch(opcion)
				{
				case '1':
					do{
						pguia = auxiliar;
						printf("\n\t Escriba el nuevo nombre del Producto que desea ingresar al inventario: ");
						fflush(stdin);
						gets(Nombre);
						Minimizar(Nombre);
						//Compara los nombres dentro del vector guia con el nombre cargado para ver que no existan repetidos.
						for(i=0;i<T;i++)
						{
							if (strcmp(Nombre,(pguia+i)->n_p)==0)
							{
								band++;
								break;
							}
						}
						
						if (band!=0)
						{
							char op2;
							puts("\n Ese Nombre ya se encuentra cargado, revise la lista de productos o ingrese otro nombre.");
							do{
							printf("\n\n\tDesea ingresar otro Nombre o volver al Menu anterior [S/N])[ ]\b\b");
							fflush(stdin);
							scanf("%c",&op2);
							op2= tolower(op2);
							if (op2=='n'){
								band = 0;
							}
							}while(op2!='s' && op2!='n');
						}
						else
						{
							pguia = auxiliar;
							// si no hallo concidencias, asigna el nombre el Nuevo Elemento.
							strcpy(Nu_El->nom_producto,Nombre);
							// Y lo mismo con el vector guia.
							strcpy((pguia+c_elementos)->n_p,Nombre);
						}
					} while(band!=0);
				break;
			case '2':
					printf("\n\t Ingrese la nueva cantidad de dicho Producto para registrar en el inventario: ");
					
					do 
					{
						fflush(stdin);
						scanf("%d",&Nu_El->cantidad);
						if ((Nu_El->cantidad)<0)
						{
							printf(" Ingreso invalido, por favor repita: ");
						}
					}while((Nu_El->cantidad)<0);
				break;
				
				}
			} while((opcion!='S')&&(opcion!='s'));
			
		}
			
		
		if(opcion=='s'){

			system("cls");
			puts("\n El producto fue modificado exitosamente. ");
			printf("\n\t\t   		 ==> PRODUCTO CREADO <==\n");
			printf(" --------------------------------------------------------------------------------------------\n");
			printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
			printf("---------------------------------------------------------------------------------------------\n");
			printf(" %8d %28d  %35s\n",Nu_El->cod_producto,Nu_El->cantidad,Nu_El->nom_producto);
		}
		
		return Nu_El;
	}
	else
	{
		puts(" ERROR: No se pudo reservar memoria.\n Contacte a servicio Tecnico.");
		return NULL;
	}
}
struct inventario *InsertarElemento(struct inventario *nodo, int cp)//Busca la posicion de un elemto en el árbol binario segun el codigo que le corresponde, manteniendo la direccion de memoria del elemnto " Padre " para no perder la trazabilidad.
{
	//si arbol==NULL, el inventario esta vacio y se debe crear el primer elemento.
	if (nodo == NULL){
		return CrearElemento(cp);
	}
	//si ya existe el primer elemento, decide si continuar por la rama derecha o izquierda.
	if (nodo->cod_producto < cp){
		nodo->h_der = InsertarElemento(nodo->h_der, cp);
	}
	if(nodo->cod_producto > cp){
		nodo->h_izq = InsertarElemento(nodo->h_izq, cp);
	}
	return nodo;
}
void In_Orden(struct inventario *actual)  // Imprimirá el árbol en formato tabla (Antes de la llamada imprime el encabezado de la tabla).
{
	if(actual == NULL)
		return;
	In_Orden(actual->h_izq);
	printf(" %8d %28d  %35s\n\n",actual->cod_producto,actual->cantidad,actual->nom_producto);
	In_Orden(actual->h_der);
}
void Busca_Nombre (struct inventario *nodo, int codigo_p)// Imprimirá los datos del Producto buscado segun su nombre,(solo se llama a la fn si el nombre fue encontrado primero dentro del vector guia).
{
	if (nodo == NULL){
		puts(" \n \t ERROR: Busqueda por Nombre Fallida. \n \t Contacte a servicio Tecnico.\n  ");
		fflush(stdin);
		scanf("%c",&z);
		return;
	}
	if (nodo->cod_producto < codigo_p){
		Busca_Nombre (nodo->h_der, codigo_p);
	}
	if(nodo->cod_producto > codigo_p){
		Busca_Nombre (nodo->h_izq, codigo_p);
	}
	if (nodo->cod_producto==codigo_p){
		/*se asigna la direccion de memoria del elemento buscado
		a un puntero auxiliar " aux " global para disponer de sus datos.*/
		aux = nodo;
		printf(" %8d %28d  %35s\n\n",nodo->cod_producto,nodo->cantidad,nodo->nom_producto);
	}
	
}
void In_Orden_Cantidad (struct inventario *actual)// Imprimirá los productos del árbol que no tengan unidades disponibles.
{
	if(actual == NULL)
		return;
	In_Orden_Cantidad (actual->h_izq);
	if (actual->cantidad==0){
		printf(" %8d %28d  %35s\n\n",actual->cod_producto,actual->cantidad,actual->nom_producto);
	}
	In_Orden_Cantidad (actual->h_der);
}
void Modifica (struct inventario *nodo)   //permite modificar los campos de un producto (Nombre o cantidad).
{
	char opc;
	int band=0, i;
	do {
		
		system("cls");
		printf("\n\t\t Que campo desea modificar? 	\n");
		printf("\n\t\t[1]. Nombre	\n");
		printf("\t\t[2]. Cantidad	\n");
		printf("\t\t[S]. Para finalizar la correccion	\n");
		printf("\n\t\tIngrese su opcion: [ ]\b\b");

			fflush(stdin);
			scanf("%c",&opc);
			opc=tolower(opc);
		
		switch (opc)
		{
		case '1':
			system("cls");
			puts("\n Ha selecionado la opcion '1': Modificar el nombre del Producto. \n");
			printf("Escriba nuevo nombre del producto: ");
			fflush(stdin);
			gets(cadena);
			Minimizar(cadena);
			band=0;
			for(i=0;i<T;i++)
			{
				if(strcmp(cadena,(pguia+i)->n_p)==0)
				{
					band++;
					break;
				}
				
			}
			if (band!=0){
				puts("\n Ese Nombre ya se encuentra cargado, revise la lista de productos o ingrese otro nombre.");
			}else
			{
				for(i=0;i<T;i++)
				{
					if (aux->cod_producto==(pguia+i)->c_p)
					{
						break;
					}
				}
				
				strcpy(aux->nom_producto,cadena);
				strcpy((pguia+i)->n_p,cadena);
				puts("\n Producto modificado, sus nuevos datos son : \n\n");
				printf(" --------------------------------------------------------------------------------------------\n");
				printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
				printf("---------------------------------------------------------------------------------------------\n");
				printf(" %8d %28d  %35s\n\n",aux->cod_producto,aux->cantidad,aux->nom_producto);
			}
			system("pause");
			break;
		case '2':
			system("cls");
			puts("\n Ha selecionado la opcion '2': Modificar la cantidad de un Productos. \n");
			do {
				scanf("Ingrese la cantidad nueva: [ ]\b\b");
				fflush(stdin);
				gets(cadena);
				aux->cantidad=atoi(cadena);
				
				if (aux->cantidad<0)
				{
					printf("\n Ingreso invalido, por favor repita: ");
				}
				
			} while(aux->cantidad<0);
			puts("\n Producto modificado, sus nuevos datos son : \n\n");
			printf(" --------------------------------------------------------------------------------------------\n");
			printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
			printf("---------------------------------------------------------------------------------------------\n");
			printf(" %8d %28d  %35s\n\n",aux->cod_producto,aux->cantidad,aux->nom_producto);
			system("pause");
			break;

		}
	} while(opc!='s');
	system("cls");
}
int E_Derecho_Minimo (struct inventario *nodo)//Busca el codigo de producto mas pequeño partiendo del hijo derecho del elemento que se quiere eliminar.
{
	struct inventario *Temporal = nodo; 
	//el valor mínimo debe estar presente en el nodo izquierdo
	while(Temporal->h_izq != NULL){
		Temporal = Temporal->h_izq;
	}
	aux=Temporal;
	return Temporal->cod_producto;
}
struct inventario *Eliminar_Elemento(struct inventario *nodo, int cp)
{
	if(nodo == NULL)
		return NULL;
	if(nodo->cod_producto < cp){
		nodo->h_der = Eliminar_Elemento(nodo->h_der,cp);
	}else{
		if(nodo->cod_producto > cp){
			nodo->h_izq = Eliminar_Elemento(nodo->h_izq,cp);
		}else{
			if((nodo->h_izq == NULL) && (nodo->h_der == NULL)){
				free(nodo);
				return NULL;
			}else{
				if((nodo->h_izq == NULL) || (nodo->h_der == NULL)){
					struct inventario *TEMP = NULL;
					if(nodo->h_izq == NULL){
						TEMP = nodo->h_der;
						free(nodo);
						return TEMP;
					}else{
						TEMP = nodo->h_izq;
						free(nodo);
						return TEMP;
					}
				}else{
					int Min = E_Derecho_Minimo (nodo->h_der);
					nodo->cod_producto = Min;
					strcpy(nodo->nom_producto,aux->nom_producto);
					nodo->cantidad=aux->cantidad;
					nodo->h_der = Eliminar_Elemento(nodo->h_der,Min);
				}
			}
		}
	}
	//devolver la dirección de la raíz real
	return nodo;
}

void Guarda_In_Orden(struct inventario *actual)// Imprimirá el árbol en formato tabla en el archivo.
{
	if(actual == NULL){
	return;}
	else{
	Guarda_In_Orden(actual->h_izq);
	fprintf(f,"%d-",actual->cantidad);
	fputs(actual->nom_producto,f);
	fprintf(f,"-");
	fprintf(f,"\n");
	Guarda_In_Orden(actual->h_der);}
}
void Guardar_Inventario()//  Funcion para generar o sobreescribir un archivo de texto que almacena los datos del inventario, funcion que se ejecuta sola al finalizar el programa.
{
	f = fopen("inventario.txt","w");
	
	if(f==NULL)
	{
		puts("Error, no se pudo abrir o crear el archivo. ");
		exit(1);
	}
	
	struct inventario *actual = arbol;
	Guarda_In_Orden(actual);
	
	fclose(f);
	
}
struct inventario *ReCrearElemento (int cp)// Devuelve un puntero con la direccion de memoria del nuevo elemento con el codigo de producto asignado.
{
	//reserva memoria para el nuevo elemento Nu_El.
	struct inventario *Nu_El=(struct inventario *) malloc(sizeof(struct inventario));
	//verifica que se haya podido reservar la memoria para elelemento.
	if(Nu_El!=NULL)
	{
		pguia=auxiliar;
		// asigna el nombre el Nuevo Elemento.
		strcpy(Nu_El->nom_producto,cadena);
		// Y lo mismo con el vector guia.
		strcpy((pguia+c_elementos)->n_p,cadena);
		c_elementos++;
		Nu_El->cod_producto=cp;									
		// asigna la cantidad al Nuevo Elemento.
		Nu_El->cantidad=CANT;
		// e inicializa los punteros.
		Nu_El->h_izq=NULL;
		Nu_El->h_der=NULL;
	
		return Nu_El;
	}
	else
	{
		puts(" ERROR: No se pudo reservar memoria.\n Contacte a servicio Tecnico.");
		return NULL;
	}
}
struct inventario *ReInsertarElemento(struct inventario *nodo, int cp)//Busca la posicion de un elemto en el árbol binario segun el codigo que le corresponde, manteniendo la direccion de memoria del elemnto " Padre " para no perder la trazabilidad.
{
	//si arbol==NULL, el inventario esta vacio y se debe crear el primer elemento.
	if (nodo == NULL){
		return ReCrearElemento(cp);
	}
	//si ya existe el primer elemento, decide si continuar por la rama derecha o izquierda.
	if (nodo->cod_producto < cp){
		nodo->h_der = ReInsertarElemento(nodo->h_der,cp);
	}
	if(nodo->cod_producto > cp){
		nodo->h_izq = ReInsertarElemento(nodo->h_izq,cp);
	}
	return nodo;
}
void Leer_Inventario()//  Funcion para leer un archivo de  texto que almacena los datos del inventario, funcion que se ejecuta al inicio del programa.
{
	
	f = fopen("inventario.txt","r");
	int i;
	
	for (i=0;i<T;i++){
		(pguia+i)->c_p = v[i];
		// asignacion de  los codigos de cada producto.
		strcpy((pguia+i)->n_p,"\0");
	}
	
	if(f==NULL){
		puts("Error, no se pudo leer el inventario de prodcutos (no esta creado o posee un error en el ordenador)\n");
	}
	else
	{
		char temp[60], cad_aux ='\0';
		c_elementos = 0;
		int cont=-1, j;

		if (arbol!=NULL){
			do {
				arbol = Eliminar_Elemento (arbol, arbol->cod_producto);
				c_elementos--;
			} while(arbol!=NULL);
		}
		

		/* Obtengo el numero de productos cargados contando la cantidad de renglones,
		dado que la informacion de cada producto se encuentra en cada renglon y va contando cuantos renglones tiene el archivo. */
		while(!feof(f))             
		{
			fgets(temp,60,f);
			cont++;
		}
		printf("\nProductos cargados: [%d]",cont);
		rewind(f);   //Pone el cursor al comienzo del archivo de texto. 
		
		pguia = auxiliar;
		//Bucle que llama a las funciones para crear y asignar memoria para armar el arbol y poder utilizarse de manera normal.
		
		for(i=0;i<cont;i++)     
		{
			
			vaciar(temp);
		
			for(j=0;cad_aux!='-';j++)
			{
				cad_aux = fgetc(f);
				
				if(cad_aux!='-')
				{
					temp[j]=cad_aux;
				}
			}
			CANT = atoi(temp);
			
			cad_aux='\0';
			// leo el nombre de cada producto.
			
			vaciar(temp);
			
			
			
			for(j=0;cad_aux!='-';j++)
			{
				cad_aux = fgetc(f);
				if(cad_aux!='-')
				{
					temp[j]=cad_aux;
				}
				
			}
			cad_aux='\0';
			
			strcpy(cadena,temp);
	        fgetc(f); //LEEMOS EL ULTIMO '-'*/
			//rearmo el arbol ubicando cada elemento.
			arbol = ReInsertarElemento (arbol,(pguia+c_elementos)->c_p);
		}
		
	}
	fclose(f);

}

void menu_inventario ()// LLamada al MENU de la estructura INVENTARIO.
{
	int i = 0, band = 0;
	char opcion, aux_opcion, opcion1;
	
	
	do {
		system("cls");
		printf("\n=================================================================");
		printf("\n			PROGRAMA V1.0								");
		printf("\n=================================================================");
		printf("\n\n\t\t MENU INVENTARIO	\n");
		printf("\n\t[1]. Agregar un Producto al Inventario	");
		printf("\n\t[2]. MENU ver Productos	");
		printf("\n\t[3]. Modificar Productos ");
		printf("\n\t[4]. Eliminar un Producto	");
		printf("\n\t[5]. Eliminar TODOS los Productos	");
		printf("\n\t[s]. Salir al menu principal ");
		printf("\n\n\tIngrese su opcion: [ ]\b\b");
		
			fflush(stdin);
			scanf("%c",&opcion);

		switch (opcion){

		case '1':
		        if(c_elementos<31)    //CAPACIDAD MAXIMA DEL INVENTARIO
				{
				pguia = auxiliar;
				system("cls");
				printf(" \nHa selecionado la opcion 1: Agregar un Producto al Inventario. \n");
				
				
				for (i=0;i<T;i++)
				{
					if(strcmp("\0",(pguia+i)->n_p)==0)
					{
					CANT=i;
					break;
					}
				}
				/* Como se van a cargar cierta cantidad de elemtos, se utilizan los valores
				del vector guia para establecer el orden que permita llegar al albol equilibrado. */
				arbol = InsertarElemento (arbol,(pguia+i)->c_p);
				}else
				{
					printf("\n\tCapacidad del inventario alcanzada, elimine algun producto o vacie la lista");
				}
			break;
		case '2':
			// Evalua si el arbol esta vacio para peermitir la muestra de datos, de lo contrario evita la ejecución del codigo. 
			if (arbol==NULL){
				puts("\n El inventario se encuentra vacio, selecione la opción 1 para cargar información.");
				system("pause");
			}else{
				do {
					
					
					system("cls");
					printf("\n=================================================================");
					printf("\n			PROGRAMA V1.0								");
					printf("\n=================================================================");
					printf("\n\n\t\t MENU INVENTARIO MOSTRAR	\n");
					printf("\n\t[1]. Mostrar todos los Productos cargados	");
					printf("\n\t[2]. Mostrar datos de un Producto segun su Nombre	");
					printf("\n\t[3]. Mostrar los Productos Actualmente Agotados	");
					printf("\n\t[s]. Salir al menu Inventario ");
					printf("\n\n\tIngrese su opcion: [ ]\b\b");
					fflush(stdin);
					scanf("%c",&opcion1);

					switch (opcion1){
					case '2':
							band = 0;
							do{
							system("cls");
							puts("\n Ha selecionado la opcion '2': Mostrar los datos de un Producto segun su Nombre. \n");
							printf(" Ingrese el nombre del Producto que busca dentro del inventario: ");
							fflush(stdin);
							gets(cadena);
							}while(strcmp(cadena,"\0")==0);
							Minimizar(cadena);
							
							
						for(i=0;i<T;i++)
						{
							if(strcmp(cadena,(pguia+i)->n_p)==0)
							{
							band++;
							break;
						    }
							
						}

							pguia = auxiliar;
						
							if (band==1){
								puts("\n Producto encontrado, sus datos son : \n\n");
								printf(" --------------------------------------------------------------------------------------------\n");
								printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
								printf("---------------------------------------------------------------------------------------------\n");
								Busca_Nombre (arbol, (pguia+i)->c_p);
							}else{
								puts("\n\t Nombre de Producto no encontrado entre los nombres cargados.\n\n\t Reintente con otro Nombre o intente con otra opción. \n\n");
							}
							
							printf("\n");
							system("pause");
						break;
					case '3':
						system("cls");
						puts(" \nHa selecionado la opcion '2': Mostrar los Productos Actualmente Agotados.\n");
						printf("\n\t\t   		 ==> PRODUCTOS AGOTADOS <==\n");
						printf(" --------------------------------------------------------------------------------------------\n");
						printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
						printf("---------------------------------------------------------------------------------------------\n");

						In_Orden_Cantidad (arbol);
						system("pause");
						break;
					case '1':
						system("cls");
						puts(" \nHa selecionado la opcion '3': Mostrar todos los Productos cargados.\n");
						printf("\n\t\t   		 ==> INVENTARIOS <==\n");
						printf(" --------------------------------------------------------------------------------------------\n");
						printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
						printf("---------------------------------------------------------------------------------------------\n");

						In_Orden(arbol);
						system("pause");
						break;
						
					}
				} while((opcion1!='S')&&(opcion1!='s'));
			}
			break;
		case '3':
			band=0;
			system("cls");
			puts("\n Ha selecionado la opcion 3: Modificar los Datos de un Producto dentro del Inventario. \n");
			if(arbol==NULL){
				puts("\n El inventario se encuentra vacio, selecione la opción 1 para cargar información.");
				system("pause");
			}else{
				do{
					printf("\n\t Ingrese el nombre del Producto del cual busca modificar sus Datos: ");
					fflush(stdin);
					gets(cadena);
					Minimizar(cadena);
					for(i=0;i<T;i++)
					{
						if(strcmp(cadena,(pguia+i)->n_p)==0)
						{
							band++;
							break;
						}
						
					}
					if (band==1){
						printf("\n\t Producto encontrado dentro del inventario, sus datos actuales son : \n\n");
						/*se mandan como parametros la direccionde memoria del elemento raiz
						y el codigo de producto que esta vinculado al Nombre segun el vector guia */
						printf(" --------------------------------------------------------------------------------------------\n");
						printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
						printf("---------------------------------------------------------------------------------------------\n");
						Busca_Nombre (arbol, (pguia+i)->c_p);
						
						do{
							printf("\n\n\tDesea modificar algun campo? [S/N]:[ ]\b\b");
							fflush(stdin);
							scanf("%c",&aux_opcion);
							aux_opcion=tolower(aux_opcion);
							
							if(aux_opcion=='s'){
								Modifica (arbol);
							}
						}while(aux_opcion!='s' && aux_opcion!='n');
						
						
					}else{
						printf("\n\t El nombre [%s] de producto no ha sido encontrado entre los nombres previamente cargados. ",cadena);
						printf("\n\n\t\tQue desea realizar?	");
						printf("\n\t[1]. Modificar otro Producto");
						printf("\n\t[S]. Salir al menu");
						fflush(stdin);
						scanf("%c",&aux_opcion);
						aux_opcion=tolower(aux_opcion);
					}
					
					
				} while(aux_opcion!='s' && aux_opcion!='n');
			}
			break;
		case '4':
			band=0;
			system("cls");
			printf("\n Ha selecionado la opcion 4: Eliminar un Producto del Inventario. \n\n  Ingrese el nombre del Producto que busca eliminar: ");
			if(arbol==NULL){
				puts("\n El inventario actualemte se encuentra vacio, selecione la opción 1 para cargar información."); 
				system("pause");
			}else{
	
					fflush(stdin);
					gets(cadena);

					Minimizar(cadena);
					for (i=0;i<T;i++){
						if(strcmp(cadena,(pguia+i)->n_p)==0){
							CANT = (pguia+i)->c_p;
							band=1;
							break;
						}
					}
					pguia = auxiliar;
					if (band==1){
						printf("\n\t Producto encontrado dentro del inventario, sus datos actuales son : \n\n");
						printf(" --------------------------------------------------------------------------------------------\n");
						printf("     Codigo                       Cantidad                  Nombre del Producto\n ");
						printf("---------------------------------------------------------------------------------------------\n");
						/*se mandan como parametros la direccionde memoria del elemento raiz
						y el codigo de producto que esta vinculado al Nombre segun el vector guia */
						Busca_Nombre (arbol, (pguia+i)->c_p);
						
						
						do
						{
							printf("\n\n\tDesea eliminar dicho producto? [S/N]:[ ]\b\b");
							fflush(stdin);
							scanf("%c",&aux_opcion);
							aux_opcion=tolower(aux_opcion);
						}while(aux_opcion!='s' && aux_opcion!='n');
				
						if(aux_opcion=='s')
						{
							arbol = Eliminar_Elemento (arbol, aux->cod_producto);
							c_elementos--;
							
						
							system("pause");
							strcpy((pguia+i)->n_p,"\0");
							
						
						}
						
					}else{
						printf("\n\t Nombre de Producto no encontrado.\n\n ");
						system("pause");
					}
				
			}
			break;
		case '5':
			system("cls");
			puts("\n Ha selecionado la opcion 5: Eliminar TODOS los Producto del Inventario.");
			if (arbol==NULL){
				puts("\n \t El inventario actualmente esta vacio, seleccione la opcion 1 primero.");
				system("pause");
			}else
			{
				printf("\nEsta seguro de eliminar TODOS los clientes de forma permanente? [S/N]: [ ]\b\b");
				do{
					scanf("%c",&aux_opcion);
					aux_opcion=toupper(aux_opcion);
				}while(aux_opcion!='S' && aux_opcion!='N');
				
				if(aux_opcion=='S'){
				do{
					arbol = Eliminar_Elemento (arbol, arbol->cod_producto);
					c_elementos--;
				} while(arbol!=NULL);
				for (i=0;i<T;i++){
					strcpy((pguia+i)->n_p,"\0");
				}
				}
			}
			break;

		}
		
	} while((opcion!='S')&&(opcion!='s'));
}
int main(int argc, char *argv[])              //Funcion principal
{
	
	char op;
	printf("\ncargando clientes....\n");
	lecturadeagenda();
	
	printf("\ncargando turnos....\n");
	lecturaturnos();
	
	// llamada a la funcion que abre el archivo de texto asociado al inventario de productos.
	printf("\n\nCargando inventario....\n");
	Leer_Inventario ();
	
	printf("\n\n");
	system("pause");
	do{
	system("cls");
	
	
	printf("\n=================================================================");
	printf("\n			PROGRAMA V1.0								");
	printf("\n=================================================================");
	printf("\n\n\t\tMENU PRINCIPAL\n");
	printf("\n\t\t[1]. Menu de turnos	\n");
	printf("\t\t[2]. Menu de clientes	\n");
	printf("\t\t[3]. Menu Inventario	\n");
	printf("\t\t[S]. Salir y guardar	\n");
	printf("\n\t\tIngrese su opcion: [ ]\b\b");
	fflush(stdin);
	scanf("%c",&op);
	
	
	switch(op)
	{
	case '3':
	menu_inventario ();
	break;
	
	case '2':
	menuclientes();
	break;
	
	case '1':
	menuturnos();
	break;
	
	case 's':
	case 'S':
	do{
	system("cls");
	printf("Esta seguro de salir del programa? [S/N]: ");
	scanf("%c",&op);
	op=toupper(op);

	}while(op!='S' && op!='N');
	
	if(op=='S'){
	guardaragenda();
	guardarturnos();
	Guardar_Inventario();} 

	break;
	
	
	}
   
	}while(op!='S'&& op!='s');
	
	// llamada a la funcion que guarda el inventario en el archivo de texto.
	
	
	// Libera la memoria reservada por las dudas.
	
	/* se asegura de liberar la memoria . */
	
	if (arbol!=NULL){
		do {
			arbol = Eliminar_Elemento(arbol, arbol->cod_producto);
			c_elementos--;
		} while(arbol!=NULL);
	}
	
	eliminarlista();
	
	eliminarpila();
	
	
	
	
}

/*
.												REFERENCIA:
 main { Linea 2806 }

 int local_time  { Linea   86 }
*/
/*
.															CALENDARIO:
 struct turno 								{ Linea   16 }

 struct dia 								{ Linea   22 }

 struct pila_turnos 						{ Linea   26 }

 void subeliminarturno 						{ Linea  140 }

 void eliminarturno 						{ Linea  151 }

 void buscarturnoporfecha					{ Linea  298 } 

 void mostrarturnos 						{ Linea  320 }

 void sub_verTurnos							{ Linea  416 } 

 void mostrarturnoscliente 					{ Linea  448 }

 void verTurnos 							{ Linea  549 }

 int crearTurno								{ Linea  903 } 

 void menuturnos 							{ Linea 1258 }

 void guardarturnos							{ Linea 1635 } 

 void lecturaturnos							{ Linea 1754 } 
*/
/*
.															CLIENTES:
 struct cliente 							{ Linea   43 }

 void mostarcliente 						{ Linea   95 }

 sctruct cliente *busqueda 					{ Linea  104 } 

 int insertar								{ Linea  624 } 

 void mostrarlista							{ Linea 1320 } 

 void modificardatos						{ Linea 1353 }  // No hace falta rezervar memoria, zolo crear un puntero auxiliar.

 void eliminarcliente						{ Linea 1534 } 

 void guardaragenda							{ Linea 1607 } 

 void menuclientes							{ Linea 1663 } 

 void vaciar								{ Linea 1745 } 

 void lecturadeagenda						{ Linea 1933 } 
*/
/*
.															INVENTARIO:

 struct guia 								{ Linea   59 }

 struct inventario 							{ Linea   70 }

 void Minimizar 							{ Linea 2074 }

 struct inventario *CrearElemento 			{ Linea 2081 } 

 struct inventario *InsertarElemento 		{ Linea 2152 }

 void In_Orden 								{ Linea 2167 }

 void Busca_Nombre 							{ Linea 2175 }

 void In_Orden_Cantidad 					{ Linea 2198 }

 void Modifica 								{ Linea 2208 }

 int E_Derecho_Minimo 						{ Linea 2331 }

 struct inventario *Eliminar_Elemento 		{ Linea 2340 }

 void Guarda_In_Orden 						{ Linea 2386 }

 void Guardar_Inventario					{ Linea 2397 }

 struct inventario *ReCrearElemento 		{ Linea 2412 }  

 struct inventario *ReInsertarElemento 		{ Linea 2442 }

 void InOrden								{ Linea 2457 } 

 void Leer_Inventario 						{ Linea 2465 }

 void menu_inventario 						{ Linea 2542 }

*/
