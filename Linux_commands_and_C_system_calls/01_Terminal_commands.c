 ***TERMINAL***

***************************************************************
********CLASE2: Todo es un File, incluso el hardware***********
***************************************************************

*
tty						devuelve el pathname del terminal actual, ojo tty es una orden, no el archivo terminal
$tty
	dev/pts/1			Los ternimales son archivos numerados como 1 2 3 ...
						OJO: el contenido de dev es archivos que representan el hardware

*
which 					devuelve el pathname de un file (y todo es un file)
$which tty			 
	/usr/bin/tty		se recibe la ruta del comando tty

$which sh 				obtiene la ruta del shell 
	/bin/sh 			sh es un enlace simbolico a dash, el shell de debian (debian almquist shell)
						ojo que sh/dash es mas pequeno que bash, el shell por defecto de mint

						si desde bash llamo a dash, entonces dash sera hijo de bash, se vera en ps -l 

*
file 					detalle de un archivo, se necesita el pathname
$file $(which tty)		aqui enviamos "which tty" al bash, y la salida de ese bash 
						que es la ruta de tty, se envia a filename
	/usr/bin/tty: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dyn...

$file $(tty)
	/dev/pts/2: character special (136/2)	//aca ha devuelto info del terminal 2

*
rm  					remove files or directories
						rm [OPTION]... [FILE]...

						-f  = force (sin confirmacion)
						-r = recursive (carpetas y subcarpetas)
						-V = verbose (muestra linea a linea que cosa se esta eliminando)
						-rf = maldad.   

						*************************
						$rm -rf es PELIGROSISIMO
						*************************
						*************
						Unlike Windows system or Linux desktop environment where a deleted file
						is moved in Recycle Bin or Trash folder respectively, a file deleted 
						with the rm command is not moved in any folder. It is deleted permanently.

						From technical point of view, no, a deleted file persists in the disk 
						until a new file or folder is stored at the same place where the file was stored.
						**************

*
shred  					The shred utility is the part of the package coreutils. 
						Since the package coreutils is the part of essential packages in default
						installation, the shred utility should almost always be installed. 

						$shred [option] file/partition/drive

						-f :- Perform delete and overwrite operation forcefully.

						-v :- Display operation’s progress.

						-z :- Add a final overwrite with zeros to hide the act of shredding.

						-n :- Perform overwrite operation number of times specified with this option.

						-u :- Delete file/partition/drive after overwriting.

						For a complete list of options with other details, you can check the manual pages with following command

*
ls   					Lista info de un archivo, -l agrega mas info, se necesita la ruta
$ls -l /dev/pts/1   =   $ls -l $(tty)	
	crw------- 1 marina tty 136, 1 Aug 29 01:18 /dev/pts/1

*
echo					La orden responde con lo mismo a una salida estandar
$echo hola
	hola

$echo hola > arch.txt	Crea un archivo arch.txt y escribr hola en el 

$echo hola > /dev/tty	El mensaje es redireccionado al archivo tty, pero como es la entrada standar aparece en el terminal
						ojo, /dev/tty es la entrada standar (hardware),  /usr/bin/tty es el comando que muestra la ruta al hwr terminal	

NOTA: $ representa variables de ambiente, para llamarlas se necesita usar el caracter $ + la variable, $HOME $$ $SHELL ... 
$echo $HOME				Imprime el contenido de la variable de ambiente HOME
	/home/marina		Esta es mi carpeta home

$echo $$				obtiene el PID de la variable que contiene el pid del proceso que ejecuta el shell actual (interprete de ordenes)

$echo $SHELL			imprime el contenido de la variable de ambiente  SHELL
	/bin/bash 			obtiene la ruta del archivo bash, que es nuestro shell actual	

$echo $0				obtiene el proceso actual (arguento 0 de la linea de ordenes)
	/bin/bash 

$echo $?				Devuelve el PID del ultimo proceso que termino	

$echo 145				devuelve 145
$echo 145> file.txt		devuelve un archivo que contiene 145

*
>						redirige la salida estandar, INSTRUCCION > SALIDA

<						redirige la entrada estandar, INSTRUCCION < ENTRADA

$cp <(echo Hola) /dev/tty
	Hola				(echo Hola) devuelve por salida estandar la palabra Hola, pero esta salida es redirigida como entrada
						al programa cp (copy), y este copia el contenido a /dev/tty
						pero esa es la pantalla, asi que se imprime Hola

*
cat 					concatena el contenido de archivos y lo imprime en salida estandar 
$which cat 				otiene la ruta del archivo cat 
	/bin/cat 
