set -e
set -x
if [ ! -d "conf" ];then
mkdir conf
fi
if [ ! -d "data" ];then
mkdir data
fi
for x in {1..64}
do
    name=`printf "81%02d" $x`
    cp conft conf/conf$name
    rm -rf data/$name
    mkdir -p data/$name
    sed -i "s/6379/$name/g" conf/conf$name
    
done
    
