#!/bin/csh
#arg1-7 = arg for makeTemplateCat_Combine.o

set j = 24
while ( $j <= 66 )
    echo "./makeTemplateCat_Combine.o $1 $2 $3 $4 $5 $6 $7 $j"
    ./makeTemplateCat_Combine.o $1 $2 $3 $4 $5 $6 $7 $j
    @ j++
end