$cat file.txt			imprime la concatenacion del archivo

$cat 					como no hay un archivo de entrada, espera la entrada estandar (por linea)
						se sale con ctrl+d fin de datos, o ctrl+c interrumpir

$cat /dev/tty > foo		/dev/tty es la pantalla I/O estandar, entonces cat concatena todo lo que ingresemos en la entrada estandar, 
						luego con > es redirigido, de la pantalla a un archivo foo

$cat file1 file2 | sort >/dev/lp 
						cat concatena el contenido de file1 y file2, la salida es redirigida por | a sort, quien lo ordena
						luego la salida de sort es redireccionada a la impresora /dev/lp 
						OJO, SI SE AGREGA & AL FINAL, SE ENVIA LA TAREA AL BACKGROUND
						esto permitira retomar el control del shell

$file /dev/input/mice	detalle del archivo mice
	/dev/input/mice: character special (13/63)
						es un archivo especial, de transferencia de datos por caracteres

$sudo cat /dev/input/mice 
						concatena el archivo mice, que es el mouse, entonces enviara a la salida estandar todas las acciones
						del mouse traducidas en simbolos. 
$sudo cat /dev/input/mice > rawmice.data
						envio esos datos a un archivo  
$sudo cat /dev/input/mice | \ > padsp tee /dev/audio > /dev/null
						la salida del cat es redirigida por | a padsp (utilidad de audio), tee redirige esa data 
						y la envia al audio. el resto es solo null para evitar errores
$sudo cat /dev/cdrom > cd.iso  
						toma el contenido del cd montado en el sistema, y lo concatena enviandolo a un archivo iso 

$sudo dd if=/dev/cdrom of= cd.iso  					
						dd hace lo mismo, solo se especifica el input file IF y el ouput file OF	

$dd if=/dev/zero of=./disquete.msdos bs=1024 count=1440
						origen es secuencia de ceros
						destino es disquete.msdos
						bs blocksize = 1024 bytes
						count numblocks = 1440 		 tenemos 1474560 bytes aprox 1.4 MiB
	
	1440+0 registros leídos
	1440+0 registros escritos
	1474560 bytes (1,5 MB, 1,4 MiB) copied, 0,0029429 s, 501 MB/s

$/sbin/mkfs.fat -n SISTEMASOPERATIVOS -s 1 disquete.msdos

						crea un sistema de archivos fat, en la imagen destino
	
	mkfs.fat 4.1 (2017-01-24)

$fatcat disquete.msdos -i
						FAT filesystem explore, extract, repair, and forensic tool
						cat a un sistema de archivos fat 

$udisksctl loop-setup -f "/home/alulab/Documentos/disquete.msdos"
	Mapped file /home/alulab/Documentos/disquete.msdos as /dev/loop0.

						Mount loop devices
						This will create a loop device and show the ISO image ready to mount. 
						Once unmounted, the loop device will be terminated by udev. 

$ seq -f "File%03g" 1 100 | while read f1 && read f2;do for i in $(seq 1 117);do \
./fragm /media/alulab/SISTEMASOPE/$f1; ./fragm /media/alulab/SISTEMASOPE/$f2;done;done

						genera una secuencia Filexxx de 3 digitos



*
<<< 					denotes a here string.

						$ cat <<< 'hi there'
						hi there

						It passes the word on the right to the standard input of the command on the left.

<< 						denotes a here document.

						$ cat <<EOF
						> hi
						> there
						> EOF
						hi
						there

						EOF can be any word.

						Here documents are commonly used in shell scripts to create whole files or to display long messages.

						cat > some-file <<FILE
						foo
						bar
						bar bar
						foo foo
						FILE



*
file 					devuelve el detalle del tipo de datos que maneja ese archivo 
$file-					- representa la entrada estandar, en ese caso ante cualquier entrada la reconocera por su tipo
	/dev/stdin: ASCII text
$echo Hola mundo! | file-
	/dev/stdin: ASCII text
$echo feliz cumpleaños! | file-	
	/dev/stdin: UTF-8 Unicode text



*
dd  					convierte y copia un archivo IF a OF (ver ejemplos)
$echo gaaaa | sed 's/aaa/eee/g'
	geeea				echo envia gaaa a la salida que es redirigida por | quien lo envia de entrada a sed que no ha definido IF
						sed cambia aaa por eee y lo envia a la salida estandar al no tener definido OF

