#!/bin/bash

num_users=10

for ((i = 1; i <= num_users; i++)); do
	temp_file=$(mktemp)
	echo "pass 123" > $temp_file
	echo "nick user_$i" >> $temp_file
	echo "user user_$i * 0 user_$i" >> $temp_file
	echo "join #42" >> $temp_file

	gnome-terminal -- bash -c "cat $temp_file - | nc localhost 4444; rm $temp_file; exec bash" &
done
sleep 1
gnome-terminal -- bash -c "./bot localhost 4444 123 marvin; exec bash" &
