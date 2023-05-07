        lw      0       2       mcand       /* test - multiplication of other numbers, not (32766, 12328) */
        lw      0       3       mplier      
        lw      0       4       one        
        nor     0       4       5           
        lw      0       7       final       
start   nor     3       5       6           
        beq     4       6       shift       
        add     1       2       1                         
shift   add     2       2       2           
        add     4       4       4           
        nor     0       4       5           
        beq     4       7       2           
        beq     0       0       start                       
        noop
done    halt
mcand   .fill   3762
mplier  .fill   127                         127 = 111111(2), so add and shift every time
one     .fill   1
final   .fill   65536
