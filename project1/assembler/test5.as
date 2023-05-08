        lw      0       1       num1      /* test - error check(5) */
        jalr    1       1                 
        noop
        asdf    0       2       num1      test error check: unrecognized opcodes 
        sw      1       2       num1        
        halt                      
num1    .fill   100                           
num2    .fill   -1
