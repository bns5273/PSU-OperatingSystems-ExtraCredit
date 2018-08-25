#!/bin/bash
commands=$(<commands.txt)

j=0;
for i in $commands
do
    if [ $(command -v $i) ]
    then
        if [ $(man -w $i) 1>/dev/null 2>/dev/null ]
        then
            search=$(man $i | grep "Stallman" --context=1)
            if [ "$search" != "" ]
            then
                echo "    "$(man $i | grep -B 1 "Stallman" | head -n 1)
                echo "   "$(man $i | grep "Stallman")
                echo "    "$(man $i | grep -A 1 "Stallman" | tail -n 1)
                echo $i ": Was written by Stallman!"
                
                output[j] = $i
                output+=("$i")
                j++
            else
                echo $i ": Was not written by Stallman"
            fi
        else
            echo $i ": Has no manual entry"
        fi
    else
        echo $i ": Is not a valid command"
    fi
    
done

echo
echo "Commands written by Stallman:" ${output[*]}