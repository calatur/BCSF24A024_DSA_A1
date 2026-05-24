#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<map>

using namespace std;


/*
Token types
 0: numbers
 1: variables
 2: operators +, -, *, /
 3: left bracket ( [ {
 4: right bracket ) ] }
*/
struct Token {
    int type;
    string sval;  // variables / operators
    int ival;     // numbers
};


int precedence(char op);
char leftBracket(char right);
bool tokenize(string& in, vector<Token>& tokens);
bool validate(vector<Token>& tokens);
bool convert(vector<Token>& infix, vector<Token>& postfix);
void extractVars(vector<Token>& postfix, vector<string>& vars);
int eval(vector<Token>& postfix, map<string, int>& varValues, int& result);


int main(){
    string in;
    getline(cin, in);

    vector<Token> tokens, postfix;
    if ((!tokenize(in, tokens)) || (!validate(tokens)) || (!convert(tokens, postfix))){
        return 1;
    }

    vector<string> vars;
    extractVars(postfix, vars);

    map<string, int> varValues;
    for (int i = 0; i < (int)vars.size(); i++) {
        cerr << "Enter value for " << vars[i] << ": ";
        int val;
        if (!(cin >> val)) {
            cerr << "Runtime error: invalid value for variable '" << vars[i] << "'" << endl;
            return 2;
        }
        varValues[vars[i]] = val;
    }

    int result = 0;
    int ret = eval(postfix, varValues, result);
    if (ret != 0){
        return ret;
    }

    string postfixStr = "";
    for (int i = 0; i < (int)postfix.size(); i++) {
        postfixStr += postfix[i].sval;
        if (i != (int)postfix.size() - 1){
            postfixStr += " ";
        }
    }

    cout << postfixStr << endl;
    cout << result;


    return 0;
}



int precedence (char op){
    int res = 0;
    if (op == '+' || op == '-'){
        res = 1;
    }
    else if (op == '*' || op == '/'){
        res = 2;
    }
    return res;
}

char leftBracket(char right) {
    char res = 0;
    if (right == ')')
        res = '(';
    if (right == ']')
        res = '[';
    if (right == '}')
        res =  '{';
    return res;
}

bool tokenize(string& in, vector<Token>& tokens) {
    int i = 0;
    int n = (int)in.size();
    bool res = true, flag = true;
    while (i < n && flag) {
        if (isspace(in[i])) {
            i++;
        }
        else if (isdigit(in[i])) {
            int start = i;
            while (i < n && isdigit(in[i])){
                i++;
            }
            Token t;
            t.type = 0;
            t.sval = in.substr(start, i - start);
            t.ival = 0;
            for (int k = 0; k < (int)t.sval.size(); k++) {
                t.ival = t.ival * 10 + (t.sval[k] - '0');
            }
            tokens.push_back(t);
        }
        else if (isalpha(in[i]) || in[i] == '_') {
            int start = i;
            while (i < n && (isalnum(in[i]) || in[i] == '_')){
                i++;
            }
            Token t;
            t.type = 1;
            t.sval = in.substr(start, i - start);
            t.ival = 0;
            tokens.push_back(t);
        }
        else if (in[i] == '+' || in[i] == '-' || in[i] == '*' || in[i] == '/') {
            Token t;
            t.type = 2;
            t.sval = string(1, in[i]);
            t.ival = 0;
            tokens.push_back(t);
            i++;
        }
        else if (in[i] == '(' || in[i] == '[' || in[i] == '{') {
            Token t;
            t.type = 3;
            t.sval = string(1, in[i]);
            t.ival = 0;
            tokens.push_back(t);
            i++;
        }
        else if (in[i] == ')' || in[i] == ']' || in[i] == '}') {
            Token t;
            t.type = 4;
            t.sval = string(1, in[i]);
            t.ival = 0;
            tokens.push_back(t);
            i++;
        }
        else {
            cerr << "Syntax error: unexpected character '" << in[i] << "'" << endl;
            flag = false;
            res = false;
        }
    }
    return res;
}


