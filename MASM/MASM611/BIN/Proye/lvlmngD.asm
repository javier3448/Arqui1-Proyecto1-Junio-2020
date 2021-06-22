;Level manager data

S_LEVEL equ sizeof LEVEL
L_LEVELS equ sizeof levels

levels LEVEL { "Level1", 25, 6, 1, -1, 1, RED }, 
             { "Level2", 30, 5, 2, -3, 2, GREEN }, 
             { "Level3", 35, 4, 3, -4, 3, BLUE }, 
             { "Level4", 40, 3, 4, -6, 4, WHITE }, 
             { "Level5", 45, 2, 5, -7, 5, RED }, 
             { "Level6", 50, 1, 6, -10, 6, GREEN }

PLEVEL TYPEDEF PTR LEVEL;No encontramos como hacer un ptr a level sin hacer el typedef

currLevel PLEVEL levels[0]

levelInitTime dd 0