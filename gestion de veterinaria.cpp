#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
struct fecha
{
	int dd,mm,aaaa;	
};
struct Dia
{
	int dias;
};

struct Auxiliar
{
	int dni;
	char nombre[60];	
};
struct Contador
{
	char NombreUsuario[60];
	int cont;
};
struct Usuarios
{
	char Usuario[10];
	char Contrasena[10];
	char ApellidoyNombre[60];
};
struct Mascota
{
	char ApellidoyNombre[60];
	char Domicilio[60];	
	int DNI_Dueno;
	char Localidad[60];
	fecha FechaNacimiento;
	float Peso;
	char Telefono[25];
};
struct Turnos
{
	int Matricula;
	fecha Fecha;
	int DNI_Dueno;
	char DetalleAtencion[380];
};
struct Veterinario
{
	char ApellidoyNombre[60];
	Dia DiaAtencion[6];
	int Matricula;
	int DNI;
	char Telefono[25];
	char Contrasena[10];
};
//------------Menu-de-consultorio--------
int MenuConsultorio()
{
	int OpcionCon=0;
	
	system("cls");
	printf("\n Modulo Consultorio Veterinario \n");
	printf("================================\n");
	printf("\n1-Iniciar Sesion.");
	printf("\n2-Visulizar Lista de Espera de Turnos.");
	printf("\n3-Registrar Evolucion de las Mascotas.");
	printf("\n4-Cerrar Aplicacion.");
	printf("\n\nIngrese una opcion: ");
	scanf("%d",&OpcionCon);
	return OpcionCon;
}
//----------inicio de sesion-----------
int IncioDeSesionVe(FILE *ArchVete,int mat)
{
	Veterinario vet;
	char contraV[10];
	ArchVete=fopen("Veterinario.dat","r+b");

	fread(&vet,sizeof(Veterinario),1,ArchVete);
	while(!feof(ArchVete))
	{
		if(vet.Matricula==mat)
		{
			printf("\nUsuario Correcto.\n");
			printf("\n Contrasena: ");
			_flushall();
			gets(vet.Contrasena);
			if(strcmp(vet.Contrasena,contraV)==1)
			{
				printf("\nContrasena Correcta.");				
				return 1;
			}
			else
			{
				printf("\nUsuario Correcto.\n");
				printf("\nContrasena Incorrecta.");

			}
		}
		
		fread(&vet,sizeof(Veterinario),1,ArchVete);
	}
	
	return 0;
	fclose(ArchVete);	
}

//----------Lista-de-turnos---------------

int FechaPaciente(FILE *ArchTur,Turnos tur,int &mat)
{
	time_t ahora;
    struct tm *hora;
    struct tm *fecha;
    int b=0;
	
	time(&ahora);
    fecha = localtime(&ahora);
	
	ArchTur=fopen("Turnos.dat","r+b");
	
	fread(&tur,sizeof(Turnos),1,ArchTur);
	
	while(!feof(ArchTur))
	{
		if((tur.Fecha.dd==fecha->tm_mday) && (tur.Fecha.mm==fecha->tm_mon+1) && (tur.Fecha.aaaa==fecha->tm_year+1900))
		{
			if(tur.Matricula==mat)
			{
				return 1;
				break;
			}
			
		}
		
		fread(&tur,sizeof(Turnos),1,ArchTur);
	}
	return 0;
	
	fclose(ArchTur);
}
void Atendidos(FILE *ArchTur,FILE *ArchAux,int &mat,int &documento)
{
	Turnos tur;
	int b=0;
	ArchAux=fopen("Auxiliar.dat","a+b");
	time_t ahora;
    struct tm *hora;
    struct tm *fecha;
	time(&ahora);
    fecha = localtime(&ahora);
	ArchTur=fopen("Turnos.dat","r+b");
	fread(&tur,sizeof(tur),1,ArchTur);
	b=0;
	while(!feof(ArchTur))
	{	
		if((tur.Fecha.dd!=fecha->tm_mday) && (tur.Fecha.mm!=fecha->tm_mon+1) && (tur.Fecha.aaaa!=fecha->tm_year+1900))
		{
			if(mat!=tur.Matricula && documento!=tur.DNI_Dueno)	
				fwrite(&tur,sizeof(tur),1,ArchAux);				
			else
				b=1;
				fread(&tur,sizeof(tur),1,ArchTur);
		}
		fread(&tur,sizeof(tur),1,ArchTur);	
	}
	if(b==1)
	{
		printf("Paciente atendido");
	}
	
	fclose(ArchTur);
	fclose(ArchAux);
	remove("Turnos.dat");
	rename("Auxiliar.dat","Turnos.dat");
} 
 
void DNI_Dueno(FILE *ArchTur,int mat,int vec[],FILE *ArchVete)
{
	time_t ahora;
    struct tm *hora;
    struct tm *fecha;
    
	time(&ahora);
    fecha = localtime(&ahora);
	
	Turnos tur;
	Auxiliar aux[12];
	
	int q,i=0,n=0;
	
	ArchTur=fopen("Turnos.dat","r+b");
	fread(&tur,sizeof(tur),1,ArchTur);
	
	while(!feof(ArchTur))
	{	
		if((tur.Fecha.dd==fecha->tm_mday) && (tur.Fecha.mm==fecha->tm_mon+1) && (tur.Fecha.aaaa==fecha->tm_year+1900))
		{
			if(mat==tur.Matricula)
			{		
				vec[i]=tur.DNI_Dueno;
				i++;
			}
		}
		n=i;
		fread(&tur,sizeof(tur),1,ArchTur);	
	}
	fclose(ArchTur);
		
	printf("\n--------------------------------------");
	FILE *archMasc;
	Mascota masc;
	archMasc=fopen("Mascotas.dat","r+b");
	fread(&masc,sizeof(Mascota),1,archMasc);
	while(!feof(archMasc))
	{
		for(i=0;i<n;i++)
		{
			if(masc.DNI_Dueno==vec[i])
			{
				strcpy(aux[i].nombre,masc.ApellidoyNombre);
				aux[i].dni=vec[i];
						
		}
		
	}
		fread(&masc,sizeof(Mascota),1,archMasc);		
	}
	
	fclose(archMasc);
		
		for(i=0;i<n;i++)
		{
			printf("\n%d",i+1);
			printf("\n\tDNI: %d",aux[i].dni);
			
					
		}
		
}

