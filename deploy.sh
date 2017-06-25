folder='T2'
cd -- "$(dirname "$BASH_SOURCE")"
rsync -e "ssh -i ~/.ssh/hercules" -azP --exclude-from 'exclude.txt' ../$folder/ negebauer@hercules.ing.puc.cl:$folder
