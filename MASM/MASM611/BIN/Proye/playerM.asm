;flags: ZF: 0 si murio, ZF: 1 si no murio
mPlayerUpdate macro
    pusha
    
    call playerUpdate

    popa
endm
