; ZF set if no character available
; AL = 00h
; ZF clear if character available
; AL = character read
; Notes: ^C/^Break are NOT checked. If the returned character is 00h, the user pressed a key with an extended keycode, which will be returned by the next call of this function. This function reads from standard input, which is always the keyboard under DOS 1.x, but may be redirected under DOS 2+. Although the return of AL=00h when no characters are available is not documented, some programs rely on this behavior
myDirectConsoleInput proc
    mov ah, 06h
    mov dl, 0ffh
    int 21h
    jz RETURN;if no character is available
    cmp al, 00
    jnz RETURN;character available and is not extended
    mov ah, 06h
    mov dl, 0ffh
    int 21h;we call the same function so we return the extended keycode
    RETURN:
    ret
myDirectConsoleInput endp

play proc
    push bp
    mov bp, sp
    sub sp, 0

    ;Reseteamos juego
    call gameReset
    ;Nota: el dibujo del fondo es un poco mas grande que el hitbox gameArea
    mDrawRectangle 16 - 2, 16, 320 - 16 + 2, 200 - 8, GRAY ;DrawBackGround
    call gameDrawAll;dibuja por primera vez todos los gameObj

    GAME_LOOP:
    ;beg checkPausa:
    call mydirectConsoleInput
    cmp al, 1bh ;ASCII: ESC
    je ENTER_PAUSE
    ;end checkPausa

    CONTINUE_GAME:
    call playerUpdate;al todavia tienen el resultado de mydirectConsoleInput
    call gameUpdateTime
    call blocksTryUpdate
    jnz EXIT_GAME
    call spawnerUpdate
    call levelManagerTryUpdate
    jnz EXIT_GAME

    jmp GAME_LOOP

    ENTER_PAUSE:
    call getMonthSecs
    mov pauseInitialTime, eax

    _PAUSE:
    cmdGetCharNoEcho
    cmp al, ' '
    je EXIT_GAME
    cmp al, 1bh ;ASCII: ESC
    jne _PAUSE
    ;Presiono ESC entonces esto que sigue es exit pause
    call getMonthSecs
    sub eax, pauseInitialTime;El resultado tiene que caber en 16 bits
    add pauseTotalTime, ax
    jmp CONTINUE_GAME

    EXIT_GAME:
    ;Save score, uploaded it to file and all that stuff
    mUsrTryAddPoints offset loggedInUsr, playerPoints
    mUsrTryAddTime offset loggedInUsr, playerTime
    mUsrTryAddLevel offset loggedInUsr, offset playerLevel
    mExitVideoMode

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
play endp

gameDrawAll proc
    call playerDrawSprite
    call blocksDrawSprites
    
    ; mDrawRectangle 24, 24, 24 + 15, 24 + 15, DARK_GREEN
    ; mDrawRectangle 24 + 3, 24 + 3, 24 + 15 - 3, 24 + 15 - 3, GREEN
    
    ; mDrawRectangle 24 + 32, 24, 24 + 32 + 15, 24 + 15, ORANGE
    ; mDrawRectangle 24 + 3 + 32, 24 + 3, 24 + 32 + 15 - 3, 24 + 15 - 3, YELLOW

    ret
gameDrawAll endp

gameReset proc
    mov playerTime, 0
    mov playerPoints, 3
    mCopyString offset levels[0]._name, offset playerLevel, lengthof USR.level
    mov playerHitbox.x0, gameInitialXPosition
    mov playerHitbox.y0, gameInitialYPosition

    mov pauseTotalTime, 0 

    call getMonthSecs
    mov gameInitialTime, eax
    mov levelInitTime, eax

    ;reset edd de blocks
    call blocksReset
    call spawnerReset

    ;RESET LEVEL POINTER
    mov currLevel, offset levels

    ;hacer el header inicial
    call gameUpdateHeaderUsr
    call gameUpdateHeaderLevel
    call gameUpdateHeaderPoints
    call gameUpdateTime

    ret
gameReset endp

