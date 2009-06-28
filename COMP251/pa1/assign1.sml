fun list_comb [] = []
  | list_comb xs = let
                       fun list ([], l) = []
                         | list (head::nil, []) = []
                         | list (head::tail, []) = list (tail, tl(tail))
                         | list (head::tail, Head::Tail) = (head, Head)::list (head::tail, Tail)
		   in
                       list (xs, tl(xs))
                   end;
fun get_unique [] = []
  | get_unique (Head::Tail) = let
                                  fun filter (e, []) = []
                                    | filter (e, head::tail) = if e = head then filter (e, tail) else (head::filter (e, tail))
                              in
                                  Head::get_unique (filter (Head, Tail))
                              end;
fun max_occur [] = []
  | max_occur x = let
                      val list = get_unique x
                      fun count (e, []) = 0
                        | count (e, head::tail) = if e = head then 1 + count (e, tail) else count (e, tail)
                      fun compare ([], list2) = []
                        | compare (m::nil, list2) = m::count (m, list2)::nil
                        | compare (list1, list2) = if count (hd(list1), list2) < hd(tl(compare (tl(list1), list2))) then compare (tl(list1), list2) else (hd(list1))::(count (hd(list1), list2))::nil
                  in 
                      compare(list, x)
                  end;
datatype vector = vector2D of real * real;
fun add_vectors (vector2D (x1, y1), vector2D (x2, y2)) = vector2D (x1 + x2, y1 + y2);
fun dot_vectors (vector2D (x1, y1), vector2D (x2, y2)) = x1 * x2 + y1 * y2;
datatype polynomial = polynomial of real list;
fun eval_polynomial x (polynomial []) = 0.0
  | eval_polynomial x (polynomial l) = let
                                           fun find_last [] = 0.0
                                             | find_last (head::nil) = head
                                             | find_last (head::tail) = find_last tail
                                           fun find_pre [] = []
                                             | find_pre (head::nil) = []
                                             | find_pre (head::tail) = head::(find_pre tail)
                                       in
                                           find_last l + x * (eval_polynomial x (polynomial (find_pre l)))
                                       end;
fun stringToInt "" = 0
  | stringToInt s = let
                        val list = explode s
                        fun construct #"0" = 0 | construct #"1" = 1
                          | construct #"2" = 2 | construct #"3" = 3
                          | construct #"4" = 4 | construct #"5" = 5
                          | construct #"6" = 6 | construct #"7" = 7
                          | construct #"8" = 8 | construct #"9" = 9
                          | construct _ = 0
                        fun find_last [] = 0
                          | find_last (head::nil) = construct head
                          | find_last (head::tail) = find_last tail
                        fun find_pre [] = []
                          | find_pre (head::nil) = []
                          | find_pre (head::tail) = head::(find_pre tail)
                        fun change [] = 0
                          | change list_ = find_last list_ + 10 * change (find_pre list_)
                     in 
                        change list
                     end;
