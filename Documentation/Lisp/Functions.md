# Functions: ![](https://img.shields.io/github/size/ChosenChris/LISP-Interpreter/Documentation/Lisp/Functions.md?label=File%20size)
> The Lisp Tutorials have been written for version _1.0_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [Overview](#overview)
2. [Parameters](#parameters)
3. [Return types](#return)
4. [Main function](#main)
5. [Calling a function](#call)

<br/>
<br/>
<br/>

***

## Overview: <a name="overview"></a>

Every piece of source code is written into functions. The first function to be executed by the interpreter is the [main](#main)-function. From there on, every other function can be called which will lead to the execution of the source code that it located in the body of the called function. A function is defined in a single list which is made up of multiple elements.

### Elements of a function's definition:
_Return type_
<br>
The function's return type resembles the data type of it's returned values.

_Name_
<br/>
The function's name is used to identify a function.

_Parameter **(Optional)**_
<br/>
The function's parameters indicate which values have to be passed onto to function when it's called.

_Body_
<br/>
The function's body contains the source code which is executed when the function is called.

<br/>

```Lisp
;Listing 001: Exemplary function definition:
(int sum ((int p1)(int p2)) (
    (return (+ p1 p2))
))
```
This function takes the two arguments _p1_ and _p2_, which both have to be integers and returns their sum, which is another integer.

**IMPORTANT:** Functions can never be defined inside other functions (_Lambda-functions_).

<br/>

***

## Parameters: <a name="parameters"></a>

A parameter indicates how many values have to be passed onto the defined function when it is called. Thereby every parameter is defined in a single list and consists of a [primitive data type](https://github.com/ChosenChris/LISP-Interpreter/blob/main/Documentation/Lisp/Data%20types.md#primitive-data-types-) and a name. The data type indicates which type of value can be passed onto the function as this parameter and the name is used to access it's value in the function's body:
```Lisp
;Listing 002: Exemplary use of parameters
(void printValue (int value) (
    (print value)
))
```
The function `printValue` possesses the parameter `value` which can contain an integer value.

If you want to have more than one parameter, you have to encapsulate the parameters into another list as follows:
```Lisp
;Listing 003: Exemplary use of multiple parameters
(void printValues ((int value1)(int value2)(int value3)) (
    (print value1 ", " value2 ", " value3)
))
```
This function possesses the three parameters `value1`, `value2` and `value3`, each of which can contain an integer value.

It is not always necessary for a function to possess parameters. A function without parameters might look like this:
```Lisp
;Listing 004: Exemplary use of no parameters
(void printHelloWorld () (
    ;                 ^^ <- You may leave out this empty list
    (print "Hello World.")
))
```

<br/>

***

## Return types: <a name="return"></a>

The return type indicates the type of value that is returned by a function. A function must always return a value of the same type as the specified return type using the `return` statement.

### Return type `void`:
In _Listing 002_ up to _Listing 004_, the return type void is used. This specific return type specifies that the function does not return any value at all.
```Lisp
;Listing 005: Exemplary use of return type void
(void printSum ((int a)(int b)) (
;^^^^ <- void indicates that no value is returned.
    (print (+ a b))
))
```
You might end the execution of a function through the `return` keyword:
```Lisp
;Listing 006: Exemplary use of return type void with return keyword
(void printSum ((int a)(int b)) (
    (if (>= a 5) (
        (return) ;Return keyword ends the function's execution.
    ))
    (print (+ a b))
))
```
In this case, the sum of the passed arguments is only printed if the value of the passed argument _a_ is less than than 5.

<br/>

### Other return types:
If a function is supposed to return a value, it is important to specify the type of the returned value. Currently, every [primitive data type](https://github.com/ChosenChris/LISP-Interpreter/blob/main/Documentation/Lisp/Data%20types.md#primitive-data-types-) can be returned by a function:
```Lisp
;Listing 007: Exemplary use of return types
(int sum ((int a)(int b)) (
    (return (+ a b))
))
```
This function takes two integer values as arguments _a_ and _b_ and returns their sum. It is important that the returned value has the same data type as the function's return type. This means that such a function must always make use of the `return` keyword.

<br/>

***

## Main function: <a name="main"></a>

The main function contains the source code which is executed at the beginning. Unlike other functions, the `main` function never has parameters. In addition to that, the main function always has the return type `void`:
```Lisp
;Listing 008: Exemplary main function
(void main () (
    (print "Hello World.")
))
```

<br/>

***

## Calling a function: <a name="call"></a>

A function can be called from anywhere in the source code. If the called function does not contain parameters, the function's name can simply be written into the source code like a variable and utilized in the same way:
```Lisp
;Listing 009: Exemplary function call without parameter and return value
(void main () (
    (printText) ;Call the function "printText"
))

(void printText () (
    (print "Hello World")
))
```
If the function has a specified return type, the function can be called as follows:
```Lisp
;Listing 010: Exemplary function call without parameter and with return value
(void main () (
    (print printText)
))

(string printText () (
    (return "Hello World")
))
```

<br/>

On the other hand, if the function has parameters, it needs to be encapsulated into a list. The function's name will make up the first element in the list while every other element make up the passed arguments:
```Lisp
;Listing 011: Exemplary function call with parameter and no return value
(void main () (
    (printSum 15 7)
))

(void printSum ((int a)(int b)) (
    (print (+ a b))
))
```
If the function has a specified return type, it might be called as follows:
```Lisp
;Listing 012: Exemplary function call with parameter and return value
(void main () (
    (print (sum 15 7))
))

(int sum ((int a)(int b)) (
    (return (+ a b))
))
```
