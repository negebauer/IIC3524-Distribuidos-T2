folder='T1'
cd -- "$(dirname "$BASH_SOURCE")"
rsync -e "ssh -i ~/.ssh/hercules" -azP --exclude-from 'exclude.txt' ../$folder/ negebauer@hercules.ing.puc.cl:$folder

source ~/Bash/uc.sh
hercules "mv $folder/MakefileTripio $folder/Makefile"
tripio "chmod +x ${folder}/run.sh ${folder}/log_run.sh"
