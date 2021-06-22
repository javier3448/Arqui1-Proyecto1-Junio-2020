writePointsHeader proc
    dmSetCursor 0, 0
    print headerPointsMsg
    ret
writePointsHeader endp


writeTimesHeader proc
    dmSetCursor 0, 0
    print headerTimesMsg
    ret
writeTimesHeader endp

drawFrame proc
    mDrawRectangle 0, 11, 319, 11, WHITE
    ret
drawFrame endp

graphUsrsPoints proc
    push bp
    mov bp, sp
    sub sp, 12 ;[bp - 2]:  max points, sirve para sacar h, que depende del valor de points de cada barra
               ;[bp - 4]:  w (constante en todas las barra)
               ;[bp - 6]:  x0
               ;[bp - 8]:  y0
               ;[bp - 10]: x1
               ;Y1:        y1 (cosntante)
               ;[bp - 12]: color (parte baja)
               ;Talvez podriamos usar menos variables locales y solo ir turnando los registro pero queda demasiado confuso y propenso a bugs

    call clearGraphingArea ;TODO: PONER OTRA VEZ !!! 

    call usrsGetMaxPoints
    cmp ax, 0
    jnz SKIP_ADJUST
    mov ax, 1
    SKIP_ADJUST:
    mov [bp - 2], ax

    mov word ptr[bp - 4], 320
    fild word ptr[bp - 4]

    call usrsCount
    mov [bp - 4], ax
    fild word ptr[bp - 4]

    fdiv ;320/usrsCount
    
    ;Sacamos el floor de 320/usrsCount
    fst realBuffer;w = 320/usrsCount
    mov eax, realBuffer
    and eax, 080000000h
    xor eax, 0BEFFFFFFh
    mov realBuffer, eax
    fadd realBuffer
    fistp word ptr[bp - 4];w = floor(320/usrsCount)

    mov si, offset usrs
    xor bx, bx ;bx va a contener el indice del usuario en el array, sirve para sacar el x0 y x1
    assume si : ptr USR
    LOOP_I:
        cmp [si]._name[0], 0 
        je RETURN
        
        mov ax, [bp - 4];ax = w
        mul bx;ax = indice * w
        mov [bp - 6], ax;x0 = indice * w
        add ax, [bp - 4];ax = x0 + w
        dec ax;ax = x0 + w - 1
        mov [bp - 10], ax

        mov word ptr[bp - 8], 185
        fild word ptr[bp - 8]
        fild word ptr[si].points
        fild word ptr[bp - 2]
        fdiv
        mov word ptr[bp - 8], 168
        fild word ptr[bp - 8]
        fmul
        fsub
        fistp word ptr[bp - 8]

        mov ax, [si].points
        call getBarColorAndFrequency 
        ;ax tienen la frecuencia, cl tiene el color
        
        mDrawBar [bp - 6], [bp - 8], [bp - 10], y1Constant, cl        
        mWriteNumberInPixel [bp - 6], y1Constant + 1, [si].points

        mUpdateTime
        mNoteOn ax ;TODO TODO TODO!!!!: PONER OTRA VEZ !!! 
        cmp delayVar, 0;Se salta el sonido si no hay delay
        je CONTINUE_I
        ;[ !!! ]
        mDelay delayVar

    CONTINUE_I:
        add si, S_USR
        inc bx
        cmp si, offset usrs + S_USR * L_USRS
    jb LOOP_I

    RETURN:
    mNoteOff
    mov sp, bp;hella safe
    pop bp
    ret
graphUsrsPoints endp

