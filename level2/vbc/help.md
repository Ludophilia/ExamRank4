# GRAMMAR RULES

A set of symbols and production rules, ie, symbols that can be replaced by
other symbols down to the terminals in the formal language
The recursive descent parser translate those productions rules to functions
that closely mirror those grammar rules below.

expr   ::= term ( '+' term )*
term   ::= factor ( '*' factor )*
factor ::= digit | '(' expr ')'
digit  ::= '0' | ... | '9'

## Examples

1 + 1

     ADD(+)
	 /   \
  VAL(1)  VAL(1)

(2 + 3)

     ADD(+)
	 /   \
  VAL(2)  VAL(3)

(3 + 4) * 5

      MUL(*)
      /    \
	ADD(+) VAL(5)
	/   \
VAL(3)  VAL(4)

2 + 3 + 5

     ADD(+)
   /       \
  ADD(+)   VAL(5)
 /     \
VAL(2)  VAL(3)

# Error management: After '+' consumed, there's supposed to be something...

What could possibly go wrong?

	- [o] Empty input: ./vbc ""

	- [o] Missing operand with + or *: ./vbc "+3"; ./vbc "2*".
		- [ ] Expected token after '+' or '*': digit 

	- [o] Missing opening, closing parentheses: ./vbc "4("; ./vbc "3)"
		- [ ] Expected token after '(': '(', digit 
		- [ ] Expected token after ')': ')', null, '+', '*' 

	- [o] Wrong starting character
		- [ ] Expected token at start: digit or '('

	- [o] Foreign symbol: outside ['0'..'9'], '(', ')', '+', '*'...
	- [o] Wrong ending character
		- [ ] Expected token at end: null
		
	- [o] allocation errors
