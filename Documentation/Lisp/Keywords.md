# Keywords: ![](https://img.shields.io/github/size/Christian-2003/LISP-Interpreter/Documentation/Lisp/Keywords.md?label=File%20size)
The implemented Lisp dialect allows the use of a variety of different keywords. Those keywords are usually the first element in (most) lists in Lisp. In some situations, other atoms may take the first position in a list.

### Example:
```Lisp
(print "Hello World")
;^^^^^ <- This is the keyword.
```

<br/>
<br/>
<br/>

# Available keywords:
The following keywords can be used with the Lisp interpreter.

<br/>

### Keywords regarding variables:
[`int`](#declaration),
[`double`](#declaration),
[`bool`](#declaration),
[`char`](#declaration),
[`string`](#declaration)
<br/>
These keywords are used in order to declare a new variable with the respective data type.

[`set`](#set)
<br/>
This keyword is used to change the content of any variable.

<br/>

### Keywords for in- and output:
[`print`](#print),
[`println`](#print)
<br/>
These keywords are used in order to print any value to the terminal.

<br/>

### Control structures:
[`if`](#if)
<br/>
This keyword indicates an if-statement.

[`while`](#if)
<br/>
This keyword indicates a while-loop.

<br/>

### Functions:
[`return`](#return)
<br/>
This keyword is used to return a value from a function.

<br/>
<br/>
<br/>

***

# Documentation of the keywords:

<br/>

## `int`, `double`, `bool`, `char`, `string`: <a name ="declaration"></a>

### Remarks:
These keywords are used to **declare** a new variable which stores a value of the respective data type. Furthermore, these keywords can be used to indicate a **function's return type**. Further information on functions can be found [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Functions.md). However, this part of the documentation will only focus on the declaration of variables.

<br/>

### Data types:
Keyword | Data type | Standard value
--- | --- | ---
int | Integers | `0`
double | Doubles | `0.0`
bool | Booleans | `nil`
char | Characters | `' '`
string | Strings | `""`

<br/>

### Arguments:
_variable name_
<br/>
Represents the name of the variable. This name can be any word as long is it does not represent any keyword, parenthesis or data value.

_variable value **optional**_
<br/>
Represents the value of the declared variable. If no value is given, the variable will be initialized with the respective standard value.

<br/>

### Example:
```Lisp
;Creates a new integer variable with 15 as it's value.
(int INTEGER_VARIABLE 15)

;Creates a new double variable with 2.5 as it's value.
(double DOUBLE_VARIABLE 2.5)

;Creates a new boolean variable with nil as it's value.
(bool BOOLEAN_VARIABLE nil)

;Creates a new character variable with 'c' as it's value.
(char CHARACTER_VARIABLE 'c')

;Creates a new string variable with "Hello World" as it's value.
(string STRING_VARIABLE "Hello World")
```

<br/>

### Standard values:
In case a variable does not get initialized during it's declaration, a standard value will be assigned to it:

<br/>

***

## `set`: <a name ="set"></a>

### Remarks:
This keyword is used to change the value of any variable. The variable needs to be declared before it's value can be changed.

<br/>

### Arguments:
_variable name_
<br/>
The name of the variable, whose value is supposed to be changed.

_variable value_
<br/>
The new value, for the variable. Please note that the new value has to be of the same type as the variable.

<br/>

### Example:
```Lisp
(int myVar) ;Declare a new variable with it's standard value.

(set myVar 15) ;Change the variable's value to 15.
(set myVar (+ 8 2)) ;Change the variable's value to the result of 8 + 2.

(set myVar "Hello World") ;This throws an error, since the value of the integer variable myVar is changed to a string.
```

<br/>

***

## `print`, `println`: <a name ="print"></a>

### Remarks:
These keywords are used to print values to the console. The difference between the keywords is, that `print` prints the elements while `println` adds a line break afterwards.

<br/>

### Arguments:
_content_
<br>
Every upcoming element in the same list as the keyword will be printed ton the terminal.

<br/>

### Example:
```Lisp
(print "2 + 5 = " (+ 2 5))
;^^^^^ ^^^^^^^^^^ ^^^^^^^
;Key-  Element    Element
;word  1          2
```

### Output:
```
2 + 5 = 7
```

<br/>

***

## `if`: <a name="if"> </a>

### Remarks:
This keyword indicates an if-statement, which is used to change the path that your program is following during it's execution. Further information on the if-statement can be found [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Control%20structures.md#if).

<br/>

### Arguments:
_condition_
<br/>
The condition states wether the expressions in the _body_ will be executed. If the condition is `T`, the expressions will be executed, if it's `NIL`, the expressions will not be executed.

_body_
<br/>
A list with every expression, that should be executed when the condition is `T`.

_else-body **(optional)**_
<br/>
A list of every expression, that is executed when the condition is `NIL`.

<br/>

### Example:
```Lisp
(if (= 5 5) (
    (println "5 is equal to 5:")
    (println "The condition is true.")
))
```

### Output:
```
5 is equal to 5:
The condition is true.
```

<br/>

***

## `while`: <a name="while"> </a>

### Remarks:
This keyword indicates a while-loop, which is used to iterate through a specifif chunk of code several times, while a specific condition is considered to be `T`. Further information on the while-loop can be found [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Control%20structures.md#while).

<br/>

### Arguments:
_condition_
<br/>
The condition states how often the _body_ of the while-loop will be executed. If the condition is `T`, the _body_ will be exectuted one time, after that the condition will be checked again. If the condition is `NIL`, the loop will be cancelled and the next statements will be executed.

_body_
<br/>
A list of every expression that should be executed while the _condition_ is `T`.

<br/>

### Example:
```Lisp
(int i 0)
(while (< i 5) (
    (println "iteration of while-loop: " i)
    (set i (+ i 1))
))
```

### Output:
```
iteration of while-loop: 0
iteration of while-loop: 1
iteration of while-loop: 2
iteration of while-loop: 3
iteration of while-loop: 4
```

<br/>

***

## `return`: <a name="return"> </a>

### Remarks:
The return keyword indicates that a specific value is returned from a function. The type of the returned value is defined within the function. More information on functions can be found [here](https://github.com/Christian-2003/LISP-Interpreter/blob/main/Documentation/Lisp/Functions.md).

<br/>

### Arguments:
_return value_
<br/>
The return value resembles the value which will be returned by the function. It's type has to match the function's defined return type.

<br/>

### Example:
```Lisp
(void main () (
    (println func)
))

(string func () (
    (return "Hello World.")
    ;^^^^^^ ^^^^^^^^^^^^^^^
    ;Key-   Returned value
    ;word   is a string.
))
```

### Output:
```
Hello World.
```

<br/>

***
