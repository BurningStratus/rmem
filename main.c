#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "optstruct.h"
#include "defs.h"

/*
    Parse options only once, and save them as filled struct to /tmp ! 
    To avoid wasteful config parsing !
*/

struct mem {
    unsigned long memtot;
    unsigned long memfree;
    unsigned long memavail;
} memstat;

options opts;

int rmeminfo (struct mem *memstruct);
int printl (struct mem *memstruct, options *optsstruct);
//int fstruct (struct opt *optsstruct); -- moved to argparse.h 

int main(int argc, char **argv)
{
    fstruct( &opts );
    rmeminfo(&memstat );
    printl ( &memstat, &opts ); 
}


int rmeminfo (struct mem *memstruct)
{
    int    memtot=0;
    int    memfree=0;
    int    memavail=0;
    FILE   *meminfo;
    char   line[64]="";
    int    ct=0;

    meminfo=fopen(PATH, "r");
    if ( meminfo == NULL )
        return 1;
    while ( (fgets(line, ARRSIZE(line), meminfo)) != NULL && ct < 3 )
    {
        if (ct == 0)
            sscanf(line, "%*[^0-9]%d", &memtot); //ignore till int 
        if (ct == 1)
            sscanf(line, "%*[^0-9]%d", &memfree);
        if (ct == 2)
            sscanf(line, "%*[^0-9]%d", &memavail);
        ct++;
    }
    (*memstruct).memtot=memtot;
    (*memstruct).memfree=memfree;
    (*memstruct).memavail=memavail;
    return 0;
}

int printl (struct mem *memstruct, struct opt *optsstruct)
{
    float   used_pc;    // used percent ( as in GNU "free" )
    int     used_symamt;// "used" symbols amount
    int     free_symamt;// "free" symbols amount

    used_pc= ((float)memstruct->memtot - (float)memstruct->memavail ) / 
                                            (float)memstruct->memtot;//let 0.4
    used_symamt = optsstruct->leng * used_pc;     // 10 * 0.4 ~ 4
    free_symamt = optsstruct->leng - used_symamt; // 10 - 4 = 6 
    //printf("[USED %: %f] ", used_pc);
    
    if ( optsstruct->info_amt == 'f') // more info
        printf("RAM ");

    switch (optsstruct->justify)
    {
        case 'r' : 
        {
            printf("%c", optsstruct->ldelim);

            // unused memory
            for (int i=0; i<free_symamt; i++)
                printf("%c", optsstruct->emptyc);
            // used memory
            for (int i=0; i<used_symamt; i++)
                printf("%c", optsstruct->fillc);
            break;
        }
        case 'l' : 
        {
            printf("%c", optsstruct->ldelim);

            // used memory
            for (int i=0; i<used_symamt; i++)
                printf("%c", optsstruct->fillc);
            // unused
            for (int i=0; i<free_symamt; i++)
                printf("%c", optsstruct->emptyc);
            break;
        }
    }

    printf("%c", optsstruct->rdelim);

    if ( optsstruct->info_amt == 'f') // more info
        printf(" %0.1f%%", (float)used_pc*(float)100);
    
    printf("\n"); //newline is needed for i3blocks
    return 0;
}

