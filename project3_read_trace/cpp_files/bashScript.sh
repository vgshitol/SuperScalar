#!/usr/bin/env bash

rob_size;

mkdir graph graph/graph1 graph/graph2 graph/graph3 graph/graph4

make clean
rm -f *.o sim
make

rob_size=512;
#
##Graph 1
#   echo 'WIDTH', 'ISSUE QUEUE SIZE', 'IPC' >> ./graph/graph1/graph1.csv
#for iq in 8 16 32 64 128 256
# do
# for width in 1 2 4 8
# do
# if (($iq >= $width)); then
#
#  ./sim ${rob_size} ${iq} ${width} ../../proj3-traces/val_trace_gcc1 > ./graph/graph1/outputgcc_m${iq}${width}.txt
#
#    variable3=$(grep '(IPC)' ./graph/graph1/outputgcc_m${iq}${width}.txt | awk '{print $7}');
#    variable2=${iq};
#    variable1=${width};
#
#    echo "$variable1, $variable2, $variable3" >> ./graph/graph1/graph1.csv
#
#  echo "Looping ... number width $width iq size $iq"
#fi
# done
#done
#
##Graph 2
#   echo 'WIDTH', 'ISSUE QUEUE SIZE', 'IPC' >> ./graph/graph2/graph2.csv
#for iq in 8 16 32 64 128 256
# do
# for width in 1 2 4 8
# do
# if (($iq >= $width)); then
#
#  ./sim ${rob_size} ${iq} ${width} ../../proj3-traces/val_trace_perl1 > ./graph/graph2/outputperl_m${iq}${width}.txt
#
#    variable3=$(grep '(IPC)' ./graph/graph2/outputperl_m${iq}${width}.txt | awk '{print $7}');
#    variable2=${iq};
#    variable1=${width};
#
#    echo "$variable1, $variable2, $variable3" >> ./graph/graph2/graph2.csv
#
#  echo "Looping ... number width $width iq size $iq"
#fi
# done
#done


#Graph 3
   echo 'ROB_SIZE','WIDTH', 'ISSUE QUEUE SIZE', 'IPC' >> ./graph/graph3/graph3.csv

for rob_size in 32 64 128 256 512
 do
 for width in 1 2 4 8
 do
    if (($width==1)); then
        iq=8
    elif (($width==2)); then
        iq=16
    elif (($width==4)); then
        iq=32
    elif (($width==8)); then
        iq=64
    fi

 if ((rob_size >= $width)); then

  ./sim ${rob_size} ${iq} ${width} ../../proj3-traces/val_trace_gcc1 > ./graph/graph3/outputgcc_m${rob_size}${iq}${width}.txt

    variable3=$(grep '(IPC)' ./graph/graph3/outputgcc_m${rob_size}${iq}${width}.txt | awk '{print $7}');
    variable2=${iq};
    variable1=${width};
    variable0=${rob_size};

    echo "$variable0, $variable1, $variable2, $variable3" >> ./graph/graph3/graph3.csv

  echo "Looping ... number robsize $rob_size width $width iq size $iq"
fi
 done
done

#Graph 4
   echo 'ROB_SIZE','WIDTH', 'ISSUE QUEUE SIZE', 'IPC' >> ./graph/graph4/graph4.csv

for rob_size in 32 64 128 256 512
 do
 for width in 1 2 4 8
 do
    if (($width==1)); then
        iq=8
    elif (($width==2)); then
        iq=32
    elif (($width==4)); then
        iq=64
    elif (($width==8)); then
        iq=128
    fi

 if ((rob_size >= $width)); then

  ./sim ${rob_size} ${iq} ${width} ../../proj3-traces/val_trace_perl1 > ./graph/graph4/outputperl_m${rob_size}${iq}${width}.txt

    variable3=$(grep '(IPC)' ./graph/graph4/outputperl_m${rob_size}${iq}${width}.txt | awk '{print $7}');
    variable2=${iq};
    variable1=${width};
    variable0=${rob_size};

    echo "$variable0, $variable1, $variable2, $variable3" >> ./graph/graph4/graph4.csv

  echo "Looping ... number robsize $rob_size width $width iq size $iq"
fi
 done
done