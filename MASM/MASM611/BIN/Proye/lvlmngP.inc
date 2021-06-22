levelManagerTryUpdate proc
    assume bx : PLEVEL

    mov bx, currLevel

    call getMonthSecs
    sub eax, levelInitTime
    cmp ax, [bx].time
    jb RETURN_TRUE;no cambiamos de nivel

    call levelNextLevel
    ret; we return whatever levelNextLevel returns

    RETURN_TRUE:
    setZF
    ret
levelManagerTryUpdate endp

;params:  bx, currLevel
;returns: bx
levelNextLevel proc
    assume bx : PLEVEL

    cmp bx, offset levels + lengthof levels * sizeof LEVEL - sizeof LEVEL
    jae RETURN_FALSE
    cmp [bx + sizeof LEVEL]._name, 0
    je RETURN_FALSE
    ;si bx esta en levels[length-1]entonces acaba de pasar el ultimo nivel

    mov ax, bx
    add ax, sizeof LEVEL + offset LEVEL._name
    mov dx, offset playerLevel
    mCopyString ax, dx, lengthof USR.level
    mov playerHitbox.x0, gameInitialXPosition
    mov playerHitbox.y0, gameInitialYPosition

    call getMonthSecs
    mov levelInitTime, eax

    call blocksReset
    call spawnerReset

    add currLevel, SIZEOF LEVEL

    call gameUpdateHeaderLevel

    mDrawRectangle 16 - 2, 16, 320 - 16 + 2, 200 - 8, GRAY ;DrawBackGround
    call gameDrawAll;dibuja por primera vez todos los gameObj

    RETURN_TRUE:
    setZF
    jmp RETURN

    RETURN_FALSE:
    clearZF
    
    RETURN:
    ret
levelNextLevel endp