//-----------Fecha-a-ingresar------------

void Fecha()
{
	time_t ahora;
    struct tm *hora;
    struct tm *fecha;
    int b=0;
    
    time(&ahora);
    
    fecha = localtime(&ahora);
	printf("\nFecha de hoy: %d-%d-%d\n", fecha->tm_mday, fecha->tm_mon+1, fecha->tm_year+1900);
	 
}
//----------------Menu-------------------

int ModuloAsistente()
{
	int OpAsis=0;
	
	printf("\nModulo Asistente.");
	printf("\n===========================");
	printf("\n1-Iniciar Secion.");
	printf("\n2-Registrar Mascota.");
	printf("\n3-Registrar Turno.");
	printf("\n4-Listado de Atenciones por Veterinario y fecha.");
	printf("\n\n5-Cerrar la aplicacion.");
	printf("\n\nIngrese la opcion deseada: ");
	scanf("%d",&OpAsis);
	
	return OpAsis;
}
//-----------Inicio-de-Sesion-------------
int InicioSesion(FILE *ArchRec,Usuarios rec,char Us[10])
{
	char Usuario[10],Contrasena[10];
	
	ArchRec=fopen("Usuarios.dat","r+b");
	if(ArchRec==NULL)
	{
		printf("Error, no existe el archivo.");
		exit(1);
	}	
	
	printf("\nUsuario: ");
	_flushall();
	gets(rec.Usuario);
	fread(&rec,sizeof(rec),1,ArchRec);
		
	while(!feof(ArchRec))
	{
		if(strcmp(rec.Usuario,Usuario)==1)
		{
			printf("\nUsuario correcto.\n");	
			printf("\nContrasena: ");
			_flushall();
			gets(rec.Contrasena);
			
			if(strcmp(rec.Contrasena,Contrasena)==1)
			{
				return 1;
			}
			else
			{
				printf("\nUsuario Correcto");
				printf("\nContraseña Incorrecta");
			}
		}
	
		fread(&rec,sizeof(rec),1,ArchRec);
	}
	return 0;
	fclose(ArchRec);	
}
//-----------Registrar-Turno--------------

void ListarVeterinario(FILE *ArchVete, Veterinario vet)
{
	ArchVete=fopen("Veterinarios.dat","r+b");
	
	fread(&vet,sizeof(Veterinario),1,ArchVete);
	printf("\n");
	printf("\n--------------------------------------------------------");
	while(!feof(ArchVete))
	{
		printf("\nApellido y Nombre :  %s (%s)",vet.ApellidoyNombre);
		printf("\nDias de Atencion: ");              
		for(int j=0;j<sizeof(vet.DiaAtencion[j]);j++)
		{
			
			if(vet.DiaAtencion[j].dias==1)
			{
				printf(" /1-Lunes/");
			}
			if(vet.DiaAtencion[j].dias==2)
			{
				printf(" /2-Martes/");
			}
			if(vet.DiaAtencion[j].dias==3)
			{
				printf(" /3-Miercoles/");
			}
			if(vet.DiaAtencion[j].dias==4)
			{
				printf(" /4-Jueves/");
			}
				if(vet.DiaAtencion[j].dias==5)
			{
				printf(" /5-Viernes/");
			}
				if(vet.DiaAtencion[j].dias==6)
			{
				printf(" /6-Sabado/ ");
			}
		}	
		printf("\n\n--------------------------------------------------------");
		fread(&vet,sizeof(Veterinario),1,ArchVete);	
	}

	fclose(ArchVete);
	
}
int SeleccionVeterinario(FILE *ArchVete,Veterinario vet,char Nombre[])
{
	
	int dia;
	int turno=0;
	ArchVete=fopen("Veterinarios.dat","r+b");
	
	if(ArchVete==NULL)
	{
		printf("\nError, el archivo no existe.");
		exit(1);
	}
	
	fread(&vet,sizeof(Veterinario),1,ArchVete);
	
	
	while(!feof(ArchVete))
	{
	
		if(strcmp(Nombre,vet.ApellidoyNombre)==0)
		{
			printf("\nVeterinario correcto.\n");
			printf("\nSeleccione el dia: ");
			scanf("%d",&dia);
			
			for(int i=0;i<sizeof(vet.DiaAtencion);i++)
			{
				if(vet.DiaAtencion[i].dias==dia)
				{
					printf("\nSeleccion de dia correcta.");
					return 1;
						
				}
				
			}
		
		}
		
		fread(&vet,sizeof(Veterinario),1,ArchVete);
	}

	fclose(ArchVete);
		
}
int GuardarCampoMatricula(FILE *ArchVete,Veterinario vet,char Nombre[])
{
	ArchVete=fopen("Veterinarios.dat","r+b");
	int mat=0;
	if(ArchVete==NULL)
	{
		printf("\nError, el archivo no existe.");
		exit(1);
	}	
	fread(&vet,sizeof(Veterinario),1,ArchVete);
	while(!feof(ArchVete))
	{
		if(strcmp(Nombre,vet.ApellidoyNombre)==0)
		{
			mat=vet.Matricula;
		}
		fread(&vet,sizeof(Veterinario),1,ArchVete);
	}
	return mat;
	system("pause");
	system("cls");

	fclose(ArchVete);
}
//---------Contador-de-Registros----------

