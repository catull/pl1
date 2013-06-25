/* encoding: UTF-8 */

#ifndef ASMCMP_COMMON_H
#define ASMCMP_COMMON_H

#include <assert.h>
#include <stdint.h>

#define DL_ASSTEXT 50
#define DL_OBJTEXT 50                             /*длина об'ектн. текста   */
#define NSYM 10                                   /*размер табл.символов    */
#define NPOP 6                                    /*размер табл.псевдоопер. */
#define NOP 10                                    /*размер табл.операций    */

/* The limited length of the line of the assembler source text 
 * It has to be 81 (80 + 1) because 'fread' function reads 
 * newline-symbol too in the end of the each line of assembler code read */
#define LINELEN 81

#define TRUE 1
#define FALSE 0

/* Enumerate with types of machine operations */
typedef enum oper_type_e {
    /* RR (Register-Register) */
    OPER_RR,
    /* RX (Register-Memory) */
    OPER_RX,
    /* SS (String-String) */
    OPER_SS
} oper_type_t;

/* Structure describes content
 * of the RR-operation type
 * - opcode - operation code
 * - R1R2 - byte containing numbers of 
 * the first destination register (elder tetrad)
 * and the second source register (lower tetrad)
 *
 * Operation's length is 2 bytes (16 bits)
 */
typedef struct oper_rr_s {
    uint8_t opcode;
    uint8_t R1R2;
} oper_rr_t;

/* Structure describes content
 * of the RX-operation type
 * - opcode - operation code
 * - R1X2 - byte containing number of 
 * the destination register (elder tetrad) as the first operand
 * and index value (lower tetrad) of the second operand
 * - B2D2 - double-byte containing number of
 * the base register (elder tetrad) of the second operand
 * and 12-bit length value of the address' displacement 
 * of the second operand
 *
 * Operation's length is 4 bytes (32 bits)
 */
typedef struct oper_rx_s {
    uint8_t opcode;
    uint8_t R1X2;
    uint16_t B2D2;
} oper_rx_t;

/* Structure describes content
 * of the SS-operation type
 * - opcode - operation code
 * - L - byte containing length of the operands
 * - B1D1 - double-byte containing number of
 * the base register (elder tetrad) of the first operand
 * and 12-bit length value of the address' displacement 
 * of the first operand 
 * - B2D2 - double-byte containing number of
 * the base register (elder tetrad) of the second operand
 * and 12-bit length value of the address' displacement 
 * of the second operand
 *
 * Operation's length is 6 bytes (48 bits)
 */
typedef struct oper_ss_s {
    uint8_t opcode;
    uint8_t L;
    uint16_t B1D1;
    uint16_t B2D2;
} oper_ss_t;

typedef struct oper_s {
    oper_type_t oper_type;
    union {
        oper_rr_t rr;
        oper_rx_t rx;
        oper_ss_t ss;
    } oper;
} oper_t;


typedef enum data_type_e {
    DATA_FIXED_BIN,
    DATA_STRING
} data_type_t;

typedef struct string_data_s {
    size_t str_length;
    uint8_t *p_string;
} string_data_t;

typedef struct data_s {
    data_type_t data_type;
    union {
        uint32_t fixed_bin;
        string_data_t string_data;
    } data;
} data_t;


#define ASMCMP_COMMON_ASSERT(condition) assert(condition)

/*
 * Macro allocates memory for new 'p_str_for' string with 'str_length' length
 *
 * @param1:
 * 'p_str_for' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'str_length' has type 'size_t'
 */
#define ASMCMP_COMMON_MALLOC_MEM_FOR_STR(p_str_for, str_length)                 \
    do {                                                                        \
        size_t __str_length = (str_length);                                     \
                                                                                \
        p_str_for = malloc(sizeof(char)*(__str_length + 1));                    \
        if (NULL == p_str_for)                                                  \
        {                                                                       \
            printf("Error of allocating memory for new string. Assert\n");      \
            ASMCMP_COMMON_ASSERT(0);                                            \
        }                                                                       \
        p_str_for[__str_length] = '\0';                                         \
    } while(0)

/* 
 * Macro reasonably allocates memory for new 'p_fp_str_to' string of file path
 * and copies 'p_fp_str_from' string to
 *
 * @param1:
 * 'p_fp_str_to' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_fp_str_from' has type 'char*' or 'char const*'
 *
 */
#define ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)              \
    do {                                                                                 \
        ASMCMP_COMMON_MALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));            \
        strcpy(p_fp_str_to, p_fp_str_from);                                              \
    } while(0)

/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type
 */
#define ASMCMP_COMMON_RELEASE_MEM(pointer)      \
    do {                                        \
        if (pointer)                            \
        {                                       \
            free(pointer);                      \
            pointer = NULL;                     \
        }                                       \
    } while(0)

void asmcmp_common_save_oper_tex_card(oper_t oper);
void asmcmp_common_save_data_tex_card(data_t data);

void asmcmp_common_swap_bytes(uint8_t *p_src, uint8_t *p_dst, size_t count_bytes);
void asmcmp_common_print_oper(oper_t oper);

#endif