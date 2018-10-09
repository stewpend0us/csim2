echo ""
echo "Running unit tests:"
rm -f tests/test.log

for i in tests/*_test
do
	if test -f $i
	then
		echo "------------------------------ " $i " ------------------------------" >> tests/test.log
		valgrind ./$i 2>> tests/test.log
	fi
done

echo ""
