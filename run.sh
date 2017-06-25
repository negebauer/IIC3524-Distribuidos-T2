# Clear previous compiled
rm t2.o

# Compile
make

# Image and param to use
test_def='t2'
if [[ -z $1 ]]; then test_path="test/${img_def}.txt"; else test_path=$1; fi

test_file="${test_path##*/}"
test_name="${test_file%.*}"
test_out="testout/${test_name}"

# Run stuff
print() {
  echo "--- $test_name ---"
}
# print 'parallel'
time ./t2.o $test_name
# mv $img_out_path i_parallel.png
# echo ''
# print 'secuential'
# time ./secuential $params
# mv $img_out_path i_secuential.png
# cp i_parallel.png $img_out_path
