

# for each file in the directory "tests/"

all_tests=0
passed_tests=0

for file in $(find ./tests/syntaxical -type f); do
    ./main < $file > test_out.ifj24

    rc=$?

    all_tests=$((all_tests+1))

     # parse file name "tests_<test_num>_<rc>.txt"
    test_num=$(echo $file | cut -d'_' -f2)
    expected_rc=$(echo $file | cut -d'_' -f3 | cut -d'.' -f1)

    if [ $rc -ne $expected_rc ]; then
        echo "Test $file: FAILED  (expected $expected_rc, got $rc)"
    else
        passed_tests=$((passed_tests+1))
    fi
done


echo "Passed tests: $passed_tests/$all_tests"