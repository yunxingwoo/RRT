TDO - TUNNEL DIODE OSCILLATOR	
VIN	3	0	0.301977
R1	2	3	0.5
LS  2 	1 	1UH	 IC=0.071780 
CS  1 	0 	1000PF
G1 1 0 	POLY(1)	1 0 0.000198 0.6 -1.5 1 	
.IC V(1)=0.276929
.TRAN 10NS 20NS 0 5NS UIC
.PRINT V(1) I(LS) I(Vin)
.OPT BRIEF numdgt=10
.END
