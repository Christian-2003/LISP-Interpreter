# Data types: ![](https://img.shields.io/github/size/ChosenChris/LISP-Interpreter/Documentation/Lisp/Data%20types.md?label=File%20size)
> The Lisp Tutorials have been written for version _1.0_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [Primitive Data Types](#primitive-types)
2. [Default values](#default-values)
3. [Variable declaration and initialization](#declaration)
4. [Function definition](#function)

<br/>
<br/>
<br/>

***

## Primitive Data Types: <a name="primitive-types"></a>

This Lisp dialect is a statically-typed programming language, which means that all variables must first be declared before they can be used. The declaration of a new variable involves stating it's type and name:
```Lisp
;Listing 001: Exemplary declaration and initialization
(void main () (
    (int field 1)
))
```
Doing this tells your program that a variable named "field" exists, holds numerical data and has an initial value of "1". A variable's data type determines the values it may contain, plus the operations that may be performed with it. In addition to `int`, this Lisp dialect supports several other primitive data types. A primitive type is predefined by the language and is named by a reserved keyword. The supported primitive data types are:
- **`int`:** The `int` data type is a 32-bit signed integer, which has a minimum value of -2<sup>31</sup> and a maximum value of 2<sup>31</sup>-1.
- **`double`:** The `double` data type is a double-precision 64-bit floating point. It accommodates seven digits. Its range is approximately 5.0 × 10<sup>-345</sup> to 1.7 × 10<sup>-308</sup>.
- **`bool`:** The `bool` data type has only two possible values: `T` (_true_) and `NIL` (_false_). This data type represents one bit of information.
- **`char`:** The `char` data type is a single 8-bit ASCII character. It's minimum value is 0 and it's maximum value is 255.
- **`string`:** The `string` has no specified size or range. It is a single text which contains an array of `char`-type values, with any length and is handled, unlike in other programming languages, like a primitive data type.

<br/>

***

## Default values: <a name="default-values"></a>

It's not always necessary to assign a value when a variable is declared. Variables that are declared but not initilaized will be set to a defaul type by the interpreter. Generally, these default values will be 0, depending on the data type. Relying on such default values is overall considered bad programming.

The following chart summarizes the default values for the earlier mentioned data types:

Data Type | Default Value | Remarks
--- | --- | ---
`int` | `0` | -
`double` | `0.0` | -
`bool` | `NIL` | Value is set to _false_
`char` | `' '` | Value is set to a whitespace.
`string` | `""` | Value is set to an empty string.

<br/>

***

## Variable declaration and initialization: <a name="declaration"></a>

In the following, the declaration and initialization of a variable is described using the `int` keyword. Other data types work in the exact the same way.

A integer variable can be declared as follows:
```Lisp
;Listing 002: Declaration of integer variable
(void main () (
    (int myVar)
))
```
By doing so, a variable named "myVar" is declared, which can store numerical values, since it is declared using the `int` keyword. The variable is not initialized in this statement, which makes the interpreter assign the default value of _0_ to the variable.

If you want to initialize the variable, there are two possible ways to do so:
```Lisp
;Listing 003: Declaration and initialization of integer variable
(void main () (
    ;Declaration and initialization in a single statement
    (int myVar 15)

    ;Declaration and initialization in two seperate statements
    (int myNewVar)
    (set myNewVar 15)
))
```
When doing this, the variable will be initialized with the value _15_.

<br/>

***

## Function definition: <a name="function"></a>

A LISP function must return a value when using any of the earlier mentioned [primitive data types](primitive-types). The keyword that indicates the return type of a function takes the first place in the list that resembles the function's definition:
```Lisp
;Listing 004: Exemplary definition of a function
(string getName () (
;^^^^^^ <- Indicates that the function returns a String
    (return "John Doe")
    ;       ^^^^^^^^^^ <- String is returned using the "return" keyword
))
```
However, it is not always neccessary to return a value. Utilizing the data type `void`, which does only work with functions, no value will be returned.