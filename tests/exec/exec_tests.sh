#!/bin/bash

check()
{
    RED='\033[0;31m'
    NC='\033[0m'
    GREEN='\033[1;32m'

    var="$*"

    echo "testing: ./42sh $var"

    ../../builddir/42sh ${var} > a.log
    dash ${var} > b.log

    DIFF=$(diff a.log b.log)

    if [ "$DIFF" != "" ]
    then
        printf "${RED}TEST: FAILED${NC}\n";

    else
        printf "${GREEN}TEST : PASSED${NC}\n";
    fi
}

check test_basic.sh
check test_and_or.sh
check test_compound_list.sh
check test_nested_if.sh
check test_pipe.sh
