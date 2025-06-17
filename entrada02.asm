start_ops:
andi x1 , x0 , 0xAA
sub x2 , x1 , x0
sh x2 , 0( x0 )

lh x3 , 0( x0 )
or x4 , x3 , x1
srl x5 , x4 , x1

beq x5 , x0 , end_ops

end_ops: