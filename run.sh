# Set vars

# Clear previous compiled
make clean

# Compile
make

# Test and params to use
test_def='t2'
if [[ -z $1 ]]; then test_path="test/${img_def}.txt"; else test_path=$1; fi

test_file="${test_path##*/}"
test_name="${test_file%.*}"
test_out="testout/${test_name}"

params="$test_path"

# Run stuff
print() {
  echo "--- $test_name ---"
}
print 'parallel'
if [[ $(pwd) == *"Nico"* ]]; then
  time TMPDIR=~/mpitemp mpirun ./parallel.o $params
else
  time mpirun -hostfile ./hosts.txt -N 4 ./parallel.o $params
fi
# mv $img_out_path i_parallel.png
# echo ''
print 'secuential'
time ./secuential.o $params
# mv $img_out_path i_secuential.png
# cp i_parallel.png $img_out_path
