#include<iostream>
#include<stack>
#include<vector>
#include<string>

using namespace std;

bool validate(string& in);
int eval(string& in);
int precedence(char op);

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