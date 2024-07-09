//Clase3

Las funciones basicas de I/O vienen unbuffered por defecto, por ello c/u llama al un syscall
pero la meta del buffering que ofrece la libreria estandar es usar lo minimo las syscall
hay tres tipos de buffering:

	- 	Fully buffered: solo hace llamadas al sistema cuando el buffer esta lleno.
		el flush sucede cuando es llamado con fflush para flushear un stream
		o cuando el buffer se llena (esto segundo es automatico)
	-	Line buffered: la libreria realiza la llamada cuando encuentra el \n en entrada o salida
	-	unbuffered: la libreria no usa el buffer

	Segun ISO
		-	standar I/O es fully buffered cuando no se trabaja un dispositivo interactivo (ejem un archivo de texto)
		-	standar error nunca es fully buffered	

	Por ello se trabaja masomenos asi 
		-	standar error siembre unbuffered
		-	los demas streams (stdin, stdout) son line buffered en el terminal, caso contrario fully buffered 


Tabla de identificadores de archivo (id flags, file pointer)
	- id 0: Entrada de archivo, lectura. Por defecto apunta al standar input.
		Cuando se usa fork(), el hijo apuntara a la misma direccion que el padre, por tanto ya no podra leer
		aquello que ya leyo el padre.
	- id 1: Salida de archivo, escritura. Por defecto apunta al standar output
	- id 2: Error estandar. Por defecto apunta al standar output-terminal



***FUNCIONES SYSCALL: write, read, close, pipe, dup, dup2***

write();  				#include <unistd.h>
						ssize_t write(int fd, const void *buf, size_t count);

						Escribe hasta la cantidad count en bytes, desde el buf, en el archivo fd
						Si lo hace bien retorna el num de bytes escrito, sino -1 y errno se levanta para indicar error

						write puede bloquear el proceso si la data no puede ser aceptada (no espacio en pipe,
						red ocupada, etc);

						si se escribe sobre un pipe en el cual el read end ha sido cerrado, la senal
						SIGPIPE es generada, write retorna -1 y en la variable errno tendra el error del pipe.


						EJEMPLO 1:
						Bufferizacion archivo vs terminal

						#include "apue.h"

						//variables globales, no estan en la pila, sino en el segmento de datos
						int glob = 6; /* external variable in initialized data */
						char  buf[] = "a write to stdout\n";
						
						int main(void)
						{
							//variables locales en la pila
							int  var; /* automatic variable on the stack */
							pid_t  pid;
							var = 88;

							if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
								//escribe a salida estandar, desde el arreglo buf, una cantidad de bytes
								//y compara si la cantidad escrita en write no es igual que lo que se pidio (es un assert)
								//si no es igual, escribe por err_sys el mensaje
								err_sys("write error");

							printf("before fork\n");  /* we don't flush stdout */

							if ((pid = fork()) < 0) { 
								err_sys("fork error"); //si fork no falla... este es un assert
							} else if (pid == 0) { //es hijo
								glob++;  /* modify variables */
								var++;
							} else {
								sleep(2); //padre
							}

							printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
							exit(0);
						}

							Podremos ver que ni la variable local ni global en el padre cambiaron
							
							$ ./a.out
							a write to stdout 	//write()
							before fork			//printf() //solo se imprime una vez porque es line buffered en terminal
							pid = 430, glob = 7, var = 89
							pid = 429, glob = 6, var = 88 

							Pero si trabajamos hacia un archivo, el printf se buferiza, asi antes del fork
							no se ha impreso aun, y cuando fork hace la copia del proceso, copia tambien el buffer 
							y por ello al finalmente imprimir el ultimo printf, libera todo lo que tenia en el buffer
							en el programa padre e hijo

							$ ./a.out > temp.out
							$ cat temp.out
							a write to stdout 	//write()
							before fork			//printf() //se imprime dos veces porque es fully buffered en archivo
							pid = 432, glob = 7, var = 89
							before fork			//printf()
							pid = 431, glob = 6, var = 88

*
read()						read - read from a file descriptor

							#include <unistd.h>
       						ssize_t read(int fd, void *buf, size_t nbytes);

       						read() lee una cantidad nbytes de bytes, desde el filedescriptor fd, 
       						hacia el buffer buf.
       						en caso de archivos binarios en que se usa seek, si la lectura supera el count,
       						no se lee nada y retorna 0

       						retorna el mum de bytes leido, 0 indica EOF.
       						retorna -1 indicando error, errno es seteada indicando error.

       						Read puede bloquear el proceso si no hay data que se pueda leer desde el pipe
       						(pipe, terminal devices, network devices)

       						Si se usa read en un pipe que ha cerrado su write, read retorna 0 como si fuese
       						EOF. Por ello hay que tener cuidado acerca de desciptores de lectura/escritura.

       						https://man7.org/linux/man-pages/man2/read.2.html
       							
