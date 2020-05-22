#include <iostream>

namespace Asm {
    char arg[100] = "";

    void FileHead(FILE *asmFile) {
        fprintf(asmFile, ""
                         "format ELF64 executable 3\n"
                         "entry start\n");
    }

    void Label(FILE *asmFile, const char *label) {
        fprintf(asmFile, "%s:\n", label);
    }

    void ImplicitLabelInit(FILE *asmFile, const int num) {
        fprintf(asmFile, ".L%d:\n", num);
    }

    void Include(FILE *asmFile, const char *name) {
        fprintf(asmFile, "include '%s'\n", name);
    }

    void Printf(FILE *asmFile, const char *name) {
        fprintf(asmFile, "    stdcall     printf, outInt,  %s\n", name);
    }

    void Scanf(FILE *asmFile) {
        fprintf(asmFile, "    call        scanf\n");
    }

    namespace segments {
        void SegmentRE(FILE *asmFile) {
            fprintf(asmFile, ""
                             "segment readable executable\n");
        }
    }

    namespace registers {
        char rax[] = "rax";
        char rbx[] = "rbx";
        char rdx[] = "rdx";
        char r0[] = "r0";

        char eax[] = "eax";
        char edx[] = "edx";

        char rbp[] = "rbp";
        char rsp[] = "rsp";
    }

    namespace commands {
        void Mov(FILE *asmFile, const char *arg1,const char *arg2) {
            fprintf(asmFile, "    mov         %s, %s\n", arg1, arg2);
        }

        void Int(FILE *asmFile, const char *arg1) {
            fprintf(asmFile, "    int         %s\n", arg1);
        }

        void Call(FILE *asmFile, const char *arg1) {
            fprintf(asmFile, "    call        %s\n", arg1);
        }


        void Ret(FILE *asmFile) {
            fprintf(asmFile, "    ret\n");
        }

        void Cmp(FILE *asmFile, const char *arg1,const char *arg2) {
            fprintf(asmFile, "    cmp         %s, %s\n", arg1, arg2);
        }
    }

    namespace binary {
        void Xor(FILE *asmFile, const char *arg1, const char *arg2) {
            fprintf(asmFile, "    xor         %s, %s\n", arg1, arg2);
        }
    }

    namespace math {
        void Add(FILE *asmFile, const char *arg1, const char *arg2) {
            fprintf(asmFile, "    add         %s, %s\n", arg1, arg2);
        }

        void Sub(FILE *asmFile, const char *arg1, const char *arg2) {
            fprintf(asmFile, "    sub         %s, %s\n", arg1, arg2);
        }

        void Imul(FILE *asmFile, const char *arg1, const char *arg2) {
            fprintf(asmFile, "    imul        %s, %s\n", arg1, arg2);
        }

        void Idiv(FILE *asmFile, const char *dest, const char *arg1, const char *arg2) {
            fprintf(asmFile, "    idiv        %s, %s, %s\n", dest, arg1, arg2);
        }
    }

    namespace jumps {
        int ja = 1;
        int jna = 2;
        int jb = 3;
        int jnb = 4;
        int je = 5;
        int jne = 6;

        void Jmp(FILE *asmFile, const char *ptr) {
            fprintf(asmFile, "    jmp         %s\n", ptr);
        }

        void JCond(FILE *asmFile, const char *ptr, int funcNum) {
            switch (funcNum) {
                case 1:
                    fprintf(asmFile, "    ja          %s\n", ptr);
                    break;

                case 2:
                    fprintf(asmFile, "    jna         %s\n", ptr);
                    break;

                case 3:
                    fprintf(asmFile, "    jb          %s\n", ptr);
                    break;

                case 4:
                    fprintf(asmFile, "    jnb         %s\n", ptr);
                    break;

                case 5:
                    fprintf(asmFile, "    je          %s\n", ptr);
                    break;

                case 6:
                    fprintf(asmFile, "    jne         %s\n", ptr);
                    break;

                default:
                    fprintf(asmFile, "    nop\n");
            }
        }
    }

    namespace stack {
        int ceil = 8;

        void Push(FILE *asmFile, const char *arg1) {
            fprintf(asmFile, "    push        %s\n", arg1);
        }

        void Pop(FILE *asmFile, const char *arg1) {
            fprintf(asmFile, "    pop         %s\n", arg1);
        }
    }

    char *Expression(const char arg1[], const char operand, const int num) {
        if (operand == '+' && num == 0)
            sprintf(arg, "%s", arg1);
        else
            sprintf(arg, "%s %c %d", arg1, operand, num);

        return arg;
    }

    char *Expression(const int arg1, const char operand, const int arg2) {
        sprintf(arg, "%d %c %d", arg1, operand, arg2);

        return arg;
    }

    char *PtrExpression(const char arg1[], const char operand, const int num) {
        if (operand == '+' && num == 0)
            sprintf(arg, "[%s]", arg1);
        else
            sprintf(arg, "[%s %c %d]", arg1, operand, num);
        return arg;
    }

    char *ImplicitLabel(const int num) {
        sprintf(arg, ".L%d", num);
        return arg;
    }
}