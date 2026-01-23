(defun c:ExportarTracado (/ ent obj i p1 p2 bulge raio comp azim arquivo)
  (vl-load-com)
  (if (setq ent (car (entsel "\nSelecione a Polilinha do eixo: ")))
    (progn
      (setq obj (vlax-ename->vla-object ent)
            i 0
            arquivo (open "C:/temp/tracado.txt" "w")) ; Ajuste o caminho
      
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
