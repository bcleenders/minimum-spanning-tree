#!/bin/bash          

LIST=${@}

if [[ $# -eq 0 ]] ; then
    echo 'Using default array';
    LIST={1000,1000,1000,10000,10000,10000,50000,50000,5000}
fi

rm output.txt

for i in $LIST; do echo `./a.out -fast -silent -v $i` >> output.txt; done
echo "Written result into output.txt (old results removed)\n\n";

cat output.txt