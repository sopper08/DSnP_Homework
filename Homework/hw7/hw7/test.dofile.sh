./taskMgr -f dofiles/test.dofile > my.test.log 2>&1
./ref/taskMgr-ref -f dofiles/test.dofile > ref.test.log 2>&1
vimdiff my.test.log ref.test.log