gameUpdateHeaderUsr proc
    mov si, offset gameHeaderUsr
    mov al, ' '
    mov cx, L_MAX_NAME
    call fillString

    mov si, offset loggedInUsr
    mov di, offset gameHeaderUsr
    call copyCString

    dmSetCursor 0, 0
    print gameHeaderUsr

    RETURN:
    ret
gameUpdateHeaderUsr endp

gameUpdateHeaderLevel proc
    assume si : ptr LEVEL
    mov si, currLevel

    dmSetCursor 15, 0
    ;Chapuz horrible: Para que no imprima de mas
    movzx dx, [si]._name[7]
    push dx
    mov [si]._name[7], "$";Para que no imprima de mas
    print [si]._name
    pop dx
    mov [si]._name[7], dl

    RETURN:
    ret
gameUpdateHeaderLevel endp

gameUpdateHeaderPoints proc
    mov al, byte ptr [playerPoints]
    mByteToDecAscii offset gameHeaderPoints, al
    
    dmSetCursor 25, 0
    print gameHeaderPoints
    
    ret
gameUpdateHeaderPoints endp

;De una vez actualiza el headerTime
gameUpdateTime proc
    ;CurrTime - inicioJuego - tiempoEnPausa
    call getMonthSecs
    sub eax, gameInitialTime
    movzx edx, pauseTotalTime
    sub eax, edx

    ;El resultado tiene que caber en 16 bits
    cmp ax, 60 * 0ffh;Si eax contiene un numero cuya division tenga un cociente mayor a 0ffh no hacemos nada para evitar el error de division
    ja RETURN;NO REPORTAMOS ESTE ERROR FATAL!

    mov playerTime, ax

    mov dl, 60
    div dl;al = quotient, ah = remainder
    mByteToDecAscii offset decBuffer, al
    mCopyString offset decBuffer + 1, offset gameHeaderMinutes, 2
    
    mByteToDecAscii offset decBuffer, ah
    mCopyString offset decBuffer + 1, offset gameHeaderSeconds, 2

    dmSetCursor 35, 0
    print gameHeaderMinutes

    RETURN:
    ret
gameUpdateTime endp

;params:   [bp + 4]: ptr to hitbox
;          [bp + 6]: ptr to hitbox
;returns:  ZF = 1 Hay colision
;          ZF = 0 no hay colision
hitboxCollides proc
    push bp
    mov bp, sp

    assume si : ptr HITBOX
    assume di : ptr HITBOX

    mov si, [bp + 4]
    mov di, [bp + 6]

    mov ax, [si].x0
    add ax, [si]._width
    mov bx, [di].x0
    cmp ax, bx
    jle RETURN_FALSE
    
    mov ax, [si].x0
    mov bx, [di].x0
    add bx, [di]._width
    cmp ax, bx
    jge RETURN_FALSE

    mov ax, [si].y0
    add ax, [si]._height
    mov bx, [di].y0
    cmp ax, bx
    jle RETURN_FALSE
    
    mov ax, [si].y0
    mov bx, [di].y0
    add bx, [di]._height
    cmp ax, bx
    jge RETURN_FALSE

    ;Si no se cumple ningun caso retornar verdadero

RETURN_TRUE:
    setZF
    jmp RETURN

RETURN_FALSE:
    clearZF

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
hitboxCollides endp

;params:   [bp + 4]: ptr to hitbox1
;          [bp + 6]: ptr to hitbox2
;returns:  ZF = 1 Si todo hitbox2 esta adentro de hitbox 1
;          ZF = 0 else
hitboxIsInside proc
    push bp
    mov bp, sp

    assume si : ptr HITBOX
    assume di : ptr HITBOX

    mov si, [bp + 4]
    mov di, [bp + 6]

    mov ax, [si].x0
    mov bx, [di].x0
    cmp ax, bx
    jg RETURN_FALSE
    
    mov ax, [si].y0
    mov bx, [di].y0
    cmp ax, bx
    jg RETURN_FALSE

    mov ax, [si].x0
    add ax, [si]._width
    mov bx, [di].x0
    add bx, [di]._width
    cmp ax, bx
    jl RETURN_FALSE
    
    mov ax, [si].y0
    add ax, [si]._height
    mov bx, [di].y0
    add bx, [di]._height
    cmp ax, bx
    jl RETURN_FALSE

    ;Si no se cumple ningun caso retornar verdadero

