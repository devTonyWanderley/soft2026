
(defun civil:norma_tx(arg / c r)
  (setq r "")
  (while (/= (setq c (substr arg 1 1) arg (substr arg 2) c c) "")
    (if (or (wcmatch c "@") (wcmatch c "#"))
      (setq r (strcat r c))
      )
    )
  r
  )

(defun civil:ler-ln-ponto(arg / r)
  (setq r (list (+ (atof (substr arg 38 4)) (/ (atof (substr arg 43 3)) 1000.0)))
	r (cons (+ (atof (substr arg 27 7)) (/ (atof (substr arg 35 3)) 1000.0)) r)
	r (cons (+ (atof (substr arg 17 7)) (/ (atof (substr arg 24 3)) 1000.0)) r)
	r (cons (civil:norma_tx (substr arg 9 8)) r)
	r (cons (civil:norma_tx (substr arg 1 8)) r)
	)
  )

(defun civil:ler-ln-contorno(arg / i r)
  (while (nth (setq i (if i (1+ i) 0)) arg)
    (setq r (cons (civil:norma_tx (nth i arg)) r))
    )
  (reverse r)
  )

(defun civil:preenche-texto-na-cabeça(tx ch tam)
  (while (< (strlen tx) tam)
    (setq tx (strcat ch tx))
    )
  tx
  )

(defun civil:preenche-texto-na-cauda(tx ch tam)
  (while (< (strlen tx) tam)
    (setq tx (strcat tx ch))
    )
  tx
  )

(defun civil:preenche-num-3dec(num tam)
  (setq num (rtos num 2 3))
  (if (null (wcmatch num "*`.*"))
    (setq num (strcat num "."))
    )
  (while (null (wcmatch num "*`.???"))
    (setq num (civil:preenche-texto-na-cauda num "0" (+ 1 (strlen num))))
    )
  (setq num (civil:preenche-texto-na-cabeça num "0" tam))
  num
  )

(defun civil:ln-ponto-export(arg / id atr x y z)
  (setq id (civil:preenche-texto-na-cabeça (car arg) " " 8)
	atr (civil:preenche-texto-na-cabeça (cadr arg) " " 8)
	x (civil:preenche-num-3dec (car (last arg)) 10)
	y (civil:preenche-num-3dec (cadr (last arg)) 11)
	z (civil:preenche-num-3dec (caddr (last arg)) 8)
	)
  (strcat id atr x y z)
  )

(defun civil:ln-aresta-export(arg)
  (strcat (civil:preenche-texto-na-cabeça (car arg) " " 8) (civil:preenche-texto-na-cabeça (cadr arg) " " 8))
  )

;|CATÁLOGO
	civil:norma_tx(arg / c r)
	civil:ler-ln-ponto(arg / r)
	civil:ler-ln-contorno(arg / i r)
	civil:preenche-texto-na-cabeça(tx ch tam)
	civil:preenche-texto-na-cauda(tx ch tam)
	civil:preenche-num-3dec(num tam)
	civil:ln-ponto-export(arg / id atr x y z)
	civil:ln-aresta-export(arg)
|;