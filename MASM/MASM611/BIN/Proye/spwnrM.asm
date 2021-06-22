mBlocksCollides macro hitboxPtr1
    pusha

    push hitboxPtr1
    call blocksCollides

    popa
endm