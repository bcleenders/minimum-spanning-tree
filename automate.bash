#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
for i in {0..10}
  do
     ./a.out -v 60000 -fast >> output.txt
 done
