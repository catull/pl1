#include <string.h>

#include "plcmp_common.h"
#include "plcmp_tables.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_synt_analyzer.h"

/* программа заполнения ячейки стека поставленных целей */
static void mcel(char *T1, int T2, int T3)
{
    strcpy(CEL[K].CEL1, T1);
    CEL[K].CEL2 = T2;
    CEL[K].CEL3 = T3;
    K++;
}

/* программа заполнения ячейки стека достигнутых целей */
static void mdst(char *T1, int T2, int T3, int T4, int T5)
{
    strcpy(DST[L].DST1, T1);
    DST[L].DST2 = T2;
    DST[L].DST3 = T3;
    DST[L].DST4 = T4;
    DST[L].DST5 = T5;
    L++;
}

/*   п р о г р а м м а    */
/*   построения  дерева   */
/* синтаксического разбора, выполняющая роль синтаксического анализатора */
int plcmp_synt_analyzer_syntax_analyzer(void)
{
    I4 = 0;

    K = 0;
    L = 0;
    I = 0;
    J = 1;


    mcel("PRO", I, 999);

    if (!TPR[numb(&compact_src_text[I], 1)][numb("PRO", 3)])
    {
        return 1;
    }

    L2:

    J = VXOD[numb(&compact_src_text[I], 1 )].VX;

    L3:

    J = SINT[J].POSL;

    L31:

    I++;

    if (I > I4)
    {
        I4 = I;
    }

    if (VXOD[numb(SINT[J].DER, 3)].TYP == 'T')
    {

        if (compact_src_text[I] == SINT[J].DER[0])
        {
            goto L3;
        }
        else
        {
            goto L8;
        }

    }

    if (SINT[SINT[J].POSL].DER[0] == '*')
    {
        I--;

        if (!strcmp(SINT[J].DER, CEL[K - 1].CEL1))
        {
            mdst(CEL[K - 1].CEL1, CEL[K - 1].CEL2, CEL[K - 1].CEL3, I, J);

            if (!strcmp(CEL[K - 1].CEL1, "PRO"))
            {
                return 0;
            }

            if (TPR[numb(CEL[K - 1].CEL1, 3)][numb(CEL[K - 1].CEL1, 3)])
            {
                J = VXOD[numb(CEL[K - 1].CEL1, 3 )].VX;
                goto L3;
            }

            L6:

            J = CEL[K - 1].CEL3;
            K--;
            goto L3;
        }

        if (!TPR[numb(SINT[J].DER, 3)][numb(CEL[K - 1].CEL1, 3)])
        {
            goto L9;
        }

        mdst(SINT[J].DER, CEL[K - 1].CEL2, 0, I, J);
        J = VXOD[numb(SINT[J].DER, 3)].VX;
        goto L3;
    }

    if (!TPR[numb(&compact_src_text[I], 1)][numb(SINT[J].DER, 3)])
    {
        goto L8;
    }

    mcel(SINT[J].DER, I, J);
    goto L2;

    L8:

    I--;

    L9:

    if (SINT[J].ALT != 0)
    {
        J = SINT[J].ALT;
        goto L31;
    }

    J = SINT[J].PRED;

    if ((VXOD[numb(SINT[J].DER, 3)].TYP == 'N' ) && (SINT[J].PRED > 0))
    {
        mcel (DST[L - 1].DST1, DST[L - 1].DST2, DST[L - 1].DST3);

        L10:

        J = DST[L - 1].DST5;
        L--;
        goto L9;
    }

    if ((VXOD[numb(SINT[J].DER, 3)].TYP == 'N' ) && (SINT[J].PRED == 0))
    {

        if (!strcmp(CEL[K - 1].CEL1, DST[L - 1].DST1))
        {
            goto L6;
        }
        else
        {
            goto L10;
        }

    }

    if (SINT[J].PRED > 0)
    {
        goto L8;
    }

    J = CEL[K - 1].CEL3;
    K--;

    if (999 == J)
    {
        return 2;
    }
    else
    {
        goto L8;
    }
}