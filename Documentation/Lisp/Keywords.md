# Keywords:
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

<br/>
<br/>
<br/>

***

# Documentation of the keywords:

<br/>

## `int`, `double`, `bool`, `char`, `string`: <a name ="declaration"></a>

### Remarks:
These keywords are used to **declare** a new variable which stores a value of the respective data type.

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
<br/>
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
<br/>
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
<br/>
<br/>

***

## `if`: <a name="if"> </a>

### Remarks:
This keyword indicates an if-statement, which is used to change the path that your program is following during it's execution. Further information on the if-statement can be found [here](https://github.com/ChosenChris/LISP-Interpreter/blob/main/Documentation/Lisp/Control%20structures.md#if).

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
