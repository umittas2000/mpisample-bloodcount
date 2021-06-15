#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <cstring>
#include <time.h>

using namespace std;
const int hastasayi =15;

int notrofil[hastasayi];
int notrofil_y[hastasayi];

int lenfosit[hastasayi];
int lenfosit_y[hastasayi];

int monosit[hastasayi];
int monosit_y[hastasayi];

int eoizonofil[hastasayi];
int eoizonofil_y[hastasayi];

int bazofil[hastasayi];
int bazofil_y[hastasayi];

int toplamhucre[hastasayi];

int toplamislem = 4000 * hastasayi;
int islemsay =0;

int hucreUret()
{
	int enFazla =5, enAz=1;
	int sayi=rand() % (enFazla - enAz +1)+enAz;
	return sayi;
}

int hastaId()
{
	int enFazla = hastasayi-1,enAz=0;
	int sayi = rand() % (enFazla - enAz +1) +enAz;
	return sayi;
}

void yuzdelerihesapla()
{
	for(int i=0;i<hastasayi;i++)
	{
		notrofil_y[i] = (notrofil[i]*100) / toplamhucre[i];
		lenfosit_y[i] = (lenfosit[i]*100) / toplamhucre[i];
		monosit_y[i] = (monosit[i]*100) / toplamhucre[i];
		eoizonofil_y[i]=(eoizonofil[i]*100) / toplamhucre[i];
		bazofil_y[i] = (bazofil[i]*100) / toplamhucre[i];
	}
}

int hesapla(int hasta,int hucre)
{
	toplamhucre[hasta]=toplamhucre[hasta]+1;
	if(hucre==1)
	{
		notrofil[hasta]++;
	}else if(hucre==2)
	{
		lenfosit[hasta]++;
	}else if(hucre==3)
	{
		monosit[hasta]++;
	}else if(hucre==4)
	{
		eoizonofil[hasta]++;
	}else if(hucre==5)
	{
		bazofil[hasta]++;
	}else{
	//deneme
}

return 0;
}


int main(int argc, char** argv)
{
	time_t seed;
	time(&seed);
	srand(seed);

	char idstr[32];
	char buff[128];
	int numprocs;
	int myid;
	int i;
	
	MPI_Status stat; 
	MPI_Init(NULL,NULL); 
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs); 
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	const int ROOT=0;

	int basla = clock();


for(int dd=0;dd<toplamislem;dd++)
{
	if(myid == 0)
	{
	  	for(i=1;i<numprocs;i++)
	    {
			int hasta = hastaId();
			int hucre = hucreUret();
			hesapla(hasta,hucre);
			islemsay++;
			printf("islem(%d), islemci(%d), HastaId(%d), GorselId(%d), KanHucresi(%d) \n",dd,numprocs,hasta,i,hucre);
			MPI_Send(buff, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD);
	    }

	    for(i=1;i<numprocs;i++)
	    {
	      MPI_Recv(buff, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat);
	    }
	}else
	{
		MPI_Recv(buff, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
	    MPI_Send(buff, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
}

	
	MPI_Comm_rank(MPI_COMM_WORLD,&myid); //Bu blogu sadece Master calistirsin.
	if(myid==0)
	{
		printf("HastaID        || Sayilan Hucre || Notrofil -- %% Orani  || Lenfosit -- %% Orani || Monosit -- %% Orani || Eoizonofil -- %% Orani  || \n");
		printf("________________________________________________________________________________________________________________\n");
		yuzdelerihesapla();

		for(int i=0;i<hastasayi;i++)
		{
			printf("%d            || %d             || %d -- %% %d       || %d -- %% %d     || %d -- %% %d      || %d -- %% %d      || %d -- %% %d     || \n",i+1,toplamhucre[i],notrofil[i],notrofil_y[i],lenfosit[i],lenfosit_y[i],monosit[i],monosit_y[i],eoizonofil[i],eoizonofil_y[i],bazofil[i],bazofil_y[i]);
		}

		printf("_________________________________________________________________________________________________________________\n");

		printf("Tamamlanma suresi: %ld Milisaniye \n", (clock()-basla));
	}

 MPI_Finalize();

   return 0;
}
