;TODO: calcular el numero maximo de blocks
; el primer block solo es para debuggin
;chapuz para debugging porque no nos dejo hacer blocks BLOCK 20 { 5, {20, 20, 15, 15}, {ORANGE, YELLOW} } dup({ })
;{ -2, {164, -20, 15, 15}, {ORANGE, YELLOW}, 0 }
blocks BLOCK 40 dup({ })
;TODO: Poner inicializar en 0 blocksCount, lo ponemos en 1 solo para debugging
blocksCount dw 0
PBLOCK TYPEDEF PTR BLOCK;No encontramos como hacer un ptr a block sin hacer el typedef
firstFreeBlock PBLOCK blocks[0]; sizeof BLOCK. TODO poner firstFreeBlock apuntando blocks[0]

;talvez?:
;todo resetearlas en game restart
;y despue de cambiar de nivel
lastPrizeSpawnTime dd 0
lastEnemySpawnTime dd 0

tmpHitbox HITBOX {0, -15, 15, 15}
