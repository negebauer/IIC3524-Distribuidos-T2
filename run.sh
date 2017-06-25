# Clear previous compiled
rm secuential
rm parallel

# Compile
make

# Image and param to use
img_def='big'
kernel_def='blur_3'
repeat_def='3'
if [[ -z $1 ]]; then img_path="test/img/${img_def}.png"; else img_path=$1; fi
if [[ -z $2 ]]; then kernel_path="test/kernel/${kernel_def}.txt"; else kernel_path=$2; fi
if [[ -z $3 ]]; then repeat=$repeat_def; else repeat=$3; fi

img_file="${img_path##*/}"
img_name="${img_file%.*}"
kernel_file="${kernel_path##*/}"
kernel_name="${kernel_file%.*}"
img_out_path="test/out/${img_name}_${kernel_name}_${repeat}.png"

params="$img_path $kernel_path $img_out_path $repeat"

echo "
Apply mask $kernel_name to $img_name $repeat times
"

# Run stuff
print() {
  echo "--- $1 ---"
}
print 'parallel'
time ./parallel $params
mv $img_out_path i_parallel.png
echo ''
print 'secuential'
time ./secuential $params
mv $img_out_path i_secuential.png
cp i_parallel.png $img_out_path
