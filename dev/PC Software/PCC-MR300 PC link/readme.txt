PCC-MR300 from http://www.lxqqfy.com/e/product.php?id=MR300

The PCC-MR300 is a simple command line program which scans in the selected frequency

range and stores the measurement results in a file format supported by ZPlots.

Usage:

PCC-MR300 -c<com port> -s<start freq> -e<end freq> -t<step value> -o<output file>

Where:

-c<com port name>	COM port name

-s<start freq>		Start frequency in Hertz

-e<end freq>		End frequency in Hertz

-t<step>		Step value in Hertz

-o<output file>		Output file name (without path)


Example:

PCC-MR300 -cCOM5 -s14000000 -e16000000 -t10000 -oDipole20m.csv