#ifndef JPASTE_H
#define JPASTE_H 1

#define JPASTEx(a, b) a##b
#define JPASTE3x(a, b, c) a##b##c
#define JPASTE(a, b) JPASTEx(a, b)
#define JPASTE3(a, b, c) JPASTE3x(a, b, c)

#endif