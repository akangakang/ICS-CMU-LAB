/*
    name:Liu Shuchang
    loginID: 518021910789
*/

#include "cachelab.h"
#include<unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>


// global variables
struct line_type{
	unsigned long int valid;
	unsigned long int tag;
	unsigned long int time_counter;    // time_stamp
};

struct cache_type{
	unsigned long int S;
	unsigned long int E;
	unsigned long int B;
	struct line_type **cache;

}my_cache;

int hits=0,misses=0,evictions=0;
void print_help()
{
	printf("-h : Optional help flag that prints usage info\n");
    printf("-v : Optional verbose flag that displays trace info\n");
    printf("-s <s>: Number of set index bits (S= 2^s is the number of sets)\n");
    printf("-E <E>: Associativity (number of lines per set)\n");
    printf("-b <b>: Number of block bits (B= 2^b is the block size)\n");
    printf("-t <tracefile>: Name of the valgrind trace to replay\n");

}

void set_cache(unsigned long int S,unsigned long int E,unsigned long int B)
{
	my_cache.S=S;
	my_cache.E=E;
	my_cache.B=B;
}
void ini_cache()
{
	my_cache.cache = (struct line_type **)malloc(my_cache.S * sizeof(struct line_type *));
    if(my_cache.cache==NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
	for(unsigned long int i=0 ; i < my_cache.S ;i++)
		
	{
		my_cache.cache[i] = (struct line_type *)malloc(my_cache.E * sizeof(struct line_type));
         if(my_cache.cache[i]==NULL)
        {
            printf("malloc failed\n");
            exit(1);
        }
		for(unsigned long int j=0; j< my_cache.E ;j++)
		{
			my_cache.cache[i][j].valid=0;
			my_cache.cache[i][j].tag=0;
			my_cache.cache[i][j].time_counter=0;
		}
	}

}

int is_hit(unsigned long int set , unsigned  long int tag)
{
	for(unsigned long int i=0;i<=my_cache.E;i++)
	{
		if(my_cache.cache[set][i].tag==tag && my_cache.cache[set][i].valid==1)
			return i;
	}

	return -1;

}
void hit_miss_replace(unsigned long int set, unsigned long int tag,unsigned long int time,bool verbose)
{
		int e= is_hit(set,tag);
			if(e!=-1)
			{
				hits++;
				if(verbose)
					printf("hits  ");
				my_cache.cache[set][e].time_counter=time;  // update time
			}
			else
			{
				misses++;
				if(verbose)
					printf("misses  ");
				bool flag=0;
				for(unsigned long int i=0;i<my_cache.E;i++)
				{
					if(my_cache.cache[set][i].valid==0)
					{
						my_cache.cache[set][i].valid=1;
						my_cache.cache[set][i].tag=tag;
						my_cache.cache[set][i].time_counter=time;
						flag=1;
						break;
					}
				}

                // LRU
				if(!flag)
				{
					unsigned long int min_counter=1000000000;
					int min_line=-1;
					for(unsigned long int i=0;i<my_cache.E;i++)
					{
						if(my_cache.cache[set][i].time_counter < min_counter)
						{
							min_counter = my_cache.cache[set][i].time_counter;
							min_line = i;
						}
					}
					if(min_line >=0 )
					{
						my_cache.cache[set][min_line].valid=1;
						my_cache.cache[set][min_line].tag=tag;
						my_cache.cache[set][min_line].time_counter=time;
						evictions++;
						if(verbose)
							printf("evictions  ");
					}
					else
					{
						if(verbose) printf("replace error");
					}

				}
			}

}

void realseMemory(struct cache_type cache, unsigned long int  S, unsigned long int E)
{
    for (unsigned long int i = 0; i < S; ++i)
    {
        free(cache.cache[i]);
    }
    free(cache.cache);
}

int main(int argc, char *const argv[])
{
    char opt,c;
	FILE *Path = NULL;
	int _s = -1, _E = -1, _b = -1;
	bool verbose = 0 ,wrong =0;
	unsigned long int addr,number;
	char  cmd[6]=" ";
	unsigned long int  time=1;


    //read the input
    while((opt=getopt(argc,argv,"hvs:E:b:t:"))!=-1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            break;
        case 'v':
            verbose =1;
            break;
        case 's':
            _s= atol (optarg);
            break;
        case 'E':
            _E = atol(optarg);
            break;
        
        case 'b':
            _b=atol(optarg);
            break;
        case 't':
            Path = fopen(optarg,"r");
            if(Path==NULL)
            {
                printf("open file failed\n");
                exit(1);
            }
            break;
        }
    }

    // if input is wrong 
    if(_s<=0 || _E <=0 || _b <=0 || Path==NULL || wrong)
	{
		print_help();
		return 0;
	}

    //initialize the cache  --using malloc
    set_cache(1<<_s,_E,1<<_b);
	ini_cache();


while(fscanf(Path,"%s%lx%c%lu",cmd,&addr,&c,&number)!=EOF)
	{
		if(cmd[0] == 'I') continue;
		if(verbose)
	 		printf("%c %lx%c%lu ",cmd[0],addr,c,number);

        // calculate the tag and set 
		unsigned long int tag = addr >> (_s + _b);
		unsigned long int set = (addr & ((1<<(_s+_b))-1))>>_b;

		if(verbose)
			printf("set= %lu tag = %lu  ",set ,tag);

        
	    if(cmd[0]=='L' || cmd[0]=='S')
		{
			hit_miss_replace(set,tag,time,verbose);
		}

		else if(cmd[0]=='M')
		{
            // a load and a store
			hit_miss_replace(set,tag,time,verbose);
			hit_miss_replace(set,tag,time,verbose);
		}
		if(verbose)  printf("time: %lu \n",time);		
		time ++;

	}
		

    realseMemory(my_cache,my_cache.S,my_cache.E);

    printSummary(hits, misses, evictions);
    return 0;
}