graphUsrsTimes proc
    push bp
    mov bp, sp
    sub sp, 12 ;[bp - 2]:  max times, sirve para sacar h, que depende del valor de times de cada barra
               ;[bp - 4]:  w (constante en todas las barra)
               ;[bp - 6]:  x0
               ;[bp - 8]:  y0
               ;[bp - 10]: x1
               ;Y1:        y1 (cosntante)
               ;[bp - 12]: color (parte baja)
               ;Talvez podriamos usar menos variables locales y solo ir turnando los registro pero queda demasiado confuso y propenso a bugs

    call clearGraphingArea ;TODO: PONER OTRA VEZ !!! 

    call usrsGetMaxTime
    cmp ax, 0
    jnz SKIP_ADJUST
    mov ax, 1
    SKIP_ADJUST:
    mov [bp - 2], ax

    mov word ptr[bp - 4], 320
    fild word ptr[bp - 4]

    call usrsCount
    mov [bp - 4], ax
    fild word ptr[bp - 4]

    fdiv ;320/usrsCount
    
    ;Sacamos el floor de 320/usrsCount
    fst realBuffer;w = 320/usrsCount
    mov eax, realBuffer
    and eax, 080000000h
    xor eax, 0BEFFFFFFh
    mov realBuffer, eax
    fadd realBuffer
    fistp word ptr[bp - 4];w = floor(320/usrsCount)

    mov si, offset usrs
    xor bx, bx ;bx va a contener el indice del usuario en el array, sirve para sacar el x0 y x1
    assume si : ptr USR
    LOOP_I:
        cmp [si]._name[0], 0 
        je RETURN
        
        mov ax, [bp - 4];ax = w
        mul bx;ax = indice * w
        mov [bp - 6], ax;x0 = indice * w
        add ax, [bp - 4];ax = x0 + w
        dec ax;ax = x0 + w - 1
        mov [bp - 10], ax

        mov word ptr[bp - 8], 185
        fild word ptr[bp - 8]
        fild word ptr[si].time
        fild word ptr[bp - 2]
        fdiv
        mov word ptr[bp - 8], 168
        fild word ptr[bp - 8]
        fmul
        fsub
        fistp word ptr[bp - 8]

        mov ax, [si].time
        call getBarColorAndFrequency 
        ;ax tienen la frecuencia, cl tiene el color
        
        mDrawBar [bp - 6], [bp - 8], [bp - 10], y1Constant, cl        
        mWriteNumberInPixel [bp - 6], y1Constant + 1, [si].time

        mUpdateTime
        mNoteOn ax ;TODO TODO TODO!!!!: PONER OTRA VEZ !!! 
        cmp delayVar, 0;Se salta el sonido si no hay delay
        je CONTINUE_I
        ;[ !!! ]
        mDelay delayVar

    CONTINUE_I:
        add si, S_USR
        inc bx
        cmp si, offset usrs + S_USR * L_USRS
    jb LOOP_I

    RETURN:
    mNoteOff
    mov sp, bp;hella safe
    pop bp
    ret
graphUsrsTimes endp

;Params:  ax: valor de 0 a 99
;returns: cl: el color correponiente
;         ax: la frequencia
getBarColorAndFrequency proc
    cmp ax, 20
    jbe T_RED

    cmp ax, 40
    jbe T_BLUE

    cmp ax, 60
    jbe T_YELLOW

    cmp ax, 80
    jbe T_GREEN

    T_WHITE:
    mov ax, 900
    mov cl, WHITE
    ret

    T_RED:
    mov ax, 100
    mov cl, RED
    ret

    T_BLUE:
    mov ax, 300
    mov cl, BLUE
    ret

    T_YELLOW:
    mov ax, 500
    mov cl, YELLOW
    ret

    T_GREEN:
    mov ax, 700
    mov cl, GREEN
    ret
getBarColorAndFrequency endp

;Si no se siente el delay solo usar regs extendidos
;Params:  ax: numero de 0 a 9
;returns: cl: el color correponiente
;         ax: la frequencia   
delay proc
    mov dx, delayMultiplier
    mul dx ;ax = ax * delayConstant
    test ax, ax;revisamos si ax es 0 retornamos para que no ocurra overflow
    jz RETURN
    mov dx, 07fffh

    mov si,ax
    D1:
    dec si
    jz RETURN
    mov di, dx
    
    D2:
    dec di
    jnz D2
    jmp D1

    RETURN:
    ret
delay endp

clearGraphingArea proc    
    mov si, 12 * 320
    mov di, 320 * 200
    mov cl, BLACK
    call fillExtendedStringByRange
    ret
clearGraphingArea endp

;El ejemplo de herber [...] min 34:06 laboratorio 3 esta bien diferente
;params:     si: frecuencia
noteOn proc
    out 42h, al
    mov al, ah
    out 42h, al
    
    mov al, 61h
    mov al, 11b
    out 61h, al
    ret
noteOn endp

noteOff proc
    mov al, 61h
    out 61h, al
    ret
noteOff endp

updateTime proc
    call getMonthSecs ;eax = currTime
    mov edx, initialTime
    sub eax, edx;el resultado seguro cabe en 16bits
    cmp ax, 60 * 0ffh;Si eax contiene un numero cuya division tenga un cociente mayor a 0ffh no hacemos nada para evitar el error de division
    ja RETURN

    mov dl, 60
    div dl;al = quotient, ah = remainder
    mByteToDecAscii offset decBuffer, al
    mCopyString offset decBuffer + 1, offset headerMin, 2
    
    mByteToDecAscii offset decBuffer, ah
    mCopyString offset decBuffer + 1, offset headerSec, 2

    dmSetCursor 24, 0
    print headerMin

    RETURN:
    ret
