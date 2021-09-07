#include <iostream>
#include <stack>
#include "math.h"

#define EP 0.001
using namespace std;

string postfix_expr_g;

int char2num(char c) {
    return c - '0';
}

int priority(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

bool IsOperand(char c) {
    if (c >= 'A' && c <= 'Z')
        return true;
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

bool IsOperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        return true;
    return false;
}

bool HasHigherPrecedence(char op1, char op2) {
    int op1Weight = priority(op1);
    int op2Weight = priority(op2);

    if (op1Weight == op2Weight) {
        if (op1 == '^')
            return false;
        return true;
    }
    return op1Weight > op2Weight;
}

string fun2postfix(string expr) {
    //convert the input into postfix
    stack<char> st;
    string postfix;

    st.push('(');
    expr += ')';

    for (int i = 0; i < expr.length(); ++i) {
        if (expr[i] == ' ') {
            continue;
        } else if (IsOperator(expr[i])) {
            while (!st.empty() && st.top() != '(' && HasHigherPrecedence(st.top(), expr[i])) {
                postfix += st.top();
                st.pop();
            }
            st.push(expr[i]);
        } else if (IsOperand(expr[i])) {
            postfix += expr[i];
        } else if (expr[i] == '(') {
            st.push(expr[i]);
        } else if (expr[i] == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            st.pop();
        }
    }
    while (!st.empty()) {
        postfix += st.top();
        st.pop();
    }
    return postfix;
}

double p_operation(double a, double b, char op) {

    if (op == '+')
        return b + a;
    else if (op == '-')
        return b - a;
    else if (op == '*')
        return b * a;
    else if (op == '/')
        return b / a;
    else if (op == '^')
        return pow(b, a);
    else
        return -1;
}

double evaluate(string postfix, double sub) {
    //evaluate the function
    stack<double> st;
    for (int i = 0; i < postfix.size(); ++i) {
        if (IsOperator(postfix[i])) {
            double a = st.top();
            st.pop();
            double b = st.top();
            st.pop();
            st.push(p_operation(a, b, postfix[i]));
        } else if (IsOperand(postfix[i])) {
            if (postfix[i] == 'x')//substitute x for sub
                st.push(sub);
            else
                st.push(char2num(postfix[i]));
        }
    }
    return st.top();
}


double sol(double x) {
    return evaluate(postfix_expr_g, x);
}

void false_position(double a, double b) {
    if (sol(a) * sol(b) > 0) {
        std::cout << "Cannot use the value of a and b";
        return;
    }
    double fa = sol(a);
    double fb = sol(b);
    int itr = 0;
    double Xn = (a * fb - b * fa) / (fb - fa);

    while (sol(Xn) >= EP || sol(Xn) < 0) {
        itr++;
        fa = sol(a);
        fb = sol(b);

        if (sol(Xn) < 0) {
            a = Xn;
        } else {
            b = Xn;
        }
        Xn = (a * fb - b * fa) / (fb - fa);
    }

    std::cout << "False Position root: " << Xn << " Iterations: " << itr << std::endl;
    return;
}

void secant(double X0, double X1) {
    if (sol(X0) * sol(X1) > 0) {
        std::cout << "Cannot use the value of X(i-1) and X0";
        return;
    }
    double fX0 = sol(X0);
    double fX1 = sol(X1);
    int itr = 0;
    double Xm, Xn;

    do {
        itr++;
        Xn = (X0 * fX1 - X1 * fX0) / (fX1 - fX0);
        if (fX0 * fX1 == 0)
            break;

        X0 = X1;
        X1 = Xn;
        fX0 = sol(X0);
        fX1 = sol(X1);

        Xm = (X0 * fX1 - X1 * fX0) / (fX1 - fX0);
    } while (fabs(Xm - Xn) >= EP);

    std::cout << "Secant root: " << Xn << " Iterations: " << itr << std::endl;
    return;
}

void bisection(double a, double b) {
    if (sol(a) * sol(b) > 0) {
        std::cout << "Cannot use the value of a and b";
        return;
    }
    int itr = 0;
    double Xn = (a + b) / 2;

    while ((b - a) >= EP) {
        itr++;
        Xn = (a + b) / 2;
        if (sol(Xn) == 0)
            break;
        if (sol(Xn) * sol(a) < 0) {
            b = Xn;
        } else {
            a = Xn;
        }

    }

    std::cout << "Bisection root: " << Xn << " Iterations: " << itr << std::endl;
    return;
}

void printMessage(char mode) {
    switch (mode) {
        case 'i':
            cout << "Enter your infix expression to calculate root (eg, x^2-2*x): ";
            break;
        case 'a':
            cout << "Enter the method to use: \n\t1. Bisection\n\t2. False Position\n\t3. Secant\n\t: ";
            break;
        case 'b':
            cout << "Enter a and b (space separated): ";
            break;
        case 'c':
            cout << "Enter X(i-1), X(0) (space separated): ";
            break;
    }

}

int main() {
    // your code goes here
    printMessage('i');
    string expr;
    cin >> expr;
    postfix_expr_g = fun2postfix(expr);
    double a, b, c;

    printMessage('a');
    int method;
    cin >> method;
    switch (method) {
        case 1:
            printMessage('b');
            cin >> a >> b;
            bisection(a, b);
            break;
        case 2:
            printMessage('b');
            cin >> a >> b;
            false_position(a, b);
            break;
        case 3:
            printMessage('c');
            cin >> a >> b;
            secant(a, b);
            break;
    }

    return 0;
}