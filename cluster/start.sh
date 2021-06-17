set -ex
killall -9 redis-server || :

rm -rf data

ipp=""
ipp2=""
for x in {1..64}
do
    name=`printf "81%02d" $x`
    mkdir -p data/$name
    redis-server conf/conf$name
    ipp="127.0.0.1:$name $ipp"
    ipp2="127.0.0.1:$name,$ipp2"
done

redis-cli -a aaa -p 8101 --cluster create $ipp --cluster-replicas 0 << EOF
yes
EOF

echo $ipp2
    
