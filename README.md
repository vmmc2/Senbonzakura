# Senbonzakura

## Intro
Compiler written in C++ for the Eta programming language, presented at "CS 4120 - Introduction to Compilers" course from Cornell University.

## To-Do List for Lexing Stage
- [ ] Add support for the CLI11 library, so it can be properly used to transform Senbonzakura into a proper CLI tool.
- [ ] Write unit tests using the GTest library for the implemented classes.
- [ ] Add support for character literals. Let's start with the most basic encoding: ASCII.
- [ ] Verify that the Lexer is correctly distinguinshing identifiers and keywords.
- [ ] According to the "Programming Assignment 1": "Non-printable and special characters in character and string literal constants should be escaped in the output, as well as Unicode character escapes as described in the Eta Language Specification, but ordinary printable ASCII characters (e.g., 'd') should not be. Comments and whitespace should not appear in the output". I need to make this work somehow.
- [ ] In my opinion, we should not allow the usage of the ```'``` character in identifiers. This seems to complicate things, since this is also used to signal a character literal value. However, I should think more about this.
- [ ] Need to think more about this fact: String literals may not span multiple lines in the source file. Like character literals, each character of a string literal must be a legal Unicode character.
- [ ] How to deal with the following:
  - [ ] Therefore ASCII input is always valid. Comments are indicated by a double slash (```//```) followed by any sequence of characters until a newline character.
  - [ ] Keywords (```use```, ```if```, ```while```, ```else```, ```return```, ```length```) may not be used as identifiers. Nor may the names or values of the primitive types (```int```, ```bool```, ```true```, ```false```).
  - [ ] String and character literals should support some reasonable set of character escapes, including at least ```"\\"```, ```"\n"```, and ```"\'"```.
  - [ ] In addition, an escape of the form ```"\x{HHHHHH}"```, where ```HHHHHH``` stands for ```1–6``` hexadecimal digits (upper or lower case), represents the Unicode character with the corresponding code. For example ```"\x{0a}"``` is the same as ```"\n"```. You may be more successful parsing negative integer literals as the negation of a positive literal.
