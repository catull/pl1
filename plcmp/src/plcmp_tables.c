/* encoding: UTF-8 */

#include "plcmp_common.h"
#include "plcmp_tables.h"


/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
sint_t synt_rules_table[NSINT] = {
    /*  __________ _________ _______ _______ ______
       |  NN      :    next : prev  :  node : alt  |
       |__________:_________:_______:_______:______| */
    {/*.    0     .*/    -1 ,    -1 , "***" ,   -1 },
     /*                                               input with the symbol - 0      */
    {/*.    1     .*/     2 ,     0 , "0  " ,    0 },
    {/*.    2     .*/     3 ,     1 , "CIF" ,    0 },
    {/*.    3     .*/     0 ,     2 , "*  " ,    0 },
     /*                                               input with the symbol - 1      */
    {/*.    4     .*/     5 ,     0 , "1  " ,    0 },
    {/*.    5     .*/     6 ,     4 , "CIF" ,    7 },
    {/*.    6     .*/     0 ,     5 , "*  " ,    0 },
    {/*.    7     .*/     8 ,     4 , "MAN" ,    0 },
    {/*.    8     .*/     0 ,     7 , "*  " ,    0 },
     /*                                               input with the symbol - 2      */
    {/*.    9     .*/    10 ,     0 , "2  " ,    0 },
    {/*.   10     .*/    11 ,     9 , "CIF" ,    0 },
    {/*.   11     .*/     0 ,    10 , "*  " ,    0 },
     /*                                               input with the symbol - 3      */
    {/*.   12     .*/    13 ,     0 , "3  " ,    0 },
    {/*.   13     .*/    14 ,    12 , "CIF" ,    0 },
    {/*.   14     .*/     0 ,    13 , "*  " ,    0 },
     /*                                               input with the symbol - 4      */
    {/*.   15     .*/    16 ,     0 , "4  " ,    0 },
    {/*.   16     .*/    17 ,    15 , "CIF" ,    0 },
    {/*.   17     .*/     0 ,    16 , "*  " ,    0 },
     /*                                               input with the symbol - 5      */
    {/*.   18     .*/    19 ,     0 , "5  " ,    0 },
    {/*.   19     .*/    20 ,    18 , "CIF" ,    0 },
    {/*.   20     .*/     0 ,    19 , "*  " ,    0 },
     /*                                               input with the symbol - 6      */
    {/*.   21     .*/    22 ,     0 , "6  " ,    0 },
    {/*.   22     .*/    23 ,    21 , "CIF" ,    0 },
    {/*.   23     .*/     0 ,    22 , "*  " ,    0 },
     /*                                               input with the symbol - 7      */
    {/*.   24     .*/    25 ,     0 , "7  " ,    0 },
    {/*.   25     .*/    26 ,    24 , "CIF" ,    0 },
    {/*.   26     .*/     0 ,    25 , "*  " ,    0 },
     /*                                               input with the symbol - 8      */
    {/*.   27     .*/    28 ,     0 , "8  " ,    0 },
    {/*.   28     .*/    29 ,    27 , "CIF" ,    0 },
    {/*.   29     .*/     0 ,    28 , "*  " ,    0 },
     /*                                               input with the symbol - 9      */
    {/*.   30     .*/    31 ,     0 , "9  " ,    0 },
    {/*.   31     .*/    32 ,    30 , "CIF" ,    0 },
    {/*.   32     .*/     0 ,    31 , "*  " ,    0 },
     /*                                               input with the symbol - A      */
    {/*.   33     .*/    34 ,     0 , "A  " ,    0 },
    {/*.   34     .*/    35 ,    33 , "BUK" ,    0 },
    {/*.   35     .*/     0 ,    34 , "*  " ,    0 },
     /*                                               input with the symbol - B      */
    {/*.   36     .*/    37 ,     0 , "B  " ,    0 },
    {/*.   37     .*/    38 ,    36 , "BUK" ,    0 },
    {/*.   38     .*/     0 ,    37 , "*  " ,    0 },
     /*                                               input with the symbol - C      */
    {/*.   39     .*/    40 ,     0 , "C  " ,    0 },
    {/*.   40     .*/    41 ,    39 , "BUK" ,    0 },
    {/*.   41     .*/     0 ,    40 , "*  " ,    0 },
     /*                                               input with the symbol - D      */
    {/*.   42     .*/    43 ,     0 , "D  " ,    0 },
    {/*.   43     .*/    44 ,    42 , "BUK" ,   45 },
    {/*.   44     .*/     0 ,    43 , "*  " ,    0 },
    {/*.   45     .*/    46 ,    42 , "C  " ,    0 },
    {/*.   46     .*/    47 ,    45 , "L  " ,    0 },
    {/*.   47     .*/    48 ,    46 , "   " ,    0 },
    {/*.   48     .*/    49 ,    47 , "IDE" ,    0 },
    {/*.   49     .*/    50 ,    48 , "   " ,    0 },
    {/*.   50     .*/    51 ,    49 , "B  " ,  187 },
    {/*.   51     .*/    52 ,    50 , "I  " ,    0 },
    {/*.   52     .*/    53 ,    51 , "N  " ,    0 },
    {/*.   53     .*/    54 ,    52 , "   " ,    0 },
    {/*.   54     .*/    55 ,    53 , "F  " ,    0 },
    {/*.   55     .*/    56 ,    54 , "I  " ,    0 },
    {/*.   56     .*/    57 ,    55 , "X  " ,    0 },
    {/*.   57     .*/    58 ,    56 , "E  " ,    0 },
    {/*.   58     .*/    59 ,    57 , "D  " ,    0 },
    {/*.   59     .*/    60 ,    58 , "(  " ,    0 },
    {/*.   60     .*/    61 ,    59 , "RZR" ,    0 },
    {/*.   61     .*/    62 ,    60 , ")  " ,    0 },
    {/*.   62     .*/    63 ,    61 , ";  " ,   65 },
    {/*.   63     .*/    64 ,    62 , "ODC" ,    0 },
    {/*.   64     .*/    65 ,    63 , "*  " ,    0 },
    {/*.   65     .*/    66 ,    61 , "I  " ,    0 },
    {/*.   66     .*/    67 ,    65 , "N  " ,    0 },
    {/*.   67     .*/    68 ,    66 , "I  " ,    0 },
    {/*.   68     .*/    69 ,    67 , "T  " ,    0 },
    {/*.   69     .*/    70 ,    68 , "(  " ,    0 },
    {/*.   70     .*/    71 ,    69 , "LIT" ,  211 },
    {/*.   71     .*/    72 ,    70 , ")  " ,    0 },
    {/*.   72     .*/    73 ,    71 , ";  " ,    0 },
    {/*.   73     .*/   186 ,    72 , "ODC" ,    0 },
     /*                                               input with the symbol - E      */
    {/*.   74     .*/    75 ,     0 , "E  " ,    0 },
    {/*.   75     .*/    76 ,    74 , "N  " ,   82 },
    {/*.   76     .*/    77 ,    75 , "D  " ,    0 },
    {/*.   77     .*/    78 ,    76 , "   " ,    0 },
    {/*.   78     .*/    79 ,    77 , "IPR" ,    0 },
    {/*.   79     .*/    80 ,    78 , ";  " ,    0 },
    {/*.   80     .*/    81 ,    79 , "OEN" ,    0 },
    {/*.   81     .*/     0 ,    80 , "*  " ,    0 },
    {/*.   82     .*/    83 ,    74 , "BUK" ,    0 },
    {/*.   83     .*/     0 ,    82 , "*  " ,    0 },
     /*                                               input with the symbol - M      */
    {/*.   84     .*/    85 ,     0 , "M  " ,    0 },
    {/*.   85     .*/    86 ,    84 , "BUK" ,    0 },
    {/*.   86     .*/     0 ,    85 , "*  " ,    0 },
     /*                                               input with the symbol - P      */
    {/*.   87     .*/    88 ,     0 , "P  " ,    0 },
    {/*.   88     .*/    89 ,    87 , "BUK" ,    0 },
    {/*.   89     .*/     0 ,    88 , "*  " ,    0 },
     /*                                               input with the symbol - X      */
    {/*.   90     .*/    91 ,     0 , "X  " ,    0 },
    {/*.   91     .*/    92 ,    90 , "BUK" ,    0 },
    {/*.   92     .*/     0 ,    91 , "*  " ,    0 },
     /*                                               input with the symbol - BUK    */
    {/*.   93     .*/    94 ,     0 , "BUK" ,    0 },
    {/*.   94     .*/    95 ,    93 , "IDE" ,  223 },
    {/*    95     .*/     0 ,    94 , "*  " ,    0 },
     /*                                               input with the symbol - IDE    */
    {/*.   96     .*/    97 ,     0 , "IDE" ,    0 },
    {/*.   97     .*/    98 ,    96 , "BUK" ,  100 },
    {/*.   98     .*/    99 ,    97 , "IDE" ,    0 },
    {/*.   99     .*/     0 ,    98 , "*  " ,    0 },
    {/*.  100     .*/   101 ,    96 , "CIF" ,  103 },
    {/*.  101     .*/   102 ,   100 , "IDE" ,    0 },
    {/*.  102     .*/     0 ,   101 , "*  " ,    0 },
    {/*.  103     .*/   104 ,    96 , "IPE" ,  105 },
    {/*.  104     .*/     0 ,   103 , "*  " ,    0 },
    {/*.  105     .*/   106 ,    96 , "IPR" ,    0 },
    {/*.  106     .*/     0 ,   105 , "*  " ,    0 },
     /*                                               input with the symbol - +      */
    {/*.  107     .*/   108 ,     0 , "+  " ,    0 },
    {/*.  108     .*/   109 ,   107 , "ZNK" ,    0 },
    {/*.  109     .*/     0 ,   108 , "*  " ,    0 },
     /*                                               input with the symbol - -      */
    {/*.  110     .*/   111 ,     0 , "-  " ,    0 },
    {/*.  111     .*/   112 ,   110 , "ZNK" ,    0 },
    {/*.  112     .*/     0 ,   111 , "*  " ,    0 },
     /*                                               input with the symbol - IPR    */
    {/*.  113     .*/   114 ,     0 , "IPR" ,    0 },
    {/*.  114     .*/   115 ,   113 , ":  " ,    0 },
    {/*.  115     .*/   116 ,   114 , "P  " ,    0 },
    {/*.  116     .*/   117 ,   115 , "R  " ,    0 },
    {/*.  117     .*/   118 ,   116 , "O  " ,    0 },
    {/*.  118     .*/   119 ,   117 , "C  " ,    0 },
    {/*.  119     .*/   120 ,   118 , "   " ,    0 },
    {/*.  120     .*/   121 ,   119 , "O  " ,    0 },
    {/*.  121     .*/   122 ,   120 , "P  " ,    0 },
    {/*.  122     .*/   123 ,   121 , "T  " ,    0 },
    {/*.  123     .*/   124 ,   122 , "I  " ,    0 },
    {/*.  124     .*/   125 ,   123 , "O  " ,    0 },
    {/*.  125     .*/   126 ,   124 , "N  " ,    0 },
    {/*.  126     .*/   127 ,   125 , "S  " ,    0 },
    {/*.  127     .*/   128 ,   126 , "(  " ,    0 },
    {/*.  128     .*/   129 ,   127 , "M  " ,    0 },
    {/*.  129     .*/   130 ,   128 , "A  " ,    0 },
    {/*.  130     .*/   131 ,   129 , "I  " ,    0 },
    {/*.  131     .*/   132 ,   130 , "N  " ,    0 },
    {/*.  132     .*/   133 ,   131 , ")  " ,    0 },
    {/*.  133     .*/   134 ,   132 , ";  " ,    0 },
    {/*.  134     .*/   135 ,   133 , "OPR" ,    0 },
    {/*.  135     .*/     0 ,   134 , "*  " ,    0 },
     /*                                                input with the symbol - CIF    */
    {/*.  136     .*/   137 ,     0 , "CIF" ,    0 },
    {/*.  137     .*/   138 ,   136 , "RZR" ,  223 },
    {/*.  138     .*/     0 ,   137 , "*  " ,    0 },
     /*                                                input with the symbol - RZR    */
    {/*.  139     .*/   140 ,     0 , "RZR" ,    0 },
    {/*.  140     .*/   141 ,   139 , "CIF" ,    0 },
    {/*.  141     .*/   142 ,   140 , "RZR" ,    0 },
    {/*.  142     .*/     0 ,   141 , "*  " ,    0 },
     /*                                                input with the symbol - MAN    */
    {/*.  143     .*/   144 ,     0 , "MAN" ,    0 },
    {/*.  144     .*/   145 ,   143 , "B  " ,  147 },
    {/*.  145     .*/   146 ,   144 , "LIT" ,    0 },
    {/*.  146     .*/     0 ,   145 , "*  " ,    0 },
    {/*.  147     .*/   148 ,   143 , "0  " ,  150 },
    {/*.  148     .*/   149 ,   147 , "MAN" ,    0 },
    {/*.  149     .*/     0 ,   148 , "*  " ,    0 },
    {/*.  150     .*/   151 ,   143 , "1  " ,    0 },
    {/*.  151     .*/   152 ,   150 , "MAN" ,    0 },
    {/*.  152     .*/     0 ,   151 , "*  " ,    0 },
     /*                                                input with the symbol - IPE    */
    {/*.  153     .*/   154 ,     0 , "IPE" ,    0 },
    {/*.  154     .*/   155 ,   153 , "=  " ,  159 },
    {/*.  155     .*/   156 ,   154 , "AVI" ,    0 },
    {/*.  156     .*/   157 ,   155 , ";  " ,    0 },
    {/*.  157     .*/   158 ,   156 , "OPA" ,    0 },
    {/*.  158     .*/     0 ,   157 , "*  " ,    0 },
    {/*.  159     .*/   160 ,   153 , "AVI" ,    0 },
    {/*.  160     .*/     0 ,   159 , "*  " ,    0 },
     /*                                                input with the symbol - LIT    */
    {/*.  161     .*/   162 ,     0 , "LIT" ,    0 },
    {/*.  162     .*/   163 ,   161 , "AVI" ,    0 },
    {/*.  163     .*/     0 ,   162 , "*  " ,    0 },
     /*.                                               input with the symbol - AVI    */
    {/*.  164     .*/   165 ,     0 , "AVI" ,    0 },
    {/*.  165     .*/   166 ,   164 , "ZNK" ,  229 },
    {/*.  166     .*/   167 ,   165 , "LIT" ,  168 },
    {/*.  167     .*/   197 ,   166 , "AVI" ,    0 },
    {/*.  168     .*/   169 ,   165 , "IPE" ,    0 },
    {/*.  169     .*/   170 ,   168 , "AVI" ,    0 },
    {/*.  170     .*/     0 ,   169 , "*  " ,    0 },
     /*                                                input with the symbol - OPR    */
    {/*.  171     .*/   172 ,     0 , "OPR" ,    0 },
    {/*.  172     .*/   173 ,   171 , "TEL" ,    0 },
    {/*.  173     .*/   174 ,   172 , "OEN" ,    0 },
    {/*.  174     .*/   175 ,   173 , "PRO" ,    0 },
    {/*.  175     .*/     0 ,   174 , "*  " ,    0 },
     /*.                                               input with the symbol - ODC    */
    {/*.  176     .*/   177 ,     0 , "ODC" ,    0 },
    {/*.  177     .*/   178 ,   176 , "TEL" ,    0 },
    {/*.  178     .*/     0 ,   177 , "*  " ,    0 },
     /*.                                               input with the symbol - TEL    */
    {/*.  179     .*/   180 ,     0 , "TEL" ,    0 },
    {/*.  180     .*/   181 ,   179 , "ODC" ,  183 },
    {/*.  181     .*/   182 ,   180 , "TEL" ,    0 },
    {/*.  182     .*/     0 ,   181 , "*  " ,    0 },
    {/*.  183     .*/   184 ,   179 , "OPA" ,    0 },
    {/*.  184     .*/   185 ,   183 , "TEL" ,    0 },
    {/*.  185     .*/     0 ,   184 , "*  " ,    0 },
    {/*.  186     .*/     0 ,    73 , "*  " ,    0 },
    {/*.  187     .*/   188 ,    49 , "C  " ,    0 },
    {/*.  188     .*/   189 ,   187 , "H  " ,    0 },
    {/*.  189     .*/   190 ,   188 , "A  " ,    0 },
    {/*.  190     .*/   191 ,   189 , "R  " ,    0 },
    {/*.  191     .*/   192 ,   190 , "(  " ,    0 },
    {/*.  192     .*/   193 ,   191 , "RZR" ,    0 },
    {/*.  193     .*/   194 ,   192 , ")  " ,    0 },
    {/*.  194     .*/   195 ,   193 , ";  " ,   65 },
    {/*.  195     .*/   196 ,   194 , "ODC" ,    0 },
    {/*.  196     .*/     0 ,   195 , "*  " ,    0 },
    {/*.  197     .*/     0 ,   166 , "*  " ,    0 },
    {/*.  198     .*/   199 ,     0 , "*  " ,    0 },
    {/*.  199     .*/   200 ,   198 , "ZNK" ,    0 },
    {/*.  200     .*/     0 ,   199 , "*  " ,    0 },
/*                                                      input with the symbol - L */
    {/*.  201     .*/   202 ,   0   , "L  " ,    0 },
    {/*.  202     .*/   203 ,   201 , "BUK" ,    0 },
    {/*.  203     .*/     0 ,   202 , "*  " ,    0 },
/*                                                      input with the symbol - STC */
    {/*.  204     .*/   205 ,     0 , "STC" ,    0 },
    {/*.  205     .*/   206 ,   204 , "BUK" ,  208 },
    {/*.  206     .*/   207 ,   205 , "STC" ,    0 },
    {/*.  207     .*/     0 ,   206 , "*  " ,    0 },
    {/*.  208     .*/   209 ,   204 , "CIF" ,    0 },
    {/*.  209     .*/   210 ,   208 , "STC" ,    0 },
    {/*.  210     .*/     0 ,   209 , "*  " ,    0 },

    {/*.  211     .*/   212 ,     0 , "'  " ,    0 },
    {/*.  212     .*/   213 ,   211 , "'  " ,  217 },
    {/*.  213     .*/   214 ,   212 , ")  " ,    0 },
    {/*.  214     .*/   215 ,   213 , ";  " ,    0 },
    {/*.  215     .*/   216 ,   214 , "ODC" ,    0 },
    {/*.  216     .*/     0 ,   215 , "*  " ,    0 },
    {/*.  217     .*/   218 ,   211 , "STC" ,    0 },
    {/*.  218     .*/   219 ,   217 , "'  " ,    0 },
    {/*.  219     .*/   220 ,   218 , ")  " ,    0 },
    {/*.  220     .*/   221 ,   219 , ";  " ,    0 },
    {/*.  221     .*/   222 ,   220 , "ODC" ,    0 },
    {/*.  222     .*/     0 ,   221 , "*  " ,    0 },
    {/*.  223     .*/   224 ,    93 , "STC" ,    0 },
    {/*.  224     .*/     0 ,   223 , "*  " ,    0 },
/*                                                      input with the symbol - ! */
    {/*.  225     .*/   226 ,     0 , "!  " ,    0 },
    {/*.  226     .*/   227 ,   225 , "!  " ,    0 },
    {/*.  227     .*/   228 ,   226 , "CON" ,    0 },
    {/*.  228     .*/     0 ,   227 , "*  " ,    0 },

    {/*.  229     .*/   230 ,   164 , "CON" ,    0 },
    {/*.  230     .*/   231 ,   229 , "IPE" ,    0 },
    {/*.  231     .*/   232 ,   230 , "AVI" ,    0 },
    {/*.  232     .*/     0 ,   231 , "*  " ,    0 }
};


