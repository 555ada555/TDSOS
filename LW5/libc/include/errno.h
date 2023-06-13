
#pragma once

enum SYMBOLIC_ERRNO {
    EPERMISSION = 1, //Operation not permitted
    ETOOMANYTASKS = 2, //Too many tasks
    ENOTIMPLEMENTED = 3, //Operation not implemented
    EINVALIDSYSCALLNO = 4, //Syscall no is not recognized
};

extern int errno;

#define ENOSYS 1
