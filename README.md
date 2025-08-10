# Senbonzakura

## Intro
Compiler written in C++ for the Eta programming language, presented at "CS 4120 - Introduction to Compilers" course from Cornell University.

## To-Do List for Lexing Stage
- [ ] __Write tests using the GTest library for the Lexing stage.__
- [ ] __Check if the code related to the Lexing stage is following the Google C++ Style Guide.__
  * __app__
    - [ ] main.cpp
  * __lib/include/senbonzakura__
    - [ ] diagnostic_reporter.hpp
    - [ ] file_scanner.hpp
    - [ ] file_writer.hpp
    - [ ] lexer.hpp
    - [ ] token_type.hpp
    - [ ] token.hpp
  * __lib/src__
    - [ ] diagnostic_reporter.cpp
    - [ ] file_scanner.cpp
    - [ ] file_writer.cpp
    - [ ] lexer.cpp
    - [ ] token_type.cpp
    - [ ] token.cpp
  * __tests__
    - [ ] diagnostic_reporter_test.cpp
    - [ ] file_scanner_test.cpp
    - [ ] file_writer_test.cpp
    - [ ] lexer_test.cpp
    - [ ] token_type_test.cpp
    - [ ] token_test.cpp
 

## To-Do List for Parsing Stage
- [ ] Study Pratt-Parsing to check if it is okay to addopt it and mesh it with Recursive-Descent Parsing, when dealing with arithmetical expressions.
- [ ] Consult books and other resources about how to properly implement the ```Parser``` in an incremental manner.
- [ ] Study a lot about OOP in C++. I think I gonna need it a lot. As well as templates.
- [ ] Make a pretty printer for the generated AST.
- [ ] Evaluate the possibility of generating the AST in a way that it can be given directly to LLVM so it can generate code for desired target architectures.

## Things to reconsider/rethink
- [ ] The API and functioning of the ```FileWriter``` class. It was kinda hard to write tests for this class when I was developing the lexing phase. It was kind of hard to develop the tests.

## Commands to Execute Tests
```sh
cd build
cmake --build .
ctest
```

## Overview of Eta Features
* __Eta programs consist of a single source file containing definitions of one or more functions.__
  * Execution of a program consists of evaluating a call to the distinguished function named ```main```.
* __The language has two primitive types:__
  * integers ```(int)``` and booleans ```(bool)```.
  * The array type ```T[]``` exists for any type ```T```, so ```T[][]``` represents an array of arrays.
* __Functions may return a value, but need not.__
  * In Eta, a function that does not return a value is called a procedure.
  * A function may take no arguments, one argument, or multiple arguments.
  * Unlike in languages such as C and Java, a function may also return multiple results.
* __There is no ```string``` type, but the type ```int[]``` may be used for most of the same purposes. Literal string constants have this type.__

## Variables in Eta
* Variables are declared by following them with a type declaration and an optional initialization expression.
* There are no holes in scope; __a variable may not be declared when another variable of the same name is already in scope.__
* Examples of variable declaration in Eta:
```rust
x:int = 2;
z:int
b: bool, i:int = f(x)
s: int[] = "Hello";
```
* __Use of the value of an uninitialized variable has undefined behavior.__
* __Eta compilers are not required to detect the use of uninitialized variables.__
* __Identifiers, including variable names, start with any letter and may continue with any sequence of letters, numbers, the underscore character ```(_)```, or single-quote characters ```(')```.__
* As in Java, variables are in scope from the point of declaration till the end of their block.
* __The value of a variable can be changed imperatively using an assignment statement, as in the following examples:__
```rust
x = x + 1;
s = {1, 2, 3};
b = !b;
```

## Function Definitions
* A program contains a sequence of function definitions, including the definitions of the function ```main```.
* __All functions in the program are in scope in the bodies of all functions, even if the use precedes the definition.__
* A function definition starts with the name of the function, followed by its argument(s), its return type(s), and the definition of its code.
  * Only one function with a given name can exist in a program. Hence, there is no function overloading.
* The result of a function is returned using the ```return``` statement. To simplify parsing, any ```return``` statement can only occur as the last statement in its block.
* Example of a function definition in Eta:
```rust
// Return the greatest common divisor of two integers
gcd(a:int, b:int):int {
  while (a != 0) {
    if (a < b) b = b - a;
    else a = a - b;
  }
  return b;
}
```

