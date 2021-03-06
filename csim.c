/*
 * csim.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss. (I examined the trace,
 *  the largest request I saw was for 8 bytes).
 *  2. Instruction loads (I) are ignored, since we are interested in evaluating
 *  trans.c in terms of its data cache performance.
 *  3. data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus an possible eviction.
 *
 * The function printSummary() is given to print output.
 * Please use this function to print the number of hits, misses and evictions.
 * This is crucial for the driver to evaluate your work.
 */
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

//#define DEBUG_ON
#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/*
 * Data structures to represent the cache we are simulating
 *
 * TODO: Define your own!
 *
 * E.g., Types: cache, cache line, cache set
 * (you can use a counter to implement LRU replacement policy)
 */

typedef struct cache_line {
    char valid;
    mem_addr_t tag;
    unsigned long long int lru;
} cache_line_t;


typedef cache_line_t *cache_set_t;
typedef cache_set_t *cache_t;



/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* associativity */
char* trace_file = NULL;

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;

cache_t cache;

unsigned long long lru_counter = 1;

/*
 * initCache - Allocate memory (with malloc) for cache data structures (i.e., for each of the sets and lines per set),
 * writing 0's for valid and tag and LRU
 *
 * TODO: Implement
 *
 */
void initCache()
{
    int i;
    int j;

    cache = (cache_set_t*)malloc(sizeof(cache_set_t) * S);

    for(i=0;i<S;i++){
    
        cache[i] = (cache_line_t*) malloc(sizeof(cache_line_t) * E);
        for(j=0;j<E;j++){
        
            cache[i][j].tag = 0;

            cache[i][j].valid=0;

            cache[i][j].lru = 0;

        }
    }

}

/*
 * freeCache - free allocated memory
 *
 * This function deallocates (with free) the cache data structures of each
 * set and line.
 *
 * TODO: Implement
 */
void freeCache()
{

    int i;

    for(i=0; i<S; i++)
    { free(cache[i]); }
    
    free(cache);
}


/*
 * accessData - Access data at memory address addr
 *   If it is already in cache, increase hit_count
 *   If it is not in cache, bring it in cache, increase miss count.
 *   Also increase eviction_count if a line is evicted.
 *
 * TODO: Implement
 */
void accessData(mem_addr_t addr)
{
    int i,j;
    mem_addr_t mask = (mem_addr_t)(pow(2,s));
    mask--;
    mem_addr_t seti = (addr >> b);
    seti = seti & mask;

    mem_addr_t tag = addr >> (s+b);


    cache_set_t cache_set = cache[seti];

    for(i=0; i < E; i++)
    {
        if(cache_set[i].tag == tag && cache_set[i].valid)
        {
            hit_count++;

            for(j = 0; j < E; j++)
            {
                if(cache_set[j].valid && (cache_set[j].lru < cache_set[i].lru))
                    {cache_set[j].lru++;}
            }

            cache_set[i].lru = 0;
            return;
        }
    }

    miss_count++;

    int k;

    int max_index = 0;
    unsigned long long max_lru = 0;

    for (k=0; k < E && (cache_set[k].valid); k++)
    {
        if(cache_set[k].lru >= max_lru)
        {
            max_lru = cache_set[k].lru;
            max_index = k;
        }
    }

    if(k!=E)
    {
        for(int L = 0; L < E; L++)
        {
            if(cache_set[L].valid)
                {cache_set[L].lru++;}
        }

        cache_set[k].valid = 1;
        cache_set[k].tag = tag;
        cache_set[k].lru = 0;
    }
    else
    {

        eviction_count++;

        for(int L =0; L< E; L++)
            {cache_set[L].lru++;}

        cache_set[max_index].lru = 0;
        cache_set[max_index].tag = tag;
    }

}


/*
 * replayTrace - replays the given trace file against the cache
 *
 * This function:
 * - opens file trace_fn for reading (using fopen)
 * - reads lines (e.g., using fgets) from the file handle (may name `trace_fp` variable)
 * - skips lines not starting with ` S`, ` L` or ` M`
 * - parses the memory address (unsigned long, in hex) and len (unsigned int, in decimal)
 *   from each input line
 * - calls `access_data(address)` for each access to a cache line
 *
 */
void replayTrace(char* trace_fn) {
  char* buffer = (char*)malloc(sizeof(char) * 20);
  FILE* f = fopen(trace_fn, "r");
  while(!feof(f)) {
    fgets(buffer, 20, f);
    char* token = strtok(&buffer[1], " ");
    if (token) {
      int modify = !strcmp(token, "M");
      if (strcmp(token, "S") && strcmp(token, "L") && strcmp(token, "M")) {
        continue;
      }

      token = strtok(NULL, ",");
      if (!token) {
        continue;
      }

      unsigned long address = strtol(token, NULL, 16);

      // We ignore the length part of the trace

      accessData(address);
      if (modify) {
        accessData(address);
      }
    }
  }
}

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 *
 * !! DO NOT MODIFY !!
 *
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded.
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/*
 * main - Main routine
 */
int main(int argc, char* argv[])
{
    char c;

    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
        switch(c){
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    /* Compute S, E and B from command line args */
    S = (unsigned int) pow(2, s);
    B = (unsigned int) pow(2, b);

    /* Initialize cache */
    initCache();

#ifdef DEBUG_ON
    printf("DEBUG: S:%u E:%u B:%u trace:%s\n", S, E, B, trace_file);
#endif

    replayTrace(trace_file);

    /* Free allocated memory */
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
