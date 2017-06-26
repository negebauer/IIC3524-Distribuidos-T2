# Set vars

# Clear previous compiled
make clean

# Compile
make

# Test and params to use
test_def='t1'
if [[ -z $1 ]]; then test_path="test/${test_def}.txt"; else test_path=$1; fi

test_file="${test_path##*/}"
test_name="${test_file%.*}"
test_out="testout/${test_name}"

params="$test_path"

# Run stuff
print() {
  echo "--- $test_name ---"
}

if [[ $1 == "-v" ]]; then valgrind --track-origins=yes --leak-check=full ./secuential.o "test/$test_def.txt"; exit; fi
if [[ $1 == "-s" ]]; then ./secuential.o "test/$test_def.txt"; exit; fi
if [[ $1 == "-m" ]]; then
  if [[ $(pwd) == *"Nico"* ]]; then
    TMPDIR=~/mpitemp mpirun ./parallel.o "test/$test_def.txt"
  else
    mpirun -hostfile ./hosts.txt -N 4 ./parallel.o "test/$test_def.txt"
  fi
  exit
fi

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
