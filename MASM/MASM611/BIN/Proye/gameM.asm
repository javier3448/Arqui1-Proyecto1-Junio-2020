HITBOX struct 2
    x0 dw 0
    y0 dw 0
    _width dw 0
    _height dw 0
HITBOX ends

mHitboxCollides macro hitboxPtr1, hitboxPtr2
    pusha

    push hitboxPtr2
    push hitboxPtr1
    call hitboxCollides

    popa
endm

mHitboxIsInside macro hitbox1, hitbox2
    pusha

    push hitboxPtr2
    push hitboxPtr1
    call hitboxIsInside

    popa
endm

mDrawHitbox macro hitbox1, color
    pusha;[ !!! ]ESTE MACRO ES DONDE UNICO NOS SIRVE QUE PUSHA Y POPA INCLUYAN EL BP!

    mov ax, hitbox1.y0
    add ax, hitbox1._height
    push ax
    mov ax, hitbox1.x0
    add ax, hitbox1._width
    push ax
    push hitbox1.y0
    push hitbox1.x0
    mov cl, color 
    call drawRectangle

    popa
endm

mDrawRectangleInGameArea macro x0, y0, x1, y1, color
    pusha

    push y1 
    push x1 
    push y0 
    push x0
    mov cl, color 
    call drawRectangleInGameArea

    popa
endm

mDrawHitboxInGameArea macro hitbox1, color
    pusha;[ !!! ]ESTE MACRO ES DONDE UNICO NOS SIRVE QUE PUSHA Y POPA INCLUYAN EL BP!

    mov ax, hitbox1.y0
    add ax, hitbox1._height
    push ax
    mov ax, hitbox1.x0
    add ax, hitbox1._width
    push ax
    push hitbox1.y0
    push hitbox1.x0
    mov cl, color 
    call drawRectangleInGameArea

    popa
endm

mDrawHitboxPtr macro hitbox1ptr, color
    pusha;[ !!! ]ESTE MACRO ES DONDE UNICO NOS SIRVE QUE PUSHA Y POPA INCLUYAN EL BP!

    mov ax, hitbox1.y0
    add ax, hitbox1._height
    push ax
    mov ax, hitbox1.x0
    add ax, hitbox1._width
    push ax
    push [hitbox1ptr].y0
    push [hitbox1ptr].x0
    mov cl, color 
    call drawRectangle

    popa
endm

mDrawHitboxPtrInGameArea macro hitbox1ptr, color
    pusha;[ !!! ]ESTE MACRO ES DONDE UNICO NOS SIRVE QUE PUSHA Y POPA INCLUYAN EL BP!

    mov ax, hitbox1.y0
    add ax, hitbox1._height
    push ax
    mov ax, hitbox1.x0
    add ax, hitbox1._width
    push ax
    push [hitbox1ptr].y0
    push [hitbox1ptr].x0
    mov cl, color 
    call drawRectangleInGameArea

    popa
endm


mCopyHitbox macro hitbox1ptr, hitbox2ptr
    pusha
    
    mov si, hitbox1ptr
    mov di, hitbox2ptr
    call copyHitbox

    popa
endm

;Returns: dx random pos
mgetRandomBlockPosX macro
    push ax
    push cx

    call getRandomBlockPosX

    pop cx
    pop ax
endm
