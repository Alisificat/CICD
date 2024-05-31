SUCCESS=0
FAIL=0
TEMPLATE="stest"
PATTERN="pattern.txt"
TEMPLATE_FILE="src/grep/grep_file_test.txt"
TEST_FILE="src/grep/grep_file_test.txt"
S21="src/grep/./s21_grep"
GREP="grep.txt"
S21_GREP="s21_grep.txt"
#LOG="src/grep/grep_log.txt"


for var in -c 
do
    $S21 $var $TEMPLATE $TEMPLATE_FILE  > $S21_GREP 
    grep $var $TEMPLATE $TEMPLATE_FILE > $GREP 
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        #echo "$var" >> $LOG
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP
done


if [ $FAIL != 0 ]
then
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
    exit 9
else
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
fi