python scripts/max_scc.py data/$1.txt $2
python scripts/re_index.py data/$1.txt
python scripts/filter.py data/$1.txt
python scripts/max_scc.py data/$1-filter.txt
python scripts/re_index.py data/$1-filter.txt
python scripts/dir3cycles.py data/$1-filter.txt
