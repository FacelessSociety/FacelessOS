#include <tty/shell/interpreter.h>
#include <util/string.h>
#include <util/mem.h>
#include <stdint.h>

// 2022 Ian Moffett <ian@kesscoin.com>


#define WORD_END(C) (C == ' ' || C == '\0')
#define MAX_KW_SIZE 15                          // Max keyword length.
#define MAX_OPERAND_SIZE 30


// NOTE: Re-calling will overwrite string.
// @step_from goes like this:
//
//  Hello, this is a word.
//    0     1   2  3   4
static char* get_command(char* str) {
    static char word[MAX_KW_SIZE];
    memzero(word, MAX_OPERAND_SIZE + 1);
    word[0] = '\0';

    for (size_t i = 0; i < strlen(str) && i < MAX_KW_SIZE; ++i) {
        if (WORD_END(str[i])) break;
        word[i] = str[i];
    }

    return word;
}


static char* get_operand(char* str) {
    size_t index_continue = 0;
    size_t word_idx = 0;
    static char word[MAX_OPERAND_SIZE + 1];
    memzero(word, MAX_OPERAND_SIZE + 1);

    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] == ' ') {
            index_continue = i + 1;
            break;
        }
    }

    for (size_t i = index_continue; i < strlen(str) && i < MAX_OPERAND_SIZE && str[i] != '\0'; ++i) {
        word[word_idx++] = str[i];
    }

    return word;
}


// COMMAND/OPERAND SHOULD NOT BE STORED AFTER ANOTHER CALL TO CMD_OP_SEP().
static struct Command cmd_op_sep(char* buffer) {
    char* command = get_command(buffer);
    char* operand = get_operand(buffer);
    struct Command cmd;

    if (strncmp(command, "echo", strlen(command)))
        cmd.command = CT_ECHO;
    else if (strncmp(command, "reboot", strlen(command)))
        cmd.command = CT_REBOOT;
    else if (strncmp(command, "date", strlen(command)))
        cmd.command = CT_DATE;
    else if (strncmp(command, "clear", strlen(command)))
        cmd.command = CT_CLEAR;
    else
        cmd.command = CT_INVALID;

    cmd.operand = operand;
    return cmd;
}



struct Command shell_interp_process(char* buffer) {
    return cmd_op_sep(buffer);
}
