;params:   al: resultado de mydirectConsoleInput
playerUpdate proc
    push bp
    mov bp, sp
    
    cmp al, 4bh ;extended code <- 
    je TRY_MOVE_LEFT
    cmp al, 4dh ;extended: ->
    je TRY_MOVE_RIGHT
    jmp RETURN

    TRY_MOVE_LEFT:
    mov si, offset playerHitbox
    mov di, offset playerTmpHitbox
    call copyHitbox
    sub playerTmpHitbox.x0, 2
    ;verificamos que el movimiento sea posible
    push offset playerTmpHitbox
    push offset gameArea
    call hitboxIsInside
    jnz RETURN

    push playerHitbox.y0
    push playerHitbox.x0
    call playerClearSprite

    sub playerHitbox.x0, 2
    call playerDrawSprite
    jmp RETURN

    TRY_MOVE_RIGHT:
    mov si, offset playerHitbox
    mov di, offset playerTmpHitbox
    call copyHitbox
    add playerTmpHitbox.x0, 2
    ;verificamos que el movimiento sea posible
    push offset playerTmpHitbox
    push offset gameArea
    call hitboxIsInside
    jnz RETURN

    push playerHitbox.y0
    push playerHitbox.x0
    call playerClearSprite

    add playerHitbox.x0, 2
    call playerDrawSprite
    jmp RETURN

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
playerUpdate endp

playerDrawSprite proc
    ;ax: x0
    ;bx: y0
    ;cx: x1
    ;dx: y1
    ;si: curr level

    assume si : PLEVEL
    mov si, currLevel

    ;BEG LLANTAS
    mov ax, playerHitbox.x0
    mov bx, playerHitbox.y0
    mov cx, playerHitbox.x0
    mov dx, playerHitbox.y0
    sub ax, 2
    add bx, 5
    sub cx, 1
    add dx, 10
    mDrawRectangle ax, bx, cx, dx, BLACK
    
    add ax, 22
    add cx, 22
    mDrawRectangle ax, bx, cx, dx, BLACK

    add bx, 17
    add dx, 17
    mDrawRectangle ax, bx, cx, dx, BLACK
    
    sub ax, 22
    sub cx, 22
    mDrawRectangle ax, bx, cx, dx, BLACK

    ;END LLANTAS

    ;BEG Carro
    add ax, 4
    sub bx, 22
    add cx, 18
    sub dx, 27
    mDrawRectangle ax, bx, cx, dx, [si].color

    sub ax, 1
    add bx, 1
    add cx, 1
    add dx, 2
    mDrawRectangle ax, bx, cx, dx, [si].color

    sub ax, 1
    add bx, 2
    add cx, 1
    add dx, 29
    mDrawRectangle ax, bx, cx, dx, [si].color
    ;END CARRO

    RETURN:
    ret
playerDrawSprite endp

;Parameters:   [bp + 4]  x0
;              [bp + 6]  y0
playerClearSprite proc
    push bp
    mov bp, sp

    ;ax: x0
    ;bx: y0
    ;cx: x1
    ;dx: y1

    ;BEG LLANTAS
    mov ax, [bp + 4]
    mov bx, [bp + 6]
    mov cx, [bp + 4]
    mov dx, [bp + 6]
    sub ax, 2
    add bx, 5
    add cx, 21
    add dx, 10
    mDrawRectangle ax, bx, cx, dx, gameBackgroundColor
    
    add bx, 17
    add dx, 17
    mDrawRectangle ax, bx, cx, dx, gameBackgroundColor
    ;END LLANTAS

    ;BEG Carro
    add ax, 4
    sub bx, 22
    sub cx, 4
    sub dx, 27
    mDrawRectangle ax, bx, cx, dx, gameBackgroundColor

    sub ax, 1
    add bx, 1
    add cx, 1
    add dx, 2
    mDrawRectangle ax, bx, cx, dx, gameBackgroundColor

    sub ax, 1
    add bx, 2
    add cx, 1
    add dx, 29
    mDrawRectangle ax, bx, cx, dx, gameBackgroundColor
    ;END CARRO

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
playerClearSprite endp

