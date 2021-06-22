;Return:
;CH = hour
;CL = minute
;DH = second
;DL = 1/100 seconds
mGetTime  macro
    mov ah, 2ch
    int 21h
endm

;Return:
;CX = year (1980-2099)
;DH = month
;DL = day
mGetDate macro
    mov ah, 2ah
    int 21h
endm

;retorna eax con el numero de segundos transcurridos desde
;el inicio del mes
;[!] le hace back up a los registro de 16 bits solamente aunque retorne
;el resultado en uno de 32
mGetMonthSecs macro
    pushaButAx

    call getMonthSecs

    pushaButAx
endm


