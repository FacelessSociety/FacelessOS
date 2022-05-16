#ifndef INTERPRETER_H
#define INTERPRETER_H

// 2022 Ian Moffett <ian@kesscoin.com>

typedef enum {
    CT_ECHO,
    CT_REBOOT,
    CT_INVALID
} SHELL_COMMAND_TYPE;


struct Command {
    SHELL_COMMAND_TYPE command;
    const char* operand;
};


struct Command shell_interp_process(char* buffer);



#endif