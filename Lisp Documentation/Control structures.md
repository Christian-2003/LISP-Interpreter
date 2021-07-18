# Control structures:
The implemented LISP dialect is capable of evaluating several important control structures to enhance your programs.

<br/>

## Table of contents:
1. [If-statements](#if)

<br/>
<br/>
<br/>

***

# If-statements: <a name="if"></a>

An if-statement is used to change the path your program is following during it's execution.

If statements consist of multiple elements, the first being the [keyword](https://github.com/ChosenChris/LISP-Interpreter/blob/main/Lisp%20Documentation/Keywords.md#if) that indicates the if-statement. The second element represents the condition of the if-statement. If this condition is `t`, the list of expressions, which is element 3, is executed.

### Example:
```Lisp
(int i 5)
(if (= i 5) (
    (println "i = 5,")
    (println "The condition is true.")
))
```
