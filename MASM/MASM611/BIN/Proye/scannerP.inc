;params: [bp + 4] cstring with file name
;return ZF=1 success, ZF=0 fail
;TODO: solo retorna true, no revisa nada
scan proc
    push bp
    mov bp, sp

    assume bx : PLEVEL

    mGetFileContentPtr [bp + 4], offset levelFileBuffer, SIZEOF levelFileBuffer, levelFileBufferLength
    jc RETURN_FALSE

    add levelFileBufferLength, offset levelFileBuffer ;[!]Chapuz levelFileBufferLength tendra la direccion maxima a la que puede llegar si
                                                      ;Se hace el chapuz para que pueda recibir menos de 6 niveles

    mov cx, 6
    mov bx, offset levels
    mov si, offset levelFileBuffer
    LOOP_I:
        cmp si, levelFileBufferLength
        jae SET_LVL_TO_NULL

        mScanNivelKeyword

        mScanLevel
        mov ax, bx
        add ax, offset LEVEL._name
        mCopyString offset scannedLevelBuffer, ax, lengthof USR.level

        mScanNumber
        mov [bx].time, ax

        mScanNumber
        mov [bx].time_enemy, ax

        mScanNumber
        mov [bx].time_prize, ax

        mScanNumber
        neg ax
        mov [bx].points_enemy, ax

        mScanNumber
        mov [bx].points_prize, ax
        
        mScanColor
        mov [bx].color, al

        add bx, sizeof LEVEL
    loop LOOP_I
    jmp RETURN_TRUE

    SET_LVL_TO_NULL:
    mov [bx]._name, 0
    jmp RETURN_TRUE

    RETURN_TRUE:
    setZF
    jmp RETURN

    RETURN_FALSE:
    clearZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 2
scan endp

;chpuz llenamos de " " el scannedLevelBuffer por si viene un nivel de 7 o menos chars
scanLevel proc
    mFillString offset scannedLevelBuffer, lengthof USR.level, ' '
    
    mov di, offset scannedLevelBuffer
    LOOP_I:
    cmp byte ptr[si], 3bh;3bh ascii de ;
    je BREAK_LOOP_I
    cmp byte ptr[si], 0ah;0ah ascii de new line
    je BREAK_LOOP_I 
    mov al, byte ptr[si]
    mov byte ptr[di], al
    inc si
    inc di
    jmp LOOP_I
    
    BREAK_LOOP_I:
    inc si
    ret
scanLevel endp

;""private methods"" retorna en el reg si la siguiente posicion a escanear (se salta el ; o \n):
scanNumber proc
    mov di, si;usamos el di porque es el unico disponible
    xor ax, ax
    LOOP_I:
        cmp byte ptr[di], 3bh;3bh ascii de ;
        je BREAK_LOOP_I
        cmp byte ptr[di], 0ah;0ah ascii de new line
        je BREAK_LOOP_I

        inc ax
        inc di
    jmp LOOP_I
    BREAK_LOOP_I:

    inc di;di + 1 va a apuntar a lo que sigue despues de ; o \n 
    push di;backup al di
    push ax
    push si
    call asciiToSWord;value en ax
    pop di

    mov si, di

    ret
scanNumber endp

;[!]Chapuz como color es lo que va antes del salto de linea revisamos si es uno de esos saltos de linea que necesitan 2 caracteres (0d 0a)
;de ser asi incrementamos si
scanColor proc
    cmp byte ptr[si], 'r'
    je ROJO
    cmp byte ptr[si], 'v'
    je VERDE
    cmp byte ptr[si], 'a'
    je AZUL
    cmp byte ptr[si], 'b'
    je BLANCO


    ROJO:
    mov ax, RED
    add si, 5;length of "rojo" + 1
    cmp byte ptr[si], 0dh
    je ADD1_RETURN
    cmp byte ptr[si], 0ah
    je ADD1_RETURN
    jmp RETURN

    VERDE:
    mov ax, GREEN
    add si, 6;length of "verde" + 1
    cmp byte ptr[si], 0dh
    je ADD1_RETURN
    cmp byte ptr[si], 0ah
    je ADD1_RETURN
    jmp RETURN

    AZUL:
    mov ax, BLUE
    add si, 5;length of "azul" + 1
    cmp byte ptr[si], 0dh
    je ADD1_RETURN
    cmp byte ptr[si], 0ah
    je ADD1_RETURN
    jmp RETURN

    BLANCO:
    mov ax, WHITE
    add si, 7;length of "blanco" + 1
    cmp byte ptr[si], 0dh
    je ADD1_RETURN
    cmp byte ptr[si], 0ah
    je ADD1_RETURN
    jmp RETURN

ADD1_RETURN:
    inc si
RETURN:
    ret

scanColor endp