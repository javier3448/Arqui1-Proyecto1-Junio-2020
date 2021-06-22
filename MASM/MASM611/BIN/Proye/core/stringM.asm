;Cambia decBuffer para que contenga el numero en decimal
;[!!!] No rerecive variable decBuffer debe ser una direccion
mByteToDecAscii macro decBuffer, _byte
    pusha

    mov bx, decBuffer
    push bx
    movzx ax, _byte
    call byteToDecAscii

    popa
endm

;reg save
mCopyString macro source, destination, _size
    pusha

    mov si, source
    mov di, destination
    mov cx, _size
    call copyString

    popa
endm

;copia de source a destinitaion hasta que source sea null
mCopyCString macro source, destination
    pusha

    mov si, source
    mov di, destination
    call copyCString

    popa
endm

;si el string destino va a estar ubicado en un offset de es podemodemos usar los
;del x86
;mov cx, _size
;mov si, source
;mov di, destination
;cld
;rep movsb
;ax: numero resultante,
;dx: muero 
mAsciiToSWord macro string, _size
    pushaButAxAndDx

    mov ax, _size
    push ax
    mov ax, offset string
    push ax
    call asciiToSWord
    
    popaButAxAndDx
endm

;ax: numero resultante,
;dx: muero 
mAsciiPtrToSWord macro string, _size
    pushaButAxAndDx

    mov ax, _size
    push ax
    mov ax, string
    push ax
    call asciiToSWord
    
    popaButAxAndDx
endm

mFillString macro sourceBeg, _size, character
    pusha

    mov si, sourceBeg
    mov cx, _size
    mov al, character
    call fillString

    popa
endm

;llenar string sin usar size, solo un inicio y un final
;di es no inclusivo
mFillStringByRange macro sourceBeg, sourceEnd, character
    pusha

    mov si, sourceBeg
    mov di, sourceEnd
    mov cl, character
    call fillStringByRange

    popa
endm

mCompareString macro s1, s2, size
    pusha
    
    lea si, s1
    lea di, s2
    mov cx, size
    call compareString

    popa
endm

mCompareCString macro s1, s2
    pusha
    
    mov si, s1
    mov di, s2
    call compareCString

    popa
endm

;retorna en ax el length de string
mCStringLength macro s1
    push si

    mov si, s1
    call cStringLength

    pop si
endm

;returns: FLAGS en ZF = 1 si es letra, ZF = 0 si no
mStringIsDigits macro s1, _length
    push si
    push cx

    mov si, s1
    mov cx, _length
    call stringIsDigits

    pop cx
    pop si
endm