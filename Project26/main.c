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
        fprintf(stderr, "스택 포화 에러\n");
    }
    else s->data[++(s->top)] = item; //전위 연산자는 사용 전에 증가시킴
}

element pop(Stacktype* s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);

    }
    else return s->data[(s->top)--]; //후위 연산자는 사용 후에 감소시킴
}

element peek(Stacktype* s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else return s->data[s->top];
}


//연산자의 우선순위 반환하는 함수
int prec(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

//중위를 후위로 바꾸는 함수
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
//후위 표기 수식 계산 함수
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
            // 숫자를 만나면 바로 스택에 넣습니다.
            while (isdigit(exp[i + 1])) {
                i++;
                value = value * 10 + (exp[i] - '0');
            }
            push(&s, value);
            // 공백을 만나면 무시합니다.
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