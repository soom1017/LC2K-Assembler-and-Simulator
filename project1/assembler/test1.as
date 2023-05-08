        lw      0       1       one      /* test - error check(1) */
        lw      1       2       10       test if assembler do NOT catch simulation-time error
        lw      0       3       undef    test error check: use of undefined labels, then exit(1) without halt operation
one     .fill   1
