;INTERACTUAR CON CONSOLA
print macro msg
    mov ah, 09h
    lea dx, msg
    int 21h
endm

printPtr macro msg
    mov ah, 09h
    mov dx, msg
    int 21h
endm

printChar macro _char
    mov ah, 02h
    mov dl, _char
    int 21h
endm

;necesita que newline este definido
;Creo que se rompio? no se porque???
println macro msg ;No se que conviene mas si hacer una copia del string y ponerle el salto de linea al principio o hacer dos llamadas a DOS
    print newline
    print msg
endm

;Returns:     al = read caracter
cmdGetChar macro
    mov ah, 01h
    int 21h
endm


;Returns:     al = read caracter
cmdGetCharNoEcho macro
    mov ah, 07h
    int 21h
endm

cmdGetString macro buffer, stringTermination
local INICIO
INICIO:
    lea bx, buffer
    push bx
    mov dl, stringTermination
    call pCmdGetString
endm

;ARCHIVOS

;MACROS PARA DEBUGGING asume que tiene procesos pCmd.inc
debPrintln macro texto
LOCAL cadena
    .data
        cadena byte texto, 0ah, 0dh, '$'
    .code
        print cadena
endm

pushaButAx macro
    push bx
    push cx
    push dx
    push bp
    push sp
    push si
    push di
endm

popaButAx macro
    pop di
    pop si
    pop sp
    pop bp
    pop dx
    pop cx
    pop bx
endm


pushaButDi macro
    push ax
    push bx
    push cx
    push dx
    push bp
    push sp
    push si
endm

popaButDi macro
    pop si
    pop sp
    pop bp
    pop dx
    pop cx
    pop bx
    pop ax
endm

pushaButBx macro
    push ax
    push cx
    push dx
    push bp
    push sp
    push si
    push di
endm

popaButBx macro
    pop di
    pop si
    pop sp
    pop bp
    pop dx
    pop cx
    pop ax
endm

pushaButAxAndDx macro
    push bx
    push cx
    push bp
    push sp
    push si
    push di
endm

popaButAxAndDx macro
    pop di
    pop si
    pop sp
    pop bp
    pop cx
    pop bx
endm


;Pone flag zero en 1
;reg safe
setZF macro
    push ax

    xor ax, ax

    pop ax
endm

;clobbers: ax
dmSetZF macro
    xor ax, ax
endm

;Pone flag zero en 0
;reg safe
clearZF macro
    push ax

    xor ax, ax
    inc ax

    pop ax
endm

;clobbers: ax
dmClearZF macro
    xor ax, ax
    inc ax
endm

