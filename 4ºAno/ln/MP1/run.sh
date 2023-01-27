#!/bin/bash

for i in tests/*; do
	echo "Compiling $i"
    fstcompile --isymbols=syms.txt --osymbols=syms.txt $i | fstarcsort > compiled/$(basename $i ".txt").fst
done

for i in sources/*; do
	echo "Compiling $i"
    fstcompile --isymbols=syms.txt --osymbols=syms.txt $i | fstarcsort > compiled/$(basename $i ".txt").fst
done

echo "compiling compositions..."
# TODO
#A2R
fstinvert compiled/R2A.fst > compiled/A2R.fst

#birthR2A
fstcompose compiled/R2A.fst compiled/d2dd.fst > r2add.fst
fstconcat r2add.fst compiled/copy.fst > birthDayr2a.fst
fstcompose compiled/R2A.fst compiled/d2dddd.fst > r2adddd.fst
fstconcat birthDayr2a.fst birthDayr2a.fst > DMr2a.fst
fstconcat DMr2a.fst r2adddd.fst > compiled/birthR2A.fst

rm r2add.fst birthDayr2a.fst r2adddd.fst DMr2a.fst

#birthA2T
fstconcat compiled/copy.fst compiled/copy.fst > copy2.fst
fstconcat copy2.fst compiled/copy.fst > copy3.fst
fstconcat copy3.fst compiled/mm2mmm.fst > dayMonth.fst
fstconcat dayMonth.fst compiled/copy.fst > copy4.fst
fstconcat copy4.fst compiled/copy.fst > copy5.fst
fstconcat copy5.fst compiled/copy.fst > copy6.fst
fstconcat copy6.fst compiled/copy.fst > copy7.fst
fstconcat copy7.fst compiled/copy.fst > compiled/birthA2T.fst

rm copy2.fst copy3.fst copy4.fst copy5.fst copy6.fst copy7.fst dayMonth.fst 

#birthT2R
fstinvert compiled/birthA2T.fst > birthT2A.fst
fstinvert compiled/birthR2A.fst > birthA2R.fst
fstcompose birthT2A.fst birthA2R.fst > compiled/birthT2R.fst

rm birthT2A.fst birthA2R.fst

#birthR2L
fstcompose compiled/birthR2A.fst compiled/date2year.fst > year.fst
fstcompose year.fst compiled/leap.fst > compiled/birthR2L.fst

rm year.fst



for i in compiled/*; do
    echo "Creating $i"
    fstdraw --portrait --isymbols=syms.txt --osymbols=syms.txt $i | dot -Tpdf > images/$(basename $i ".fst").pdf
done
