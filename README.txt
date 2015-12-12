# Vertical Mining Of Time Fading Data Streams
Morgan Epp, Tayler Frederick, and Evan Spearman

Applying vertical mining techniques to the time fading model of data stream mining. Implementation of VIPER and ECLAT algorithms and comparing their effectiveness against each other.


To build implementation, use Microsoft Visual Studio 2015.
How-To Run:

To run on Visual Studio 2015, add the following commandline arguments for the project's release mode:
"[datafile].txt [#OfTransactionsInFile] > [output].txt"

An example would be:
1k5L.txt 1000 > output.txt

To run on Windows Command Prompt (compiled from Visual Studio 2015):
VerticalTimeFading.exe 1k5L.txt 1000 > output.txt
