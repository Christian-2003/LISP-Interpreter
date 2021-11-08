# Operations: ![](https://img.shields.io/github/size/Christian-2003/LISP-Interpreter/Documentation/Lisp/Operations.md?label=File%20size)
> The Lisp Tutorials have been written for version _alpha-0.4_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [General information](#general)
2. [Arithmetic operations](#arithmetic)
3. [Relational operations](#relational)
4. [Boolean operations](#boolean)

<br/>
<br/>
<br/>

***

## General information: <a name="general"></a>

Operations are translated into LISP through the [polish notation](https://en.wikipedia.org/wiki/Polish_notation). In the following, this topic will be explained with an exemplary arithmetic operation, although this applies as well to relational- and boolean operations:

A mathmatical arithmetic operation, such as _7 + 15_ is translated to `(+ 7 15)`. This Lisp expression consists of three atoms, which are `+`, `7` and `15`. Out of those elements, the first one resembles the operator and the subsequent elements resemble it's operands.
<br/>
You can add another operand to the earlier mentioned operation, for instance add `-3` to make the operation `(+ 7 15 -3)`, which translates to the mathmatical operation _7 + 15 + (-3)_. Arithmetic- and boolean operations work with as many operands as needed, relational operations do only work with exactly two operands.
<br/>
Operations can be encapsulated, which allows the creation of operations with different operands. The earlier mentioned operation could be extended with the different operation `(* 2 5)` to make `(+ 7 15 (* 2 5) -3)`, which translates to the mathmatical operation _7 + 15 + (2 × 5) + (-3)_, or _7 + 15 + 2 × 5 + (-3)_. The encapsulated operation gets solved first and it's result will be used as operand in it's parent.

<br/>

***

## Arithmetic operations: <a name="arithmetic"></a>

This Lisp dialect provides operators that perform addition, subtraction, multiplication and division:
Operator | Algebraic Symbol | Compatible Data Types | Description
--- | --- | --- | ---
`+` | + | `int`, `double`, `char`, `string` | Addition operator
`-` | - | `int`, `double` | Subtraction operator
`*` | × | `int`, `double` | Multiplication operator
`/` | ÷ | `int`, `double` | Division operator<br/>_No knwon division through 0!_

**IMPORTANT:** Although the addition operator does work with strings and characters, every other operator does only work with numerical values!


The following program tests the arithmetic operators:
```Lisp
;Listing 001: Exemplary use of arithmetic operators
(int result)

;Test addition operator
(set result (+ 1 2))
(println "1 + 2 = " result)

;Test subtraction operator
(set result (- 3 1))
(println "3 - 1 = " result)

;Test multiplication operator
(set result (* 2 2))
(println "2 * 2 = " result)

;Test division operator
(set result (/4 2))
(println "4 / 2 = " result)
```
This program prints the following:
```
1 + 2 = 3
3 - 1 = 2
2 * 2 = 4
4 / 2 = 2
```

<br/>

***

## Relational operations: <a name="relational"></a>

Relational operators determine if one operand is greater than, less than, equal to or not equal to another operand:
Operator | Algebraic Symbol | Compatible Data Types | Description
--- | --- | --- | ---
`=` | = | `int`, `double`, `bool`, `char`, `string` | Determines if both operands are equal to each other
`!` | ≠ | `int`, `double`, `bool`, `char`, `string` | Determines if both operands are not equal to each other
`>` | > | `int`, `double` | Determines if the first operand is greater than the second operand
`<` | < | `int`, `double` | Determines if the first operand is less than the second operand
`>=` | ≥ | `int`, `double` | Determines if the first operand is greater than or equal to the second operand
`<=` | ≤ | `int`, `double` | Determines if the first operand is less than or equal to the second operand

**IMPORTANT:** Relational operators do only work with exactly two operands!

The following program tests the relational operators:
```Lisp
;Listing 002: Exemplary use of relational operators
(int value1 1)
(int value2 2)

;Test = operator
(if (= value1 value2) (
    (println "value1 = value2")
))
;Test ! operator
(if (! value1 value2) (
    (println "value1 ! value2")
))
;Test > operator
(if (> value1 value2) (
    (println "value1 > value2")
))
;Test < operator
(if (< value1 value2) (
    (println "value1 < value2")
))
;Test >= operator
(if (>= value1 value2) (
    (println "value1 >= value2")
))
;Test <= operator
(if (<= value1 value2) (
    (println "value1 <= value2")
))
```
The program prints the following:
```
value1 ! value2
value1 < value2
value1 <= value2
```

<br/>

***

## Boolean operations: <a name="boolean"></a>

The `&` and `|` operators perform _Conditional-AND_ and _Conditional-OR_ operations on multiple boolean expressions:
Operator | Algebraic Symbol | Compatible Data Types | Description
--- | --- | --- | ---
`&` | ∧ | `bool` | Conditional-AND
`\|` | ∨ | `bool` | Conditional-OR

**IMPORTANT:** Boolean operators work with an unlimited number of operands!

The following program tests the boolean operators:
```Lisp
;Listing 003: Exemplary use of boolean operators
(int value1 1)
(int value2 2)

;Test & operator
(if (& (= value1 1) (= value2 2)) (
    (println "value1 = 1 AND value2 = 2")
))

;Test | operator
(if (| (= value1 1) (= value2 1)) (
    (println "value1 = 1 OR value2 = 1")
))
```
The program prints the following:
```
value1 = 1 AND value2 = 2
value1 = 1 OR value2 = 1
```