/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
vxod_t VXOD[NVXOD] = {
/*    ___________ ___________ _____ ______
     |    NN     |    symbol |input| type |
     |___________|___________|_____|______|                                  */
    {/*.   1     .*/   "AVI" , 164 , 'N' },
    {/*.   2     .*/   "BUK" ,  93 , 'N' },
    {/*.   3     .*/   "CIF" , 136 , 'N' },
    {/*.   4     .*/   "IDE" ,  96 , 'N' },
    {/*.   5     .*/   "IPE" , 153 , 'N' },
    {/*.   6     .*/   "IPR" , 113 , 'N' },
    {/*.   7     .*/   "LIT" , 161 , 'N' },
    {/*.   8     .*/   "MAN" , 143 , 'N' },
    {/*.   9     .*/   "ODC" , 176 , 'N' },
    {/*.  10     .*/   "OEN" ,   0 , 'N' },
    {/*.  11     .*/   "OPA" ,   0 , 'N' },
    {/*.  12     .*/   "OPR" , 171 , 'N' },
    {/*.  13     .*/   "PRO" ,   0 , 'N' },
    {/*.  14     .*/   "RZR" , 139 , 'N' },
    {/*.  15     .*/   "TEL" , 179 , 'N' },
    {/*.  16     .*/   "ZNK" ,   0 , 'N' },
    {/*.  17     .*/   "STC" , 204 , 'N' },
    {/*.  18     .*/   "CON" ,   0 , 'N' },
    {/*.  19     .*/   "A  " ,  33 , 'T' },
    {/*.  20     .*/   "B  " ,  36 , 'T' },
    {/*.  21     .*/   "C  " ,  39 , 'T' },
    {/*.  22     .*/   "D  " ,  42 , 'T' },
    {/*.  23     .*/   "E  " ,  74 , 'T' },
    {/*.  24     .*/   "M  " ,  84 , 'T' },
    {/*.  25     .*/   "P  " ,  87 , 'T' },
    {/*.  26     .*/   "X  " ,  90 , 'T' },
    {/*.  27     .*/   "0  " ,   1 , 'T' },
    {/*.  28     .*/   "1  " ,   4 , 'T' },
    {/*.  29     .*/   "2  " ,   9 , 'T' },
    {/*.  30     .*/   "3  " ,  12 , 'T' },
    {/*.  31     .*/   "4  " ,  15 , 'T' },
    {/*.  32     .*/   "5  " ,  18 , 'T' },
    {/*.  33     .*/   "6  " ,  21 , 'T' },
    {/*.  34     .*/   "7  " ,  24 , 'T' },
    {/*.  35     .*/   "8  " ,  27 , 'T' },
    {/*.  36     .*/   "9  " ,  30 , 'T' },
    {/*.  37     .*/   "+  " , 107 , 'T' },
    {/*.  38     .*/   "-  " , 110 , 'T' },
    {/*.  39     .*/   ":  " ,   0 , 'T' },
    {/*.  40     .*/   "I  " ,   0 , 'T' },
    {/*.  41     .*/   "R  " ,   0 , 'T' },
    {/*.  42     .*/   "N  " ,   0 , 'T' },
    {/*.  43     .*/   "O  " ,   0 , 'T' },
    {/*.  44     .*/   "T  " ,   0 , 'T' },
    {/*.  45     .*/   "S  " ,   0 , 'T' },
    {/*.  46     .*/   "(  " ,   0 , 'T' },
    {/*.  47     .*/   ")  " ,   0 , 'T' },
    {/*.  48     .*/   "   " ,   0 , 'T' },
    {/*.  49     .*/   ";  " ,   0 , 'T' },
    {/*.  50     .*/   "L  " , 201 , 'T' },
    {/*.  51     .*/   "F  " ,   0 , 'T' },
    {/*.  52     .*/   "=  " ,   0 , 'T' },
    {/*.  53     .*/   "H  " ,   0 , 'T' },
    {/*.  54     .*/   "*  " , 198 , 'T' },
/**********************************************/
    {/*.  55     .*/   "'  " ,   0 , 'T' },
    {/*.  56     .*/   "!  " , 225 , 'T' }
};

