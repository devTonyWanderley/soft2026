;|
(defun c:ExportarTracado (/ ent obj i p1 p2 bulge raio comp azim arquivo)
  (vl-load-com)
  (if (setq ent (car (entsel "\nSelecione a Polilinha do eixo: ")))
    (progn
      (setq obj (vlax-ename->vla-object ent)
            i 0
            arquivo (open "C:/Tony/Soft/soft2026/Instâncias/tracado.txt" "w")) ; Ajuste o caminho C:/Tony/Soft/soft2026/Instâncias
      
      (while (< i (vlax-get obj 'NumberOfVertices))
        (setq p1 (vlax-curve-getPointAtParam obj i)
              p2 (vlax-curve-getPointAtParam obj (1+ i))
              bulge (vla-getbulge obj i))
        
        (if p2 ; Se houver um próximo ponto
          (progn
            (setq comp (vlax-curve-getDistAtParam obj (1+ i))
                  comp (- comp (vlax-curve-getDistAtParam obj i))
                  azim (angle p1 p2))
            
            (if (= bulge 0.0)
              ; ESCREVE RETA: Tipo(5) Comprimento(10) Raio(10) Azimute(10)
              (write-line (strcat "RETA " (rtos comp 2 2) " 0.00 " (rtos azim 2 4)) arquivo)
              
              ; ESCREVE CURVA: Calcula o raio baseado no bulge
              (progn
                (setq raio (/ (/ (distance p1 p2) 2.0) (sin (* 2.0 (atan (abs bulge))))))
                (write-line (strcat "CURVA" (rtos comp 2 2) " " (rtos (abs raio) 2 2) " " (rtos azim 2 4)) arquivo)
              )
            )
          )
        )
        (setq i (1+ i))
      )
      (close arquivo)
      (princ "\nTraçado exportado com sucesso!")
    )
  )
  (princ)
)
|;

;; Função auxiliar corrigida para formatar números com largura fixa
(defun printf-fixed (num width dec / s)
  (setq s (rtos num 2 dec))
  (while (< (strlen s) width) 
    (setq s (strcat " " s))
  )
  s
)

(defun c:ExportarTracado (/ ent i fim p1 p2 bulge raio comp azim arquivo obj)
  (vl-load-com)
  (if (setq ent (car (entsel "\nSelecione a Polilinha do eixo: ")))
    (progn
      (setq obj (vlax-ename->vla-object ent)
            i 0
            fim (vlax-curve-getEndParam ent)
            arquivo (open "C:/Tony/NovaSede/Tracado.txt" "w"))
      
      ;; Escreve o ponto de partida (X Y) para o Qt saber onde o estaqueamento começa
      (setq p1 (vlax-curve-getPointAtParam ent 0))
      (write-line (strcat "INICIO " (rtos (car p1) 2 4) " " (rtos (cadr p1) 2 4)) arquivo)

      (while (< i fim)
        (setq p1 (vlax-curve-getPointAtParam ent i)
              p2 (vlax-curve-getPointAtParam ent (1+ i))
              bulge (vla-getbulge obj i))
        
        (setq comp (- (vlax-curve-getDistAtParam ent (1+ i)) 
                      (vlax-curve-getDistAtParam ent i))
              azim (angle p1 p2))
        
        (if (equal bulge 0.0 1e-6)
          (write-line (strcat "RETA  " (printf-fixed comp 10 4) " 0.00       " (printf-fixed azim 10 4)) arquivo)
          (progn
            (setq raio (abs (/ (/ (distance p1 p2) 2.0) (sin (* 2.0 (atan bulge))))))
            (write-line (strcat "CURVA " (printf-fixed comp 10 4) " " (printf-fixed raio 10 4) " " (printf-fixed azim 10 4)) arquivo)
          )
        )
        (setq i (1+ i))
      )
      (close arquivo)
      (princ "\nArquivo exportado com sucesso em C:/temp/tracado.txt")
    )
  )
  (princ)
)

(defun c:ExpEixo (/ ent sel file path i pt next-pt bulge x-str y-str b-str line)
  (vl-load-com)
  
  ;; 1. Seleção da Polilinha
  (setq sel (entsel "\nSelecione a LWPOLYLINE do eixo: "))
  (if (and sel (= (cdr (assoc 0 (entget (setq ent (car sel))))) "LWPOLYLINE"))
    (progn
      ;; 2. Definição do arquivo de saída
      (setq path (getfiled "Salvar Eixo Horizontal" "" "txt" 1))
      (if path
        (progn
          (setq file (open path "w"))
          (setq i 0)
          (setq obj (vlax-ename->vla-object ent))
          (setq coords (vlax-get obj 'Coordinates))
          (setq num-verts (/ (length coords) 2))

          (while (< i num-verts)
            ;; Extração do Ponto Atual (X Y)
            (setq pt (list (nth (* i 2) coords) (nth (1+ (* i 2)) coords)))
            
            ;; Extração do Bulge do segmento atual
	    ;; Se for o último vértice, não existe bulge (forçamos 0.0)
            (if (< i (1- num-verts))
              (setq bulge (vla-getbulge obj i))
              (setq bulge 0.0)
            )

            ;; 3. Formatação com Comprimento Fixo e Decimais
            ;; X e Y: 15 caracteres total, 4 decimais
            ;; Bulge: 12 caracteres total, 6 decimais
            (setq x-str (rtos (car pt) 2 4))
            (setq y-str (rtos (cadr pt) 2 4))
            (setq b-str (rtos bulge 2 6))

            ;; Montagem da linha com preenchimento (Padding)
            (setq line (strcat 
              (setq x-str (substr (strcat x-str "               ") 1 15))
              (setq y-str (substr (strcat y-str "               ") 1 15))
              (setq b-str (substr (strcat b-str "            ") 1 12))
            ))

            (write-line line file)
            (setq i (1+ i))
          )
          
          (close file)
          (princ (strcat "\nSucesso! " (itoa i) " segmentos exportados."))
        )
      )
    )
    (princ "\nErro: Selecione uma LWPOLYLINE válida.")
  )
  (princ)
)

(defun c:DesPerfil (/ file line est cota pts)
  (setq file (open (getfiled "Selecione o arquivo de PERFIL" "" "txt" 0) "r"))
  (if file
    (progn
      (while (setq line (read-line file))
        (if (> (strlen line) 28)
          (progn
            ;; Lê estaca (0-12) e cota (28-40) convertendo de décimos de mm para metros
            (setq est (/ (atof (substr line 1 12)) 10000.0))
            (setq cota (/ (atof (substr line 29 12)) 10000.0))
            (setq pts (cons (list est (* cota 10.0)) pts))
          )
        )
      )
      (close file)
      (entmake (append (list '(0 . "LWPOLYLINE") '(100 . "AcDbEntity") '(100 . "AcDbPolyline") 
                             (cons 90 (length pts)) '(70 . 0))
                       (mapcar '(lambda (p) (cons 10 p)) (reverse pts))))
      (command "_.zoom" "_e")
    )
  )
  (princ)
)

(defun c:DesSecao (/ file line off cota pts)
  (setq file (open (getfiled "Selecione o arquivo da SEÇÃO" "" "txt" 0) "r"))
  (if file
    (progn
      (while (setq line (read-line file))
        (if (> (strlen line) 20)
          (progn
            ;; Lê offset (0-12) e cota (12-24) convertendo para metros
            (setq off (/ (atof (substr line 1 12)) 10000.0))
            (setq cota (/ (atof (substr line 13 12)) 10000.0))
            (setq pts (cons (list off cota) pts))
          )
        )
      )
      (close file)
      (entmake (append (list '(0 . "LWPOLYLINE") '(100 . "AcDbEntity") '(100 . "AcDbPolyline") 
                             (cons 90 (length pts)) '(70 . 0))
                       (mapcar '(lambda (p) (cons 10 p)) (reverse pts))))
      (princ "\nSeção desenhada com sucesso.")
    )
  )
  (princ)
)
