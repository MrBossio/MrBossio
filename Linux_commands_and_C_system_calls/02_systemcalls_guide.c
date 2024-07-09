


//***Process management***

pid = fork( )
 //Create a child process identical to the parent
pid = waitpid(pid, &statloc, options)
 //Wait for a child to terminate
s = execve(name, argv, environp)
 //Replace a process’ core image
exit(status)
 //Terminate process execution and return status

//***File management***

fd = open(file, how, ...)
 //Open a file for reading, writing, or both
s = close(fd)
 //Close an open file
n = read(fd, buffer, nbytes)
 //Read data from a file into a buffer
n = write(fd, buffer, nbytes)
 //Write data from a buffer into a file
position = lseek(fd, offset, whence)
 //Move the file pointer
s = stat(name, &buf)
 //Get a file’s status infor mation

//***Directory- and file-system management***

s = mkdir(name, mode)
 //Create a new directory
s = rmdir(name)
 //Remove an empty directory
s = link(name1, name2)
 //Create a new entr y, name2, pointing to name1
s = unlink(name)
 //Remove a director y entr y
s = mount(special, name, flag)
 //Mount a file system
s = umount(special)
 //Unmount a file system

//***Miscellaneous***

s = chdir(dir name)
//Change the wor king directory
s = chmod(name, mode)
//Change a file’s protection bits
s = kill(pid, signal)
//Send a signal to a process
seconds = time(&seconds)
//Get the elapsed time since Jan. 1, 1970






