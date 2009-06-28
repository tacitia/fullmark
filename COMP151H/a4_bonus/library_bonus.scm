(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- 0 x) x)))
(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))
(define list (lambda args args))
(define for-each (lambda a (define for (lambda (f l) (if (nullp l) (quote ()) (apply f (car l))) (if (nullp l) (quote ()) (for f (cdr l))))) (if (nullp a) (quote ()) (if (nullp (cdr a)) (quote ()) (for (car a) (cdr a))))))

