/* encoding: UTF-8 */

#ifndef ABSLOAD_MACHINE_OPER_H
#define ABSLOAD_MACHINE_OPER_H

/* Lengths of the machine operations in bytes */
#define OPER_RR_LEN 2
#define OPER_RX_LEN 4
#define OPER_SS_LEN 6
/* Maximum length of operation */
#define OPER_MAX_LEN OPER_SS_LEN

typedef enum absload_machine_oper_error_code_e {
    ABSLOAD_MACHINE_OPER_SUCCESS = 0,
    ABSLOAD_MACHINE_OPER_WRONG_ADDR_ALIGMENT_ERROR
} absload_machine_oper_error_code_t;

/* Structure describes type of 
 * table of machine operations */
typedef struct machine_operations_table_s {
    char MNCOP[5];
    unsigned char CODOP;
    unsigned char DLOP;
    enum absload_machine_oper_error_code_e (*BXPROG)(void);
} machine_operations_table_t;

int P_BALR(void);
int P_BCR(void);
int P_ST(void);
int P_L(void);
int P_A(void);
int P_S(void);
int P_LR(void);
int P_LA(void);
int P_AR(void);
int P_MVC(void);

#endif /* ABSLOAD_MACHINE_OPER_H */