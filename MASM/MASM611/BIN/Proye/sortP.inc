;Sorts para arreglo usrs

bubbleSortPoints proc
    push bp
    mov bp, sp

    call writeBubbleHeader
    call usrsUpdateRam
    call usrsCount
    mov cx, ax
    cmp cx, 1
    jbe RETURN 
    dec cx                         ; cx = N - 1
    mov si, offset usrs
    mov ax, si                     ; ax = inicio de usrs (usrs[0])

    assume si : ptr USR
    assume di : ptr USR

    LOOP_I:
        mov bx, cx

        mov si, ax ;si=usrs[0]
        mov di, ax 
        add di, S_USR ;di = usrs[1]

        LOOP_J:
            mov dx, [si].points

            cmp dx, [di].points
            ja MAYOR
            jb MENOR
            jmp CONTINUE_J

            MAYOR:
            cmp direccion, ASCENDENTE
            je SWAP
            jmp CONTINUE_J
            
            MENOR:
            cmp direccion, DESCENDENTE
            je SWAP
            jmp CONTINUE_J

            SWAP:
            mSwapUsrSIDI
            mGraphUsrsPoints

        CONTINUE_J:
            add si, S_USR ;si = usrs[curr + 1]
            add di, S_USR ;di = usrs[curr + 1]

        dec bx
        jnz LOOP_J

    loop LOOP_I

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
bubbleSortPoints endp

bubbleSortTimes proc
    push bp
    mov bp, sp

    call writeBubbleHeader
    call usrsUpdateRam
    call usrsCount
    mov cx, ax                     
    cmp cx, 1
    jbe RETURN 
    dec cx                         ; cx = N - 1
    mov si, offset usrs
    mov ax, si                     ; ax = inicio de usrs (usrs[0])

    assume si : ptr USR
    assume di : ptr USR

    LOOP_I:
        mov bx, cx

        mov si, ax ;si=usrs[0]
        mov di, ax 
        add di, S_USR ;di = usrs[1]

        LOOP_J:
            mov dx, [si].time

            cmp dx, [di].time
            ja MAYOR
            jb MENOR
            jmp CONTINUE_J

            MAYOR:
            cmp direccion, ASCENDENTE
            je SWAP
            jmp CONTINUE_J
            
            MENOR:
            cmp direccion, DESCENDENTE
            je SWAP
            jmp CONTINUE_J

            SWAP:
            mSwapUsrSIDI
            mGraphUsrsTimes

        CONTINUE_J:
            add si, S_USR ;si = usrs[curr + 1]
            add di, S_USR ;di = usrs[curr + 1]

        dec bx
        jnz LOOP_J

    loop LOOP_I

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
bubbleSortTimes endp

writeBubbleHeader proc
    dmSetCursor 12, 0
    print headerBubbleMsg

    mov al, byte ptr[delayVar]
    add al, '0'
    mov headerSpeedNumber, al
    dmSetCursor 35, 0
    print headerSpeed

    dmInitTime
    ret 
writeBubbleHeader endp

;params:   [bp + 4]: low
;          [bp + 6]: high
quickSortPoints proc
    push bp
    mov bp, sp

    mov si, [bp + 4]
    mov di, [bp + 6]

    cmp si, di
    jae RETURN
    mPartitionPoints [bp + 4], [bp + 6];ax = partitioning index

    ;quickSort(low, pi - 1);  // Before pi
    push ax
    sub ax, S_USR ;ax = pi - 1
    push ax
    mov dx, [bp + 4]
    push dx
    call quickSortPoints
    pop ax

    ;quickSort(pi + 1, high); // After pi
    mov dx, [bp + 6]
    push dx
    add ax, S_USR;pi + 1
    push ax
    call quickSortPoints

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
quickSortPoints endp

;params:   [bp + 4]: low
;          [bp + 6]: high
quickSortTimes proc
    push bp
    mov bp, sp

    mov si, [bp + 4]
    mov di, [bp + 6]

    cmp si, di
    jae RETURN
    mPartitionTimes [bp + 4], [bp + 6];ax = partitioning index

    ;quickSort(low, pi - 1);  // Before pi
    push ax
    sub ax, S_USR ;ax = pi - 1
    push ax
    mov dx, [bp + 4]
    push dx
    call quickSortTimes
    pop ax

    ;quickSort(pi + 1, high); // After pi
    mov dx, [bp + 6]
    push dx
    add ax, S_USR;pi + 1
    push ax
    call quickSortTimes

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
quickSortTimes endp

