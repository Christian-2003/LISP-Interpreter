# Operations:
The implemented LISP dialect is capable of evaluating a variety of different operations. Operations take place in a single list, in which the operator resembles the first element and the other elements make up the operands.

<br/>

## Table of contents:
1. [Arithmetic operations](#arithmetic)
2. [Relational operations](#relational)
3. [Boolean operations](#boolean)

<br/>
<br/>
<br/>

***

<br/>

# Arithmetic operations <a name="arithmetic"></a>

Arithmetic operations such as _7 + 15_ are translated into LISP through the [polish notation](https://en.wikipedia.org/wiki/Polish_notation) into `(+ 7 15)`. This LISP-expression consists of three atoms (`+`, `7` & `15`). Out of those elements, the first one resembles the operator, and the other elements make up the operands of the operation.
<br/>
You can add another operand to the earlier mentioned operation, for instance `(+ 7 5 -3)`, which translates to _7 + 5 + (-3)_. Arithmetic operations work with as many operands as needed.
<br/>
Arithmetic operations can be encapsulated, which allows the user to create operations with different operands. For this, another list (which resembles another operation) needs to be inserted into the operation as operand: `(+ 7 15 (* 2 5) -3)` (translates to _7 + 15 + 2 * 5 + (-3)_).

**IMPORTANT:** Arithmetic operations do only work with numeric values.

The result of such operations can be incorporated into the sourcecode in many different ways:
```Lisp
;Print the result:
(print (+ 7 15))

;Initialize variable with the result:
(int VAR (- 25 17))

;...
```

<br/>

## Available operators:
Operator | Usage | Compatible data types | Restrictions
--- | --- | --- | ---
`+` | Addition | `int`, `double`, `char`, `string` | -
`-` | Subtraction | `int`, `double` | -
`*` | Multiplication | `int`, `double`| -
`/` | Division | `int`, `double` | _No known division through 0._

<br/>
<br/>
<br/>

***

<br/>

# Relational operations <a name="relational"></a>

Relational operations compare always two values. If the condition of the operator is considered to be _true_, the value `T` is returned. If the condition of the operator is considered to be _false_, the value `NIL` is returned.

<br/>

## Available operators:
Operator | Mathmatical symbol | Condition | Compatible data types | Restrictions
--- | --- | ---| --- | ---
`=` | = | ... is equal to ... | `int`, `double`, `bool`, `char`, `string` | -
`!` | ≠ | ... is not equal to ... | `int`, `double`, `bool`, `char`, `string` | -
`>` | > | ... is greater than ... | `int`, `double` | -
`<` | < | ... is less than ... | `int`, `double` | -
`>=` | ≥ | ... is greater than or equal to ... | `int`, `double` | -
`<=` | ≤ |... is less than or equal to ... | `int`, `double` | -

<br/>
<br/>
<br/>

***

<br/>

# Boolean operations <a name="boolean"></a>

Boolean operations compares the state of the booleans that are passed.

<br/>

### Available operators:
Operator | Algebraic symbol | Condition | Compatible data types | Restrictions
--- | --- | --- | --- | ---
`&` | ∧ | Every element is `T` | `bool` | -
`\|` | ∨ | At least one element is `T` | `bool` | -
