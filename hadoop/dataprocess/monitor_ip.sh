#!/bin/bash

function monitor_ip
{
	if test $# -eq 0
	then
		echo "monitor_ip netport"
		return 1
	else
		echo $0 $1
	fi


	iptables -t nat -A POSTROUTING -o $1 -p tcp --syn -j LOG --log-level 5 --log-prefix "iptables:"

	iptables -t nat -A PREROUTING -i $1 -p tcp --syn -j LOG --log-level 5 --log-prefix "iptables:"

	return $?
}

#main
monitor_ip $1
