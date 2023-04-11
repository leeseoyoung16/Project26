#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX_STACK 100

typedef char element;

typedef struct {
    element data[MAX_STACK];
    int top;
}Stacktype;

void init_stack(Stacktype* s) {
    s->top = -1;
}

int is_full(Stacktype* s) {
    return (s->top == (MAX_STACK - 1));
}
int is_empty(Stacktype* s) {
    return(s->top == -1);
}

void push(Stacktype* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "���� ��ȭ ����\n");
    }
    else s->data[++(s->top)] = item; //���� �����ڴ� ��� ���� ������Ŵ
}

element pop(Stacktype* s) {
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);

    }
    else return s->data[(s->top)--]; //���� �����ڴ� ��� �Ŀ� ���ҽ�Ŵ
}

element peek(Stacktype* s) {
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);
    }
    else return s->data[s->top];
}


//�������� �켱���� ��ȯ�ϴ� �Լ�
int prec(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

//������ ������ �ٲٴ� �Լ�
char* infix_to_postfix(char exp[]) {
    int i = 0;
    char ch, top_op;
    int len = strlen(exp);
    Stacktype s;

    init_stack(&s);
    char* postfix = (char*)malloc(sizeof(char) * (len * 2+ 1));
    int j = 0;

    int count = 0;
    for (int i = 0; i < len; i++) {
        if (exp[i] == '(') {
            count++;
        }
        else if (exp[i] == ')') {
            count--;
            if (count < 0) {
                fprintf(stderr, "Error: Mismatched parentheses\n");
                exit(1);
            }
        }
    }
    if (count != 0) {
        fprintf(stderr, "Error: Mismatched parentheses\n");
        exit(1);
    }
    ch = exp[i];
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
        fprintf(stderr, "Error: Invalid expression\n");
        exit(1);
    }


    while (i < len) {
        ch = exp[i];
        if (ch == ' ') {
            i++;
            continue;
        }
        switch (ch) {
        case '+': case '-': case '*': case '/':
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            push(&s, ch);
            break;

        case '(':
            push(&s, ch);
            break;

        case ')':
            top_op = pop(&s);
            while (top_op != '(') {
                postfix[j++] = top_op;
                postfix[j++] = ' ';
                top_op = pop(&s);

            }
            break;

        default:
            if (isdigit(ch)) {
                while (isdigit(ch)) {
                    postfix[j++] = ch;
                    i++;
                    ch = exp[i];
                }
                postfix[j++] = ' ';
                i--;
            }
            
            else {
                fprintf(stderr, "Error: Invalid character\n");
                exit(1);
            }
            break;
        }
        i++;
    }
    while (!is_empty(&s)) {
        postfix[j++] = pop(&s);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    return postfix;
}
//���� ǥ�� ���� ��� �Լ�
double eval(char exp[]) {
    double op1, op2, value;
    int len = strlen(exp);
    char ch;
    Stacktype s;


    init_stack(&s);

    

    for (int i = 0; i < len; i++) {
        ch = exp[i];
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            if (ch < '0' || ch > '9') {
                if (ch == ' ') {
                    continue; // ignore spaces
                }
                fprintf(stderr, "Error: Invalid character '%c'\n", ch);
                exit(1);
            }

            value = ch - '0';
            // ���ڸ� ������ �ٷ� ���ÿ� �ֽ��ϴ�.
            while (isdigit(exp[i + 1])) {
                i++;
                value = value * 10 + (exp[i] - '0');
            }
            push(&s, value);
            // ������ ������ �����մϴ�.
            if (exp[i + 1] == ' ') {
                i++;
            }
        }
        else {
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
            
        }
    }
    return pop(&s);
}

int main(void) {
    char exp[MAX_STACK];
    int re;
    char* in_re;
    printf("Enter an infix expression :");
    fgets(exp, sizeof(exp), stdin);

    exp[strcspn(exp, "\n")] = '\0';

    in_re = infix_to_postfix(exp);
    printf("Postfix expression :%s\n", in_re);
    re = eval(in_re);
    printf("Result : %d", re);

    return 0;

}