*
stat()						 stat, fstat, lstat, fstatat - get file status

							#include <sys/stat.h>

					        int stat(const char *restrict pathname, struct stat *restrict statbuf); //ruta
					        int fstat(int fd, struct stat *statbuf);  //archivo abierto 
					        int lstat(const char *restrict pathname, struct stat *restrict statbuf); //ruta
 
					        These functions return information about a file, in the buffer
							pointed to by statbuf.  No permissions are required on the file
							itself, but—in the case of stat(), fstatat(), and lstat()—execute
							(search) permission is required on all of the directories in
							pathname that lead to the file.

							stat() and fstatat() retrieve information about the file pointed
							to by pathname; the differences for fstatat() are described below.

							lstat() is identical to stat(), except that if pathname is a
							symbolic link, then it returns information about the link itself,
							not the file that the link refers to. ***VER SYMLINK***

							fstat() is identical to stat(), except that the file about which
							information is to be retrieved is specified by the file descriptor fd.

							-return	
							todos retornan 0 if OK, -1 if ERROR 

							La estructura statbuf tiene la siguiente forma

							struct stat {
								dev_t     	st_dev;         /* ID of device containing file */
								ino_t     	st_ino;         /* Inode number */
								mode_t    	st_mode;        /* File type and mode */
								nlink_t   	st_nlink;       /* Number of hard links */
								uid_t     	st_uid;         /* User ID of owner */
								gid_t     	st_gid;         /* Group ID of owner */
								dev_t     	st_rdev;        /* Device ID (if special file) */
								off_t     	st_size;        /* Total size, in bytes */
								blksize_t 	st_blksize;     /* Block size for filesystem I/O */
								blkcnt_t  	st_blocks;      /* Number of 512B blocks allocated */
								time_t 		st_atime; 		/* time of last access */
								time_t 		st_mtime; 		/* time of last modification */
								time_t 		st_ctime; 		/* time of last status change */

							};



*
open()						open, openat, creat - open and possibly create a file
							#include <fcntl.h>

      						int open(const char *pathname, int flags);
       						int open(const char *pathname, int flags, mode_t mode);

       						-pathname:
       						The open() system call opens the file specified by pathname.  If the specified file
       						does not exist, it may optionally (if O_CREAT is specified in flags) be created by open().	

     						-mode:
       						The argument flags must include one of the following access modes: 
       							O_RDONLY, O_WRONLY, or O_RDWR.  
       						These request opening the file read-only, write-only, or read/write, respectively.

       						-flags:
					       	In addition, zero or more file creation flags and file status flags can be bitwise-or`d in flags.  

							The file creation flags are O_CLOEXEC, O_CREAT, O_DIRECTORY, O_EXCL, O_NOCTTY, 
							O_NOFOLLOW, O_TMPFILE, and O_TRUNC.  

							The file status flags are all of the remaining flags listed below.  
							Son un huevo, asi que aqui esta el link
							https://man7.org/linux/man-pages/man2/openat.2.html	

								- The distinction between these two groups of flags is that the file creation flags 
								affect the semantics of the open operation itself, while the file status
								flags affect the semantics of subsequent I/O operations.  The file status flags 
								can be retrieved and (in some cases) modified; see fcntl(2) for details.

							-return:	
							On success, open(), openat(), and creat() return the new file descriptor 
							(a nonnegative integer).  On error, -1 is returned and errno is set to indicate the error.

     						The return value of open() is a file descriptor, a small,
       						nonnegative integer that is an index to an entry in the process`s
       						table of open file descriptors.  The file descriptor is used in
       						subsequent system calls (read(2), write(2), lseek(2), fcntl(2),
       						etc.) to refer to the open file.  The file descriptor returned by
       						a successful call will be the lowest-numbered file descriptor not
       						currently open for the process.

       						-otras cositas:       						
							By default, the new file descriptor is set to remain open across
							an execve(2) (i.e., the FD_CLOEXEC file descriptor flag described
							in fcntl(2) is initially disabled); the O_CLOEXEC flag, described
							below, can be used to change this default.  The file offset is
							set to the beginning of the file (see lseek(2)).       						

							A call to open() creates a new open file description, an entry in
					       	the system-wide table of open files.  The open file description
					      	records the file offset and the file status flags (see below).  A
					       	file descriptor is a reference to an open file description; this
					       	reference is unaffected if pathname is subsequently removed or
					       	modified to refer to a different file.  For further details on
					       	open file descriptions, see NOTES.

					       	-errores:
					       	https://man7.org/linux/man-pages/man2/openat.2.html	