;[...] https://www.geeksforgeeks.org/quick-sort/
;params:   [bp + 4]: low
;          [bp + 6]: high
;returns:  ax: partitioning index
partitionPoints proc
    push bp
    mov bp, sp

    assume bx : ptr USR
    assume si : ptr USR
    assume di : ptr USR

    ;si: i
    ;di: j
    ;bx: high
    ;ax: pivot

    mov si, [bp + 4]
    mov di, si;j = low
    sub si, S_USR;i = (low - 1)
    mov bx, [bp + 6]
    mov ax, [bx].points;pivot = arr[high].points;  

    LOOP_J:
        cmp di, bx;Condicion del loop j < high;
        jae BREAK_J

        cmp [di].points, ax
        ja MAYOR
        jb MENOR
        jmp CONTINUE_J

        MAYOR:
        cmp direccion, DESCENDENTE
        je SWAP
        jmp CONTINUE_J
        
        MENOR:
        cmp direccion, ASCENDENTE
        je SWAP
        jmp CONTINUE_J

        SWAP:
        add si, S_USR;i++
        cmp si, di
        je CONTINUE_J
        mSwapUsrSIDI;swap si y di
        cmp shouldQuickSortDraw, 0
        je CONTINUE_J
        mGraphUsrsPoints
    CONTINUE_J:
    add di, S_USR;j++
    jmp LOOP_J

    BREAK_J:
    add si, S_USR;i = i + 1
    mov ax, si;retornamos el partitioning index en ax
    cmp si, bx
    je RETURN
    mov di, bx
    mSwapUsrSIDI

    cmp shouldQuickSortDraw, 0
    je RETURN
    mGraphUsrsPoints

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
partitionPoints endp

;[...] https://www.geeksforgeeks.org/quick-sort/
;params:   [bp + 4]: low
;          [bp + 6]: high
;returns:  ax: partitioning index
partitionTimes proc
    push bp
    mov bp, sp

    assume bx : ptr USR
    assume si : ptr USR
    assume di : ptr USR

    ;si: i
    ;di: j
    ;bx: high
    ;ax: pivot

    mov si, [bp + 4]
    mov di, si;j = low
    sub si, S_USR;i = (low - 1)
    mov bx, [bp + 6]
    mov ax, [bx].time;pivot = arr[high].time;  

    LOOP_J:
        cmp di, bx;Condicion del loop j < high;
        jae BREAK_J

        cmp [di].time, ax
        ja MAYOR
        jb MENOR
        jmp CONTINUE_J

        MAYOR:
        cmp direccion, DESCENDENTE
        je SWAP
        jmp CONTINUE_J
        
        MENOR:
        cmp direccion, ASCENDENTE
        je SWAP
        jmp CONTINUE_J

        SWAP:
        add si, S_USR;i++
        cmp si, di
        je CONTINUE_J
        mSwapUsrSIDI;swap si y di
        cmp shouldQuickSortDraw, 0
        je CONTINUE_J
        mGraphUsrsTimes
    CONTINUE_J:
    add di, S_USR;j++
    jmp LOOP_J

    BREAK_J:
    add si, S_USR;i = i + 1
    mov ax, si;retornamos el partitioning index en ax
    cmp si, bx
    je RETURN
    mov di, bx
    mSwapUsrSIDI

    cmp shouldQuickSortDraw, 0
    je RETURN
    mGraphUsrsTimes

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
partitionTimes endp

writeQuickHeader proc
    dmSetCursor 12, 0
    print headerQuickMsg

    mov al, byte ptr[delayVar]
    add al, '0'
    mov headerSpeedNumber, al
    dmSetCursor 35, 0
    print headerSpeed

    dmInitTime
    ret
writeQuickHeader endp

