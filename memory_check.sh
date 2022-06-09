apt install valgrind
if [ ! -f valgrind-python.supp ]
then
    wget http://svn.python.org/projects/python/trunk/Misc/valgrind-python.supp
fi
valgrind --tool=memcheck --suppressions=valgrind-python.supp python3 -E -tt ./kmeans_pp.py $ARGS