*
close()						close - close a file descriptor
							#include <unistd.h>
       						int close(int fd);

       						close() closes a file descriptor, so that it no longer refers to any file and may be reused.
       						Any record locks (see fcntl(2)) held on the file it was associated with, and owned by the 
       						process, are removed (regardless of the file descriptor that was used to obtain the lock).

					        If fd is the last file descriptor referring to the underlying
					        open file description (see open(2)), the resources associated
					        with the open file description are freed; if the file descriptor
					        was the last reference to a file which has been removed using
					        unlink(2), the file is deleted.

					        return:
					        close() returns zero on success.  On error, -1 is returned, and
					        errno is set to indicate the error.

					       	errors:

					        EBADF  fd isn t a valid open file descriptor.

					        EINTR  The close() call was interrupted by a signal; see
					              signal(7).

					        EIO    An I/O error occurred.

					        ENOSPC, EDQUOT
					               On NFS, these errors are not normally reported against the
					               first write which exceeds the available storage space, but
					               instead against a subsequent write(2), fsync(2), or
					               close().

*  
lseek()						lseek - reposition read/write file offset
       						
       						#include <unistd.h>
       						off_t lseek(int fd, off_t offset, int whence);

       						lseek() repositions the file offset of the open file description
					        associated with the file descriptor fd to the argument offset
					        according to the directive whence as follows:

					        SEEK_SET
					              The file offset is set to offset bytes.

					        SEEK_CUR
					              The file offset is set to its current location plus offset
					              bytes.

					        SEEK_END
					              The file offset is set to the size of the file plus offset
					              bytes.

					        lseek() allows the file offset to be set beyond the end of the
					        file (but this does not change the size of the file).  If data is
					        later written at this point, subsequent reads of the data in the
					        gap (a "hole") return null bytes ('\0') until data is actually
					        written into the gap.


*
mkdir()					    mkdir - make directories   
							mkdir() attempts to create a directory named pathname.
       						
       						#include <sys/stat.h>
       						int mkdir(const char *pathname, mode_t mode);

       												ruta	modo: modo de acceso (bit de proteccion)
       						retorna 0 si Ok, -1 si Error

*
rmdir()						rmdir - remueve directorios, solo si esta vacio
							#include <unistd.h>
							int rmdir(const char *pathname);

							retorna 0 si Ok, -1 si Error

*
link()						link, linkat - make a new name for a file						
	
							#include <unistd.h>
							int link(const char *existingpath, const char *newpath);

							link() creates a new link (also known as a hard link) to an existing file.

       							If newpath exists, it will not be overwritten.

							This new name may be used exactly as the old one for any
							operation; both names refer to the same file (and so have the
							same permissions and ownership) and it is impossible to tell
							which name was the "original"   

							- return
							On success, zero is returned.  On error, -1 is returned, and
       						errno is set to indicate the error.

*
unlink()					unlink, unlinkat - delete a name and possibly the file it refers to

							#include <unistd.h> 
							int unlink(const char *pathname);								    							

							unlink() deletes a name from the filesystem.  

							-si fue ultimo link y no esta en uso:
							If that name was the last link to a file and no processes have the file open, the
							file is deleted and the space it was using is made available for
							reuse.

							-si fue ultimo link y archivo esta en uso:
							If the name was the last link to a file but any processes still
							have the file open, the file will remain in existence until the
							last file descriptor referring to it is closed.

							If the name referred to a symbolic link, the link is removed.

							If the name referred to a socket, FIFO, or device, the name for
							it is removed but processes which have the object open may
							continue to use it.

							- return
							On success, zero is returned.  On error, -1 is returned, and
       						errno is set to indicate the error.

*
symlink()					symlink, symlinkat - make a new name for a file       

							#include <unistd.h>
							int symlink(const char *target, const char *linkpath);
										ruta actual 			ruta enlace 

							symlink() creates a symbolic link named linkpath which contains
       						the string target.

       						El enlace linkpath va a apuntar a actualpath, no al inode, por eso es llamado softlink

							- return
							On success, zero is returned.  On error, -1 is returned, and
       						errno is set to indicate the error.

       						***Como open() al abrir un softlink leeria la direccion hacia el softlink apunta, 
       						no el archivo, se necesita otra herramienta que abra ese link y llegue al archivo ***

*
readlink()					readlink, readlinkat - read value of a symbolic link

							#include <unistd.h>
							ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsiz);

							readlink() places the contents of the symbolic link pathname in
							the buffer buf, which has size bufsiz.  readlink() does not
							append a terminating null byte to buf.  It will (silently)
							truncate the contents (to a length of bufsiz characters), in case
							the buffer is too small to hold all of the contents.

							-return
							On success, these calls return the number of bytes placed in buf.
							(If the returned value equals bufsiz, then truncation may have occurred.)  
							On error, -1 is returned and errno is set to indicate the error.

