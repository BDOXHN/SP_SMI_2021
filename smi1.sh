#!/bin/bash
 
echo "Stanishevsky Mikhail. Deleting files by size.
Deletes files largen than the specified size.
You enter the route to the directory and the size of the file.
Files larger than the specified size are deleted. " 
 
#STDERR >&2
#./smi 2> errors 
 
deleting(){
    echo "Welcome.Choose:1 - deleting of files.2 - shot down"
    read onetwo
    case "$onetwo" in
		("1")
		read -p "Enter the rout (patch): " route
		if [ -e "$route" ]
		then
			read -p "Enter the size" size
			find "$route" -maxdepth 1 -type f -size +"$size" -exec ls -ld {} \;
			echo "Delete this files? [Y/N]: "
			read yesno1
			case "$yesno1" in
				[yY] | [""] | [yY][eE][sS] )
				find "$route" -maxdepth 1 -type f -size +"$size" -exec rm -f {} \;
				echo "Files are deleted. Exit."
				;;
				[nN] | [nN][oO] )
				echo "Files arent deleted. Exit."
				;;
				* )
				echo "Incorrect input. Files arent deleted. Exit." >&2
				;;
			
			esac
		else
			echo "Incorrect route $route. Exit." >&2
		fi
		;;  
		("2")
		echo "Program is shutting down. Have a good day!"
		exit 0
		;;
		* )
		echo "Incorrect command, only 1 or 2. Exit" >&2
		;;
		
	esac	
}
 
continue_dialog(){
    echo "Continue? [Y/N]: "
    read yesno2
    case "$yesno2" in
        [yY] | [""] | [yY][eE][sS] )
        return 0
		;;
        [nN] | [nN][oO] )
        return 1
		;;
        * )
        echo "Incorrect number" >&2 
        return 2
		;;
		
	esac
}
 
# Здесь начинается зацикленное выполнение
while true
do
    deleting
    continue_dialog
    last_return_code=$?
 
    if ["$last_return_code" = "1"]
    then
        echo "Program is shutting down. Have a good day!"
        exit 0
	fi
    if ["$last_return_code" = "2"]
    then
        echo "ERROR. Program is shutting down. Have a good day!" >&2
        exit 1
    fi
done
