# Gorts_C
This is a wingding based programming language written in C 

29/02/2024
 Right now the project is at the start, I need to start to define the following:
1: Design of Language:
1.1: Syntax and Semantics
1.2: Type System
1.3: Character Encoding

2: Compiler Setup
 This is coming to be a compiled language as opposed to an interpereted language, this will keep edge cases to a minimum.
2.1: Tools required for running the programming language and developing it (tool chain)
 GCC, Make ect
3: Parsing
3.1: Lexer
 Write a lexer that can read wingding characters and convert them into tokens, this involves handling unique encoding of wingdings and mapping it to tokens
3.2: Parser
 Develop a parser that takes the tokens from the lexer and and builds an abstract syntax tree that represents the program, this will rely heavily
 on the syntax rules you've defined
4: Semantic Analysis: 
4.1: Type Checking
 Implement type checking based on your statically typed system. This involves verifying that 
 operations are performed on compatible types and that variable types are consistent throughout their use.
4.2: Scope Resolution
 Handle the scoping rules of your language, ensuring that variables and functions are accessed within their valid scopes.
5: Code Generation
5.1: Target Platform
 LLVM Bytecode for portability
5.2: Code Generator: 
 Write the components that translates the ast to the target platforms code, this will involve deep knowlege of the targets instruction set and calling conentions
 if you're generating machine code
.5

