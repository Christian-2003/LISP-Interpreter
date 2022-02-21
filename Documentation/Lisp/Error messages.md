# Error messages: ![](https://img.shields.io/github/size/Christian-2003/LISP-Interpreter/Documentation/Lisp/Error%20messages.md?label=File%20size)

### Table of contents:
FileScanner Error Messages:
- [FileScanner Error 1](#1)

<br/>

Tokenizer Error Messages:
- [Tokenizer Error 2](#2)
- [Tokenizer Error 3](#3)
- [Tokenizer Error 4](#4)
- [Tokenizer Error 5](#5)

<br/>

Parser Error Messages:
- [Parser Error 6](#6)
- [Parser Error 8](#8)

<br/>

Interpreter Error Messages:
- [Interpreter Error 9](#9)
- [Interpreter Error 10](#10)
- [Interpreter Error 11](#11)
- [Interpreter Error 12](#12)
- [Interpreter Error 13](#13)
- [Interpreter Error 14](#14)
- [Interpreter Error 15](#15)
- [Interpreter Error 16](#16)
- [Interpreter Error 17](#17)
- [Interpreter Error 19](#19)
- [Interpreter Error 20](#20)
- [Interpreter Error 21](#21)
- [Interpreter Error 22](#22)
- [Interpreter Error 43](#43)
- [Interpreter Error 44](#44)
- [Interpreter Error 45](#45)
- [Interpreter Error 46](#46)
- [Interpreter Error 47](#47)
- [Interpreter Error 48](#48)
- [Interpreter Error 49](#49)
- [Interpreter Error 50](#50)
- [Interpreter Error 51](#51)
- [Interpreter Error 52](#52)
- [Interpreter Error 54](#54)
- [Interpreter Error 55](#55)

<br/>

Convert Error Messages:
- [Convert Error 23](#23)
- [Convert Error 24](#24)
- [Convert Error 25](#25)
- [Convert Error 26](#26)
- [Convert Error 27](#27)
- [Convert Error 28](#28)
- [Convert Error 29](#29)
- [Convert Error 30](#30)
- [Convert Error 31](#31)
- [Convert Error 32](#32)
- [Convert Error 33](#33)
- [Convert Error 34](#34)
- [Convert Error 35](#35)
- [Convert Error 36](#36)
- [Convert Error 37](#37)
- [Convert Error 38](#38)
- [Convert Error 39](#39)
- [Convert Error 40](#40)
- [Convert Error 41](#41)
- [Convert Error 42](#42)

<br/>
<br/>
<br/>

***
## FileScanner Error 1: <a name="1"></a>
_The file does not exist._

The file which is supposed to contain the LISP source code does not exist.

<br/>

***
## Tokenizer Error 2: <a name="2"></a>
_The number has too many decimal points._

A number with the data type `double` has more than one decimal point.

### Example:
The following source code generates Error 2.
```Lisp
;Error2.lsp
(void main () (
    (double myVar 1.23.45)
    ;                 ^ <- This second decimal point can never exist
))
```

<br/>

## Tokenizer Error 3: <a name="3"></a>
_The source code is not long enough._

The source code does abruptly end without the correct amount of closed parentheses.

### Example:
The following source code generates Error 3.
```Lisp
;Error3.lsp
(void main () (
    (println "Hello World")
)
;^ <- Missing parentheses
```

<br/>

## Tokenizer Error 4: <a name="4"></a>
_Encountered char-type value with no content._

A value with the data type `char` is encountered in the source code without any character as it's content.

### Example:
The following source code generates Error 4.
```Lisp
;Error4.lsp
(void main () (
    (char myVar '')
    ;           ^^ <- value has no content
))
```

<br/>

## Tokenizer Error 5: <a name="5"></a>
_Missing quotation marks._

The quotation marks at the end of a String are missing.

### Example:
The following source code generates Error 5.
```Lisp
;Error5.lsp
(void main () (
    (println "Hello World)
    ;                    ^ <- Missing quotation marks at the end of String
))
```

<br/>

***
## Parser Error 6: <a name="6"></a>
_Encountered Syntax Error while Parsing._

The parser encountered a syntax error.

<br/>

## Parser Error 8: <a name="8"></a>
_Encountered parentheses as atom._

The parser encountered a parentheses instead of an atom.

<br/>

***
## Interpreter Error 9: <a name="9"></a>
_Encountered syntax error while interpreting._

The interpreter encountered a syntax error.

### Example:
The following source code generates Error 9.
```Lisp
;Error9.lsp
(void main () (
    (Println "Hello World")
    ;^^^^^^^ <- Incorrect syntax
))
```

<br/>

## Interpreter Error 10: <a name="10"></a>
_Encountered operands of different type, that are not compatible with each other._

The interpreter encounteres two operands in an operation that are not compatible with each other.

**Deprecated:** This error message has been replaced by convert error messages.

<br/>

## Interpreter Error 11: <a name="11"></a>
_Cannot add multiple boolean values._

An argument with data type `bool` was passed to an arithmetic operation utilizing the `+`-operator.

### Example:
The following source code generates Error 11.
```Lisp
;Error11.lsp
(void main () (
    (print (+ NIL NIL))
    ;       ^ ^^^ ^^^ <- Arithmetic operation does not work with booleans
))
```

<br/>

## Interpreter Error 12: <a name="12"></a>
_Cannot subtract multiple non-numeric values._

A non-numeric value was passed to an arithmetic operation utilizing the `-`-operator.

### Example:
The following source code generates Error 12.
```Lisp
;Error12.lsp
(void main () (
    (print (- 45 "Hello World"))
    ;            ^^^^^^^^^^^^^ <- String cannot be subtracted
))
```

<br/>

## Interpreter Error 13: <a name="13"></a>
_Cannot multiply multiple non-numeric values._

A non-numeric value was passed to an arithmetic operation utilizing the `*`-operator.

### Example:
The following source code generates Error 13.
```Lisp
;Error13.lsp
(void main () (
    (print (* 2 T))
    ;           ^ <- Boolean cannot be multiplied
))
```

<br/>

## Interpreter Error 14: <a name="14"></a>
_Cannot divide multiple non-numeric values._

A non-numeric value was passed to an arithmetic operation utilizing the `/`-operator.

### Example:
The following source code generates Erorr 14.
```Lisp
;Error14.lsp
(void main () (
    (print (/ 50 'c'))
    ;            ^^^ <- Character cannot be divided
))
```

<br/>

## Interprerer Error 15: <a name="15"></a>
_Encountered division through zero._

A divison through zero was encountered in the source code.

### Example:
The following source code generates Erorr 15.
```Lisp
;Error15.lsp
(void main () (
    (print (/ 15 0))
    ;            ^ <- Cannot divide by zero
))
```

<br/>

## Interpreter Error 16: <a name="16"></a>
_Variable does not exist._

A variable was used which has not been declared before.

### Example:
The following source code generates Error 16.
```Lisp
;Error16.lsp
(void main () (
    (print myVar)
    ;      ^^^^^ <- "myVar" has not been declared yet
))
```

<br/>

## Interpreter Error 17: <a name="17"></a>
_Variable with this name is already in use._

A variable is declared whose identifier is currently used by another variable.

### Example:
The following source code generates Error 17.
```Lisp
;Error 17.lsp
(void main () (
    (int myVar 2)
    (int myVar 15)
    ;    ^^^^^ <- "myVar" has already been declared
))
```

<br/>

## Interpreter Error 19: <a name="19"></a>
_Missing token._

A token is missing in the source code.

<br/>

## Interpreter Error 20: <a name="20"></a>
_Encountered incorrect token._

An incorrect token was encountered in the source code.

<br/>

## Interpreter Erorr 21: <a name="21"></a>
_Too many arguments._

The interpreter encountered too many arguments that are passed to a function.

### Example:
The following source code generates Error 21.
```Lisp
;Error21.lsp
(void main () (
    (int myVar 15 7)
    ;             ^ <- This argument cannot be evaluated
))
```

<br/>

## Interpreter Error 22: <a name="22"></a>
_Not enough arguments._

Not enough arguments are passed to a function.

### Example:
The following source code generates Error 22.
```Lisp
;Error22.lsp
(void main () (
    (int)
    ;^^^ <- Missing variable name as argument
))
```

<br/>

## Interpreter Error 43: <a name="43"></a>
_Missing condition of statement._

An if-clause or while-loop is missing a condition.

### Example:
The following source code generates Error 43.
```Lisp
;Error43.lsp
(void main () (
    (if (
    ;^^ <- If-clause has no condition
        (print "Hello World")
    ))
))
```

<br/>

## Interpreter Error 44: <a name="44"></a>
_Missing body of statement._

An if-clause or while-loop is missing a body.

### Example:
The following source code generates Error 44.
```Lisp
;Error44.lsp
(void main () (
    (if T)
    ;^^ <- If-clause has no body with expressions
))
```

<br/>

## Interpreter Error 45: <a name="45"></a>
_Incorrect function definition._

A function is defined incorrect.

### Example:
The following source code generates Error 45.
```Lisp
;Error45.lsp
(void main () (
    (func)
))

(void func)
;^^^^^^^^^ <- Function has no expressions and is therefore incorrectly defined
```

<br/>

## Interpreter Error 46: <a name="46"></a>
_Incorrect function name._

The passed argument, which resembles the function's name is not available as a function name.

### Example:
The following source code generates Error 46.
```lisp
;Error46.lsp
(void main () (

))

(void int () (
;     ^^^ <- "int" is a keyword and cannot be a function's name
))
```

<br/>

## Interpreter Error 47: <a name="47"></a>
_LISP main-function is missing._

The source code does not have a main-function.

### Example:
The following source code generates Erorr 47.
```Lisp
;Error47.lsp

;Main function is missing. Obviously.
```

<br/>

## Interpreter Error 48: <a name="48"></a>
_LISP main-function has too many parameters._

The main-function has one ore more parameters.

### Example:
The following source code generates Error 48.
```Lisp
;Error48.lsp
(void main (int p1) (
;           ^^^^^^ <- Main function can never have parameters.
    (print "Hello World")
))
```

<br/>

## Interpreter Error 49: <a name="49"></a>
_LISP main-function has incorrect return type._

The main-function's return type is not `void`.

### Example:
```Lisp
;Error49.lsp
(int main () (
;^^^ <- Incorrect return type
    (print "Hello World")
))
```

<br/>

## Interpreter Error 50: <a name="50"></a>
_Function has no expressions._

A function's body has no expressions.

### Example:
The following source code generates Error 50.
```Lisp
;Error50.lsp
(void main () (
    (func)
))

(void func ())
;^^^^^^^^^^^^^ <- Function has no expressions
```

<br/>

## Interpreter Error 51: <a name="51"></a>
_The called function does not exist._

A function was called which has not been defined.

### Example:
The following source code generates Erorr 51.
```Lisp
;Error51.lsp
(void main () (
    (func)
    ;^^^^ <- "func" has not been defined
))
```

<br/>

## Interpreter Error 52: <a name="52"></a>
_An incorrect number of arguments is passed._

The number of arguments that are passed to a function do not match it's number of parameters.

### Example:
The following source code generates Error 52.
```Lisp
;Error52.lsp
(void main () (
    (sum 5)
    ;    ^ <- Only one out of two arguments are passed
))

(void sum ((int a)(int b)) (
    (print (+ a b))
))
```

<br/>

## Interpreter Error 54: <a name="54"></a>
_LISP-function can only return up to exactly one argument._

A function returns more than one value.

### Example:
The following source code generates Error 54.
```Lisp
;Error54.lsp
(void main () (
    (int myVar getVar)
))

(int getVar () (
    (return 15 22)
    ;          ^^ <- "return" keyword takes only one argument
))
```

<br/>

## Interpreter Error 55: <a name="55"></a>
_Stack overflow occured._

A stack overflow occured.

<br/>

***
## Convert Error 23: <a name="23"></a>
_Cannot convert integer to double._

The interpreter cannot convert an integer value to a double value.

### Example:
The following example generates Error 23.
```Lisp
;Error23.lsp
(void main () (
    (double myVar 15)
    ;             ^^ <- Cannot convert int to double
))
```

<br/>

## Convert Error 24: <a name="24"></a>
_Cannot convert integer to boolean._

The interpreter cannot convert an integer value to a boolean value.

### Example:
The following example generates Error 24.
```Lisp
;Error24.lsp
(void main () (
    (bool myVar 15)
    ;           ^^ <- Cannot convert int to bool
))
```

<br/>

## Convert Error 24: <a name="25"></a>
_Cannot convert integer to character._

The interpreter cannot convert an integer value to a character value.

### Example:
The following example generates Error 25.
```Lisp
;Error25.lsp
(void main () (
    (char myVar 15)
    ;           ^^ <- Cannot convert int to char
))
```

<br/>

## Convert Error 24: <a name="26"></a>
_Cannot convert integer to String._

The interpreter cannot convert an integer value to a String.

### Example:
The following example generates Error 26.
```Lisp
;Error26.lsp
(void main () (
    (string myVar 15)
    ;             ^^ <- Cannot convert int to string
))
```

<br/>

## Convert Error 27: <a name="27"></a>
_Cannot convert double to integer._

The interpreter cannot convert a double value to an integer value.

### Example:
The following example generates Error 27.
```Lisp
;Error27.lsp
(void main () (
    (int myVar 2.5)
    ;          ^^^ <- Cannot convert double to int
))
```

<br/>

## Convert Error 28: <a name="28"></a>
_Cannot convert double to boolean._

The interpreter cannot convert a double value to a boolean value.

### Example:
The following example generates Error 28.
```Lisp
;Error28.lsp
(void main () (
    (bool myVar 2.5)
    ;           ^^^ <- Cannot convert double to bool
))
```

<br/>

## Convert Error 29: <a name="29"></a>
_Cannot convert double to character._

The interpreter cannot convert a double value to a character value.

### Example:
The following example generates Error 29.
```Lisp
;Error29.lsp
(void main () (
    (char myVar 2.5)
    ;           ^^^ <- Cannot convert double to char
))
```

<br/>

## Convert Error 30: <a name="30"></a>
_Cannot convert double to String._

The interpreter cannot convert a double value to a String.

### Example:
The following example generates Error 30.
```Lisp
;Error30.lsp
(void main () (
    (string myVar 2.5)
    ;             ^^^ <- Cannot convert double to string
))
```

<br/>

## Convert Error 31: <a name="31"></a>
_Cannot convert boolean to integer._

The interpreter cannot convert a boolean value to an integer value.

### Example:
The following example generates Error 31.
```Lisp
;Error31.lsp
(void main () (
    (int myVar NIL)
    ;          ^^^ <- CAnnot convert bool to int
))
```

<br/>

## Convert Error 32: <a name="32"></a>
_Cannot convert boolean to double._

The interpreter cannot convert a boolean value to a double value.

### Example:
The following example generates Error 32.
```Lisp
;Error32.lsp
(void main () (
    (double myVar NIL)
    ;             ^^^ <- CAnnot convert bool to double
))
```

<br/>

## Convert Error 33: <a name="33"></a>
_Cannot convert boolean to character._

The interpreter cannot convert a boolean value to a character value.

### Example:
The following example generates Error 33.
```Lisp
;Error33.lsp
(void main () (
    (char myVar NIL)
    ;           ^^^ <- Cannot convert bool to char
))
```

<br/>

## Convert Error 34: <a name="34"></a>
_Cannot convert boolean to String._

The interpreter cannot convert a boolean value to a String.

### Example:
The following example generates Error 34.
```Lisp
;Error34.lsp
(void main () (
    (string myVar NIL)
    ;             ^^^ <- Cannot convert bool to string
))
```

<br/>

## Convert Error 35: <a name="35"></a>
_Cannot convert character to integer._

The interpreter cannot convert a character value to an integer value.

### Example:
The following example generates Error 35.
```Lisp
;Error35.lsp
(void main () (
    (int myVar 'c')
    ;          ^^^ <- Cannot convert char to int
))
```

<br/>

## Convert Error 36: <a name="36"></a>
_Cannot convert character to double._

The interpreter cannot convert a character value to a double value.

### Example:
The following example generates Error 36.
```Lisp
;Error36.lsp
(void main () (
    (double myVar 'c')
    ;             ^^^ <- Cannot convert char to double
))
```

<br/>

## Convert Error 37: <a name="37"></a>
_Cannot convert character to boolean._

The interpreter cannot convert a character value to a boolean value.

### Example:
The following example generates Error 37.
```Lisp
;Error37.lsp
(void main () (
    (bool myVar 'c')
    ;           ^^^ <- Cannot convert char to bool
))
```

<br/>

## Convert Error 38: <a name="38"></a>
_Cannot convert character to String._

The interpreter cannot convert a character value to a String.

### Example:
The following example generates Error 38.
```Lisp
;Error38.lsp
(void main () (
    (string myVar 'c')
    ;             ^^^ <- Cannot convert char to string
))
```

<br/>

## Convert Error 39: <a name="39"></a>
_Cannot convert String to integer._

The interpreter cannot convert a String to an integer value.

### Example:
The following example generates Error 39.
```Lisp
;Error39.lsp
(void main () (
    (int myVar "Hello World")
    ;          ^^^^^^^^^^^^^ <- Cannot convert string to int
))
```

<br/>

## Convert Error 40: <a name="40"></a>
_Cannot convert String to double._

The interpreter cannot convert a String to a double value.

### Example:
The following example generates Error 40.
```Lisp
;Error40.lsp
(void main () (
    (double myVar "Hello World")
    ;             ^^^^^^^^^^^^^ <- Cannot convert string to double
))
```

<br/>

## Convert Error 41: <a name="41"></a>
_Cannot convert String to boolean._

The interpreter cannot convert a String to a boolean value.

### Example:
The following example generates Error 41.
```Lisp
;Error41.lsp
(void main () (
    (bool myVar "Hello World")
    ;           ^^^^^^^^^^^^^ <- Cannot convert string to bool
))
```

<br/>

## Convert Error 42: <a name="42"></a>
_Cannot convert String to character._

The interpreter cannot convert a String to a character value.

### Example:
The following example generates Error 42.
```Lisp
;Error42.lsp
(void main () (
    (char myVar "Hello World")
    ;           ^^^^^^^^^^^^^ <- Cannot convert string to char
))
```
