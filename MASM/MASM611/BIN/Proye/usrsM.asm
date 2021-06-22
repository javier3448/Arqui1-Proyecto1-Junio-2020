USR struct 2
    _name db 8 dup(0)
    password db 4 dup(0) ;Este no tiene que ser null terminated porque siempre tienen de tamanno 4
    _type dw 0
    points dw 0
    time dw 0
    level db 8 dup(" ")
USR ends


mUsrsUpdateRam macro
    pusha

    call usrsUpdateRam

    popa
endm

mUsrsUploadToFile macro
    pusha

    call usrsUploadToFile

    popa
endm

;[ ! ] No revisa si no existe el mismo usuario en el sistema.
;      No revisa que los parametros sean validos para la logica del manejo usuarios
;params:    _name: c string
;           password: c string
;           
mUsrsAddUsr macro _name, password
    pusha

    mov ax, password
    push ax
    mov ax, _name
    push ax
    call usrsAddUsr

    popa
endm

;params:    _name: c string
mUsrsContains macro _name
    pusha

    mov ax, _name
    push ax
    call usrsContains

    popa
endm

;params: puntero a usr
mUsrIsNull macro usrPtr
    assume usrPtr : ptr USR
    cmp [usrPtr]._name[0], 0
endm

;params: c strings a nombre y contrasenna
mUsrsTryLogin macro _name, password
    pusha

    mov ax, password
    push password
    mov ax, _name
    push _name
    call usrsTryLogin

    popa
endm

;params:    _name: c string
;           score: unsigned word score
;returns:   FLAGS en ZF = 1 si actualizo el score, ZF = 0 si el usuario ya tenia un score mas grande
mUsrTryAddPoints macro _name, score
    pusha

    mov ax, score
    push ax
    mov ax, _name
    push ax
    call usrTryAddPoints

    popa
endm

;params:    _name: c string
;           time: unsigned word time
;returns:   FLAGS en ZF = 1 si actualizo el score, ZF = 0 si el usuario ya tenia un score mas grande
mUsrTryAddTime macro _name, time
    pusha

    mov ax, time
    push ax
    mov ax, _name
    push ax
    call usrTryAddTime

    popa
endm

;TODO: Probar
;params:    _name: c string
;           time: unsigned word time
;returns:   FLAGS en ZF = 1 si actualizo el score, ZF = 0 si el usuario ya tenia un score mas grande
mUsrTryAddLevel macro _name, level
    pusha

    push level
    push _name
    call usrTryAddLevel

    popa
endm

;ZF=1 si es admin, ZF=0 si no es admin o no existe
mUsrIsAdmin macro _name
    pusha

    mov ax, _name
    push ax
    call usrIsAdmin

    popa
endm

;Retorna en ax el numero de usuarios
mUsrsCount macro
    pushaButAx

    call usrsCount

    pushaButAx
endm

;Retorna en di un puntero al final del arreglo (i.e el primero terminado en null o usrs[20])
mUsrsGetEnd macro
    pushaButDi

    call usrsGetEnd

    popaButDi
endm

;Retorna en ax el puntaje maximo
mUsrsGetMaxPoints macro
    pushaButAx

    call usrsGetMaxPoints

    pushaButAx
endm

;Retorna en ax el tiempo maximo
mUsrsGetMaxTime macro
    pushaButAx

    call usrsGetMaxTime

    pushaButAx
endm


mSwapUsr macro usr1, usr2
    pusha

    mov si, usr1
    mov di, usr2
    call swapUsr

    popa
endm

;identico a mSwapUsr pero espera que ya tenga si y di con los punteros a usr
;TODO: no usar pusha y popa, usamos como 3 registros, solo pushear y popear esos
mSwapUsrSIDI macro
    pusha

    call swapUsr

    popa
endm

;TODO: no usar pusha y popa, usamos como 3 registros, solo pushear y popear esos
mCopyUsr macro source, destination
    pusha

    mov si, source
    mov di, destination
    call copyUsr

    popa
endm

;TODO: no usar pusha y popa, usamos como 3 registros, solo pushear y popear esos
mCopyUsrSIDI macro
    pusha

    call copyUsr

    popa
endm
