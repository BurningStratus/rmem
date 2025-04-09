
#include "optstruct.h"
#include "defs.h"
#include "stdlib.h"

#ifndef ARGPARSE__
#define ARGPARSE__

char *getopt(const char *tk_name, const char *opt)
{
/*
    going crazy with goto(i saw that it is used in i3wm code, so its ok :p)
*/
    char *val;
    if ( !strcmp(tk_name, opt) )
    {
goto getmem;
write_ptr:
        sscanf(tk_name, "%*[\'\"\n]%s", val);
    }

    return val;


getmem:
    val   =(char *)calloc(STRLEN, sizeof(char));
    if ( !val )
    {
        printf("calloc error on %d!\n", __LINE__);
        exit(1);
    }
goto write_ptr;
}

int fstruct (options *opt)
{
    const char  optval[STRLEN] = "";

    int         cfg_exist=0;
    FILE        *fd_cfg;
    char        cfg_path  [STRLEN] ="";
    char        home_path [STRLEN] ="";
    char        token_name[STRLEN] ="";
    char        token_val [STRLEN] ="";

    char        rawline [STRLEN]="";
    char        line    [STRLEN]="";
    
    opt->leng=15;
    opt->fillc='/';
    opt->emptyc=' ';
    opt->ldelim='[';
    opt->rdelim=']';
    opt->justify='l';
    opt->info_amt='s';

    strcpy(cfg_path, getenv(ENV_HOME)); // get $HOME env var
    strcat(cfg_path, CFG_DEFPATH);
    strcat(cfg_path, CFG_NAME);
    fd_cfg=fopen(cfg_path, "r");
    if (fd_cfg == NULL)
    {
        //fprintf(stderr, "Cannot open config file! [%s]", cfg_path);
        return 1;
    }

    while ( (fgets(rawline, ARRSIZE(rawline), fd_cfg)) != NULL)
    {
        int     s=0;
        int     ls=0;
        int     bool_comment=0;
        int     TOKEN_CONVERTED=0;

        /* remove redundant whitespace and convert delims to tokens */
        while ( rawline[s] )
        {
            if ( rawline[s] != 0x20 && rawline[s] != '\n' )
            {
                // convert DELIMITER to an internal token $
                if ( rawline[s] == '=' && !TOKEN_CONVERTED )
                {
                    line[ls] = DELIMITER_STRING[0]; // TODO
                    TOKEN_CONVERTED=1; // do only once
                } else {
                    line[ls]=rawline[s];
                }
                ls++;
            }
            s++;
        }

        /* comment handling */
        s=-1;
        while ( line[++s] )
        {
            if ( line[s] == '#' )
                bool_comment=1; // symbol at which comment was encountered
            if ( bool_comment )
                line[s]=0;      // nullify symbols after comment
            //s++; TODO: beware!!!
        } 
        if ( !*line ) continue; // skip on empty line

        strcpy(token_name, strtok(line, DELIMITER_STRING));
        strcpy(token_val,  strtok(NULL, DELIMITER_STRING));
        /*
        if ( !strcmp(token_name, "leng") && *token_val ) {
            sscanf(token_val, "%d", &(opt->leng));// leng
        } 
        */


        int     o=-1;
        char    *ps_v; // parsed value

        // check if token matches a "control string", and if it does, 
        ps_v=getopt(token_name, "leng");
        if (ps_v) {
            sscanf(token_val, "%*[\'\"\n]%d", &(opt->leng)); 
            continue;
        }

        ps_v=getopt(token_name, "fillc");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->fillc));continue;}

        ps_v=getopt(token_name, "emptyc");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->emptyc));continue;}

        ps_v=getopt(token_name, "ldelim");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->ldelim));continue;}

        ps_v=getopt(token_name, "rdelim");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->rdelim));continue;}

        ps_v=getopt(token_name, "justify");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->justify));continue;}

        ps_v=getopt(token_name, "info_amt");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->info_amt));continue;}

        ps_v=getopt(token_name, "fillc_p");
        if (ps_v) {sscanf(token_val, "%*[\'\"\n]%c", &(opt->fillc_p));continue;}

