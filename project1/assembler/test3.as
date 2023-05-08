        lw      0       1       num1      /* test - error check(3) */
        lw      0       2       num2        
        add     1       2       3
        add     3       3       3               
num1     .fill   2147483647              test max numeric value for .fill instructions   
num2     .fill   -1000  
num3     .fill   4.44                    test error check: non-integer register arguments