bool validate(vector<Token>& tokens) {
    if (tokens.empty()) {
        cerr << "Syntax error: empty expression" << endl;
        return false;
    }

    stack<char> brackets;

    for (int i = 0; i < (int)tokens.size(); i++) {
        const Token& t = tokens[i];
        if (t.type == 3) {
            brackets.push(t.sval[0]);
        }
        else if (t.type == 4) {
            if (brackets.empty()) {
                cerr << "Syntax error: lone closing bracket '" << t.sval << "'" << endl;
                return false;
            }
            char expected = leftBracket(t.sval[0]);
            if (brackets.top() != expected) {
                cerr << "Syntax error: inalid brackets, expected closing for '" << brackets.top() << "' but got '" << t.sval << "'" << endl;
                return false;
            }
            brackets.pop();
        }
    }
    if (!brackets.empty()) {
        cerr << "Syntax error: unclosed bracket '" << brackets.top() << "'" << endl;
        return false;
    }

    const Token& first = tokens[0];
    if (first.type != 0 && first.type != 1 && first.type != 3) {
        cerr << "Syntax error: expression cannot begin with '" << first.sval << "'" << endl;
        return false;
    }
    const Token& last = tokens[tokens.size() - 1];
    if (last.type != 0 && last.type != 1 && last.type != 4) {
        cerr << "Syntax error: expression cannot end with '" << last.sval << "'" << endl;
        return false;
    }

    for (int i = 0; i + 1 < (int)tokens.size(); i++) {
        Token& cur = tokens[i];
        Token& nxt = tokens[i + 1];

        bool curIsOperand = (cur.type == 0 || cur.type == 1);
        bool curIsOp = (cur.type == 2);
        bool curIsLeft = (cur.type == 3);
        bool curIsRight = (cur.type == 4);

        bool nxtIsOperand = (nxt.type == 0 || nxt.type == 1);
        bool nxtIsOp = (nxt.type == 2);
        bool nxtIsLeft = (nxt.type == 3);
        bool nxtIsRight = (nxt.type == 4);

        if (curIsOperand && !(nxtIsOp || nxtIsRight)) {
            cerr << "Syntax error: operand '" << cur.sval << "' followed by unexpected token '" << nxt.sval << "'" << endl;
            return false;
        }
        if (curIsOp && !(nxtIsOperand || nxtIsLeft)) {
            cerr << "Syntax error: operator '" << cur.sval << "' followed by unexpected token '" << nxt.sval << "'" << endl;
            return false;
        }
        if (curIsLeft && !(nxtIsOperand || nxtIsLeft)) {
            cerr << "Syntax error: '" << cur.sval << "' followed by unexpected token '" << nxt.sval << "'" << endl;
            return false;
        }
        if (curIsRight && !(nxtIsOp || nxtIsRight)) {
            cerr << "Syntax error: '" << cur.sval << "' followed by unexpected token '" << nxt.sval << "'" << endl;
            return false;
        }
    }

    return true;
}

bool convert(vector<Token>& infix, vector<Token>& postfix) {
    stack<Token> opStack;

    for (int i = 0; i < (int)infix.size(); i++) {
        Token& t = infix[i];

        if (t.type == 0 || t.type == 1) {
            postfix.push_back(t);
        }
        else if (t.type == 2) {
            while (!opStack.empty() &&
                   opStack.top().type == 2 &&
                   precedence(opStack.top().sval[0]) >= precedence(t.sval[0])) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(t);
        }
        else if (t.type == 3) {
            opStack.push(t);
        }
        else if (t.type == 4) {
            while (!opStack.empty() && opStack.top().type != 3) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty()) {
                cerr << "Syntax error: invalid brackets" << endl;
                return false;
            }
            char expected = leftBracket(t.sval[0]);
            if (opStack.top().sval[0] != expected) {
                cerr << "Syntax error: invalid brackets" << endl;
                return false;
            }
            opStack.pop();
        }
    }

    while (!opStack.empty()) {
        if (opStack.top().type == 3 || opStack.top().type == 4) {
            cerr << "Syntax error: open bracket" << endl;
            return false;
        }
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return true;
}

void extractVars(vector<Token>& postfix, vector<string>& vars) {
    map<string, int> seen;
    for (int i = 0; i < (int)postfix.size(); i++) {
        if (postfix[i].type == 1) {
            string& name = postfix[i].sval;
            if (seen.find(name) == seen.end()) {
                seen[name] = 1;
                vars.push_back(name);
            }
        }
    }
}


int eval(vector<Token>& postfix, map<string, int>& varValues, int& result) {
    stack<int> s;

    for (int i = 0; i < (int)postfix.size(); i++) {
        Token& t = postfix[i];
        if (t.type == 0) {
            s.push(t.ival);
        }
        else if (t.type == 1) {
            map<string, int>::iterator it = varValues.find(t.sval);
            if (it == varValues.end()){
                cerr << "Runtime error: no value for variable '" << t.sval << "'" << endl;
                return 2;
            }
            s.push(it->second);
        }
        else if (t.type == 2) {
            if (s.size() < 2) {
                cerr << "Logical error: insufficient operands for operator '" << t.sval << "'" << endl;
                return 3;
            }
            int b = s.top(); 
            s.pop();
            int a = s.top();
            s.pop();
            char op = t.sval[0];
            if (op == '+'){
                s.push(a + b);
            }
            else if (op == '-'){
                s.push(a - b);
            }
            else if (op == '*'){
                s.push(a * b);
            }
            else if (op == '/') {
                if (b == 0) {
                    cerr << "Runtime error: division by zero" << endl;
                    return 2;
                }
                s.push(a / b);
            }
        }
    }

    if (s.size() != 1) {
        cerr << "Logical error: malformed expression (too many operands)" << endl;
        return 3;
    }
    result = s.top();
    return 0;
}