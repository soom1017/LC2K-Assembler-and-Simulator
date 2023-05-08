        lw      0       1       one      /* test - error check(2) */
label1  lw      1       2       10       
        lw      0       3       undef    test if label check occurs first    
label2  .fill   1
label3  .fill   label2
label4  .fill   label3
label1  .fill   10                       test error check: duplicate definition of labels
