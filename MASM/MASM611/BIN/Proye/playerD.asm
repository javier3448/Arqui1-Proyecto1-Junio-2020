;playerName: solo usar logged in usr
playerTime dw 0
playerPoints dw 0
playerLevel db 8 dup(" ")

;playerSprite implicita en el codigo

;x y position van estar 'implicitas' en el hitbox

playerHitbox HITBOX { 0 , 0, 20, 32 }
playerTmpHitbox HITBOX { ? } ;Tenemos esta var porque no podemos pasar a hitboxCollides un puntero a un valor en el stack
;alguna mem para que no se mueva cada tick que puede