$dd if=/dev/zero bs=1024 count=1000 of=foo
						el dispositivo zero crea 1000 ceros con tamano de bloque 1024, este es el if, que sera enviado al of foo
						se puede hacer lo mismo con otros dispositivos como /dev/urandom
						es tan poderoso que puede recibir hasta particiones, porque son archivos
						if=/dev/sda1 of =/dev/sdb1 bs=4096
$sudo dd if=/dev/sda | sed 's/Wendy/Janet/g' | dd of=/dev/sda	
						dd toma de entrada al sda
						| envia a sed (string editor) el contenido de sda, busca la palabra Wendy y la cambia por Janet
						| toma la salida de sed y la envia a dd, que la toma de entrada y lo reescribe en sda 						

*
mount					Monta una unidad en el arbol de archivos del sistema
						Sea un wd(working directory) que contiene un archivo a.iso y un directorio foo con dos archivos: baz y bar  
$ls /media/marina		Imprime la carpera que esta en media, en este caso mi unidad work montada en media
	Work				Mi disco Work que es la particion E en windows 

$ls -l foo/ 			Mostrara los dos archivos que estan en foo
$sudo mount a.iso foo	monta a.iso en el directorio foo, esto desmonta a baz y bar. 
						Si se lanza $ls -l foo/ de nuevo mostrara solo el contenido del iso montado en foo 
						los dos archivos baz bar no seran accesibles hasta que se desmonte a.iso 
$sudo unmount foo		desmonta el contenido del directorio foo, entonces se retoma el acceso a los dos archivos originales

*
chmod					Cambia el nivel de privilegios rwx sobre los archivos
$chmod a+x cat?			agrega a todos los grupos el permiso de ejecucion, para los archivos que comienzan con cat 
						recuerda que ?, * y [] es comodin.
						Recuerda que se tienen 3 juegos de permisos rwx para user, group (grupo del propietario), others
						tambien puedes usar a para all. Se puede retirar o agregar permisos independientemente o en general
$chmod 0777	file*		Cambia los permisos de todos los grupos, activandolos
											 210   210   210 potencias
						0 7 7 7 = 0(octal) + 111 + 111 + 111 en binario 
											 421   421   421
						https://blog.desdelinux.net/permisos-basicos-en-gnulinux-con-chmod/

*
hexdump					The hexdump utility is a filter which displays the specified files, or the standard input, 
						if no files are specified, in a user specified format.
							tambienn puede llamarse con hd  
						usar hexdump -C  es igual que usar hd 

$echo Hola mundo! | hexdump -C
	00000000  48 6f 6c 61 20 6d 75 6e  64 6f 21 0a              |Hola mundo!.|
	0000000c

$echo Hola mundo! | hd 
	00000000  48 6f 6c 61 20 6d 75 6e  64 6f 21 0a              |Hola mundo!.|
	0000000c

	




***************************************************************
********CLASE3: Procesos, padres e hijos, uso de PIPE***********
***************************************************************
*
ps
$ps 						obtiene listado de PID
$ps u 					obtiene listado de PID del usuario en detalle
$ps -l x					obtiene PID Y PPID (parent PID)  (-l es long format que agrega el ppid, x es mas detalle)


$ps $$					obtiene el pid del proceso 


*pstree					obtiene un arbol del proceso
$pstree -h PID 			obtiene un arbol chiquito donde el padre es el proceso de PID indicado
$pstree -c				No mezcla los procesos hijos si estos llevan el mismo nombre.

*** EN UNIX EXISTEN TRES ARCHIVOS POR DEFECTO ***

						[0] standar imput, se redirige con <
						[1] standar output, se redirige con >
						[2] standar error


*
tee 					lee una entrada estándar y la escribe en la salida estándar y en uno o mas archivos
$tee  					recibira una entrada por terminal y la replicara por la misma terminal
$tee archivo    		Comenzara a recibir cada linea ingresada al terminal y lo replicara en el terminal
						ademas de escribirla en el archivo
						Se termina el ingreso de lineas con ctrl+c

						Si se hace $tee > archivo obviara la replica en el terminal y solo escribira en archivo 			

$tee < /bin/cat cat1 > cat2
						tee recibe la entrada estandar que ha sido redirigida por < al archivo cat 
						tee escribe en un archivo, en este caso el archivo cat1
						luego redirige cat1 a cat2, tenemos 3 archivos cat
	

