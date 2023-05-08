        lw      0       1       num1      /* test - error check(4) */
        lw      0       2       num2    
        add     1       2       7    
        add     1       2       8        test error check: registers outside the range [0, 7]             
num1     .fill   100               
num2     .fill   -12147483648            test min numeric value for .fill instructions
