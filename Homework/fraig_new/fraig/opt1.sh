:> opt1.test.dofile
echo "cirr $1
cirp -s
cirp -n
cirp -pi
cirp -po
cirp -fl
ciropt
cirp -s
cirp -n
cirp -pi
cirp -po
cirp -fl
q -f" >> opt1.test.dofile

./fraig -f opt1.test.dofile > my.log 2>&1
./ref/fraig -f opt1.test.dofile > ref.log 2>&1
diff my.log ref.log

