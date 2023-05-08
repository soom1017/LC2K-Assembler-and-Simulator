        lw      0       1       num1      /* test - jump from jr1 to jr2, from jr2 to noop, again from jr2 to halt */
        jalr    1       1                 test if opcode `jalr` works successfully, when regA == regB
        lw      0       2       jr2Ad
jr1     jalr    2       1                 test if opcode `jalr` works successfully, when regA != regB  
        noop
        add     0       2       1         reg1 == reg2 == jr2Ad + 1, so halt                 
jr2     jalr    1       2        
        halt                      
num1    .fill   100                           
jr2Ad   .fill   jr2