void ContadorTurnos(FILE *ArchCon,FILE *ArchVete,Contador con,char Usuario[10],int cont)
{
	ArchCon=fopen("MayorRegistros.dat","a+b");
	fread(&con,sizeof(Contador),1,ArchCon);
	while(!feof(ArchCon))
	{
		
		strcpy(con.NombreUsuario,Usuario);
	    con.cont=con.cont+cont;
		fwrite(&con,sizeof(Contador),1,ArchCon);
    }
	fclose(ArchCon);
}
 //----------------Calendario---------------------
 void Calendario()
{

	int i,j,k,m,l,cont;				 
	int dia,mes,anio,semana,moduloMes,febrero,total_mes;
	int seleccion=1;				
	i=j=k=m=l=cont=0; 		
	
	time_t sisTime; 
	struct tm *tiempo;	
	
	time(&sisTime);		
	tiempo=localtime(&sisTime);
	
	dia=tiempo->tm_mday;	
	mes=tiempo->tm_mon;	
	anio=(tiempo->tm_year)+1900;	
	
	system ("cls");
	
	printf("\t=============================>CALENDARIO<=============================\n\n");
	printf("Hoy es: %d/%d/%d \n\n\n",dia,mes+1,anio);
	
		while(seleccion != 0)
		{
		
			if ((anio%4==0) && (anio%100!=0) || anio%400==0)
			febrero=29;
			else
			febrero=28;
			
			switch (mes)
			{
				case 3: case 5: case 8: case 10: 
				total_mes=30; 
				break; 
				case 1: 
				total_mes=febrero; 
				break; 
				case 0: case 2: case 4: case 6: case 7: case 9: case 11: 
				total_mes=31; 
				break; 
			}
			
			switch (mes)
			{
				case 0:printf("\t\tENERO\t%d",anio);break;
				case 1:printf("\t\tFEBRERO\t%d",anio);break;
				case 2:printf("\t\tMARZO\t%d",anio);break;
				case 3:printf("\t\tABRIL\t%d",anio);break;
				case 4:printf("\t\tMAYO\t%d",anio);break;
				case 5:printf("\t\tJUNIO\t%d",anio);break;
				case 6:printf("\t\tJULIO\t%d",anio);break;
				case 7:printf("\t\tAGOSTO\t%d",anio);break;
				case 8:printf("\t\tSEPTIEMBRE\t%d",anio);break;
				case 9:printf("\t\tOCTUBRE\t%d",anio);break;
				case 10:printf("\t\tNOVIEMBRE\t%d",anio);break;
				case 11:printf("\t\tDICIEMBRE\t%d",anio);break;
			}
			
			if (febrero==29) 
			{
				switch (mes)
				{
					case 0:moduloMes=0;break;
					case 1:moduloMes=3;break;
					case 2:moduloMes=4;break;
					case 3:moduloMes=0;break;
					case 4:moduloMes=2;break;
					case 5:moduloMes=5;break;
					case 6:moduloMes=0;break;
					case 7:moduloMes=3;break;
					case 8:moduloMes=6;break;
					case 9:moduloMes=1;break;
					case 10:moduloMes=4;break;
					case 11:moduloMes=6;break;
				}
			
			}else
			{
				switch (mes)
				{
					case 0:moduloMes=0;break;
					case 1:moduloMes=3;break;
					case 2:moduloMes=3;break;
					case 3:moduloMes=6;break;
					case 4:moduloMes=1;break;
					case 5:moduloMes=4;break;
					case 6:moduloMes=6;break;
					case 7:moduloMes=2;break;
					case 8:moduloMes=5;break;
					case 9:moduloMes=0;break;
					case 10:moduloMes=3;break;
					case 11:moduloMes=5;break;
				}
			}
			
			semana=((anio-1)%7+ ((anio-1)/4 -(3*((anio-1)/100+1)/4))%7+moduloMes+1%7)%7;
			
			
			printf("\nDo\tLu\tMar\tMier\tJue\tVier\tSab\n");
				
					for(m=0;m<semana;m++)//dar espacios para que el primer dia calce.
					printf("\t");
			
					for(i=1;i<=total_mes;i++)//imprimir dias correspondientes al mes.
					{
						if(i==8-semana||i==15-semana||i==22-semana||i==29-semana || i== 36-semana)
							printf("\n%d\t",i);
						else	
							printf("%d\t",i);
					}
			
			printf("\n\nDesea Buscar otra Fecha? \n\t1=SI 0=NO\t>>> ");
			scanf("%d",&seleccion);
			
			while (seleccion<0 || seleccion>1)
			{
				printf("ERROR, Ingrese 1/0\n");
				printf("Desea Buscar otra Fecha? \n\t1=SI 0=NO\t>>> ");
				scanf("%d",&seleccion);
			}
			
			if (seleccion == 1)
			{
				printf("\nIngrese Mes: ");
				scanf("%d",&mes);
				
				mes=1;
				
				printf ("Ingrese Ano: ");
				scanf("%d",&anio);
				
				system("cls");
				printf("\t=============================>CALENDARIO<=============================\n\n");
				
			}
		}

}