$ ps -o pid,ppid,pgid,sid,comm | ./cat1 | ./cat2
						*** este es un ejemplo de pipe ***
						se ejecuta ps, estableciendo los datos que queremos ver 
						con | se redirige la salida estandar al ejecutable ./cat1 (recuerda que cat muestra el contenido de archivos)
							cat1 deberia coger esa salida estandar como entrada y mostrarla en la salida estandar pero...
						con | se redirige la salida estandar de ./cat1 a ./cat2
							cat2 recoge la salida estandar de cat1 como entrada y la muestra como salida estandar 


***SABORES DEL SHELL**************************

$which sh 				obtiene la ruta del shell 
	/bin/sh 			sh es un enlace simbolico a dash, el shell de debian (debian almquist shell)
						ojo que sh/dash es mas pequeno que bash, el shell por defecto de mint

						si desde bash llamo a dash, entonces dash sera hijo de bash, se vera en ps -l 


$exit 					salir de interprete de ordenes, si se estaba en dash se vuelve a bash



***************************************************************
********CLASE 10 11 12: ULTIMAS CLASES**********
***************************************************************

grep					Se usa para obtener la linea que contenga la cadena solicitada
						
						Ejem:
						int a;
						FILE *file;
						int fd,pidof;
						char orden[50];
						char salida[100];
						pidof = getpid();
						//ensamblamos en orden el comando para obtener la linea que contiene stack
						sprintf(orden,"cat /proc/%d/maps | grep stack ",pidof); 
						//usamos popen, ver popen en 04_syscall
						//es parecido a execl, ver explicacion de arriba, devuelve un fd hacia un hijo que envia el string usando un pipe
						file = popen(orden,"r");
						//se usa porque obtuvimos la cadena en un file y no en un string
						fgets(salida,sizeof(salida),file);

						Se obtiene la salida de cat, en la cual se busca con grep la palabra stack
						esta linea que contiene stack


fdisk					fdisk - manipulate disk partition table
						fdisk is a dialog-driven program for creation and manipulation of
       					partition tables. It understands GPT, MBR, Sun, SGI and BSD
       					partition tables.

       					https://man7.org/linux/man-pages/man8/fdisk.8.html


proc 					proc - process information pseudo-filesystem
						The proc filesystem is a pseudo-filesystem which provides an
				       interface to kernel data structures.  It is commonly mounted at
				       /proc.  Typically, it is mounted automatically by the system, but
				       it can also be mounted manually using a command such as:

			           mount -t proc proc /proc

				       Most of the files in the proc filesystem are read-only, but some
				       files are writable, allowing kernel variables to be changed.

				       Ejem:
				       sprintf(orden,"cat /proc/%d/maps",pidof); 

				       
fsstat				   Estado del archivo

						
						sudo fsstat /dev/sda5 | less
						/*
						Buscamos el primer inode en

						BLOCK GROUP INFORMATION
						--------------------------------------------
						Number of Block Groups: 200
						Inodes per group: 8176     //de aca sale inodes por group
						Blocks per group: 32768


						Group: 0:
						...
						    Inode bitmap: 1043 - 1043
						    Inode Table: 1059 - 1569
						    Data Blocks: 9235 - 32767
						  Free Inodes: 7949 (97%)
						  Free Blocks: 1077 (3%)
						...

						*/

						Teniendo el inode, pasamos a 

blkcat					Display the contents of file system data unit in a disk image. 

						sudo blkcat /dev/sda5 1059 > PUCP_ext2.inode_table_1st_block


ghex					editor hexadecimal //ojo todo aparece en little indian

						GHex can load raw data from binary files and display them
						for editing in a traditional hex editor view. 
						
						ghex PUCP_ext2.inode_table_1st_block

stat 	

$stat pagemap.c
	Fichero: «pagemap.c»						
	Tamaño: 2632  Bloques: 8  Bloque E/S: 4096  fichero regular
	Dispositivo: 807h/2055d Nodo-i: 12370985  Enlaces: 1  //de aca sale el nodo i
	Acceso: (0644/-rw-r--r--) Uid: ( 1000/ vk)  Gid: ( 1000/ vk)
	Acceso: 2015-07-06 21:41:32.833569666 -0500
	Modificación: 2015-07-06 21:41:07.693570723  -0500
	Cambio: 2015-07-06 21:41:07.693570723  -0500
	Creación: -

						//Which block is inode 12370985 in?
						// 8192 inodes per block group.
						// 12370985 – 1 / 8192 = 1510 
						//						 1510* 8192 + 1064
						// 						-------------  ====
						//                      groups 0-1509  in group 1510
						// Formula:
						// 		bg = (inode_num – 1) / inodes_per_group
						// 						  ^
						//						  |--- because inode 0 is defined not to exist


