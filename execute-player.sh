#!/usr/bin/bash

TERMFS=10

SPORT=$(( ( RANDOM % 1000 ) + 2000 ))

source ./execute-lib.sh ; cd ..
make_empire

# Demarrage des programmes.
launch_xterm "./empire-server/Main.native -sport ${SPORT} 2>&1 | tee out_S" SPID 0 0
launch_xterm "./empire-client/Main.native -sport ${SPORT}" PPID1 470 0
launch_xterm "./empire-client/Main.native -sport ${SPORT}" PPID2 470 370

PIDS="${SPID} ${PPID1} ${PPID2}"

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