int SeleccionDni(FILE *ArchMasc,Mascota masc,int DNI)
{
	ArchMasc=fopen("Mascotas.dat","r+b");
	fread(&masc,sizeof(masc),1,ArchMasc);
	while(!feof(ArchMasc))
	{
		if(DNI==masc.DNI_Dueno)
		{
			
			return 1;
		}
		fread(&masc,sizeof(masc),1,ArchMasc);
	}
	return 0;
	fclose(ArchMasc);
}
void parar()
{
	printf("\n\n");
	system("pause");
	system("cls");
}
//----------------Menu-------------------

int ModuloAdministracion()
{
	int OpAd=0;
	
	printf("\nModulo Administracion.");
	printf("\n===========================");
	printf("\n1-Registrar Veterinario.");
	printf("\n2-Registrar Usuario Asistente.");
	printf("\n3-Ranking de Veterinarios.");
	printf("\n\n4-Cerrar la aplicacion.");
	printf("\n\nIngrese la opcion deseada: ");
	scanf("%d",&OpAd);
	
	return OpAd;
}

//----------------Veterinario-----------------

int Condicion1(FILE *achivo,Veterinario vet)
{
	char *Vma="ABCDEFGHIJKLMNPOQRSTUVWXYZ";
	char *Vmi="abcdefghijklmnopqrstuvwxyz";	
	char *Vnu="123456789";
	
		
	if(strpbrk(Vma,vet.Contrasena))
	{
		if(strpbrk(Vmi,vet.Contrasena))
			{
				if(strpbrk(Vnu,vet.Contrasena))
					{
						return 1;
					}
			}
	}
	else
	{
		return 0;
	}
	
}
int Condicion2(FILE *achivo,Veterinario vet)
{
	char *Vsim="¿?!¡+=%&()/$@-.,:;_'*^áÁéÉíÍóÓúÚ ";	
	
	if(strpbrk(Vsim,vet.Contrasena))
	{
		return 0;		
	}
	else
	{
		return 1;
	}
	
}
int Condicion3(FILE *achivo,Veterinario vet)
{
		
	if(strlen(vet.Contrasena)>5 && strlen(vet.Contrasena)<=32)
	{
		return 1;		
	}
	else
	{
		return 0;
	}
	
}
int Condicion4(FILE *achivo,Veterinario vet)
{
	int cantN=0;
	char c;
	
	for(int i=0;i<strlen(vet.Contrasena);i++)
	{
		c=vet.Contrasena[i];
		
		if((isdigit(c)) && (vet.Contrasena[i]<vet.Contrasena[i-1]))
		{
			cantN++;		
		}
		else
		{
			cantN=0;
		}
	}
	
	if(cantN>=4)
	{
		return 0;
	}
	if(cantN<4)
	{
		return 1;
	}
}
int Condicion5(FILE *achivo,Veterinario vet)
{
	int cantC=0;
	char c,aux[10];
	
	strcpy(aux,vet.Contrasena);
	strupr(aux);
	
	for(int i=0;i<strlen(aux);i++)
	{
		c=aux[i];
		if(c==aux[i-1])
		{
			cantC++;		
		}
		else
		{
			cantC=0;
		}
	}
	
	if(cantC!=2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//---------------Asistente-----------------

int CondicionR1(FILE *achivo, Usuarios rec)
{
		
	if(strlen(rec.Usuario)>5 && strlen(rec.Usuario)<=11)
	{
		return 1;		
	}
	else
	{
		return 0;
	}
	
}
int CondicionR2(FILE *ArchRec,Usuarios rec)
{
	char Usuario[10];
	
	strcpy(Usuario,rec.Usuario);

	fread(&rec,sizeof(rec),1,ArchRec);
	
	while(!feof(ArchRec))
	{
		if(strcmp(Usuario,rec.Usuario)==0)
		{
			return 0;	
		}
		else
		{	
			fread(&rec,sizeof(rec),1,ArchRec);	
		}
		
	}
	
	return 1;
}
int CondicionR3(FILE *achivo, Usuarios rec)
{

	for(int i=0;i<strlen(rec.Usuario);i++)
	{
		if(islower(rec.Usuario[0]))
		{
			return 1;
			break;
		}	
		else
		{
			return 0;
		}
	}
	
}
int CondicionR4(FILE *achivo, Usuarios rec)
{
	int CaracterMayus=0;
	
	for(int i=0;i<strlen(rec.Usuario);i++)
	{
		if(isupper(rec.Usuario[i]))
		{
			CaracterMayus++;
		}
	}
	
	if(CaracterMayus>=2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}
int CondicionR5(FILE *achivo, Usuarios rec)
{
	int Digitos=0;
	
	for(int i=0;i<strlen(rec.Usuario);i++)
	{
		if(isdigit(rec.Usuario[i]))
		{
			Digitos++;
		}
	}
	
	if(Digitos<4)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

//-----------------------------------------------

int CondicionCR1(FILE *achivo, Usuarios rec)
{
	char *Vma="ABCDEFGHIJKLMNPOQRSTUVWXYZ";
	char *Vmi="abcdefghijklmnopqrstuvwxyz";	
	char *Vnu="123456789";
	
		
	if(strpbrk(Vma,rec.Contrasena))
	{
		if(strpbrk(Vmi,rec.Contrasena))
			{
				if(strpbrk(Vnu,rec.Contrasena))
					{
						return 1;
					}
			}
	}
	else
	{
		return 0;
	}
	
}
int CondicionCR2(FILE *achivo, Usuarios rec)
{
	char *Vsim="¿?!¡+=%&()/$@-.,:;_'*^áÁéÉíÍóÓúÚ ";	
	
	if(strpbrk(Vsim,rec.Contrasena))
	{
		return 0;		
	}
	else
	{
		return 1;
	}
	
}
int CondicionCR3(FILE *achivo, Usuarios rec)
{
		
	if(strlen(rec.Contrasena)>5 && strlen(rec.Contrasena)<=32)
	{
		return 1;		
	}
	else
	{
		return 0;
	}
	
}
int CondicionCR4(FILE *achivo, Usuarios rec)
{
	int cantN=0;
	char c;
	
	for(int i=0;i<strlen(rec.Contrasena);i++)
	{
		c=rec.Contrasena[i];
		
		if((isdigit(c)) && (rec.Contrasena[i]<rec.Contrasena[i-1]))
		{
			cantN++;		
		}
		else
		{
			cantN=0;
		}
	}
	
	if(cantN>=4)
	{
		return 0;
	}
	if(cantN<4)
	{
		return 1;
	}
}
int CondicionCR5(FILE *achivo, Usuarios rec)
{
	int cantC=0;
	char c,aux[10];
	
	strcpy(aux,rec.Contrasena);
	strupr(aux);
	
	for(int i=0;i<strlen(aux);i++)
	{
		c=aux[i];
		if(c==aux[i-1])
		{
			cantC++;		
		}
		else
		{
			cantC=0;
		}
	}
	
	if(cantC!=2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void MayorRegistros(FILE *ArchCon,FILE *ArchRec,FILE *ArchVete,Contador con,Usuarios rec,Veterinario vet,char Usuario[10])
{
		int may,Total=0;
		
		ArchCon=fopen("MayorRegistros.dat","r+b");
		
		if(ArchCon==NULL)
		{
			printf("\nError. No existe.");
		}
		fread(&con,sizeof(Contador),1,ArchCon);
		while(!feof(ArchCon))
		{
			if(con.cont==0)
			{
				con.cont++;
				strcpy(Usuario,con.NombreUsuario);
			}
			fread(&con,sizeof(Contador),1,ArchCon);
		}
		fclose(ArchCon);
		printf("\n Total de turnos dados: %d.",con.cont);
}
main()
{
	int OpcionCon=0,OpcionAsis=0,OpcionAdm=0,inicio=0,iniciado=0,dd=0,mm=0,aaaa=0,Seleccion=0,documento=0;
	int mat=0,i,b,res=0,bu=0,bandera,Regla1=0,Regla2=0,Regla3=0,Regla4=0,Regla5=0,n=0;;
	int vec[12];
	char desconocido[100];
	int contador,OpcionRecep,Validacion,Correcto,Obra,matricula,espl,Sm,dia,T,Valor;
	char Name[10],Nombre[30];
	
	FILE *ArchRec,*ArchMasc,*ArchTur,*ArchVete,*ArchAux,*ArchCon;;
	Veterinario vet;Usuarios rec;Mascota masc;Turnos tur;Contador con;
	int opc;
	system ("color 0e");
	printf ("%cQui%cn est%c ingresando hoy%c: ",168,130,160,63);
	gets(desconocido);
	printf ("\n\t\t\t\t\t***BIENVENIDO***%s",desconocido);
	do
	{   
	    Sleep(2000);
	    printf ("\n\nC");
	    Sleep(700);
	    system ("color 0b");
	    printf ("	A");
	    Sleep(700);
	    system ("color 0c");
	    printf ("	R");
	    Sleep(700);
	    system ("color 0d");
	    printf ("	G");
	    Sleep(700);
	    system ("color 0e");
	    printf ("	A");
	    Sleep(700);
	    system ("color 0f");
	    printf ("	N");
	    Sleep(700);
	    system ("color 0a");
	    printf ("	D");
	    Sleep(700);
	    system ("color 0e");
	    printf ("	O\n");
	    Sleep(700);
	    system("cls");
	    printf ("***  ***  ***  *** ***  ***  ***  ***  ***  ***  ***  ***  ***  ");
	    printf ("\n\t %c%c%c%c%c VETERINARIA PELUDO AMOR  %c%c%c%c%c%c",174,174,174,174,174,175,175,175,175,175);
	    printf ("\n***  ***  ***  *** ***  ***  ***  ***  ***  ***  ***  ***  ***  \n");
		printf(".1-MODULO CONSULTORIO\n.");
		printf("2-MODULO DEL ASISTENTE\n.");
		printf("3-MODULO ADMINISTRACION\n.");
		printf("4-Salir\n");
		printf("================================\n");
		printf("\nIngrese una opcion:");
		scanf("%d",&opc);
	    printf ("\n\t\t\t\t\t...ingresando....\t\t\t\t\t\n");
	    Sleep(2000);
	    strupr(desconocido);
		switch(opc){
			case 1:
				do{
					system("cls");
					system ("color 3f");
		            OpcionCon=MenuConsultorio();
     	            system("cls");
     	        switch(OpcionCon){
			    case 1:{
					Fecha();
					printf("\n Matricula: ");
					scanf("%d",&mat);
					inicio=IncioDeSesionVe(ArchVete,mat);
					printf("\n");
				    if(inicio==1){
						printf("\nSeccion iniciada correctamente. ");
						iniciado=1;
					}
					if(inicio==0){
						printf("\nError, seccion no iniciada. Reintentar nuevamente.");
						iniciado=0;
					}
					break;
				}
			    case 2:{
					if(iniciado==0){
						printf("Inicie sesion antes para ingresar en esta opcion.");
					}
					if(iniciado==1){
						Fecha();
						
						if(Seleccion=FechaPaciente(ArchTur,tur,mat)==0){
							printf("\nLista de Espera de Turnos");
							DNI_Dueno(ArchTur,mat,vec,ArchMasc);
							printf("\n\nIngrese el DNI del paciente atendido: ");
							scanf("%d",&masc.DNI_Dueno);
							Atendidos(ArchTur,ArchAux,mat,documento);
							printf("\nExiste un paciente registrado.");
						}
						if(Seleccion=FechaPaciente(ArchTur,tur,mat)==1){
							printf("\nEn la fecha de hoy usted no tiene pacientes registrados.");
						}
					}
					break;
				}
			    case 3:{
					printf("\n\n");
					system("pause");
					system("cls");
					if(iniciado==0){
						printf("Inicie sesion antes para ingresar en esta opcion.");
					}
					if(iniciado==1){
						printf("\nRegistro de Evolucion de Pacientes");	
					}					
					break;
				}
                case 4:{
					printf("\nFin del Programa");
					break;
				}
		}
		printf("\n\n");	
		system("pause");
	}while(OpcionCon!=4);	
	case 2:
	       do{
	       	    system("cls");
	       	    system ("color 3f");
	       	    OpcionAsis=ModuloAsistente();
	       	    system("cls");
	       	    switch(OpcionAsis){
	       	    	case 1:{
				
					parar();
					printf("\n+++++++++++++++++++++++");
					printf("\n  Inicio de sesion.");
					printf("\n+++++++++++++++++++++++\n");
										
					if(Validacion=InicioSesion(ArchRec,rec,Name)==1)
					{
						printf("\nSesion iniciada correctamente.");
						Correcto=1;
					}
					else
					{
						printf("\nError, sesion no iniciada. Reintentelo.");
						Correcto=0;
					}
					contador=0;
					
					parar();
					
					break;
				
				}
			    case 2:
				{
					
					parar();
					
					printf("\nRegistro de la Mascota.\n");
					ArchMasc=fopen("Mascotas.dat","a+b");
					if(Correcto==0)
					{
						printf("\n\nInicie sesion antes de entrar a esta opcion.");
					}
					
					if(Correcto==1)
					{
						
						ArchMasc=fopen("Mascotas.dat","a+b");
						
						if(ArchMasc==NULL)
						{
							printf("\nError, el archivo no existe.");
							exit(1);
						}
							
						do
						{
							res=0;
							printf("\nIngrese Apellido y Nombre de la Mascota: ");
							_flushall();
							gets(masc.ApellidoyNombre);
							printf("\nIngrese Domicilio de la Mascota: ");
							_flushall();
							gets(masc.Domicilio);
							printf("\nIngrese el numero de DNI del DUENO: ");
							scanf("%d",&masc.DNI_Dueno);
							printf("\nIngrese Localidad: ");
							_flushall();
							gets(masc.Localidad);
							printf("\nIngrese Fecha de nacimiento: ");
							printf("\nDia: ");
							scanf("%d",&masc.FechaNacimiento.dd);
							printf("\nMes: ");
							scanf("%d",&masc.FechaNacimiento.mm);
							printf("\nAno: ");
							scanf("%d",&masc.FechaNacimiento.aaaa);						
							printf("\nIngrese el Peso de la Mascota: ");
							scanf("%f",&masc.Peso);
							printf("\nIngrese el Numero de telefono: ");
							_flushall();
							gets(masc.Telefono);
							parar();
							fwrite(&masc,sizeof(Mascota),1,ArchMasc);
							
							parar();
							printf("\n\nDesea ingresar otro Paciente? <1-Si/No-0>: ");
							scanf("%d",&res);
														
						}while(res!=0);
						
						fclose(ArchMasc);
						parar();
						printf("\nFin de la Carga.");
					}
					
					parar();
					break;
				
				}
			case 3:
				{
					parar();	
					printf("\n\tRegistro del Turno.");
					if(Correcto==0)
					{
						printf("\n\nInicie sesion antes de entrar a esta opcion.");
						printf("\n\n");
						system("pause");
						system("cls");
					}
					if(Correcto==1)
					{
						contador++;
						
						printf("\n%sva dando %d turnos.",Name,contador);
						
						printf("\nTodos los Veterinarios trabajan de 8:00 a 21:00 hs.");
						printf("\nSe dara turno hasta llenar horarios.");
						
						ArchTur=fopen("Turnos.dat","a+b");
						
						printf("\nIngrese matricula:");
						scanf("%d",vet.Matricula);
						
						if(ArchTur==NULL)
						{
							printf("\nError, el archivo no existe.");
							exit(1);
						}
						
						do
						{	
							b=0;
							printf("\n\nSeleccione Veterinario: ");
							_flushall();
							gets(Nombre);
																											
							if(Sm=SeleccionVeterinario(ArchVete,vet,Nombre)==1)
							{
								mat=GuardarCampoMatricula(ArchVete,vet,Nombre);
								tur.Matricula=mat;
								
								do
								{
									b=0;
									printf("\nMarque la fecha seleccionada.");
									Calendario();
									printf("\nSeleccione el dia: ");
									scanf("%d",&tur.Fecha.dd);
									printf("\nSeleccione el mes: ");
									scanf("%d",&tur.Fecha.mm);
									printf("\nSeleccione el ano: ");
									scanf("%d",&tur.Fecha.aaaa);
									b=1;
									
								}while(b!=1);
							_flushall();
					     	printf("\nDetalle de atencion:");
						    gets(tur.DetalleAtencion);	
							
							}
							else
							{
								b=0;
								printf("\nError, seleccion incorrecta.\n");
							}				
						    
						}while(b!=1);
						do
						{
							b=0;
							printf("\n\nDNI del paciente: ");
							scanf("%d",&tur.DNI_Dueno);
							documento=SeleccionDni(ArchMasc,masc,masc.DNI_Dueno);
								
							if(documento==1)
							{
								printf("\nDNI correcto.");
								break;
							}
							if(documento==0)
							{
								printf("\nEl paciente no se encuentra registrado.");
								printf("\nIngrese a la opcion 2 --> Registrar paciente.");
								b=0;
								break;
							}
							
						}while(b!=1);
						fwrite(&tur,sizeof(Turnos),1,ArchTur);
						
					    fclose(ArchTur);
						
					    printf("\n\nFin de la Carga.");
					    parar();
			       }
			    
			       break;
			}
					
			case 4:
				{
					
					res=0;
						
					printf("\n\nListado de medicos.  ");
					ListarVeterinario(ArchVete,vet); //Lista de veterinario con su especialidad y dias de atencion
				}
			case 5:
				{
					printf("\nUsted salio del programa.");
					ContadorTurnos(ArchCon,ArchVete,con,Name,contador);
					parar();
					break;
				}
			    default:
				{
					break;
				}
	    }
    }while(OpcionAsis!=5);
	    case 3:
	    	do{
	    		system("cls");
	    	    system ("color 3f");
	    		OpcionAdm=ModuloAdministracion();
	    		system("cls");
	    		switch(OpcionAdm)
	    		{
	    			case 1:
				{
					parar();
					printf("\nRegistro del Veterinario.\n");
					
					ArchVete=fopen("Veterinarios.dat","a+b");
					
					if(ArchVete==NULL)
					{
						printf("\nError, el archivo no existe.");
						exit(1);
					}
					do{
					    res=0;
						printf("\nIngrese Apellido y Nombre: ");
						_flushall();
						gets(vet.ApellidoyNombre);
						printf("\nDNI: ");
						scanf("%d",&vet.DNI);
						_flushall();
	                 	printf("\nTelefono: ");
	                 	gets(vet.Telefono);
						
						printf("\nDias de atencion disponibles:");
						printf("\n\n1:Lunes\n2:Martes\n3:Miercoles\n4:Jueves\n5:Viernes.");
						
						printf("\n\nIngrese cuantos dias atendera: ");
						scanf("%d",&n);
						
						for(int j=0;j<n;j++)
						{
							printf("\nSeleccione los dias: ");
							scanf("%d",&vet.DiaAtencion[j].dias);
						}
												
						printf("\nIngrese el numero de Matricula: ");
						scanf("%d",&vet.Matricula);
						
						
						do
						{
							b=0;
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\nCondiciones para la Contrasena:");
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\n*Debe tener al menos una letra mayuscula, una letra minuscula y un numero."); //Condicion 1
							printf("\n*No puede contener ningun caracter de puntuacion, ni acentos, ni espacios."); //Condicion 2
							printf("\n*Debera tener entre 6 y 32 caracteres."); //Condicion 3
							printf("\n*No debe tener mas de 3 caracteres numéricos consecutivos."); //Condicion 4
							printf("\n*No debe tener 2 caracteres consecutivos  alfabeticos (ascendentes)."); //Condicion 5
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\n\nIngrese la Contrasena: ");
							_flushall();
							gets(vet.Contrasena);
							
							Regla1=0;
							Regla2=0;
							Regla3=0;
							Regla4=0;
							
							if((Regla3=Condicion3(ArchVete,vet)==1) && (Regla1=Condicion1(ArchVete,vet)==1) && (Regla2=Condicion2(ArchVete,vet)==1) && (Regla4=Condicion4(ArchVete,vet)==1) && (Regla5=Condicion5(ArchVete,vet)==1))
							{
								printf("\nContrasena valida.\n");
								b=1;
							}
							else
							{
								printf("\nContrasena invalida.\n");
								
								if(Regla1=Condicion1(ArchVete,vet)==0)
								{
									printf("\nError, no existe letra mayuscula, una letra minuscula o un numero.");
								}
								if(Regla2=Condicion2(ArchVete,vet)==0)
								{
									printf("\nError, posee algun caracter de puntuacion, acentos o espacios.");
								}
								if(Regla3=Condicion3(ArchVete,vet)==0)
								{
									printf("\nError, no tiene entre 6 y 32 caracteres.");
								}
								if(Regla4=Condicion4(ArchVete,vet)==0)
								{
									printf("\nError, tiene más de 3 caracteres numéricos consecutivos.");
								}
								if(Regla5=Condicion5(ArchVete,vet)==0)
								{
									printf("\nError, tiene 2 caracteres consecutivos alfabeticos (ascendentes).");
								}
								
							
							}
								fwrite(&vet,sizeof(Veterinario),1,ArchVete);			
						}while(b!=1);
					parar();
					printf("\n\nDesea ingresar otro Veterinario? <1-Si/No-0>: ");
					scanf("%d",&res);
				}while(res!=0);
				fwrite(&vet,sizeof(Veterinario),1,ArchVete);							
				fclose(ArchVete);
				parar();
				
				printf("\nFin de la Carga.");
				
					
				break;
				}
			case 2:
				{
					printf("\nRegistro del Asistente.");
					
					ArchRec=fopen("Usuarios.dat","a+b");
					
					if(ArchRec==NULL)
					{
						printf("\nError, el archivo no existe.");
					}
					
					res=0;
					system("cls");
	
					printf("\nIngrese Apellido y Nombre: ");
					_flushall();
					gets(rec.ApellidoyNombre);
						
						do
						{
							bu=0;
							
							printf("\n+++++++++++++++++++++++++++++++++++++++++");
							printf("\nCondiciones de Nombre de Usuario:");
							printf("\n+++++++++++++++++++++++++++++++++++++++++");
							printf("\n*Debera tener entre 6 y 32 caracteres."); //CondicionR1
							printf("\n*Debe ser unico."); // CondicionR2
							printf("\n*Debe comenzar con una letra minuscula."); //CondicionR3
							printf("\n*Debe tener al menos 2 letras mayusculas."); //CondicionR4
							printf("\n*Debe tener 3 digitos como maximo."); //CondicionR5
							printf("\n+++++++++++++++++++++++++++++++++++++++++");
							printf("\n\nIngrese Nombre de Usuario: ");
							_flushall();
							gets(rec.Usuario);
							
							Regla1=0;
							Regla2=0;
							Regla3=0;
							Regla4=0;
							Regla5=0;
							
							if((Regla1=CondicionR1(ArchRec,rec)==1) && (Regla2=CondicionR2(ArchRec,rec)==1) && (Regla3=CondicionR3(ArchRec,rec)==1) && (Regla4=CondicionR4(ArchRec,rec)==1) && (Regla5=CondicionR5(ArchRec,rec)==1))
							{
								printf("\nNombre de Usuario valido.\n");
								bu=1;
									
							}
							else
							{
								printf("\nNombre de Usuario invalido.\n");
								if(Regla1=CondicionR1(ArchRec,rec)==0)
								{
									printf("\nError, no tiene entre 6 y 32 caracteres.");
								}
								if(Regla2=CondicionR2(ArchRec,rec)==0)
								{
									printf("\nError, el nombre de usuario esta repetido.");
								}
								if(Regla3=CondicionR3(ArchRec,rec)==0)
								{
									printf("\nError, no comienza con una letra minuscula.");
								}
								if(Regla4=CondicionR4(ArchRec,rec)==0)
								{
									printf("\nError, tiene menos de 2 mayusculas.");
								}
								if(Regla5=CondicionR5(ArchRec,rec)==0)
								{
									printf("\nError, tiene mas de 3 digitos.");
								}
								
								
								printf("\n\n");
								system("pause");
								system("cls");
							}
							
						}while(bu!=1);
						
						//contraseña usuario
						
						do
						{
							b=0;
							
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\nCondiciones para la contraseña:");
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\n*Debe tener al menos una letra mayuscula, una letra minuscula y un numero."); //Condicion 1
							printf("\n*No puede contener ningun caracter de puntuacion, ni acentos, ni espacios."); //Condicion 2
							printf("\n*Debera tener entre 6 y 32 caracteres."); //Condicion 3
							printf("\n*No debe tener más de 3 caracteres numéricos consecutivos."); //Condicion 4
							printf("\n*No debe tener 2 caracteres consecutivos  alfabeticos (ascendentes)."); //Condicion 5
							printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
							printf("\n\nIngrese la Contrasena: ");
							_flushall();
							gets(rec.Contrasena);
							
							Regla1=0;
							Regla2=0;
							Regla3=0;
							Regla4=0;
							Regla5=0;
							
							if((Regla3=CondicionCR3(ArchRec,rec)==1) && (Regla1=CondicionCR1(ArchRec,rec)==1) && (Regla2=CondicionCR2(ArchRec,rec)==1) && (Regla4=CondicionCR4(ArchRec,rec)==1) && (Regla5=CondicionCR5(ArchRec,rec)==1))
							{
								printf("\nContrasena valida.\n\n");
								system("pause");
								b=1;
							}
							else
							{
								printf("\nContrasena invalida.");
								
								if(Regla1=CondicionCR1(ArchRec,rec)==0)
								{
									printf("\nError, no una letra mayuscula, una letra minuscula o un numero.");
								}
								if(Regla2=CondicionCR2(ArchRec,rec)==0)
								{
									printf("\nError, posee algun caracter de puntuacion, acentos o espacios.");
								}
								if(Regla3=CondicionCR3(ArchRec,rec)==0)
								{
									printf("\nError, no tiene entre 6 y 32 caracteres.");
								}
								if(Regla4=CondicionCR4(ArchRec,rec)==0)
								{
									printf("\nError, tiene más de 3 caracteres numéricos consecutivos.");
								}
								if(Regla5=CondicionCR5(ArchRec,rec)==0)
								{
									printf("\nError, tiene 2 caracteres consecutivos alfabeticos (ascendentes).");
								}
								
								printf("\n\n");
								system("pause");
							}
						system("cls");
											
						}while(b!=1);
					
				fwrite(&rec,sizeof(rec),1,ArchRec);
							
				fclose(ArchRec);
				
				printf("\nFin de la Carga.");
				parar();
				break;
				
				}
			
			
			case 3:
				{
					parar();
					
					ArchRec=fopen("Usuarios.dat","r+b");
										
					if(ArchRec==NULL)
					{
						printf("\nError, el archivo no existe.");
						exit(1);
					}
					
					MayorRegistros(ArchCon,ArchRec,ArchVete,con,rec,vet,Name);
					
					fclose(ArchRec);
					
					parar();					
					
					break;
				}
			    default:
				{
					break;
				}
			}
		}while(OpcionAdm!=4);
     
	}
   }while(opc!=4);

}
