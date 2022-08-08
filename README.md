# Simple Calculator version 3.0

## By SHI Yifan

## Instructions

### *MENU*
Choose to use chain or sequence structure for storage of data.
User should enter the code name of preferred mode (1 or 2).

### *GENERAL RULES*
A valid command is IN A LINE, with a valid command head and several parameters.

Spaces are needed among parameters, without spaces the parameters will be considered as a whole.

If there are more-than-needed parameters, the system will give up the rear ones.
It should be noted that in "NEW" command, the definition of a vector should always be in the next line.

Command like "NEW vec 3 1 2 3" will NOT be considered as "vec:=[1, 2, 3]".
The system will wait for the explicit definition.

### *ARITHMETICS RULES*

A valid arithmetics calculation should be IN ONE LINE.

Normal arithmetics calculation has the same priority and syntax as python.

Be aware, users are advised to use the natural expression method to avoid mistakes.

Assigning value to a variable should add '|' at the back of expression.
The value of variable should be an instant value.

Type "TEST" command to have a view of testing points.
There are three error types: SYNTAX ERROR, MATH ERROR and VOID ERROR.

### *FUNCTION RULES*

Using "LET" command to assign a function expression. E.g. LET f(x,y_)=x+y_

Using "CLEAR" command to clear a function definition, and remove it from the database. E.g. CELAR f

A valid function starts with '_' or numbers or alphabets.

To calculate the function, just use them naturally. E.g. f(2)*3+g(f(2))

### *LINEAR ALGEBRA*

Using "NEWMAT" command to assign a matrix. E.g. NEWMAT mat 2 3

Using "DELMAT" command to delete a matrix definition, and remove it from the database.

A valid matrix definition should be exactly the same m*n with the "NEWMAT" command's arguments.

Using "DET" command to calculate the determinant value of a SQUARE matrix. E.g. DET mat1

Using "ADDMAT", "SUBMAT", "MULTIMAT" to calculate arithmetics of matrix.

### *VECTOR COMMANDS*

#### Part1 - Vector Commands

Create a new vector with a dimension of dim and name it "vectorName":

NEW vectorName dim

Then input the numbers (integers or float numbers) in A LINE.

Delete a vector called "vectorName", and free the storage space:

DELETE vectorName

Show a vector called "vectorName":

SHOW vectorName

Add operation between same-dimension vectors "vector1" and "vector2" [Optional: and save the value as "value"]:

ADD vector1 vector2 [value]

If a vector called "value" is existed, then the operating value will NOT be stored as "value".

Sub operation between same-dimension vectors "vector1" and "vector2" 

[Optional: and save the value as "value"]:

SUB vector1 vector2 [value]

If a vector called "value" is existed, then the operating value will NOT be stored as "value".

Solve the cosine value of the intersection angle of two same-dimension vectors "vector1" and "vector2":

COS vector1 vector2

#### Part2 - Polynomial Commands

Create a polynomial called "polyName", whose highest degree is deg:

NEWPOLY polyName deg

Then input the coefficient of each term in A LINE, from the zero degree to the highest one.

If there is a term whose coefficient is zero, the ZERO should also be INCLUDED.

Show the polynomial called "polyName":

SHOW polyName

Add two polynomial called "poly1" and "poly2" [Optional: and save the value as "poly3"]:

ADDPOLY poly1 poly2 [poly3]

Sub two polynomial called "poly1" and "poly2" [Optional: and save the value as "poly3"]:

SUBPOLY poly1 poly2 [poly3]

Multiply two polynomial called "poly1" and "poly2" [Optional: and save the value as "poly3"]:

MULTIPOLY poly1 poly2 [poly3]

Solve the nth derivative of polynomial called "poly" [Optional: and save the value as "value"]:

DSOLVE poly n [value]

#### Part3 - General Commands

Show all the elements in storage:

SHOWALL

Have a view of testing point of arithmetic system:

TEST

Quit the calculator system:

QUIT
