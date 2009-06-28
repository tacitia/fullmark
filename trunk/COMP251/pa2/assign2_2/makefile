cal1:
	flex -ocal1.lex.yy.c q1/cal1.lex
	bison -d q1/PrefixCal.y
	gcc -o cal1 PrefixCal.tab.c cal1.lex.yy.c

cal2_a:
	flex -ocal2_a.lex.yy.c q2_a/cal2.lex
	bison -d q2_a/InfixToPostfix.y
	gcc -o cal2_a InfixToPostfix.tab.c cal2_a.lex.yy.c

cal2_b:
	flex -ocal2_b.lex.yy.c q2_b/cal2.lex
	bison -d q2_b/PostfixToPrefix.y
	gcc -o cal2_b PostfixToPrefix.tab.c cal2_b.lex.yy.c

clean:
	rm *.c* *.h*
	rm cal1
	rm cal2_a
	rm cal2_b


