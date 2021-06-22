debStartVideoMode proc
    mov ah,00h
    mov al,13h
    int 10h
    mov ax, 0A000h
    mov es, ax

    ret
debStartVideoMode endp

debExitVideoMode proc
    mov ah,00h
    mov al,03h
    int 10h

    ret
debExitVideoMode endp

;Parameters:   [bp + 4]  x
;              [bp + 6]  y
;              cl        color
paintPixel proc
    push bp
    mov bp, sp

    mov ax, [bp + 6]
    mov dx, 320
    mul dx ;ax tiene 320 * y
    add ax, [bp + 4] ;ax + coordenada x

    mov bx, ax

    cmp bx, 320*200
    jae RETURN
    mov byte ptr es:[bx], cl

RETURN:
    pop bp
    ret 4
paintPixel endp

;Parameters:   [bp + 4]  x
;              [bp + 6]  y
;              [bp + 8]  puntero a cadena terminada en $
writeText proc
    push bp
    mov bp, sp

    dmSetCursor byte ptr[bp + 4], byte ptr[bp + 6]
    printPtr [bp + 8]

RETURN:
    pop bp
    ret 6
writeText endp

;(x0, y0) debe ser menor que (x1, y1)
;Parameters:   [bp + 4]  x0
;              [bp + 6]  y0
;              [bp + 8]  x1
;              [bp + 10] y1
;              cl        color
drawRectangle proc
    push bp
    mov bp, sp

    mov dx, [bp + 6]
    mov ax, 320
    mul dx
    mov bx, ax;bx = y0 * 320

    mov dx, [bp + 10];y1
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
drawRectangle endp

;(x0, y0) debe ser menor que (x1, y1)
;Parameters:   [bp + 4]  x
;              [bp + 6]  y
;              [bp + 8]  number (0 - 99)
writeNumberInPixel proc
    push bp
    mov bp, sp

    fld columnPerPixel
    fild word ptr[bp + 4]
    fmul
    fistp word ptr[bp + 4];x = x * columnPerPixel

    fld rowPerPixel
    fild word ptr[bp + 6]
    fmul
    fistp word ptr[bp + 6];y = y * rowPerPixel

    mov bx, offset dosDecBuffer
    push bx
    mov ax, word ptr[bp + 8]
    call byteToDecAscii

    dmSetCursor [bp + 4], [bp + 6]
    printPtr offset dosDecBuffer + 1

    RETURN:
    mov sp, bp;hella SAFE
    pop bp
    ret 6
writeNumberInPixel endp

; No lo vamos a usar por ahora:
; ;Parameters:   [bp + 4]  x
; ;              [bp + 6]  y
; drawInCartPlane proc
;     push bp
;     mov bp, sp

;     mov ax, [bp + 6]
;     neg ax
;     add ax, 100
;     mov dx, 320
;     imul dx

;     mov dx, [bp + 4]
;     add dx, 159
;     add ax, dx

;     mov bx, ax

;     cmp bx, 320*200
;     jae RETURN
;     mov byte ptr es:[bx], 31

; RETURN:
;     pop bp
;     ret 4
; drawInCartPlane endp

