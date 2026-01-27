(defun c:DesPerfilPro (/ file line est cota tipo pts p-base p-topo)
  (setq file (open (getfiled "Selecione o arquivo de PERFIL IDENTIFICADO" "" "txt" 0) "r"))
  (if file
    (progn
      (command "_.undo" "_begin")
      (while (setq line (read-line file))
        (if (> (strlen line) 50)
          (progn
            ;; Parsing seguindo o layout 12|16|12|16|12
            (setq est (/ (atof (substr line 1 12)) 10000.0))
            (setq cota (/ (atof (substr line 29 12)) 10000.0))
            (setq tipo (str-trim (substr line 57 12))) ;; Pega PC, PT ou INT
            
            (setq p-base (list est (* cota 10.0)))
            (setq pts (cons p-base pts))

            ;; Desenha a linha vertical de chamada (20m de altura no CAD)
            (setq p-topo (list (car p-base) (+ (cadr p-base) 20.0)))
            (command "_.line" p-base p-topo "")
            
            ;; Escreve Estaca e Cota rotacionados (90 deg)
            (setq txt (strcat tipo " Est: " (rtos est 2 2) " Z: " (rtos cota 2 2)))
            (command "_.text" "_br" p-topo 1.5 90 txt)
          )
        )
      )
      (close file)
      ;; Desenha a polilinha do terreno
      (entmake (append (list '(0 . "LWPOLYLINE") '(100 . "AcDbEntity") '(100 . "AcDbPolyline") 
                             (cons 90 (length pts)) '(70 . 0))
                       (mapcar '(lambda (p) (cons 10 p)) (reverse pts))))
      (command "_.zoom" "_e")
      (command "_.undo" "_end")
    )
  )
  (princ)
)

;; Função auxiliar para limpar espaços do texto
(defun str-trim (str)
  (vl-string-trim " " str)
)
