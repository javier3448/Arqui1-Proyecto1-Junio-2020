;retorna eax con el numero de segundos transcurridos desde
;el inicio del mes
getMonthSecs proc
    mGetDate
    mov al, dl ;al = dia del mes
    mGetTime   ;ch = hour
               ;cl = minute
               ;dh = second
               ;dl = 1/100 seconds
    dec al ;day = day - 1

    ;performance: Hacer mucho cambio entre fpu y cpu puede afectar performance, no se :/

    ;Ponemos en 0 el timeBuffer porque vamos a usarlo como word ptr para meterlo al fpu
                               ;pero solo usamos su parte baja para meter el dia, la hora, el minuto y el segundo
    mov word ptr[timeBuffer], 0
    mov byte ptr[timeBuffer], al
    fild word ptr[timeBuffer]
    mov timeBuffer, 24 * 60 * 60
    fild timeBuffer
    fmul;st(0) = (day - 1) * (24 * 60 * 60)

    mov word ptr[timeBuffer], 0
    mov byte ptr[timeBuffer], ch
    fild word ptr[timeBuffer]
    mov timeBuffer, 60 * 60
    fild timeBuffer
    fmul;st(0) = (hour) * (60 * 60)
        ;st(1) = (day - 1) * (24 * 60 * 60)

    mov word ptr[timeBuffer], 0
    mov byte ptr[timeBuffer], cl
    fild word ptr[timeBuffer]
    mov timeBuffer, 60
    fild timeBuffer
    fmul;st(0) = (minute) * (60)
        ;st(1) = (hour) * (60 * 60)
        ;st(2) = (day - 1) * (24 * 60 * 60)

    mov word ptr[timeBuffer], 0
    mov byte ptr[timeBuffer], dh
    fild word ptr[timeBuffer];st(0) = (second)
                             ;st(1) = (minute) * (60)
                             ;st(2) = (hour) * (60 * 60)
                             ;st(3) = (day - 1) * (24 * 60 * 60)

    fadd
    fadd
    fadd

    fistp dword ptr[timeBuffer]
    mov eax, dword ptr[timeBuffer]
    ret
getMonthSecs endp