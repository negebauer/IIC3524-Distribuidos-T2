# Set vars

# Clear previous compiled
make clean

# Compile
make

# Test and params to use
test_def='t1'
if [[ -z $1 ]] || [[ $1 == "-"* ]] && [[ -z $2 ]]; then
  test_path="test/${test_def}.txt"
else
  if [[ $1 == "-"* ]]; then test_path=$2; else test_path=$1; fi
fi

test_file="${test_path##*/}"
test_name="${test_file%.*}"
test_out="testout/${test_name}"

params="$test_path"

# Run stuff
is_local_nico() { [[ $(pwd) == *"Nico"* ]] || [[ $(pwd) == *'Parallels'* ]]; }

if [[ $1 == "-v" ]]; then valgrind --track-origins=yes --leak-check=full ./secuential.o "$params"; exit; fi
if [[ $1 == "-s" ]]; then ./secuential.o "$params"; exit; fi
if [[ $1 == "-m" ]]; then
  if $(is_local_nico); then
    TMPDIR=~/mpitemp mpirun ./parallel.o "$params"
  else
    mpirun -hostfile ./hosts.txt -N 4 ./parallel.o "$params"
  fi
  exit
fi

echo "--- parallel $test_name ---"
if $(is_local_nico); then
  time TMPDIR=~/mpitemp mpirun ./parallel.o $params
else
  time mpirun -hostfile ./hosts.txt -N 4 ./parallel.o $params
fi
echo ''
echo "--- secuential $test_name ---"
time ./secuential.o $params
cp i_parallel.png $img_out_path
