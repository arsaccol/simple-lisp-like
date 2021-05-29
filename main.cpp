#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <functional>

using TokenStream = std::vector<std::string>;
using Token = std::string;

using BinaryOpType = std::function<int (int, int)>;
using UnaryOpType = std::function<int (int)>;

using BinaryOpsMapType = std::unordered_map<Token, BinaryOpType>;
const BinaryOpsMapType BinaryOps = {
    {"+", [](int a, int b) { return a + b; }},
    {"-", [](int a, int b) { return a - b; }},
    {"*", [](int a, int b) { return a * b; }},
    {"/", [](int a, int b) { return a / b; }}
};

std::vector<std::string> tokenize(std::string code);
void print_tokens(const TokenStream& tokens);
int eval(const TokenStream& tokens);


int main()
{
    std::string lisp_code{ "( * 2 ( * ( - 3 4 ) 1 ) )" };
    printf("Expression: %s\n", lisp_code.c_str());

    TokenStream tokens = tokenize(lisp_code);
    print_tokens(tokens);

    int result = eval(tokens);

    printf("Result = %i", result);

    return 0;
}

// ====================== tokenize ========================
TokenStream tokenize(std::string code)
{
    TokenStream tokens;
    std::stringstream tok_stream(code);

    Token current_token;


    while(tok_stream >> current_token)
    {
        tokens.push_back(current_token);
    }

    return tokens;
}
// =========================================================



// ==================== print_tokens =======================
void print_tokens(const TokenStream& tokens)
{
    printf("Tokens:\n");
    for(const auto& tok : tokens)
        printf("%s ", tok.c_str());

    printf("\n");
} // ========================================================


// ========================== eval ========================
int eval(const TokenStream& tokens)
{
    int accumulator = 0xbadc0de;
    std::stack<Token> token_stack;


    for(const Token& token : tokens)
    {
        if(token == ")")
        {
            // time to calculate shit
            std::stack<Token> postfix_stack;
            while(token_stack.top() != "(")
            {
                postfix_stack.push(token_stack.top());
                token_stack.pop();
            } 

            // element at top of postfix stack should be our operation
            BinaryOpType operation = BinaryOps.find(postfix_stack.top())->second;
            postfix_stack.pop();

            // get our first value
            int postfix_accumulator = std::stoi(postfix_stack.top());
            postfix_stack.pop();
            
            // pop all items off the postfix stack, accumulating operation results
            while(!postfix_stack.empty())
            {
                postfix_accumulator = operation(postfix_accumulator, std::stoi(postfix_stack.top()));
                postfix_stack.pop();
            }

            // remove pesky remaining opening parenthesis lest it bites us in the ass in next iteration
            token_stack.pop();
            
            // now put the accumulated value back on top of the token stack
            token_stack.push(std::to_string(postfix_accumulator));
        }
        else
        {
            token_stack.push(token);
        }
    }

    return std::stoi(token_stack.top());
}
// ========================================================