;[...] https://www.geeksforgeeks.org/shellsort/
shellSortPoints proc
    push bp
    mov bp, sp

    assume bx : ptr USR
    assume di : ptr USR
    assume si : ptr USR

    ;cx: gap
    ;bx: i
    ;di: j
    ;temp: tempUsr(variable global)

    call writeShellHeader
    call usrsUpdateRam
    call usrsCount;ax = numero de usuarios
    mov dl, 2
    div dl;ax = L_USRS / 2
    movzx cx, al ; cx = gap
    LOOP_GAP:
        cmp cx, 0;gap > 0
        je BREAK_GAP
        ;Nota: metemos a ax el valor gap * S_USR + offset usrs dos veces en un mismo ciclo
        mov ax, cx ;ax = gap
        mov dx, S_USR
        mul dx ; ax = gap * S_USR
        add ax, offset usrs; ax = gap * S_USR + offset usrs
        mov bx, ax;i = gap * S_USR + offset usrs
        LOOP_I:
            cmp bx, offset usrs + S_USR * L_USRS;Condicion del loop i < N
            jae BREAK_I
            cmp [bx]._name[0], 0;Condicion del loop i < N
            je BREAK_I

            mCopyUsr bx, offset tempUsr;temp = arr[i]

            mov di, bx;j = i
            LOOP_J:
                mov ax, cx ;ax = gap
                mov dx, S_USR
                mul dx ; ax = gap * S_USR
                add ax, offset usrs; ax = gap * S_USR + offset usrs
                cmp di, ax ; j >= gap * S_USR + offset usrs
                jb BREAK_J
                sub ax, offset usrs;ax = gap * S_USR
                ;&& arr[j - gap] > temp
                mov si, di
                sub si, ax;si: j - gap
                mov dx, [si].points
                cmp dx, tempUsr.points
                ja MAYOR
                jb MENOR
                jmp BREAK_J

                MAYOR:
                cmp direccion, ASCENDENTE
                je COPY
                jmp BREAK_J
                
                MENOR:
                cmp direccion, DESCENDENTE
                je COPY
                jmp BREAK_J

                COPY:
                mCopyUsrSIDI;si: j - gap
                            ;di: j
                mGraphUsrsPoints

            CONTINUE_J:
                sub di, ax ;j -= gap * S_USR
                jmp LOOP_J
            BREAK_J:
                mov si, offset tempUsr
                mCopyUsrSIDI;si: temp
                            ;di: j
                ;arr[j] = temp
                mGraphUsrsPoints

        CONTINUE_I:
            add bx, S_USR
            jmp LOOP_I
        BREAK_I:

    CONTINUE_GAP:
        mov ax, cx
        mov dl, 2
        div dl ;ax = gap / 2
        movzx cx, al ;gap /= 2
        jmp LOOP_GAP
    BREAK_GAP:

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
shellSortPoints endp

;[...] https://www.geeksforgeeks.org/shellsort/
shellSortTimes proc
    push bp
    mov bp, sp

    assume bx : ptr USR
    assume di : ptr USR
    assume si : ptr USR

    ;cx: gap
    ;bx: i
    ;di: j
    ;temp: tempUsr(variable global)

    call writeShellHeader
    call usrsUpdateRam
    call usrsCount;ax = numero de usuarios
    mov dl, 2
    div dl;ax = L_USRS / 2
    movzx cx, al ; cx = gap
    LOOP_GAP:
        cmp cx, 0;gap > 0
        je BREAK_GAP
        ;Nota: metemos a ax el valor gap * S_USR + offset usrs dos veces en un mismo ciclo
        mov ax, cx ;ax = gap
        mov dx, S_USR
        mul dx ; ax = gap * S_USR
        add ax, offset usrs; ax = gap * S_USR + offset usrs
        mov bx, ax;i = gap * S_USR + offset usrs
        LOOP_I:
            cmp bx, offset usrs + S_USR * L_USRS;Condicion del loop i < N
            jae BREAK_I
            cmp [bx]._name[0], 0;Condicion del loop i < N
            je BREAK_I

            mCopyUsr bx, offset tempUsr;temp = arr[i]

            mov di, bx;j = i
            LOOP_J:
                mov ax, cx ;ax = gap
                mov dx, S_USR
                mul dx ; ax = gap * S_USR
                add ax, offset usrs; ax = gap * S_USR + offset usrs
                cmp di, ax ; j >= gap * S_USR + offset usrs
                jb BREAK_J
                sub ax, offset usrs;ax = gap * S_USR
                ;&& arr[j - gap] > temp
                mov si, di
                sub si, ax;si: j - gap
                mov dx, [si].time
                cmp dx, tempUsr.time
                ja MAYOR
                jb MENOR
                jmp BREAK_J

                MAYOR:
                cmp direccion, ASCENDENTE
                je COPY
                jmp BREAK_J
                
                MENOR:
                cmp direccion, DESCENDENTE
                je COPY
                jmp BREAK_J

                COPY:
                mCopyUsrSIDI;si: j - gap
                            ;di: j
                mGraphUsrsTimes

            CONTINUE_J:
                sub di, ax ;j -= gap * S_USR
                jmp LOOP_J
            BREAK_J:
                mov si, offset tempUsr
                mCopyUsrSIDI;si: temp
                            ;di: j
                ;arr[j] = temp
                mGraphUsrsTimes

        CONTINUE_I:
            add bx, S_USR
            jmp LOOP_I
        BREAK_I:

    CONTINUE_GAP:
        mov ax, cx
        mov dl, 2
        div dl ;ax = gap / 2
        movzx cx, al ;gap /= 2
        jmp LOOP_GAP
    BREAK_GAP:

RETURN:
    mov sp, bp;hella safe
    pop bp
    ret
shellSortTimes endp

writeShellHeader proc
    dmSetCursor 12, 0
    print headerShellMsg

    mov al, byte ptr[delayVar]
    add al, '0'
    mov headerSpeedNumber, al
    dmSetCursor 35, 0
    print headerSpeed

    dmInitTime
    ret
writeShellHeader endp
