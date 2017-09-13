echo "Running unit tests:"

for i in tests/*_test
do
	if test -f $i
	then
		if $VALGRIND ./$i 2>> tests/test.log
		then
			echo $i PASS
		else
			echo "ERROR in test $i: here's tests/tests.log"
			echo "------"
			tail tests/test.log
			exit 1
		fi
	fi
done

echo ""