*
mount()						mount - mount filesystem

							#include <sys/mount.h>

     						int mount(const char *source, const char *target,
                 			const char *filesystemtype, unsigned long mountflags, const void *data);					

                 			mount() attaches the filesystem specified by source (which is
							often a pathname referring to a device, but can also be the
							pathname of a directory or file, or a dummy string) to the
							location (a directory or file) specified by the pathname in target.

							Appropriate privilege (Linux: the CAP_SYS_ADMIN capability) is
							required to mount filesystems.

							filesystemtype argument supported by the kernel
							are listed in /proc/filesystems (e.g., "btrfs", "ext4", "jfs",
							"xfs", "vfat", "fuse", "tmpfs", "cgroup", "proc", "mqueue",
							"nfs", "cifs", "iso9660").  Further types may become available
							when the appropriate modules are loaded.

							On success, zero is returned.  On error, -1 is returned, and
							errno is set to indicate the error.

*
umount()					umount, umount2 - unmount filesystem

							#include <sys/mount.h>

							int umount(const char *target);
							
							umount() and umount2() remove the attachment of the (topmost)
							filesystem mounted on target.

							Appropriate privilege (Linux: the CAP_SYS_ADMIN capability) is
							required to unmount filesystems.		

							On success, zero is returned.  On error, -1 is returned, and
							errno is set to indicate the error.	

*
chdir(), fchdir()			chdir, fchdir - change working directory

							#include <unistd.h>

							int chdir(const char *path);
							int fchdir(int fd);										

							chdir() changes the current working directory of the calling
							process to the directory specified in path.

							fchdir() is identical to chdir(); the only difference is that the
							directory is given as an open file descriptor.

							On success, zero is returned.  On error, -1 is returned, and
							errno is set to indicate the error.

*
chmod() fchmod()			change the file access permissions for an existing file

							#include <sys/stat.h>
							int chmod(const char *pathname, mode_t mode);
							int fchmod(int filedes, mode_t mode);

							The chmod() and fchmod() system calls change a files mode bits.
							(The file mode consists of the file permission bits plus the set-
							user-ID, set-group-ID, and sticky bits.)  These system calls
							differ only in how the file is specified:

							* chmod() changes the mode of the file specified whose pathname
							is given in pathname, which is dereferenced if it is a symbolic
							link.

							* fchmod() changes the mode of the file referred to by the open
							file descriptor fd.

							The new file mode is specified in mode, which is a bit mask
							created by ORing together zero or more of the following:

							https://man7.org/linux/man-pages/man2/chmod.2.html

							On success, zero is returned.  On error, -1 is returned, and
       						errno is set to indicate the error.

*
kill()						kill - send signal to a process
							#include <signal.h>

       						int kill(pid_t pid, int sig);

							The kill() system call can be used to send any signal to any
							process group or process.

							*If pid is positive, then signal sig is sent to the process with
							the ID specified by pid.
							*If pid equals 0, then sig is sent to every process in the process
							group of the calling process.
							*If pid equals -1, then sig is sent to every process for which the
							calling process has permission to send signals, except for
							process 1 (init), but see below.
							*If pid is less than -1, then sig is sent to every process in the
							process group whose ID is -pid.
							*If sig is 0, then no signal is sent, but existence and permission
							checks are still performed; this can be used to check for the
							existence of a process ID or process group ID that the caller is
							permitted to signal.

							On success (at least one signal was sent), zero is returned.  On
							error, -1 is returned, and errno is set to indicate the error.

							The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.

							ver: $man 7 signal   $kill -l

*
popen()						FILE *popen(const char *command, const char *mode);

							popen - initiate pipe streams to or from a process

							The popen() function shall execute the command specified by the string command. 
							It shall create a pipe between the calling program and the executed command, 
							and shall return a pointer to a stream that can be used to either read from 
							or write to the pipe.

							The environment of the executed command shall be as if a child process were 
							created within the popen() call using the fork() function, 
							and the child invoked the sh utility using the call:

							execl(shell path, "sh", "-c", command, (char *)0);

*
create()					create a new file or rewrite an existing one
						    
						    #include <sys/stat.h>
						    #include <fcntl.h>
						    int creat(const char *path, mode_t mode);

						    The creat() function shall behave as if it is implemented as follows:
				            int creat(const char *path, mode_t mode)
				            {
				                return open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
				            }

				            Ejem 
				            The following example creates the file /tmp/file with read and
					       	write permissions for the file owner and read permission for
					       	group and others. The resulting file descriptor is assigned to
					       	the fd variable.

				            #include <fcntl.h>
				            ...
				            int fd;
				            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; //rw- r-- r--
				            char *pathname = "/tmp/file";
				            ...
				            fd = creat(pathname, mode);
				            ...

				            
