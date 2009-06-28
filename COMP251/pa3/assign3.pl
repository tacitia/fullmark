length_type([], even).
length_type([_ | T], even) :- length_type(T, odd).
length_type([_ | T], odd) :- length_type(T, even).

triple([], []).
triple([H | T1], [H, H, H | T2]) :- triple(T1, T2).

my_member(X, [X | _]).
my_member(X, [Y | Tail]) :- X \== Y, my_member(X, Tail).
dedup([], []).
dedup([H | T], L) :- dedup(T, L), my_member(H, L), !.
dedup([H | T], [H | L]) :- dedup(T, L).

my_append([], L2, L2).
my_append([H | T], L2, [H | R]) :- my_append(T, L2, R).
first_comb(_, [], []).
first_comb(X, [H | T], [[X, H] | L]) :- first_comb(X, T, L).
list_comb([], []).
list_comb([_], []) :- !.
list_comb([H | T], L) :- first_comb(H, T, L1), list_comb(T, L2), my_append(L1, L2, L), !.

count(_, [], 0).
count(X, [X | L], N) :- count(X, L, N1), N is N1 + 1.
count(X, [H | L], N) :- X \== H, count(X, L, N).
max_occur([], []).
max_occur([H | T], [H, N]) :- count(H, [H | T], N), max_occur(T, []).
max_occur([H | T], [H, N]) :- count(H, [H | T], N), max_occur(T, [_, M]), N >= M, !.
max_occur([_ | T], [H, N]) :- max_occur(T, [H, N]).

cal([0, C], _,  C).
cal([E1, C], X, R1) :- E1 > 0, E2 is E1 - 1, cal([E2, C], X, R2), R1 is (R2 * X).
poly([], _, 0).
poly([[E1, C]|L], X, Value) :- cal([E1, C], X, R1), poly(L, X, R2), Value is R1 + R2.