/* Adjacency matrix which will become reachability matrix afterward */
char TPR[NVXOD][NNETRM] = {
/*   __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
    |       AVI:BUK:CIF:IDE:IPE:IPR:LIT:MAN:ODC:OEN:OPA:OPR:PRO:RZR:TEL:ZNK:STC|CON|
    |__________:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___|___| */
    {/*AVI*/ 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*BUK*/ 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 },
    {/*CIF*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 0 },
    {/*IDE*/ 0 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*IPE*/ 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*IPR*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*LIT*/ 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*MAN*/ 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*ODC*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 },
    {/*OEN*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*OPA*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*OPR*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 },
    {/*PRO*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*RZR*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 },
    {/*TEL*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 },
    {/*ZNK*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*STC*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 },
    {/*CON*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    /*
     __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
    |       AVI:BUK:CIF:IDE:IPE:IPR:LIT:MAN:ODC:OEN:OPA:OPR:PRO:RZR:TEL:ZNK:STC|CON|
    |__________:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___|___| */
    {/*  A*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  B*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  C*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  D*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  E*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  M*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  P*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  X*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  0*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  1*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  2*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  3*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  4*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  5*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 },
    {/*  6*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 },
    {/*  7*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
     /*
     __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
    |       AVI:BUK:CIF:IDE;IPE:IPR:LIT:MAN:ODC:OEN:OPA:OPR:PRO:RZR:TEL:ZNK:STC|CON|
    |__________:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___|___| */
    {/*  8*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  9*/ 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  +*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 },
    {/*  -*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 },
    {/*  :*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  I*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  R*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  N*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  O*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  T*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  S*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  (*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  )*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  ;*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*   */ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  L*/ 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
     /*
     __________ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
    |       AVI:BUK:CIF:IDE:IPE:IPR:LIT:MAN:ODC:OEN;OPA:OPR:PR0:RZR:TEL:ZNK:STC|CON|
    |__________:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___:___|___| */
    {/*  F*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  =*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  H*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  **/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 },
    {/*  '*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {/*  !*/ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 }
    /*|____________________________________________________________________________| */
};

/* Function finds necessary string with sought-for symbol in the VXOD-table 
 * and returns string-index of VXOD-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
unsigned int plcmp_tables_sym_sint_ind(char const *p_str_symbol, unsigned int symbol_str_len)
{
    unsigned int i;
    for (i = 0; i < NVXOD; i++)
    {
        unsigned int k = 0;
        /* Comparing each symbol each other */
        while((k != symbol_str_len) && 
              (p_str_symbol[k] == VXOD[i].SYM[k]))
        {
            ++k;
        }
        if ((k == symbol_str_len) &&
            (VXOD[i].SYM[k] == '\0' || VXOD[i].SYM[k] == ' '))
        {
            /* We found necessary string with sought-for symbol in the VXOD-table */
            break;
        }
        continue;
    }

    return NVXOD == i ? -1 : i;
}
