#!/bin/bash

cd src/cat/

RESULT=$(bash test_cat.sh | grep "FAIL: 0") 
#  в "RESULT" сохраняется результат выполнения команды  с поиском Fail:0
if [ -z "$RESULT" ]; then
    echo 'Integration tests failed.'
    exit 1
fi
cd ../grep/
RESULT=$(bash 1_grep.sh | grep "FAIL: 0")
if [ -z "$RESULT" ]; then
    echo 'Integration tests failed.'
    exit 1
fi
echo 'Integration tests are successful.'
exit 0