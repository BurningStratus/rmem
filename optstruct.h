
#ifndef OPTSSTRUCT__
#define OPTSSTRUCT__

typedef struct opt {
    int     leng;       // amount of symbols(int)
    char    fillc;      // char which represents used mem(any char)
    char    emptyc;     // char which represents free mem(any char)
    char    ldelim;     // delimiter on the left(any char)
    char    rdelim;     // delimiter on the right(any char)
    char    justify;    // from which side to start counting(r/l)
    char    info_amt;   // amount of info.(s=only bar; f=bar,%used,label;
    char    fillc_p;    // char which represents used mem,
                        // but for not completely filled bars
} options;

/*
const char opts_arr[] = {
    "leng",
    "fillc",
    "emptyc",
    "ldelim",
    "rdelim",
    "justify",
    "info_amt",
    "fillc_p"
};
*/

#endif

