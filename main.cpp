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

bool validate(string& in);
int eval(string& in);
int precedence(char op);
char leftBracket(char right);
bool tokenize(const string& in, vector<Token>& tokens);


int main(){
    string in;
    cout << "Enter the expression: ";
    getline(cin, in);
    return eval(in);
}

bool validate(){

    return true;
}

int eval(string& in){
    vector<string> variables;
    vector<int> tokens;
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

bool tokenize(const string& in, vector<Token>& tokens) {
    int i = 0;
    int n = (int)in.size();
    bool res = true, flag = true;
    while (i < n && flag) {
        if (isspace(in[i])) {
            i++;
        }
        else if (isdigit(in[i])) {
            int start = i;
            while (i < n && isdigit(in[i])) i++;
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
            while (i < n && (isalnum(in[i]) || in[i] == '_')) i++;
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