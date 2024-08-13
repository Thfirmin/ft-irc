#!/bin/bash

read -p "Users number to start: " num_users
read -p "IRC server port: " irc_port
read -p "IRC server password: " irc_passwd

for ((i = 1; i <= num_users; i++)); do
	temp_file=$(mktemp)
	echo "PASS ${irc_passwd}" > $temp_file
	echo "NICK lorem_${i}" >> $temp_file
	echo "USER ipsum_${i} * 0 Lorem Ipsum ${i}" >> $temp_file
	echo "JOIN #42" >> $temp_file
	echo "PRIVMSG #42 : Hello from Lorem Ipsum ${i}!!! XD" >> $temp_file

	gnome-terminal -- bash -c "cat $temp_file - | nc localhost ${irc_port}; rm $temp_file; exec bash" &
done
