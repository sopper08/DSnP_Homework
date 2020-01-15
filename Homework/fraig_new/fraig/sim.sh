:> sim.my.test.dofile
echo "cirr tests.fraig/sim$1.aag
cirp -n
cirsim -f tests.fraig/pattern.$1 -o my.pattern.log
usage
q -f" >> sim.my.test.dofile

:> sim.ref.test.dofile
echo "cirr tests.fraig/sim$1.aag
cirp -n
cirsim -f tests.fraig/pattern.$1 -o ref.pattern.log
usage
q -f" >> sim.ref.test.dofile

echo "
run my
======
"
./fraig -f sim.my.test.dofile > my.log 2>&1
echo "
run ref
=======
"
./ref/fraig -f sim.ref.test.dofile > ref.log 2>&1
echo "
diff log
=======
"
diff my.log ref.log
echo "
diff opt
=======
"
diff my.pattern.log ref.pattern.log
