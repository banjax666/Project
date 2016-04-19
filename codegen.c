/* codegen.c: implements the code generator
 *
 * Group 41 - Jayanth N. Koushik (2010A7PS021P), Naresh Raghuraman (2010A7PS090P)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "symbolTables.h"

// code for the read function
const char *readProc = "\
_read proc\n\
    push si\n\
    mov dx, offset inputbuf\n\
    mov bx, dx\n\
    mov byte ptr[bx], 10\n\
    mov ah, 0ah\n\
    int 21h\n\
    \n\
    mov ah, 02h\n\
    mov dl, 10\n\
    int 21h\n\
    \n\
    mov bx, offset inputbuf + 1\n\
    mov cx, 0\n\
    mov cl, [bx]\n\
    mov ax, 0\n\
    \n\
    mov si, 0\n\
    \n\
    inc bx\n\
    mov dl, [bx]\n\
    cmp dl, 45\n\
    jne _startgetting\n\
    	mov si, 1\n\
    	dec cx\n\
    	jmp _getdigitfrombuf\n\
    _startgetting:\n\
    	dec bx\n\
    _getdigitfrombuf:\n\
        inc bx\n\
        mov dx, 10\n\
        mul dx\n\
        mov dh, 0\n\
        mov dl, [bx]\n\
        sub dx, 48\n\
        add ax, dx\n\
        \n\
        loop _getdigitfrombuf\n\
    \n\
    cmp si, 1\n\
    jne _readend\n\
    	not ax\n\
    	add ax, 1\n\
    \n\
    _readend:\n\
        pop si\n\
        ret\n\
_read endp";

// code for the write function
const char *writeProc = "\
_write proc\n\
    mov bx, ax\n\
    and bx, 8000h\n\
    cmp bx, 0\n\
    je _writesignhandled\n\
    not ax\n\
    add ax, 1\n\
    mov bx, ax\n\
    mov ah, 02h\n\
    mov dl, 45\n\
    int 21h\n\
    mov ax, bx\n\
    \n\
    _writesignhandled:\n\
    mov cx, 0\n\
    mov bx, 10\n\
    mov dx, 0\n\
    _getdigitfromnum:\n\
        div bx\n\
        \n\
        push dx\n\
        inc cx\n\
        \n\
        mov dx, 0\n\
        \n\
        cmp ax, 0\n\
        jne _getdigitfromnum\n\
        \n\
        mov ah, 02h\n\
    \n\
    _writedigit:\n\
        pop dx\n\
        add dl, 48\n\
        int 21h\n\
        \n\
        dec cx\n\
        cmp cx, 0\n\
        jne _writedigit\n\
    ret\n\
_write endp";

int ifNo = 0;
char ifNoBuf[11];
int loopNo = 0;
char loopNoBuf[11];

extern RecordTable *recs;

void genCode(parseTree *p, FILE *out) {
    if(p->isTerminal == false) {
        switch(p->nonTerminal) {
            case program:
                fprintf(out, ".model small\n.stack\n\n.code\n\n%s\n\n%s\n\n", readProc, writeProc);
                genCode(&p->children[1], out);
                fprintf(out, ".data\n");
                fprintf(out, "inputbuf db 12 dup(0)\n");
                genCode(&(p->children[1].children[1].children[1]), out);
                fprintf(out, "\nend main");
                break;

            case mainFunction:
                fprintf(out, "main proc\n\nmov ax, seg inputbuf\nmov ds, ax\n\n");
                genCode(&p->children[1], out);
                break;

            case declarations:
                if(p->noOfChildren != 1) {
                    genCode(&p->children[0], out);
                    genCode(&p->children[1], out);
                }
                break;

            case declaration:
                if(p->children[1].children[0].nonTerminal == primitiveDatatype) {
                    genCode(&p->children[3], out);
                    fprintf(out, " dw 0\n");
                }
                else {
                    IdentifierTable *result, *iter;
                    result = findRecordDetails(recs,  p->children[1].children[0].children[1].terminal.lexeme, result);
                    for(iter = result; iter != NULL; iter = iter->next) {
                        genCode(&p->children[3], out);
                        fprintf(out, "_%s dw 0\n", iter->identifier.name);
                    }
                }
                break;

            case stmts:
                genCode(&p->children[2], out);
                fprintf(out, "mov ah, 4ch\nmov al, 0\nint 21h\n\nmain endp\n\n");
                break;

            case otherStmts:
                if(p->noOfChildren != 1) {
                    // not eps
                    genCode(&p->children[0], out);
                    genCode(&p->children[1], out);
                }
                break;

            case stmt:
                genCode(&p->children[0], out);
                break;

            case assignmentStmt:
                genCode(&p->children[2], out);
                fprintf(out, "mov ");
                genCode(&p->children[0], out);
                fprintf(out, ", ax\n\n");
                break;

            case singleOrRecId:
                genCode(&p->children[0], out);
                if(p->children[1].noOfChildren != 1) {
                    fprintf(out, "_");
                    genCode(&p->children[1].children[1], out);
                }
                break;

            case conditionalStmt:
                snprintf(ifNoBuf, 10, "%d", ifNo);
                genCode(&p->children[2], out);
                fprintf(out, "cmp ax, 0\n");
                fprintf(out, "je ELSE%s\n", ifNoBuf);
                genCode(&p->children[5], out);
                genCode(&p->children[6], out);
                fprintf(out, "jmp ENDIF%s\n", ifNoBuf);
                fprintf(out, "ELSE%s:\n", ifNoBuf, ifNoBuf);
                genCode(&p->children[7], out);
                fprintf(out, "ENDIF%s:\n\n", ifNoBuf);
                ifNo++;
                break;

            case elsePart:
                if(p->noOfChildren != 1) {
                    genCode(&p->children[1], out);
                    genCode(&p->children[2], out);
                }
                break;

            case ioStmt:
                if(p->children[0].terminal.tokenClass == TK_READ) {
                    fprintf(out, "call _read\n");
                    fprintf(out, "mov ");
                    genCode(&p->children[2], out);
                    fprintf(out, ", ax\n\n");
                }
                else {
                    genCode(&p->children[2], out);
                    fprintf(out, "\ncall _write\n\n");
                }
                break;

            case iterativeStmt:
                snprintf(loopNoBuf, 10, "%d", loopNo);
                fprintf(out, "STARTLOOP%s:\n", loopNoBuf);
                genCode(&p->children[2], out);
                fprintf(out, "cmp ax, 0\nje ENDLOOP%s\n", loopNoBuf);
                genCode(&p->children[4], out);
                genCode(&p->children[5], out);
                fprintf(out, "jmp STARTLOOP%s\nENDLOOP%s:\n\n", loopNoBuf, loopNoBuf);
                loopNo++;
                break;

            case allVar:
                if(p->noOfChildren == 1)
                    genCode(&p->children[0], out);
                break;

            case var:
                genCode(&p->children[0], out);
                break;

            case arithmeticExpression:
                genCode(&p->children[0], out);
                if(p->children[1].noOfChildren != 1) {
                    fprintf(out, "push ax\n");
                    genCode(&p->children[1], out);
                    fprintf(out, "mov bx, ax\npop ax\n");
                    if(p->children[1].children[0].children[0].terminal.tokenClass == TK_PLUS)
                        fprintf(out, "add ax, bx\n\n");
                    else
                        fprintf(out, "sub ax, bx\n\n");
                }
                break;

            case term:
                genCode(&p->children[0], out);
                if(p->children[1].noOfChildren != 1) {
                    fprintf(out, "push ax\n");
                    genCode(&p->children[1], out);
                    fprintf(out, "mov bx, ax\npop ax\n");
                    if(p->children[1].children[0].children[0].terminal.tokenClass == TK_MUL)
                        fprintf(out, "mul bx\n");
                    else
                        fprintf(out, "mov dx, 0\ndiv bx\n");
                }
                break;

            case factor:
                if(p->noOfChildren != 1) 
                    genCode(&p->children[1], out);
                else
                    genCode(&p->children[0], out);
                break;

            case termPrime:
                if(p->noOfChildren != 1) {
                    genCode(&p->children[1], out);
                    if(p->children[2].noOfChildren != 1) {
                        fprintf(out, "push ax\n");
                        genCode(&p->children[2], out);
                        fprintf(out, "mov bx, ax\npop ax\n");
                        if(p->children[2].children[0].children[0].terminal.tokenClass == TK_MUL)
                            fprintf(out, "mul ax, bx\n");
                        else
                            fprintf(out, "div ax, bx\n");
                    }
                }
                break;

            case expPrime:
                if(p->noOfChildren != 1) {
                    genCode(&p->children[1], out);
                    if(p->children[2].noOfChildren != 1) {
                        fprintf(out, "push ax\n");
                        genCode(&p->children[2], out);
                        fprintf(out, "mov bx, ax\npop ax\n");
                        if(p->children[2].children[0].children[0].terminal.tokenClass == TK_PLUS)
                            fprintf(out, "add ax, bx\n");
                        else
                            fprintf(out, "sub ax, bx\n");
                    }
                }
                break;

            case all:
                if(p->noOfChildren == 1 || p->children[1].noOfChildren == 1) {
                    fprintf(out, "mov ax, ");
                    genCode(&p->children[0], out);
                    fprintf(out, "\n");
                }
                break;

            case booleanExpression:
                if(p->children[0].terminal.tokenClass == TK_OP) {
                    genCode(&p->children[1], out);
                    fprintf(out, "push ax\n");
                    genCode(&p->children[5], out);
                    fprintf(out, "pop bx\n");
                    if(p->children[3].children[0].terminal.tokenClass == TK_AND)
                        fprintf(out, "and ax, bx\n\n");
                    else
                        fprintf(out, "or ax, bx\n\n");
                }
                else if(p->children[0].terminal.tokenClass == TK_NOT) {
                    genCode(&p->children[1], out);
                    fprintf(out, "not ax\n\n");
                }
                else {
                    fprintf(out, "mov ax, ");
                    genCode(&p->children[0], out);
                    fprintf(out, "\npush ax\nmov ax, ");
                    genCode(&p->children[2], out);
                    fprintf(out, "\nmov bx, ax\npop ax\ncmp ax, bx\n");
                    fprintf(out, "pushf\npop ax\n");
                    switch(p->children[1].children[0].terminal.tokenClass) {
                        case TK_LT:
                            fprintf(out, "and ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(out, "mov cl, 7\nshr al, cl\n");
                            fprintf(out, "xor al, ah\nmov ah, 0\n\n");
                            break;

                        case TK_LE:
                            fprintf(out, "mov bl, al\nand ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(out, "mov cl, 7\nshr al, cl\n");
                            fprintf(out, "xor al, ah\nand bl, 40h\nmov cl, 6\nshr bl, cl\n");
                            fprintf(out, "or al, bl\nmov ah, 0\n\n");
                            break;

                        case TK_EQ:
                            fprintf(out, "and ax, 0040h\nmov cl, 6\nshr al, cl\n\n");
                            break;

                        case TK_GT:
                            fprintf(out, "mov bl, al\nand ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(out, "mov cl, 7\nshr al, cl\n");
                            fprintf(out, "xor al, ah\nnot al\nand al, 01h\nnot bl\nand bl, 40h\n");
                            fprintf(out, "mov cl, 6\nshr bl, cl\n");
                            fprintf(out, "and al, bl\nmov ah, 0\n\n");
                            break;

                        case TK_GE:
                            fprintf(out, "and ax, 0880h\nmov cl, 3\nshr ah, cl\n");
                            fprintf(out, "mov cl, 7\nshr al, cl\n");
                            fprintf(out, "xor al, ah\nnot al\nand ax, 0001h\n\n");
                            break;

                        case TK_NE:
                            fprintf(out, "and ax, 0040h\nmov cl, 6\nshr al, cl\n");
                            fprintf(out, "not al\nand ax, 0001h\n\n");
                            break;
                    }
                }                        
                                
            default:
                ;
        }
    }

    else
        fprintf(out, "%s", p->terminal.lexeme);
        
}
