/* encoding: UTF-8 */

#ifndef PLCMP_GOAL_H
#define PLCMP_GOAL_H

/* Structure for the stack of goals */
typedef struct goals_stack_s {
    char CEL1[4];
    int CEL2;
    int CEL3;
} goals_stack_t;

/* */
typedef struct cel_s {
    unsigned int count;
    goals_stack_t *p_cel_stack;
} cel_t;

/* Structure for the stack of goals achieved */
typedef struct goals_achieved_stack_s {
    char DST1[4];
    int DST2;
    int DST3;
    int DST4;
    int DST5; 
} goals_achieved_stack_t;

/* */
typedef struct dst_s {
    unsigned int count;
    goals_achieved_stack_t *p_dst_stack;
} dst_t;


void plcmp_goal_add(cel_t *p_goals,
                    char const *goal_name,
                    int src_text_begin_index,
                    int src_text_end_index);

void plcmp_goal_remove_last(cel_t *p_goals);

void plcmp_goal_add_achieved(dst_t *p_goals_achieved,
                             char const *goal_achieved_name,
                             int src_text_begin_index,
                             int sint_index,
                             int src_text_end_index,
                             int next_goal_sint_index);

void plcmp_goal_remove_last_achieved(dst_t *p_goals_achieved);

#endif /* PLCMP_GOAL_H */