usrsUpdateRam proc
    push bp
    mov bp, sp
    sub sp, 2

    mGetFileContent usrsPath, usrs, S_USR * L_USRS, word ptr[bp - 2]
    mov ax, S_USR * L_USRS
    cmp [bp - 2], ax
    jne ERROR_CARGANDO_USRS
    jmp RETURN

    ERROR_CARGANDO_USRS:
    debPrintln<"ERROR FATAL: No se pudo completar usrsUpdateRam">

    RETURN:
    mov sp, bp
    pop bp
    ret
usrsUpdateRam endp

usrsUploadToFile proc
    push bp
    mov bp, sp
    sub sp, 2

    mMakeAndWriteFile usrsPath, usrs, S_USR * L_USRS
    jc ERROR_CARGANDO_USRS
    jmp RETURN

    ERROR_CARGANDO_USRS:
    debPrintln<"ERROR FATAL: No se pudo completar usrsUploadToFile">

    RETURN:
    mov sp, bp
    pop bp
    ret
usrsUploadToFile endp

;Params:       [bp + 4]  puntero a nombre
;              [bp + 6]  puntero a password
usrsAddUsr proc
    push bp
    mov bp, sp

    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 ;isNull
    jne CONTINUE

    mov si, [bp + 4]
    lea di, [bx]._name
    call copyCString

    mov si, [bp + 6]
    lea di, [bx].password
    call copyCString

    call usrsUploadToFile
    jmp RETURN

    CONTINUE:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I;puede hacer saltos pequennos y nuestro loop es muy largo\

    RETURN:
    mov sp, bp
    pop bp
    ret 4
usrsAddUsr endp

;Params:       [bp + 4]  puntero a nombre
;returns:      FLAGS en ZF = 1 si contiene, ZF = 0 si no
usrsContains proc
    push bp
    mov bp, sp
    
    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN_FALSE
    mov si, [bp + 4]
    lea di, [bx]._name
    call compareCString
    je RETURN_TRUE

    CONTINUE_I:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 2
usrsContains endp


;Params:       [bp + 4]  c string nombre
;              [bp + 6]  c string password
;desc:         Supone que el nombre de usuario es unico
;returns:      FLAGS en ZF = 1 si existe un usuario , ZF = 0 si no
usrsTryLogin proc
    push bp
    mov bp, sp

    ;Chequeamos si es admin
    mov si, [bp + 4]
    mov di, offset adminName
    call compareCString
    jne NOT_ADMIN

    mov si, [bp + 6]
    mov di, offset adminPassword
    mov cx, L_PASSWORD
    call compareString
    je RETURN_TRUE  

    NOT_ADMIN:

    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN_FALSE
    mov si, [bp + 4]
    lea di, [bx]._name
    call compareCString
    jne CONTINUE_I

    mov si, [bp + 6]
    lea di, [bx].password
    mov cx, L_PASSWORD
    call compareString
    je RETURN_TRUE
    jmp RETURN_FALSE;Si encontro un usuario con el mismo usuario pero diferente contrasena entonces retorna false porque el nombre de usuario se supone que es unico

    CONTINUE_I:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    mov si, [bp + 4]
    lea di, loggedInUsr
    call copyCString
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
usrsTryLogin endp


;params:    [bp + 4]: c string, nombre del usuario
;           [bp + 6]: unsigned word del nuevo score
;returns:   FLAGS en ZF = 1 si actualizo el score, ZF = 0 si el usuario ya tenia un score mas grande
usrTryAddPoints proc
    push bp
    mov bp, sp

    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN_FALSE
    mov si, [bp + 4]
    lea di, [bx]._name
    call compareCString
    jne CONTINUE_I

    mov ax, [bp + 6]
    cmp ax, [bx].points
    jbe RETURN_FALSE

    mov [bx].points, ax
    call usrsUploadToFile
    jmp RETURN_TRUE

    CONTINUE_I:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
usrTryAddPoints endp


