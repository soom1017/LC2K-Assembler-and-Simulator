        lw      0       2       mcand       reg2(tmp) == mcand
        lw      0       3       mplier      reg3 == mplier
        lw      0       4       one        
        nor     0       4       5           reg5 == mask bit
        lw      0       7       final       reg7 == 65536 == 2^16
start   nor     3       5       6           reg6(tmp) == ~ (masked bit)
        beq     4       6       shift       if masked bit == 0, just shift
        add     1       2       1                         else, add reg2(tmp) and shift   
shift   add     2       2       2           left shift reg2(tmp)
        add     4       4       4           
        nor     0       4       5           update mask bit
        beq     4       7       2           if shift count is 16, end the program
        beq     0       0       start                       else, loop
        noop
done    halt
mcand   .fill   32766
mplier  .fill   12328
one     .fill   1
final   .fill   65536