/*
        if ( !strcmp(token_name, "leng") && *token_val ) {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->leng));// leng
        } 
        else if ( !strcmp(token_name, "fillc")  && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->fillc));
            //sscanf(token_val, "%c", &(opt->fillc));
        } 
        else if ( !strcmp(token_name, "emptyc") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->emptyc));
            //sscanf(token_val, "%c", &(opt->emptyc));
        } 
        else if ( !strcmp(token_name, "ldelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->ldelim));
            //sscanf(token_val, "%c", &(opt->ldelim));
        } 
        else if ( !strcmp(token_name, "rdelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->rdelim));
            //sscanf(token_val, "%c", &(opt->rdelim));
        } 
        else if ( !strcmp(token_name, "ldelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->ldelim));
            //sscanf(token_val, "%c", &(opt->ldelim));
        } 
        else if ( !strcmp(token_name, "rdelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(opt->rdelim));
            //sscanf(token_val, "%c", &(opt->rdelim));
        } 
*/
        /* TODO: treat everyting as a string initially
        else if ( !strcmp(token_name, "justify") && *token_val ) 
        {
            sscanf(token_val, "%*[^rl]%c", &(opt->justify) );
            //sscanf(token_val, "%c", &(opt->justify) );
        }
        else if ( !strcmp(token_name, "info_amt") && *token_val ) 
        {
            sscanf(token_val, "%*[^sf]%c", &(opt->info_amt) );
            //sscanf(token_val, "%c", &(opt->justify) );
        }
        */
    }
    return 0;
}


#endif
/*
int fstruct (struct opt *optsstruct)
{
    const char  optval[STRLEN] = "";

    int         cfg_exist=0;
    FILE        *fd_cfg;
    char        cfg_path  [STRLEN] ="";
    char        home_path [STRLEN] ="";
    char        token_name[STRLEN] ="";
    char        token_val [STRLEN] ="";

    char        rawline [STRLEN]="";
    char        line    [STRLEN]="";
    
    optsstruct->leng=15;
    optsstruct->fillc='/';
    optsstruct->emptyc=' ';
    optsstruct->ldelim='[';
    optsstruct->rdelim=']';
    optsstruct->justify='l';
    optsstruct->info_amt='s';

    strcpy(cfg_path, getenv(ENV_HOME)); // get $HOME env var
    strcat(cfg_path, CFG_DEFPATH);
    strcat(cfg_path, CFG_NAME);
    fd_cfg=fopen(cfg_path, "r");
    if (fd_cfg == NULL)
    {
        //fprintf(stderr, "Cannot open config file! [%s]", cfg_path);
        return 1;
    }

    while ( (fgets(rawline, ARRSIZE(rawline), fd_cfg)) != NULL)
    {
        int     s=0;
        int     ls=0;
        int     bool_comment=0;
        int     TOKEN_CONVERTED=0;

        // remove redundant whitespace and convert delims to tokens
        while ( rawline[s] )
        {
            if ( rawline[s] != 0x20 && rawline[s] != '\n' )
            {
                // convert DELIMITER to an internal token $
                if ( rawline[s] == '=' && !TOKEN_CONVERTED )
                {
                    line[ls] = DELIMITER_STRING[0]; // TODO
                    TOKEN_CONVERTED=1; // do only once
                } else {
                    line[ls]=rawline[s];
                }
                ls++;
            }
            s++;
        }

        // comment handling
        s=0;
        while ( line[s] )
        {
            if ( line[s] == '#' )
                bool_comment=1; // symbol at which comment was encountered
            if ( bool_comment )
                line[s]=0; // nullify symbols after comment
            s++;
        } 

        if ( !*line )
            continue;

        strcpy(token_name, strtok(line, DELIMITER_STRING));
        strcpy(token_val,  strtok(NULL, DELIMITER_STRING));
    
        if ( !strcmp(token_name, "leng") && *token_val ) {
            sscanf(token_val, "%d", &(optsstruct->leng));// leng
        } 
        else if ( !strcmp(token_name, "fillc")  && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(optsstruct->fillc));
            //sscanf(token_val, "%c", &(optsstruct->fillc));
        } 
        else if ( !strcmp(token_name, "emptyc") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(optsstruct->emptyc));
            //sscanf(token_val, "%c", &(optsstruct->emptyc));
        } 
        else if ( !strcmp(token_name, "ldelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(optsstruct->ldelim));
            //sscanf(token_val, "%c", &(optsstruct->ldelim));
        } 
        else if ( !strcmp(token_name, "rdelim") && *token_val ) 
        {
            sscanf(token_val, "%*[\'\"\n]%c", &(optsstruct->rdelim));
            //sscanf(token_val, "%c", &(optsstruct->rdelim));
        } 
        else if ( !strcmp(token_name, "justify") && *token_val ) 
        {
            sscanf(token_val, "%*[^rl]%c", &(optsstruct->justify) );
            //sscanf(token_val, "%c", &(optsstruct->justify) );
        }
        else if ( !strcmp(token_name, "info_amt") && *token_val ) 
        {
            sscanf(token_val, "%*[^sf]%c", &(optsstruct->info_amt) );
            //sscanf(token_val, "%c", &(optsstruct->justify) );
        }
    }
    return 0;
}
*/
