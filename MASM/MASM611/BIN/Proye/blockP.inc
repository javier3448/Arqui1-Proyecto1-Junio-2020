;params: bx: pointer al block
;returns: ZF = 0 si mato al player, lo retorna para que el gameloop sepa cuando terminar el juego
;         pone su [bx].points en 0 si tiene que despawnarse
blockTryUpdate proc
    push bp
    mov bp, sp
    
    assume bx : PBLOCK

    push bx ;le hacemos backup a bx antes de empezar a llamar hitboxCollides
    add bx, offset BLOCK.blockHitbox
    push bx
    push offset playerHitbox
    call hitboxCollides
    pop bx
    jnz NO_COLLISION

    mov ax, [bx].points
    mov [bx].points, 0
    add playerPoints, ax
    cmp playerPoints, 0
    jg DIDNT_KILL;no lo mato
    ;
    DID_KILL:
    mov playerPoints, 0 ;ponemos en 0 playerpoints para que no se
    jmp RETURN_FALSE

    DIDNT_KILL:
    call blockClearSprite;bx todavia tiene el puntero a este bloque
    call gameUpdateHeaderPoints
    call playerDrawSprite
    jmp RETURN_TRUE

    NO_COLLISION:
    inc [bx].tickAccumulator
    cmp [bx].tickAccumulator, 32
    jb RETURN_TRUE
    ;Aqui se muevo
    mov [bx].tickAccumulator, 0
    push bx ;todo: revisar si necesitamos proteger ese registro
    push [bx].blockHitbox.y0
    push [bx].blockHitbox.x0
    call blockClearOldSprite
    pop bx ;todo: revisar si necesitamos proteger ese registro

    add [bx].blockHitbox.y0, 1
    cmp [bx].blockHitbox.y0, gameAreaY0 + gameAreaHeight
    jle DONT_DESPAWN
    
    DE_SPAWN:
    mov [bx].points, 0
    jmp RETURN_TRUE

    DONT_DESPAWN:
    push bx ;todo: revisar si necesitamos proteger ese registro
    call blockDrawSprite
    pop bx
    jmp RETURN_TRUE

    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
blockTryUpdate endp

;params: bx: pointer al block
blockDrawSprite proc
    ;ax: x0
    ;bx: y0
    ;cx: x1
    ;dx: y1

    assume bx : PBLOCK

    mDrawHitboxInGameArea [bx].blockHitbox, [bx].colors[0]
    
    mov ax, [bx].blockHitbox.y0
    add ax, 12
    push ax
    mov dx, [bx].blockHitbox.x0
    add dx, 12
    push dx
    sub ax, 9
    push ax
    sub dx, 9
    push dx
    mov cl, [bx].colors[1]
    call drawRectangleInGameArea

    RETURN:
    ret
blockDrawSprite endp

;params: bx: puntero a block
blockClearSprite proc
    assume bx : PBLOCK

    mDrawHitboxInGameArea [bx].blockHitbox, gameBackgroundColor

    ret
blockClearSprite endp

;bad name :/
;Se supone que llamamos esta funcion despues de mover el bloque entonces necesitamos las coordenadas x y anteriores
;params: bx: puntero a block
;            [bp + 4]  old x0
;            [bp + 6]  old y0
blockClearOldSprite proc
    push bp
    mov bp, sp

    assume bx : PBLOCK

    mov ax, [bp + 4]
    add ax, [bx].blockHitbox._width
    mov dx, [bp + 6]
    add dx, [bx].blockHitbox._height
    mDrawRectangleInGameArea [bp + 4], [bp + 6], ax, dx, gameBackgroundColor

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
blockClearOldSprite endp

;params: bx: puntero al block
;        di: al nivel
;No inicializa hitbox porque es mas facil hacerlo desde el spawner
;escribe sobre la direccion bx un enemigo, sus valores dependen de
;en que nivel se encuentra el juego actualmente
;clobbers: ax
blockInitEnemy proc
    assume bx : PBLOCK
    assume di : PLEVEL

    mov ax, [di].points_enemy
    mov [bx].points, ax
    mov [bx].tickAccumulator, 0

    mov [bx].colors[0], DARK_GREEN
    mov [bx].colors[1], GREEN

    ret
blockInitEnemy endp

;params: bx: puntero al block
;        si: al nivel
;No inicializa hitbox porque es mas facil hacerlo desde el spawner
;escribe sobre la direccion bx un enemigo, sus valores dependen de
;en que nivel se encuentra el juego actualmente
;clobbers: ax, bx, si
blockInitPrize proc
    assume bx : PBLOCK
    assume di : PLEVEL

    mov ax, [di].points_prize
    mov [bx].points, ax
    mov [bx].tickAccumulator, 0

    mov [bx].colors[0], ORANGE
    mov [bx].colors[1], YELLOW

    ret
blockInitPrize endp
