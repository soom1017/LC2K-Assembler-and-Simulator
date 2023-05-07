        lw      0       1       zero    load reg1 with 0 (symbolic)             /* test - calc and store fibonacci number in memory `next` */
        beq     0       1       1                                               // test if reg0 initialized to 0 when simulator starts
        beq     0       0       noop           
        lw      0       2       15      load reg2 with 1 (numeric)       
        lw      1       2       one     load reg2 with 1 (symbolic)             // test if both numeric and symbolic caculation work
        lw      0       4       nineth                                          // fibonacci until nineth                               
loop    beq     3       4       noop                                            // test opcode `noop` increases PC to PC + 1, so halt successfully
        add     1       2       3       add to reg3, and store to `next`
        sw      0       3       next     
        add     0       2       1       mov reg2 to reg1, reg3 to reg2
        add     0       3       2
        beq     0       0       loop                                            // test loop that checks condition first, not afterwards
noop    noop
        halt
zero    .fill   0
one     .fill   1
nineth  .fill   55
next    .fill   one                                                             // test if label's value can be symbolic address