updateTime endp


;TODO TODO: Probar
;[ !!! ] No actualiza el valor de la Ram
printTopPoints proc
    push bp
    mov bp, sp
    sub sp, 4 ;[bp - 2]: handle
    newFile topPointsTitle
    mov [bp - 2], ax
    writeContent [bp - 2], lengthof frameLine - 1, frameLine
    print frameLine
    writeContent [bp - 2], lengthof topPointsMsg - 1, topPointsMsg
    print topPointsMsg

    mov bx, offset usrs
    mov word ptr[bp - 4], 1
    LOOP_I:
        cmp [bx]._name[0], 0 ;isNull
        je BREAK_I

        call clearTopUsrBufferName

        mByteToDecAscii offset decBuffer, byte ptr[bp - 4]
        mCopyString offset decBuffer + 1, offset topUsrBuffer, 2

        lea dx, [bx]._name ;creo que este es el unico caso donde no podemos usar un offset envez de lea
        lea si, [bx]._name
        call cStringLength ;ax = lenght of name
        mCopyString dx, offset topUsrBufferName, ax

        mov ax, bx
        add ax, offset  USR.level
        mov dx, offset topUsrBufferLevel
        mCopyString ax, dx, lengthof USR.level

        mov ax, [bx].points
        mByteToDecAscii offset decBuffer, al
        mCopyString offset decBuffer, offset topUsrBufferValue, 3

        push bx
        writeContent [bp - 2], L_TOP_USR_BUFFER - 1,  topUsrBuffer
        print topUsrBuffer
        pop bx
    CONTINUE:
    inc word ptr[bp - 4]
    add bx, S_USR
    cmp bx, offset usrs + S_USR * 10
    jb LOOP_I
    

    BREAK_I:
    writeContent [bp - 2], lengthof frameLine - 1, frameLine
    print frameLine
    closeFile [bp - 2]

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
printTopPoints endp

;TODO TODO: Probar
;[ !!! ] No actualiza el valor de la Ram
printTopTimes proc
    push bp
    mov bp, sp
    sub sp, 4 ;[bp - 2]: handle
              ;[bp - 4]: 1 a 10
    newFile topTimesTitle
    mov [bp - 2], ax
    writeContent [bp - 2], lengthof frameLine - 1, frameLine
    print frameLine
    writeContent [bp - 2], lengthof topTimesMsg - 1, topTimesMsg
    print topTimesMsg

    mov bx, offset usrs
    mov word ptr[bp - 4], 1
    LOOP_I:
        cmp [bx]._name[0], 0 ;isNull
        je BREAK_I

        call clearTopUsrBufferName

        mByteToDecAscii offset decBuffer, byte ptr[bp - 4]
        mCopyString offset decBuffer + 1, offset topUsrBuffer, 2

        lea dx, [bx]._name ;creo que este es el unico caso donde no podemos usar un offset envez de lea
        lea si, [bx]._name
        call cStringLength ;ax = lenght of name
        mCopyString dx, offset topUsrBufferName, ax

        mov ax, bx
        add ax, offset  USR.level
        mov dx, offset topUsrBufferLevel
        mCopyString ax, dx, lengthof USR.level

        mov ax, [bx].time
        mByteToDecAscii offset decBuffer, al
        mCopyString offset decBuffer, offset topUsrBufferValue, 3

        push bx
        writeContent [bp - 2], L_TOP_USR_BUFFER - 1,  topUsrBuffer
        print topUsrBuffer
        pop bx
    CONTINUE:
    inc word ptr[bp - 4]
    add bx, S_USR
    cmp bx, offset usrs + S_USR * 10
    jb LOOP_I
    

    BREAK_I:
    writeContent [bp - 2], lengthof frameLine - 1, frameLine
    print frameLine
    closeFile [bp - 2]

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
printTopTimes endp

clearTopUsrBufferName proc
    push bp
    mov bp, sp
    
    mov si, offset topUsrBuffer
    mov al, ' '
    mov cx, lengthof topUsrBufferName
    call fillString
    
    mov sp, bp;hella safe
    pop bp
    ret
clearTopUsrBufferName endp
