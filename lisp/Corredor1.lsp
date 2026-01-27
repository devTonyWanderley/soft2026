(defun c:DesPerfilFinal (/ file line est cota tipo pts p-base p-topo txt)
  (setq file (open (getfiled "Selecione o arquivo de PERFIL" "" "txt" 0) "r"))
  (if file
    (progn
      (command "_.undo" "_begin")
      (setq pts nil)
      (while (setq line (read-line file))
        (if (> (strlen line) 60)
          (progn
            ;; 1. Extração de Dados (Escala 10000)
            (setq est (/ (atof (substr line 1 12)) 10000.0))
            (setq cota (/ (atof (substr line 29 12)) 10000.0))
            (setq tipo (str-trim (substr line 57 12))) ;; Coluna do TIPO
            
            (setq p-base (list est (* cota 10.0)))
            (setq pts (cons p-base pts))

            ;; 2. Lógica de Desenho: SÓ desenha chamada se TIPO não for vazio
            (if (/= tipo "")
              (progn
                ;; Linha Vertical (Pente)
                (setq p-topo (list (car p-base) (+ (cadr p-base) 15.0)))
                (entmake (list '(0 . "LINE") (cons 10 p-base) (cons 11 p-topo) '(62 . 8))) ;; Cor cinza (8)
                
                ;; Texto Rotacionado (Estaca e Cota)
                (setq txt (strcat tipo ": " (rtos est 2 2) " Z:" (rtos cota 2 2)))
                (command "_.text" "_br" p-topo 1.2 90 txt)
              )
            )
          )
        )
      )
      (close file)

      ;; 3. Desenha a Polilinha do Terreno (Fiel aos acidentes)
      (if pts
        (entmake (append (list '(0 . "LWPOLYLINE") '(100 . "AcDbEntity") '(100 . "AcDbPolyline") 
                               (cons 90 (length pts)) '(70 . 0) '(62 . 3)) ;; Cor verde (3)
                         (mapcar '(lambda (p) (cons 10 p)) (reverse pts))))
      )
      (command "_.zoom" "_e")
      (command "_.undo" "_end")
    )
  )
  (princ "\nPerfil processado com sucesso.")
)

;; Auxiliar para remover espaços
(defun str-trim (str)
  (vl-string-trim " " str)
)
