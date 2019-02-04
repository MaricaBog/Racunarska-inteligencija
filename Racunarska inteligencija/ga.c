


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define max 500

float bestfitness,prevDistance;
int bestOrder[max];
int bestFitIndex;
float bestDistance;
int k,size,generation;
int verification_count,verification_max;
float fit[max], normal, mean;

double besttime=0,elapsedtime;

time_t start_t, end_t;

void Genpoints();
void shuffle(int a[max]);
void GenPop();
void FitPopulation();
void SelectPopulation();
void mutate();
void evolve();

float calculatedistance(int order[max]);
float distance(int x0,int x1,int y0,int y1);
float dist(int i);

float Max(float a[max]);
float performanceDensity();

void swap(int i,int a,int b);
void swap_array(int array[max],int a,int b);
void copy(int a[max],int b[max]);

void viewpoints();
void showdists();


struct point
{
        int x,y;
} points[max];


struct pop
{
        int order[max];
} *population;


//maksimum niza
float Max(float a[max])
{
        int i;
        float best=0;
        for(i=0; i<size; i++)
                if(a[i]>best)
                    best=a[i];
        return(best);
}

//k je broj tacaka bez prve i poslednje
//mesanje rasporeda tacaka
void shuffle(int a[max])
{
        int i,temp,r1,r2;
        for(i=1; i<10; i++)
        {
                r1=rand()%k;
                r2=rand()%k;

                if(r1>0 && r2>0 && r1<k-1 && r2<k-1)
                {
                        temp=a[r1];
                        a[r1]=a[r2];
                        a[r2]=temp;
                }
        }
}

//size-100
//100 populacija 01230
void GenPop()
{
        int i,j;
        for(i=0; i<size; i++)
                for(j=0; j<=k-2; j++)
                {
                        population[i].order[j]=j; 
                        population[i].order[k-1]=0;
                }

}

//prva i poslednja su (0,0) izmedju su random tacke
void Genpoints()
{
        int i;
        points[0].x=0;
        points[0].y=0;
        points[k-1].x=0;
        points[k-1].x=0;
        for (i=1; i<k-1; i++)
                points[i].x=rand()%10;
        for(i=1; i<k-1; i++)
                points[i].y=rand()%10;
}

//zamena redosleda tacaka u populaciji
void swap(int i,int a,int b)
{
        int temp;
        temp=population[i].order[a];
        population[i].order[a]=population[i].order[b];
        population[i].order[b]=temp;

}

//zamena mesta elemenata u nizu 
void swap_array(int array[max],int a,int b)
{
        int temp;
        temp=array[a];
        array[a]=array[b];
        array[b]=temp;
}

//racunanje rastojanja izmedju 2 tacke
float distance(int x0,int x1,int y0,int y1)
{
        float dist;
        int x2,y2;
        x2=(x0-x1)*(x0-x1);
        y2=(y0-y1)*(y0-y1);

        dist=sqrt(x2+y2);

        return(dist);
}



//racuna rastojanje izmedju susednih tacaka(gradova) i vraca rastojanje
float dist(int i)
{
        int m;
        float sum,d;
        sum=0;
        for(m=0; m<k-1; m++)
        {
d=distance(points[population[i].order[m]].x, points[population[i].order[m+1]].x, points[population[i].order[m]].y,
points[population[i].order[m+1]].y);
                sum=sum+d;

        }
        return sum;
}

//fitnes za populaciju 
//racuna sumu rastojanja i na osnovu nje fitnes f-ju koja predstavlja ukupnu duzinu puta predstavljenu tom jedinkom
void FitPopulation()
{
        int i;
        float sum;

        for(i=0; i<size; i++)
        {
                sum=dist(i);
                fit[i]=10/log10(sum);

        }

}

//Mutacija ne sme da dodaje i brise grad iz rute,ali sme da menja rutu
//jedinka mutira i racunamo rastojanje 
void mutate()
{
        int i,j,r1,r2,*order,d;
        order=(int *)malloc(k*sizeof(int));
        for(i=0; i<size; i++)
        {
                copy(order,population[i].order);
                d=dist(i);
                for(j=0; j<abs((int)(mean-fit[i])); j++)
                {
                        r1=rand()%k+1;
                        r2=rand()%k+1;
                        if(r1<k-1 && r2<k-1)
                                swap(i,r1,r2);
                        if(d<dist(i))
                                copy(population[i].order,order);
                        r1=rand()%k+1;
                        r2=rand()%k+1;
                }
        }
        free(order);
}



