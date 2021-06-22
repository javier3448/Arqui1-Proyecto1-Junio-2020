;DEPRICATED
;params:  al: color a obscurecer, debe ser parte de los colores definidos en colrsD
;returns; al: color obscurecido
; darkenColor proc
;     cmp al, WHITE
;     je IS_WHITE

;     cmp al, GRAY
;     je IS_GRAY

;     cmp al, RED
;     je IS_RED

;     cmp al, GREEN
;     je IS_GREEN

;     cmp al, BLUE
;     je IS_BLUE

;     cmp al, YELLOW
;     je IS_YELLOW

;     ret
    
;     IS_WHITE:
;     mov al, BLACK
;     ret

;     IS_GRAY:
;     mov al, BLACK
;     ret

;     IS_RED:
;     mov al, DARK_RED
;     ret

;     IS_GREEN:
;     mov al, DARK_GREEN
;     ret

;     IS_BLUE:
;     mov al, DARK_BLUE
;     ret

;     IS_YELLOW:
;     mov al, DARK_YELLOW
;     ret
; darkenColor endp