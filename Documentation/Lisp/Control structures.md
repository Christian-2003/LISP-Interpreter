# Control Flow statements: ![](https://img.shields.io/github/size/ChosenChris/LISP-Interpreter/Documentation/Lisp/Control%20structures.md?label=File%20size)
> The Lisp Tutorials have been written for version _alpha-0.4_. Examples and practices that are described in this document may not be up to date.

<br/>

### Table of contents:
1. [Overview](#overview)
2. [`if` and `if-else` Statement](#if)

<br/>
<br/>
<br/>

***

## Overview <a name="overview"></a>

The statements inside your sourcecode are generally executed from top to bottom, in the order they appear. In order to break up the flow of execution, **Control Flow statements** employ decision making, looping and branching, enabling your program to _conditionally_ execute particular parts of code.

<br/>

***

## `if` and `if-else` Statement <a name="if"></a>

### The `if` Statement
The `if` statement is the most basic of all control statements. It tells your program to execute a particular part of your code **only if** a certain condition evaluates to _true_ (or `T`, as it would be called in Lisp). For example, a chewing gum machine could allow a chewing gum to be dispensed _only_ if there is a certain amount of money inserted into the machine. One possible implementation for such an algorith, could be as follows:
```Lisp
;Listing 001: Chewing gum machine
(double amountOfMoney)
(set amountOfMoney 0.5) ;The inserted amount of money is changed to 0.50€

;The if clause: inserted amount of money must be greater than or equal to 1.0€
(if (>= amountOfMoney 1.0) (
    ;Body of the if clause: Dispense chewing gum
    (println "Dispensed chewing gum.")
))
```
If this condition evaluates to _false_ (or `NIL`, as it would be called in Lisp), which means that not enough money was inserted into the machine, the interpreter jumps to the end of the `if` statement.

In addition, encapsulating statements into a list is optional, but only if there is exactly one statement in the body:
```Lisp
;Listing 002: Chewing gum machine
(double amountOfMoney)
(set amountOfMoney 0.5)

;Same as in Listing 001, but without encapsulated statements
(if (>= amountOfMoney 1.0) (println "Dispensed chewing gum."))
```
Deciding when to use encapsulated statements is a matter of personal taste. Making use of encapsulated statements may make the sourecode easiert to read and understand.

<br/>

### The `if-else` Statement
The `if-else` statement provides a secondary path of execution when the condition of an `if` clause evaluates to _NIL_. You can use an `if-else` statement to print an error message when the inserted amount of money is not large enough:
```Lisp
;Listing 003: More advanced chewing gum machine
(double amountOfMoney)
(set amountOfMoney 0.5)

(if (>= amountOfMoney 1.0) (
    (println "Dispensed chewing gum.")
)(
    ;Body of the else clause: Prints error message
    (println "Error: Not enough money inserted!")
))
```
As before, you can choose not to encapsulate the statement of the else clause, but only if there is a single statement:
```Lisp
;Listing 004: More advanced chewing gum machine
(double amountOfMoney)
(set amountOfMoney 0.5)

;Same as in Listing 003, but without encapsulated statements
(if (>= amountOfMoney 1.0) (println "Dispensed chewing gum.") (println "Error: Not enough money inserted!"))
```

<br/>

The following program utilizes encapsulated `if` and `if-else` statements in order to give a better feedback to the user:
```Lisp
;Listing 005: Most advanced chewing gum machine
(double amountOfMoney)
(set amountOfMoney 0.5)

(if (>= amountOfMoney 1.0) (
    (println "Dispensed chewing gum.")
    (if (> amountOfMoney 1.0) (
        (println "Change: " (- amountOfMoney 1.0))
    ))
)(
    (println "Error: Not enough money inserted!")
))
```

<br/>
<br/>
<br/>

***
