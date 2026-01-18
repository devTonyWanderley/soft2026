
(defun civil:get_na(msg dir ex f) (getfiled msg dir ex f))

(defun civil:ler-arquivo(chave / na pa ln lt)
  (if (setq na (civil:get_na
		 (cadr (assoc chave #param-leitura))
		 (caddr (assoc chave #param-leitura))
		 (nth 3 (assoc chave #param-leitura))
		 0
		 )
	    )
    (if (setq pa (open na "r"))
      (progn
	(while (setq ln (read-line pa))
	  (setq lt (cons ln lt))
	  )
	(setq pa (close pa))
	)
      (princ "erro no arquivo\n")
      )
    (princ "erro no nome do arquivo\n")
    )
  (if lt
    (reverse lt)
    (princ)
    )
  )

(defun civil:exportar-arquivo(chave lt / na pa i)
  (if (setq na (civil:get_na
		 (cadr (assoc chave #param-leitura))
		 (caddr (assoc chave #param-leitura))
		 (nth 3 (assoc chave #param-leitura))
		 1
		 )
	    )
    (if (setq pa (open na "w"))
      (progn
	(while (nth (setq i (if i (1+ i) 0)) lt)
	  (write-line (nth i lt) pa)
	  )
	(setq pa (close pa))
	)
      (princ "erro no arquivo\n")
      )
    (princ "erro no nome do arquivo\n")
    )
  )

;|CATÁLOGO
	civil:get_na(msg dir ex f)
	civil:ler-arquivo(chave / na pa ln lt)
	civil:exportar-arquivo(chave lt / na pa i)
|;