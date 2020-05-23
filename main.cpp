#include <iostream>
#include "vector.h"
#include "tree.h"
#include "asmCommand.h"

enum errors {
    SUCCESS = 0,
    PARSING_ERROR = 1,
};

const char *operations[] = {
        "PROGRAM_ROOT",     // 0
        "DECLARATION",      // 1
        "FUNCTION",         // 2
        "INITIALIZE",       // 3
        "VARLIST",          // 4
        "OP",               // 5
        "IF",               // 6
        "C",                // 7
        "RETURN",           // 8
        "ABOVE",            // 9
        "BELOW",            // 10
        "OUTPUT",           // 11
        "INPUT",            // 12
        "ASSIGNMENT",       // 13
        "CALL",             // 14
        "BLOCK",            // 15
        "EQUAL",            // 16
        "WHILE",            // 17
        "ADD",              // 18
        "SUB",              // 19
        "MUL",              // 20
        "DIV",              // 21
        "SQR",              // 22
};

Vector <int> nameTable = {};

//const int memoryForFunc = 10;
int implicitLabels = 0;

int getAsm(tree::Tree <char *> &langTree, FILE *asmFile);

int getNameTable(tree::Tree <char *> &langTree, FILE *asmFile);

int getDeclaration(tree::Node <char *> *node, FILE *asmFile, bool setTable = false);

int getFunction(tree::Node<char *> *node, FILE *asmFile, bool setTable = false);

int getFuncId(tree::Node<char *> *node, FILE *asmFile, int *varCount = nullptr, bool setTable = false);

int getVarList(tree::Node<char *> *node, FILE *asmFile, int *varCount = nullptr, bool setTable = false);

int getVarId(tree::Node<char *> *node, FILE *asmFile, int *varCount = nullptr, bool setTable = false);

int getBlock(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr, bool setTable = false);

int getOp(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr, bool setTable = false);

int getInitialize(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr, bool setTable = false);

