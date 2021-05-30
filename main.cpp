#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <functional>
#include <fstream>

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

std::string read_expression_from_file(const std::string& filename);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <input filename>", argv[0]);
        return 1;
    }

    std::string filename{argv[1]};
    printf("Reading expression from file \"%s\"...\n", filename.c_str());


    std::string code = read_expression_from_file(filename);
    TokenStream tokens = tokenize(code);
    print_tokens(tokens);
    
    auto result = eval(tokens);
    printf("Result: %i", result);

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


// =================== read expression from file ====================
std::string read_expression_from_file(const std::string& filename)
{
    std::ifstream file{filename};
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
// ==================================================================