fgd=$1

indir=/home/coplowe/Selection_Evaluation/
infile=Selection_Evaluation_FGD${fgd}_060317

list=/data/t2k/coplowe/transverseanalysis/mclists/P6BNeutWAFull.txt

cd ${indir}
#FGD1 = 1006 (minus 1) = 1005
#FGD2 = 1273 (minus 1) = 1272
events=1005
#1006
if [ fgd == 2 ]; then
    events=1273
fi

for ii in $(seq -w 0 ${events}); do

    jobfile=${infile}_event${ii}.sh

    cat > ${jobfile} << EOF

module load root
PATH=${PATH}:/data/t2k/coplowe/software/t2k_selection
echo "${PATH}"
echo "FindFiles -i ${indir}${infile}.root -l ${list} -n ${ii}"
FindFiles -i ${indir}${infile}.root -l ${list} -n ${ii}

EOF

    chmod +x ${jobfile}
    qsub -l cput=02:00:00 -N $jobfile  ./$jobfile
    qsubresult=$?
    echo "Sub result: $qsubresult"
    while ! [ $qsubresult  -eq 0 ];do
        sleep 4
        qsub -l cput=02:00:00 -N $jobfile  ./$jobfile
        qsubresult=$?
    done

    echo "Submitted: $jobfile"

    nq=$(qstat | grep $(whoami) | grep Q |wc | awk '{print $1}' )
    while [ $nq -gt 10 ]; do
        echo "Pausing for 10"
        sleep 10
        nq=$(qstat | grep $(whoami) | grep Q |wc | awk '{print $1}' )
    done

    ii=$( expr $ii + 1 )

done