int getAssign(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getCall(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getCallArgs(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, int pos);

int getExpression(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

void expressionPrint(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getInput(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getOutput(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getIf(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr, bool setTable = false);

int getWhile(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr, bool setTable = false);

int getCond(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getReturn(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount = nullptr);

int getArgId(tree::Node<char *> *node, FILE *asmFile, int *argCount = nullptr, bool setTable = false);

int main(const int argc, const char *argv[]) {
    FILE *asmFile = fopen(argv[argc - 1], "w");
    tree::Tree <char *> langTree ('L', argv[argc - 2]);

    int err = getNameTable(langTree, asmFile);
    if (err)
        return err;

//    nameTable.dump();
//    langTree.dump();

    err = getAsm(langTree, asmFile);
    if (err)
        return err;

    fclose(asmFile);
    return 0;
}

int getNameTable(tree::Tree <char *> &langTree, FILE *asmFile) {
    if (!strcmp(langTree.getRoot()->rightChild->value, operations[1]))
        getDeclaration(langTree.getRoot()->rightChild, asmFile, true);
    else {
        printf("Parsing error: await %s after %s", operations[1], operations[0]);
        return PARSING_ERROR;
    }

    return SUCCESS;
}

int getAsm(tree::Tree <char *> &langTree, FILE *asmFile) {
    if (langTree.getRoot() && !strcmp(langTree.getRoot()->rightChild->value, operations[1])) {
        Asm::          FileHead(asmFile);
        Asm::          Include(asmFile, "terminalLib/printf.asm");
        Asm::          Include(asmFile, "terminalLib/scanf.asm");
        Asm::          Include(asmFile, "terminalLib/sqrt.asm");
        Asm::segments::SegmentRE(asmFile);

        Asm::          Label(asmFile, "start");
        Asm::commands::Call(asmFile, "main");
        Asm::commands::Mov(asmFile, Asm::registers::rax, "1");
        Asm::binary::  Xor(asmFile, Asm::registers::rbx, Asm::registers::rbx);
        Asm::commands::Int(asmFile, "0x80");

        getDeclaration(langTree.getRoot()->rightChild, asmFile);
    }
    else {
        printf("Parsing error: await %s after %s", operations[1], operations[0]);
        return PARSING_ERROR;
    }

    return SUCCESS;
}

int getDeclaration(tree::Node <char *> *node, FILE *asmFile, bool setTable) {

    if (node->leftChild && !strcmp(node->leftChild->value, operations[1]))
        getDeclaration(node->leftChild, asmFile, setTable);


    if (node->rightChild && !strcmp(node->rightChild->value, operations[2]))
        getFunction(node->rightChild, asmFile, setTable);
    else return PARSING_ERROR;
}

int getFunction(tree::Node<char *> *node, FILE *asmFile, bool setTable) {
    int varCount = 0;
    int funcId = 0;

    if (node->rightChild)
        funcId = getFuncId(node->rightChild, asmFile, &varCount, setTable);
    else return PARSING_ERROR;

    if (node->leftChild && setTable) {
        if (!strcmp(node->leftChild->value, operations[4])) {
            int argCount = 0;
            getVarList(node->leftChild, asmFile, &argCount, setTable);
        }
        else return PARSING_ERROR;
    }

    if (!setTable) {
        int varNum = nameTable.countFromToZero(funcId);

        Asm::stack::   Push(asmFile, Asm::registers::rbp);
        Asm::commands::Mov(asmFile, Asm::registers::rbp, Asm::registers::rsp);
        Asm::math::    Sub(asmFile, Asm::registers::rsp, Asm::Expression(varNum, '*', Asm::stack::ceil));
    }

    if (node->rightChild->rightChild && !strcmp(node->rightChild->rightChild->value, operations[15]))
        getBlock(node->rightChild->rightChild, asmFile, funcId, &varCount, setTable);
    else return PARSING_ERROR;
}

int getFuncId(tree::Node<char *> *node, FILE *asmFile, int *varCount, bool setTable) {
    if (!setTable) {
        Asm::Label(asmFile, node->value);
        return nameTable.find(node->value);
    }


    nameTable.pushBack({node->value, *varCount});
    ++(*varCount);
    return 0;
}

int getVarList(tree::Node<char *> *node, FILE *asmFile, int *varCount, bool setTable) {
    if (!setTable)
        return 0;

    if (node->rightChild)
        getArgId(node->rightChild, asmFile, varCount, setTable);
    else return PARSING_ERROR;

    if (node->leftChild && !strcmp(node->leftChild->value, operations[4]))
        getVarList(node->leftChild, asmFile, varCount, setTable);
}

int getVarId(tree::Node<char *> *node, FILE *asmFile, int *varCount, bool setTable) {
    nameTable.pushBack({node->value, *varCount});
    ++(*varCount);
}

int getArgId(tree::Node<char *> *node, FILE *asmFile, int *argCount, bool setTable) {
    ++(*argCount);
    nameTable.pushBack({node->value, -(*argCount) - 1});
}

int getBlock(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, bool setTable) {
    if (node->rightChild && !strcmp(node->rightChild->value, operations[5]))
        getOp(node->rightChild, asmFile, funcId, varCount, setTable);
    else return PARSING_ERROR;
}

int getOp(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, bool setTable) {
    if (node->rightChild) {
        if (!strcmp(node->rightChild->value, operations[3]))
            getInitialize(node->rightChild, asmFile,funcId, varCount, setTable);

        else if (!setTable && !strcmp(node->rightChild->value, operations[13]))
            getAssign(node->rightChild, asmFile, funcId, varCount);

        else if (!strcmp(node->rightChild->value, operations[6]))
            getIf(node->rightChild, asmFile, funcId, varCount, setTable);

        else if (!strcmp(node->rightChild->value, operations[17]))
            getWhile(node->rightChild, asmFile, funcId, varCount, setTable);

        else if (!setTable && !strcmp(node->rightChild->value, operations[12]))
            getInput(node->rightChild, asmFile, funcId, varCount);

        else if (!setTable && !strcmp(node->rightChild->value, operations[11]))
            getOutput(node->rightChild, asmFile,funcId, varCount);

        else if (!setTable && !strcmp(node->rightChild->value, operations[14]))
            getCall(node->rightChild, asmFile, funcId, varCount);

        else if (!setTable && !strcmp(node->rightChild->value, operations[8]))
            getReturn(node->rightChild, asmFile, funcId, varCount);

        else if (!setTable)
            return PARSING_ERROR;
    }
    else return PARSING_ERROR;

    if (node->leftChild && !strcmp(node->leftChild->value, operations[5]))
        getOp(node->leftChild, asmFile, funcId, varCount, setTable);
}

int getInitialize(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, bool setTable) {
    if (node->rightChild && setTable)
        getVarId(node->rightChild, asmFile, varCount);

    if (node->leftChild && !setTable)
        getAssign(node, asmFile, funcId, varCount);
}

int getAssign(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    int varId = 0;
    if (!strcmp(node->value, operations[3])) {
        if (node->leftChild)
            getExpression(node->leftChild, asmFile, funcId, varCount);
        else return PARSING_ERROR;

        varId = nameTable.findFrom(funcId, node->rightChild->value);
    }

    else if (!strcmp(node->value, operations[13])) {
        if (node->rightChild)
            getExpression(node->rightChild, asmFile, funcId, varCount);
        else return PARSING_ERROR;

        varId = nameTable.findFrom(funcId, node->leftChild->value);
    }

    Asm::commands::Mov(asmFile, Asm::PtrExpression(Asm::registers::rbp, '-', Asm::stack::ceil * varId), Asm::registers::rax);
}

int getCall(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    if (node->leftChild && !strcmp(node->leftChild->value, operations[4])) {
        int pos = nameTable.find(node->rightChild->value);

        getCallArgs(node->leftChild, asmFile, funcId, varCount, pos);

        Asm::commands::Call(asmFile, nameTable[pos].name);
    }
}

int getCallArgs(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, int pos) {
    if (node->leftChild && !strcmp(node->leftChild->value, operations[4]))
        getCallArgs(node->leftChild, asmFile, funcId, varCount, ++pos);

    if (node->rightChild)
        getExpression(node->rightChild, asmFile, funcId, varCount);
    else return PARSING_ERROR;

    Asm::stack::Push(asmFile, Asm::registers::rax);
}

int getExpression(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    if (isdigit(*node->value) || *node->value == '-')
        Asm::commands::Mov(asmFile, Asm::registers::rax, node->value);

    else if (!strcmp(node->value, operations[14])) {
        Asm::stack::Push(asmFile, Asm::registers::rdx);
        getCall(node, asmFile, funcId, varCount);
        Asm::stack::Pop(asmFile, Asm::registers::rdx);
    }

    else if (!strcmp(node->value, operations[18])) {
        expressionPrint(node, asmFile, funcId, varCount);
        Asm::math::Add(asmFile, Asm::registers::rax, Asm::registers::rdx);
    }

    else if (!strcmp(node->value, operations[19])) {
        expressionPrint(node, asmFile, funcId, varCount);
        Asm::math::Sub(asmFile, Asm::registers::rax, Asm::registers::rdx);
    }

    else if (!strcmp(node->value, operations[20])) {
        expressionPrint(node, asmFile, funcId, varCount);
        Asm::math::Imul(asmFile, Asm::registers::rax, Asm::registers::rdx);
    }

    else if (!strcmp(node->value, operations[21])) {
        getExpression(node->rightChild, asmFile, funcId, varCount);
        Asm::stack::Push(asmFile, Asm::registers::rax);
        getExpression(node->leftChild, asmFile, funcId, varCount);

        Asm::stack::   Pop(asmFile, Asm::registers::rcx);
        Asm::commands::Mov(asmFile, Asm::registers::rdx, Asm::registers::rax);
        Asm::binary::  Shr(asmFile, Asm::registers::rdx, 32);
        Asm::math::    Idiv(asmFile, Asm::registers::ecx);
        Asm::commands::Cdqe(asmFile);
    }

    else if (!strcmp(node->value, operations[22])) {
        getExpression(node->rightChild, asmFile, funcId, varCount);

        Asm::stack::   Push(asmFile, Asm::registers::rax);
        Asm::commands::Call(asmFile, "sqrt");
        Asm::math::Add(asmFile, Asm::registers::rsp, "8");
    }

    else if (isalpha(*node->value)) {
        int varId = nameTable.findFrom(funcId, node->value);
        Asm::commands::Mov(asmFile, Asm::registers::rax, Asm::PtrExpression(Asm::registers::rbp, '-', Asm::stack::ceil * varId));
    }
}

void expressionPrint(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    getExpression(node->rightChild, asmFile, funcId, varCount);
    Asm::stack::Push(asmFile, Asm::registers::rax);
    getExpression(node->leftChild, asmFile, funcId, varCount);
    Asm::stack::Pop(asmFile, Asm::registers::rdx);
}

int getInput(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    if (node->rightChild) {
        int varId = nameTable.findFrom(funcId, node->rightChild->value);

        Asm::Scanf(asmFile);
        Asm::commands::Mov(asmFile, Asm::PtrExpression(Asm::registers::rbp, '-', Asm::stack::ceil * varId), Asm::registers::rax);
    }
}

int getOutput(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    if (node->rightChild) {
        getExpression(node->rightChild, asmFile, funcId, varCount);

        Asm::Printf(asmFile, Asm::registers::rax);
    }
    else return PARSING_ERROR;
}

int getIf(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, bool setTable) {
    int cond = 0;
    int elseLabel = implicitLabels + 1;
    int skipLabel = implicitLabels + 2;
    implicitLabels += 2;

    if (!setTable && node->leftChild)
        cond = getCond(node->leftChild, asmFile, funcId, varCount);

    if (!setTable && node->rightChild->leftChild) {
//        implicitLabels += 1;
        Asm::jumps::JCond(asmFile, Asm::ImplicitLabel(elseLabel), cond);
    } else if (!setTable) {
//        ++implicitLabels;
        Asm::jumps::JCond(asmFile, Asm::ImplicitLabel(skipLabel), cond);
    }

    if (node->rightChild->rightChild) {
        getBlock(node->rightChild->rightChild, asmFile, funcId, varCount, setTable);
        if (!setTable && node->rightChild->leftChild) {
//            implicitLabels += 1;
            Asm::jumps::Jmp(asmFile, Asm::ImplicitLabel(skipLabel));
        }
    }

    if (node->rightChild->leftChild) {
        if (!setTable)
            Asm::ImplicitLabelInit(asmFile, elseLabel);
        getBlock(node->rightChild->leftChild, asmFile, funcId, varCount, setTable);
    }

    if (!setTable)
        Asm::ImplicitLabelInit(asmFile, skipLabel);
}

int getWhile(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount, bool setTable) {
    int cond = 0;

    if (!setTable) {
        implicitLabels += 2;

        Asm::ImplicitLabelInit(asmFile, implicitLabels - 1);

        if (node->leftChild)
            cond = getCond(node->leftChild, asmFile, funcId, varCount);

        Asm::jumps::JCond(asmFile, Asm::ImplicitLabel(implicitLabels), cond);
    }

    getBlock(node->rightChild, asmFile, funcId, varCount, setTable);

    if (!setTable) {
        Asm::jumps::Jmp(asmFile, Asm::ImplicitLabel(implicitLabels - 1));

        Asm::ImplicitLabelInit(asmFile, implicitLabels);
    }
}

int getCond(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    getExpression(node->rightChild, asmFile, funcId, varCount);
    Asm::commands::Mov(asmFile, Asm::registers::rdx, Asm::registers::rax);

    getExpression(node->leftChild, asmFile, funcId, varCount);
    Asm::commands::Cmp(asmFile, Asm::registers::rax, Asm::registers::rdx);

    if (!strcmp(node->value, operations[16]))
        return Asm::jumps::jne;
    else if (!strcmp(node->value, operations[9]))
        return Asm::jumps::jna;
    else if (!strcmp(node->value, operations[10]))
        return Asm::jumps::jnb;
}

int getReturn(tree::Node<char *> *node, FILE *asmFile, int funcId, int *varCount) {
    if (node->rightChild)
        getExpression(node->rightChild, asmFile, funcId, varCount);
    else return PARSING_ERROR;

    Asm::commands::Mov(asmFile, Asm::registers::rsp, Asm::registers::rbp);
    Asm::stack::   Pop(asmFile, Asm::registers::rbp);
    Asm::commands::Ret(asmFile);
}
