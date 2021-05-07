set -x
set -e
rm -f redis_ys.zip
if [ ! -d "target" ]; then
  mkdir -p target/include
fi
cp *.h target/include
cp -r adapters target/include
cp libhiredis_ys.a target
zip -r redis_ys.zip target