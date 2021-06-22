;returns: ZF = 0 si algun bloque mato al player, lo retorna para que el gameloop sepa cuando terminar el juego
blocksTryUpdate proc
    assume bx : ptr BLOCK
    
    mov bx, offset blocks
    xor cx, cx ;aqui llevamos cuenta de cuantos blocks hemos tratado de actualizar, no deberiamos de tratar de actualizar mas de blocksCount
    xor ax, ax ;aqui llevamos cuenta de cuantos blocks han sido despawned por su propio metodo update
    mov dx, 0ffffh ;lowest block mem address that was despawned in this cycle
    LOOP_I:
        cmp cx, blocksCount
        jae BREAK_I

        cmp [bx].points, 0 ;isnull
        je CONTINUE_I

        ;todo: que termine cuando haya tratado de actualizar 

        push ax
        push bx;TODO: ver si es necesario proteger estos dos registros, tal vez blockTryUpdate no los usa al final
        push cx
        push dx
        call blockTryUpdate;ZF = 0 si no mato al usuario
        pop dx
        pop cx
        pop bx
        pop ax
        jnz RETURN_FALSE
        inc cx;updatedBlocks++

        cmp [bx].points, 0;si despues de blockTyeUpdate points esta en null es porque block se autodestruyo
        jne CONTINUE_I
        ;No se si en algun momento pasa por aqui
        ;TODO: Agregar un redraw all para que todavia se pueda ver el juego
        inc ax;deSpawnedBlocks++
        cmp dx, bx
        jb CONTINUE_I;[!!!]dx == bx nunca se deberia dar
        mov dx, bx;minIndexDespawnedBlock = currBlock

    CONTINUE_I:
    add bx, sizeof BLOCK
    jmp LOOP_I

    BREAK_I:
    sub blocksCount, ax
    cmp dx, firstFreeBlock;[!!!]dx == firstFreeBlock nunca se deberia dar porque no deberia de hacer update a un bloque que ya estaba en null
    ja RETURN_TRUE ;
    mov firstFreeBlock, dx

    RETURN_TRUE:
    setZF;return true
    jmp RETURN

    RETURN_FALSE:
    ;si retornamos false no nos preocupamos de mantener una EDD de blocks valida porque suponemos que el juego se acaba
    clearZF

    RETURN:
    ret
blocksTryUpdate endp

blocksDrawSprites proc
    assume bx : PBLOCK
    
    mov bx, offset blocks
    xor cx, cx ;aqui llevamos cuenta de cuantos blocks hemos tratado de actualizar, no deberiamos de tratar de actualizar mas de blocksCount
    LOOP_I:
        cmp cx, blocksCount
        jae RETURN

        cmp [bx].points, 0 ;isnull
        je CONTINUE_I

        push bx
        call blockDrawSprite
        pop bx
        inc cx

    CONTINUE_I:
    add bx, sizeof BLOCK
    jmp LOOP_I

    RETURN:
    ret
blocksDrawSprites endp

;params: [bp + 4]  puntero a hitbox
;Chequea todos los bloques con el hibox [bp + 4] si colisiona con 1
;retorna ZF = 1
blocksCollides proc
    push bp
    mov bp, sp

    assume bx : PBLOCK

    mov bx, offset blocks
    xor cx, cx ;aqui llevamos cuenta de cuantos blocks hemos tratado de actualizar, no deberiamos de tratar de actualizar mas de blocksCount
    LOOP_I:
        cmp cx, blocksCount
        jae RETURN_FALSE

        cmp [bx].points, 0 ;isnull
        je CONTINUE_I

        inc cx

        mov ax, bx
        add ax, offset BLOCK.blockHitbox
        mHitboxCollides [bp + 4], ax
        jz RETURN_TRUE 

    CONTINUE_I:
    add bx, sizeof BLOCK
    jmp LOOP_I

    RETURN_TRUE:
    setZF
    jmp RETURN

    RETURN_FALSE:
    clearZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 2
