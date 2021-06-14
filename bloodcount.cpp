#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

using namespace std;
const int hastasayi =16;

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

int toplamislem = 300000*hastasayi;
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
	MPI_Init(NULL,NULL);
	MPI_Comm comm;
	int gsize, *sendbuf;
	int root,rbuf[100],i,*displs,*scounts;

	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	//MPI_Comm_size(comm, &gsize);
	//sendbuf = (int *)malloc(gsize*sizeof(int));
	//displs =(int *)malloc(gsize*sizeof(int));
	//scounts =(int *)malloc(gsize*sizeof(int));


	const int ROOT=0;
	int basla = clock();
	int hasta=0;
	int hucre=0;
	int count = toplamislem/(world_size-1);
	int *localArray =(int *)malloc(count*sizeof(int));

	int local_x = 0;
	int local_notrofil[hastasayi];
	int local_lenfosit[hastasayi];
	int local_monosit[hastasayi];
	int local_eoizonofil[hastasayi];
	int local_bazofil[hastasayi];


	for(int i=0;i<toplamislem;i++)
	{
		hasta = hastaId();
		hucre = hucreUret();
		hesapla(hasta,hucre);
		//printf("islemci(%d), HastaId(%d), GorselId(%d), KanHucresi(%d) \n",world_rank,hasta,i,hucre);
		islemsay++;

	}
	//MPI_Reduce(&local_notrofil,&notrofil,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	MPI_Finalize();

	printf("HastaID        || Sayilan Hucre || Notrofil -- %% Orani  || Lenfosit -- %% Orani || Monosit -- %% Orani || Eoizonofil -- %% Orani  || \n");
	printf("________________________________________________________________________________________________________________\n");
	yuzdelerihesapla();

	for(int i=0;i<hastasayi;i++)
	{
	printf("%d            || %d             || %d -- %% %d       || %d -- %% %d     || %d -- %% %d      || %d -- %% %d      || %d -- %% %d     || \n",i+1,toplamhucre[i],notrofil[i],notrofil_y[i],lenfosit[i],lenfosit_y[i],monosit[i],monosit_y[i],eoizonofil[i],eoizonofil_y[i],bazofil[i],bazofil_y[i]);

	}

	printf("_________________________________________________________________________________________________________________\n");

	printf("Tamamlanma suresi: %ld Milisaniye \n", (clock()-basla));

   return 0;
}