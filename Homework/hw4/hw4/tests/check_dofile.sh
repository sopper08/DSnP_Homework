echo "###########"
echo "#   do1   #"
echo "###########"
./memTest -File tests/do1 > my.log 2>&1 
./ref/memTest -File tests/do1 > ref.log 2>&1
diff my.log ref.log

echo "###########"
echo "#   do2   #"
echo "###########"
./memTest -File tests/do2 > my.log 2>&1 
./ref/memTest -File tests/do2 > ref.log 2>&1
diff my.log ref.log

echo "###########"
echo "#   do3   #"
echo "###########"
./memTest -File tests/do3 > my.log 2>&1 
./ref/memTest -File tests/do3 > ref.log 2>&1
diff my.log ref.log

echo "###########"
echo "#   do4   #"
echo "###########"
./memTest -File tests/do4 > my.log 2>&1 
./ref/memTest -File tests/do4 > ref.log 2>&1
diff my.log ref.log

echo "###########"
echo "#   do5   #"
echo "###########"
./memTest -File tests/do5 > my.log 2>&1 
./ref/memTest -File tests/do5 > ref.log 2>&1
diff my.log ref.log