blocksCollides endp

;Si se llena blocks ocurre comportamiento no definido
;Deberiamos determina el tiempo con precision de centesimas para el spawner en vez de precision de segundos
spawnerUpdate proc
    call trySpawnPrize
    call trySpawnEnemy
    ret
spawnerUpdate endp

trySpawnPrize proc
    assume di : PLEVEL

    mov di, currLevel
    call getMonthSecs;eax = currTime
    mov edx, eax
    sub edx, lastPrizeSpawnTime
    cmp dx, [di].time_prize
    jb RETURN
    
    LOOP_PRIZE:
        mgetRandomBlockPosX;dx = random pos
        mov tmpHitbox.x0, dx
        ; int 3 ;TODO: quitar
        ; mov ax, ax
        mBlocksCollides offset tmpHitbox
        ; int 3 
        ; mov bx, bx
        jnz BREAK_LOOP_PRIZE
    jmp LOOP_PRIZE

    BREAK_LOOP_PRIZE:
    call blocksReserveBlock ;bx = reservedBlock
    mov ax, bx
    add ax, offset BLOCK.blockHitbox
    mCopyHitbox offset tmpHitbox, ax
    call blockInitPrize
    call getMonthSecs;eax = currTime
    mov lastPrizeSpawnTime, eax

    RETURN:
    ret
trySpawnPrize endp

trySpawnEnemy proc
    assume di : PLEVEL

    mov di, currLevel
    call getMonthSecs;eax = currTime
    mov edx, eax
    sub edx, lastEnemySpawnTime
    cmp dx, [di].time_enemy
    jb RETURN
    
    LOOP_PRIZE:
        mgetRandomBlockPosX;dx = random pos
        mov tmpHitbox.x0, dx
        mBlocksCollides offset tmpHitbox
        jnz BREAK_LOOP_PRIZE
    jmp LOOP_PRIZE

    BREAK_LOOP_PRIZE:
    call blocksReserveBlock ;bx = reservedBlock
    mov ax, bx
    add ax, offset BLOCK.blockHitbox
    mCopyHitbox offset tmpHitbox, ax
    call blockInitEnemy
    call getMonthSecs;eax = currTime
    mov lastEnemySpawnTime, eax
    
    RETURN:
    ret
trySpawnEnemy endp

;Retorna en bx el primer bloque disponible
;Si BLOCKS SE LLENA OCURRE COMPORTAMIENTO NO DEFINIDO
;[ !!! ] QUIEN LLAME ESTA FUNCION DEBE PONER ALGO EN [bx].points 
; EN ALGO QUE NO SEA 0 PARA NO ARRUINAR LA EDD DE BLOCKS (porque block.points!=0 es lo usamos para determinar si algo es null)
;regs ax, si
blocksReserveBlock proc
    assume si : PBLOCK
    mov bx, firstFreeBlock
    mov si, bx
    add si, sizeof BLOCK

    WHILE_TRUE:
    cmp [si].points, 0
    je WHILE_BREAK
    add si, sizeof BLOCK
    jmp WHILE_TRUE

    WHILE_BREAK:
    mov firstFreeBlock, si
    
    inc blocksCount

    ret
blocksReserveBlock endp

spawnerReset proc
    call getMonthSecs;eax = currTime
    mov lastPrizeSpawnTime, eax
    mov lastEnemySpawnTime, eax

    ret
spawnerReset endp

blocksReset proc
    ;TODO: HACER ESTE PROC

    assume bx : PBLOCK

    mov bx, offset blocks
    LOOP_I:
        cmp bx, lengthof blocks * sizeof BLOCK + offset blocks
        jae BREAK_I

        mov [bx].points, 0 ;isnull

    CONTINUE_I:
    add bx, sizeof BLOCK
    jmp LOOP_I

    BREAK_I:
    mov blocksCount, 0
    mov firstFreeBlock, offset blocks[0]
    ret
blocksReset endp
