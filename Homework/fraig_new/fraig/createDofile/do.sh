for i in {01..06}
do
    echo opt$i.aag
    python3 createDofile/createDofile.py tests.fraig/opt$i.aag
done

for i in {01..15}
do
    echo sim$i.aag
    python3 createDofile/createDofile.py tests.fraig/sim$i.aag
done