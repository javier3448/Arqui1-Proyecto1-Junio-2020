;params: si: word1 ptr to word
;        di: word2 ptr to word
;clobbers:   ax
dmWordSwap macro
    mov ax, word ptr[si]
    xchg ax, word ptr[di]
    mov word ptr[si], ax
endm

mWordSwap macro word1, word2
    push ax
    push si
    push di 

    mov si, word1
    mov di, word1
    dmWordSwap

    pop di
    pop si
    pop ax
endm