float performanceDensity()
{
        int i,c=0;
        float d=0;
        for(i=0; i<size; i++)
                d=d+dist(i);
        d=d/size;//sr vr distance
        mean=10/log10(d);
        for(i=0; i<size; i++)
                if(dist(i)>d)
                        c++;
        return(c);
}
//stampa tacke (x,y)
void viewpoints()
{
        int i;
        printf( "Points: ");
        for(i=0; i<k; i++)
        {
                printf( "%d,",points[i].x);
                printf("%d |",points[i].y);

        }
        printf("\n\n");

}


void copy(int a[max],int b[max])
{
        int i;
        for(i=0; i<k; i++)
                a[i]=b[i];
}

//u procesu selekcije trazimo populaciju sa najboljim fitnesom i rastojanjem
void SelectPopulation()
{
        int i;
        FitPopulation();
        bestfitness=Max(fit);
        for(i=0; i<size; i++)
        {
			//stampaj indeks najboljeg fitnesa
                if(fit[i]==bestfitness)
                {
                        printf(" ");
                        bestFitIndex=i;
                }

        }
	   //trazimo populaciju s najboljim fitnesom
        if(dist(bestFitIndex)<bestDistance)
        {
                copy(population[0].order,population[bestFitIndex].order);
                bestFitIndex=0;
        }
		//ako je najbolje rastojanje lose, bice jednako dist(0)
        if(bestDistance>dist(0))
        {
                time(&end_t);
                besttime = difftime(end_t, start_t);
                verification_count=0;

        }
        else verification_count+=1;
        bestDistance=dist(0);
}


float calculatedistance(int order[max])
{
        int i;
        float d=0;
        for(i=0; i<k-1; i++)
        {
                d=d+distance(points[order[i]].x,points[order[i+1]].x,points[order[i]].y,points[order[i+1]].y);
        }
        return d;
}


void evolve()
{
        int i,var,p,order[max];
        for(i=0; i<size; i++)
        {
                copy(order,population[i].order);
                for(var=rand()%k+1; var<k-1; var++)
                {
                        for(p=rand()%k+1; p<k-1; p++)
                        {
                                swap_array(order,var,p);
                                if(calculatedistance(order)<dist(i))
                                        copy(population[i].order,order);
                                else
                                        copy(order,population[i].order);
                        }
                }
        }
        p=1;
        for(i=0; i<size; i++)
        {
                var=rand()%size+1;
                copy(order,population[i].order);
                if(p<k-2)
                {
                        swap(var,p,p+1);
                        if(dist(i)<calculatedistance(order)) ;
                        else copy(population[i].order,order);
                }
                else p=1;
                p++;
                var=rand()%size+1;
        }
        SelectPopulation();
        generation++;
}


int main()
{
        int i,points,m;
        float pd;
	   clock_t start,end;

	   start=clock();
        printf("Number of Points:");
        scanf("%d",&m);
	   points=m;
        time(&start_t);
        k=m+2;

        Genpoints();
        Genpoints();
        size=100;
        population=(struct pop *)malloc(size*sizeof(struct pop));
        verification_max=1000;//broj generacije
        GenPop();
        for(i=0; i<size; i++)
                shuffle(population[i].order);
		
	
        SelectPopulation();
        printf( "\nBest fitness: %f\n",bestfitness);
        verification_count=0;
        m=1;
        printf( "Best Performance: %f\n",dist(bestFitIndex));
        while(verification_count<verification_max)
        {

                printf("\n" );
                viewpoints();
                printf("Total Points: %d\n",k);
                pd=performanceDensity();
                printf( "Generation:%d\tGFitness:%f\tMean:%f\n",generation,pd/size,mean);

                printf( "\nBest fitness: %f\n",bestfitness);

                printf( "Best Distance: %f\t",bestDistance);
                printf( "Time:%ds\n",(int)besttime);

                copy(bestOrder,population[bestFitIndex].order);
                printf( "Best Sequence: ");
                for(i=0; i<k; i++)
                        printf("%d ",bestOrder[i] );
                printf("\n");
                printf( "UC Check: %d%c\t",(100*verification_count/verification_max),'%' );
                time(&end_t);
                

                evolve();
                if(generation/100>m)
                {
                        m++;
                        mutate();
                }
        }
	   
        SelectPopulation();
	   printf("\n");
    	   printf("\nNumber of Points: %d",points);
	   printf( "\nBest fitness: %f\n",bestfitness);
        printf( "Best Distance: %f\n",dist(bestFitIndex));
        printf( "Best Sequence: ");
        for(i=0; i<k; i++)
                printf("%d ",bestOrder[i] );
        printf("\n");
		

	   end=clock();
	   double t = (double)(end-start)/CLOCKS_PER_SEC;
	   printf("\nTime: %f s\n ", t);


}

 