### Multiple Results
* Unlike in C or Java, in Eta, a function may return multiple results, indicated in the function definition by giving a list of return types, separated by commas.
* If this list is absent, the function is a procedure. It means that it does not return a value.
* The result of a function that return multiple results is a list of expressions, separated by commas.
* The example below shows how this is done in Eta:
```rust
// Add two rational numbers p1/q1 and p2/q2, returning
// two numbers p3, q3 representing their sum, p3/q3.
ratadd(p1:int, q1:int, p2:int, q2:int) : int, int {
  g:int = gcd(q1,q2);
  p3:int = p1*(q2/g) + p2*(q1/g);
  return p3, q1/g*q2;
}
```
* __Results from a function that returns multiple values can be used only through a multiple assignment in which the left-hand side is a sequence of either assignable expressions or variable declarations.__
* The example below show how this is done in Eta:
```rust
q:int
p:int, q = ratadd(2, 5, 1, 3)
_, q’:int = ratadd(1, 2, 1, 3)
```
* In the code snippet above, we see that the value ```11``` is assigned to the variable ```p``` and the value ```15```  is assigned to ```q```. Moreover, the value ```6``` is assigned to ```q'``` and the first value, ```5```, is discarded.
* __The pseudo-variable ```_``` can be used to discard one of the results. It also can be used to explicitly discard the result of a function call that returns a single result.__
* __A feature present in Eta that resembles one that is also present in Python is: A multiple assignment can also be used with a right-hand side that is a comma-separated list of expressions. In this case, the expressions on the right-hand side are all evaluated before any assignments. For example, the following statement swaps the values of the variables ```x``` and ```y```:__
```rust
x, y = y, x;
```
* __Assignable expressions also include array index expressions and ```_```, so it is also possible to swap two array elements without a temporary variable:__
```rust
a[i], a[j] = a[j], a[i];
```

### Global Variables
* A program can contain global variable declarations, written at the top level outside any function definition.
* Global variables are in scope throughout the program, just like function definitions.
* Example of global variable declarations in Eta:
```rust
len: int = 100;
n’: int = -1;
debug: bool = false;
points: int[];
```
* Integer variables may be initialized at the point of declaration to an integer literal, and boolean variables may be initialized to a boolean literal.
* __Global arrays can be declared but cannot be initialized at the point of declaration.__
* __Important: All global variables in scope may have their value changed by assignments in functions, like shown in the code snippet below:__
```rust
debug: bool;

setDebug(b: bool) {
  debug = b
}
```

## Data Types

### Integers
* The type ```int``` describes integers from ```−2^63``` to ```2^63 − 1```. Which means they are represented internally as 64-bits signed integers.
* They support the usual operations: ```+```, ```-```, ```*```, ```/```, and ```%```, which all operate modulo ```2^64```. 
* In addition, the “high multiplication” operator ```*>>``` returns the high 64-bits of the 128-bit product of its operands. For example, the high multiplication of ```2^32``` with itself has the result ```1```. This operator is helpful for doing certain kinds of number crunching.
* __Division by zero causes the program to halt with an error.__
* Integers can be compared with the usual Java/C relational operators: ```==```, ```!=```, ```<```, ```<=```, ```>```, and ```>=```.
* __A literal integer constant is denoted by an optional minus sign (```-```) and a sequence of digits starting with a digit in ```1–9```, or a single ```0```.__
* As in Java, a character literal may be used to denote an integer, so ```'a'``` is the same as ```97```. __Character literals may signify any legal Unicode character, with allowed codes ranging from ```U+000000``` to ```U+10FFFF```.__

### Booleans
* The type bool has two values: ```true``` and ```false```.
* The binary operator ```&``` is a short-circuit ```and``` and the operator ```|``` is short-circuit ```or```. The unary operation ```!``` is negation, also known as ```not```.
* Booleans can also be compared using the operators ```==``` and ```!=```.

