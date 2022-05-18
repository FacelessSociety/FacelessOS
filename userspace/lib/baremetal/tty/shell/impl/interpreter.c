#include <lib/baremetal/tty/shell/interpreter.h>
#include <lib/libstring/string.h>
#include <stdint.h>

// 2022 Ian Moffett <ian@kesscoin.com>


#define WORD_END(C) (C == ' ' || C == '\0')
#define MAX_KW_SIZE 15                          // Max keyword length.
#define MAX_OPERAND_SIZE 30


// NOTE: Re-calling will overwrite string.
static char* get_command(char* str) {
    static char word[MAX_KW_SIZE];
    _memzero(word, MAX_OPERAND_SIZE + 1);
    word[0] = '\0';

    for (size_t i = 0; i < _strlen(str) && i < MAX_KW_SIZE; ++i) {
        if (WORD_END(str[i])) break;
        word[i] = str[i];
    }

    return word;
}


static char* get_operand(char* str) {
    size_t index_continue = 0;
    size_t word_idx = 0;
    static char word[MAX_OPERAND_SIZE + 1];
    _memzero(word, MAX_OPERAND_SIZE + 1);

    for (size_t i = 0; i < _strlen(str); ++i) {
        if (str[i] == ' ') {
            index_continue = i + 1;
            break;
        }
    }

    for (size_t i = index_continue; i < _strlen(str) && i < MAX_OPERAND_SIZE && str[i] != '\0'; ++i) {
        word[word_idx++] = str[i];
    }

    return word;
}


// COMMAND/OPERAND SHOULD NOT BE STORED AFTER ANOTHER CALL TO CMD_OP_SEP().
static struct Command cmd_op_sep(char* buffer) {
    char* command = get_command(buffer);
    char* operand = get_operand(buffer);
    struct Command cmd;

    if (_strncmp(command, "echo", _strlen(command)))
        cmd.command = CT_ECHO;
    else if (_strncmp(command, "reboot", _strlen(command)))
        cmd.command = CT_REBOOT;
    else if (_strncmp(command, "date", _strlen(command)))
        cmd.command = CT_DATE;
    else if (_strncmp(command, "clear", _strlen(command)))
        cmd.command = CT_CLEAR;
    else if (_strncmp(command, "spktest", _strlen(command)))
        cmd.command = CT_PCSPKR_TOGGLE;
    else if (_strncmp(command, "piano", _strlen(command)))
        cmd.command = CT_PIANO;
    else if (_strncmp(command, "dmppci", _strlen(command)))
        cmd.command = CT_DMPPCI;
    else
        cmd.command = CT_INVALID;

    cmd.operand = operand;
    return cmd;
}



struct Command shell_interp_process(char* buffer) {
    return cmd_op_sep(buffer);
}
