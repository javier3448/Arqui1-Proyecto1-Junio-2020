mStartVideoMode macro
    push ax

    mov ah,00h
    mov al,13h
    int 10h
    mov ax, 0A000h
    mov es, ax

    pop ax
endm

mExitVideoMode macro
    push ax
    
    mov ah,00h
    mov al,03h
    int 10h

    pop ax
endm

;Para tener el menor numero de instrucciones y ser lo menos destructivo posible
;Talvez convenga poner que no se borre toda la pantalla
mDebStartVideoMode macro
    pusha

    call debStartVideoMode

    popa
endm

mDebExitVideoMode macro
    pusha

    call debExitVideoMode

    popa
endm

;color debe ser 1 byte
mPaintPixel macro x, y, color
    push ax
    push bx
    push cx
    push dx

    mov al, color
    mov ax, y
    push ax
    mov ax, x
    push ax
    call paintPixel

    pop dx
    pop cx
    pop bx
    pop ax
endm

;text: puntero a string terminado con $
mWriteText macro col, row, text
    pusha

    mov ax, text
    push ax
    mov ax, row
    push ax
    mov ax, col
    push ax
    call writeText

    popa
endm

;number tiene que estar de 0 99
mWriteNumberInPixel macro x, y, number
    pusha

    mov ax, number
    push ax
    mov ax, y
    push ax
    mov ax, x
    push ax
    call writeNumberInPixel

    popa
endm

dmSetCursor macro x, y
    mov ah, 02h
    xor bh, bh
    mov dl, x
    mov dh, y
    int 10h
endm

mDrawRectangle macro x0, y0, x1, y1, color
    pusha

    push y1 
    push x1 
    push y0 
    push x0
    mov cl, color 
    call drawRectangle

    popa
endm



;Ambos parametros tienen signo 
;Por ahora vamos a usar las coordenadas normales
; mDrawInCartPlane macro x, y, color
;     push ax
;     push bx
;     push dx

;     mov ax, color

;     mov ax, y
;     push ax
;     mov ax, x
;     push ax
;     call drawInCartPlane

;     pop dx
;     pop bx
;     pop ax
; endm