### Arrays
#### One-Dimensional Arrays
* An array ```T[]``` is a __fixed-length sequence of mutable cells__ of type ```T```.
* When indexing an array keep in mind that to be a valid index, __an index ```i``` must be nonnegative and less than the length of the array (i.e., ```0 <= i < length(array)```).__
* __If ```i``` is not valid (i.e., does not satisfies the requirements presented above), then this is caught during runtime and the program halts, displaying an error message.__
* The expression ```length(e)``` is responsible for providing the length of the array ```e```. In other words, it tells us the amount of elements it stores.
* Array cells may be assigned to using an array index expression on the left-hand side of an assignment.
* The code snippet below, shows us the Insertion Sort algorithm implemented in Eta:
```rust
sort(a: int[]) {
  i:int = 0;
  n:int = length(a);
  while i < n {
    j:int = i
    while j > 0 {
      if a[j-1] > a[j] {
        swap:int = a[j];
        a[j] = a[j-1];
        a[j-1] = swap;
      }
      j = j-1;
    }
    i = i+1;
  }
}
```
* An array can be constructed by using an array constructor, specifying its elements inside braces. Similar to the array initializer syntax in Java and C, elements of the array constructor are separated by commas, and the final element may be followed by a comma.
  * Thus, ```{}``` can be used as an array of length ```0```,
  * Similarly, ```{2,}``` can be used as an array of length ```1```.
* An array constructor can be used anywhere that an array is expected.
* __A string literal such as ```"Hello"``` may also be used as an array constructor.__ Given this fact, the following two definitions of an array are equivalent:
```rust
a: int[] = { 72,101,108,108,111 };
a: int[] = "Hello";
```
* __String literals may not span multiple lines in the source file.__
* __Like character literals, each character of a string literal must be a legal Unicode character.__
* An array of arbitrary length ```n```, whose cells are not initialized, may be created at the point of declaration by including the length in the declaration of the array.
* __The length is not part of the array’s type and it need not be a constant.__
* __Use of uninitialized array cells has undefined results (i.e., it is considered undefined behavior).__
* __Arrays may be compared with ```==``` and ```!=``` to determine whether they are aliases for the same array. Different arrays with the same contents are considered unequal. Arrays are considered pointers basically.__
* Arrays are implemented by placing the representations of the values of each of their cells contiguously in memory. They also record their lengths.
* The operator ```+``` may be used to concatenate two arrays whose elements are of the same type, which is particularly handy for arrays of ```int``` representing strings, e.g.:
```rust
s: int[] = "Hello" + {13, 10};
```

#### Multi-Dimensional Arrays


## Precedence

## Statements

## Lexical Considerations
* The language is case-sensitive. __An input file is a sequence of Unicode characters, encoded using UTF-8. Therefore ASCII input is always valid.__
* Comments are indicated by a double slash // followed by any sequence of characters until a newline character.
* Keywords (```use```, ```if```, ```while```, ```else```, ```return```, ```length```) may not be used as identifiers. Nor may the names or values of the primitive types (```int```, ```bool```, ```true```, ```false```).
* __String and character literals should support some reasonable set of character escapes, including at least ```"\\"```, ```"\n"```, and ```"\'"```. In addition, an escape of the form ```"\x{HHHHHH}"```, where ```HHHHHH``` stands for ```1–6``` hexadecimal digits (upper or lower case), represents the Unicode character with the corresponding code. For example ```"\x{0a}"``` is the same as ```"\n"```.__
* __You may be more successful parsing negative integer literals as the negation of a positive literal.__

## Doubts
* What about the ```char``` type? If the type ```int[]``` can be used to represent a ```string```. Then, by following this logic, can we use the ```int``` type to represent a ```char``` ?
* If the answer to the question above is yes, then which character set are we representing? ASCII? UTF-8? How does one represent any UTF-8 character using an ```int``` value?
* What exactly is undefined behavior? How should we approach the usage of a variable that has not been initialized? Need to see examples from other programming languages like C, C++, Python, Java.
* It would be cool to detect the use of uninitialized variables and, then, maybe throw a warning or an error during the compilation phase.
* It seems that the specification document has left ambiguous whether or not a statement has a semicolon ```(;)``` at its end. Since most languages opt to use it, I'll also do so.
* Honestly, I don't know how the following observation present in Eta specification can make the parsing easier: "To simplify parsing, any ```return``` statement can only occur as the last statement in its block."
* Honestly, I got a little confused about the following statement declared within the Eta programming language specification: "All global variables in scope may have their value changed by assignments in functions". Does this mean that everytime that I want to change the value of a global variable I must call a specific function that does that for me?
* What does it mean to say that every binary arithmetic operation operates modulo ```2^64```?
* The Eta Specification document says that "Like character literals, each character of a string literal must be a legal Unicode character". But what exactly is a legal Unicode character? And how should I represent it inside within a token?
* I don't think I fully understood this statement: "Arrays are implemented by placing the representations of the values of each of their cells contiguously in memory. They also record their lengths."