RETURN_TRUE:
    setZF
    jmp RETURN

RETURN_FALSE:
    clearZF

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
hitboxIsInside endp

copyHitbox proc
    mov cx, sizeof HITBOX
    add cx, si

    LOOP_I:
    cmp si, cx
    jae RETURN
    mov ax, word ptr[si]
    mov word ptr[di], ax
    add si, 2
    add di, 2
    jmp LOOP_I

    RETURN:
    ret 
copyHitbox endp

;CHAPUZ: EN VEZ DE COPIAR TODO EL drawRectangle podriamos mejor hacer solo
;un wrapper que limite el los y0
;(x0, y0) debe ser menor que (x1, y1)
;Parameters:   [bp + 4]  x0
;              [bp + 6]  y0
;              [bp + 8]  x1
;              [bp + 10] y1
;              cl        color
;comments:     no toma en cuenta si el rectangulo se sale de la pantalla por los lados izq, der. Pasaria comportamiento no definido en cuyo caso
;              no toma en cuenta numero negativos
;              no toma en cuenta si el rectangulo se sale por arriba
drawRectangleInGameArea proc
    push bp
    mov bp, sp

    mov dx, [bp + 6]
    cmp dx, 200 ;si y0 es mayor o igual a 200 es porque esta afuera de la pantalla
    jae RETURN
    cmp dx, gameAreaY0
                ;si y1 es mayor o igual a 200 lo ponemos en 199 para que no dibuje afuera de la pantalla
    jge SKIP_Y0_ADJUSTMENT
    mov dx, 16
    SKIP_Y0_ADJUSTMENT:
    mov ax, 320
    mul dx
    mov bx, ax;bx = y0 * 320

    mov dx, [bp + 10];y1
    cmp dx, gameAreaY0 + gameAreaHeight
                ;si y1 es mayor o igual a 200 lo ponemos en 199 para que no dibuje afuera de la pantalla
    jb SKIP_Y1_ADJUSTMENT
    mov dx, 200 - 8
    SKIP_Y1_ADJUSTMENT:
    mov ax, 320
    mul dx
    mov [bp + 10], ax;y1 = y1 * 320

    mov ax, [bp + 4] ;ax = x0
    mov dx, [bp + 8] ;dx = x1
    
    ;Antes de emprezar a corre el ciclo, lo siguente se tiene que cumplir:
    ;ax:        direccion absoluta de x0 
    ;bx:        direccion absoluta de y0
    ;dx:        direccion absoluta de x1
    ;[bp + 10]: direccion absoluta de y1

    cmp bx, [bp + 10]
    ja RETURN
    cmp ax, dx
    ja RETURN

    LOOP_I:
    mov si, bx 
    add si, ax;si = yCurr + x0
    mov di, bx
    add di, dx;di = yCurr + x1
    inc di; ++di porque las coordenadas de drawRectangle son inclusivas
    call fillExtendedStringByRange

    CONTINUE_I:
    add bx, 320
    cmp bx, [bp + 10]
    ja RETURN
    jmp LOOP_I

RETURN:
    mov sp, bp;HELLA SAFE
    pop bp
    ret 8
drawRectangleInGameArea endp

;Returns: dx random pos
;clobbers:    ax, cx, dx
getRandomBlockPosX proc
    call getRandomWord;ax = random word
    xor dx, dx;ponemos en 0 dx porque la div es dx:ax / cx
    mov cx, gameAreaWidth - 15;CONSTANTE QUEMADA, 15 width de los bloques
    div cx;dx = remainder
    add dx, 16
    ret
getRandomBlockPosX endp
