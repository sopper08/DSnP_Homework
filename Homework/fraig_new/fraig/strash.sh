:> strash.test.dofile
echo "cirr $1
cirp -s
cirp -n
cirp -pi
cirp -po
cirp -fl
cirstrash
cirp -s
cirp -n
cirp -pi
cirp -po
cirp -fl
q -f" >> strash.test.dofile

./fraig -f strash.test.dofile > my.log 2>&1
./ref/fraig -f strash.test.dofile > ref.log 2>&1
diff my.log ref.log

