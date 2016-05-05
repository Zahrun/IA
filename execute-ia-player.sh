#!/usr/bin/bash

TERMFS=10

SPORT=$(( ( RANDOM % 1000 ) + 2000 ))
PPORT1=$(( ( RANDOM % 1000 ) + 3000 ))
PPORT2=$(( ( RANDOM % 1000 ) + 4000 ))
OPORT2=$(( ( RANDOM % 1000 ) + 6000 ))

IA1=$1

if test x$IA1 = x; then
	echo "usage: $0 <ia1>"
	exit 1
fi

source ./execute-lib.sh ; cd ..
make_empire

# Demarrage des programmes.
launch_xterm "./empire-server/Main.native -sport ${SPORT} 2>&1 | tee out_S" SPID 0 0 
launch_xterm "./empire-client/Main.native -sport ${SPORT}" PPID1 470 0
launch_xterm "./empire-tee/tee.py localhost ${SPORT} ${PPORT2} ${OPORT2}" TPID2 0 370
launch_xterm "./empire-client/Main.native -obs -sport ${OPORT2}" OPID2 470 370 
launch_xterm "./empire-captain/ai${IA1}.py localhost ${PPORT2} | tee out_P2" PPID2 940 370

PIDS="${SPID} ${TPID1} ${PPID1} ${TPID2} ${OPID2} ${PPID2}"

# Regarde si un des programmes est stoppe.
STOPPED=0
while test $STOPPED -eq 0; do
	sleep 2
	check_processes STOPPED ${PIDS}
done

echo "Kill xterms (press a key)"
read n

# Arret de tous les programmes.
stop_processes ${PIDS}
