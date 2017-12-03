#infile=./data/Jadipix.rawdat
infile=./data/Jadipix_sample.rawdat
#outfile=./data/Jadipix.bin
outfile=./data/Jadipix_sample.bin

./MimosaRecon  -b ${infile} ${outfile} 