;params:    [bp + 4]: c string, nombre del usuario
;           [bp + 6]: unsigned word del nuevo time
;returns:   FLAGS en ZF = 1 si actualizo el time, ZF = 0 si el usuario ya tenia un time mas grande
usrTryAddTime proc
    push bp
    mov bp, sp

    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN_FALSE
    mov si, [bp + 4]
    lea di, [bx]._name
    call compareCString
    jne CONTINUE_I

    mov ax, [bp + 6]
    cmp ax, [bx].time
    jbe RETURN_FALSE

    mov [bx].time, ax
    call usrsUploadToFile
    jmp RETURN_TRUE

    CONTINUE_I:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
usrTryAddTime endp

;params:    [bp + 4]: c string, nombre del usuario
;           [bp + 6]: dos string del nuevo level
;returns:   FLAGS en ZF = 1 si actualizo el time, ZF = 0 si el usuario ya tenia un time mas grande
usrTryAddLevel proc
    push bp
    mov bp, sp

    call usrsUpdateRam

    mov bx, offset usrs
    assume bx : ptr USR
    
    LOOP_I:
    cmp [bx]._name[0], 0
    je RETURN_FALSE
    mov si, [bp + 4]
    lea di, [bx]._name
    call compareCString
    jne CONTINUE_I

    mov ax, [bp + 6]
    lea dx, [bx].level
    mCopyString ax, dx, lengthof USR.level
    call usrsUploadToFile
    jmp RETURN_TRUE

    CONTINUE_I:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 4
usrTryAddLevel endp

;params:    [bp + 4]: c string, nombre del usuario
;returns:   FLAGS en ZF = 1 si actualizo el score, ZF = 0 si el usuario ya tenia un score mas grande
;comments:  supone que el nombre del usuario es unico
usrIsAdmin proc
    push bp
    mov bp, sp
    
    mov si, offset loggedInUsr
    mov di, offset adminName
    call compareCString
    je RETURN_TRUE

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN_FALSE:
    clearZF
    jmp RETURN

    RETURN_TRUE:
    setZF

    RETURN:
    mov sp, bp;hella safe
    pop bp
    ret 2
usrIsAdmin endp

;[ !!! ] No actualiza su valor de ram!
usrsCount proc
    xor ax, ax

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN
    inc ax
    
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN:
    ret
usrsCount endp
    

usrsGetEnd proc

    mov di, offset usrs
    assume di : ptr USR
    LOOP_I:
    cmp [di]._name[0], 0 
    je RETURN
    
    add di, S_USR
    cmp di, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN:
    ret

usrsGetEnd endp

;[ !!! ] No actualiza su valor de ram!
usrsGetMaxPoints proc
    xor ax, ax
    
    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN
    mov dx, [bx].points
    cmp ax, dx
    jae CONTINUE
    mov ax, dx
    
    CONTINUE:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN:
    ret
usrsGetMaxPoints endp

usrsGetMaxTime proc
    xor ax, ax

    mov bx, offset usrs
    assume bx : ptr USR
    LOOP_I:
    cmp [bx]._name[0], 0 
    je RETURN
    mov dx, [bx].time
    cmp ax, dx
    jae CONTINUE
    mov ax, dx
    
    CONTINUE:
    add bx, S_USR
    cmp bx, offset usrs + S_USR * L_USRS
    jb LOOP_I

    ;Si llega aqui es porque termino de revisar todos los usuarios
    RETURN:
    ret
usrsGetMaxTime endp

;STRUCT USR DEBE DE TENER ALINEACION 2!
;params:    si: usr1
;           di: usr2
swapUsr proc
    mov cx, S_USR
    add cx, si

    LOOP_I:
    cmp si, cx
    jae RETURN
    dmWordSwap
    add si, 2
    add di, 2
    jmp LOOP_I

    RETURN:
    ret
swapUsr endp


copyUsr proc
    mov cx, S_USR
    add cx, si

    LOOP_I:
    cmp si, cx
    jae RETURN
    mov ax, [si]
    mov word ptr[di], ax
    add si, 2
    add di, 2
    jmp LOOP_I

    RETURN:
    ret
copyUsr endp
