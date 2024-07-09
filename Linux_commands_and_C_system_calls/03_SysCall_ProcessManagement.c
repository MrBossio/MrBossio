
//Clase 3
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


Al iniciar un proceso se tienen 3 segmentos: Text (programa), Data (variables), Stack(pila)
entre data y stack hay un espacio libre que estos dos iran llenando conforme lo necesiten
(data hacia arriba, stack hacia abajo). El stack se llena automatico, pero el data se llena usando malloc



***FUNCIONES QUE LLAMAN AL BASH***

system("un_string");	int system(const char*un_string) //recibe una cadena
						ejecuta una accion llamando al shell, empleando el string en su interior
						retorna -1 si error, 0 si no hay error, un numero x no cero si hay error en el shell
						
						system("echo and bye."); imprime:  
							and bye.
						esto sucede porque el string es la linea de comando que se escribiria en el shell 

						si son varias lineas, se separan con ;
						system("echo and bye.; ps -l"); imprime:  and bye. y luego llama a ps -l

						//si se indica un exit para la funcion system imprime ese exit en WEXITSTATUS
						status = system("echo and bye.; foo; exit 44"); //exit 44 retorna 44 a status
 						printf("status = %d\n", WEXITSTATUS(status)); imprime: 
 							sh: 1: foo: not found
							status = 44

						en cambio 
						status = system("echo and bye.; foo"); //no se especifico un exit
						printf("status = %d\n", WEXITSTATUS(status));  imprime:
							sh: 1: foo: not found //error
							status = 127	//no cero

***FUNCIONES SYSCALL***

getpid();				obtiene el PID del proceso actual
						printf("Hola desde el proceso ID: %d\n", getpid());

getppid();				obtiene el PID del proceso padre del proceso actual
						printf("Hola, mi padre es el proceso ID: %d\n", getppid());

fork();					#include <unistd.h>
						pid_t fork(void);  //ojo, no recibe parametros, devuelve una variable tipo pid_t

						crea un proceso hijo duplicando el proceso original, 
						guardamos el return de fork: devuelve el PID del hijo al padre, 0 al hijo, -1 en caso de error
						entonces en runtime se tienen dos momentos, en el padre se usa if(pid =! 0)
						y en hijo if(pid == 0)
						en el shell, el ppid del hijo es el pid del padre

						En resumen,busca una entrada libre de la tabla de procesos y la reserva para el hijo
						luego le asigna un pid libre
						copia todo el contexto, comparte todo lo compartible (no lo copia)
						las tablas de control a archivos tambien se copian, 
						pero el kernel toma cuenta de ello incrementando los contadores obtiene de
						la tabla de archivos, de inodes, y tablas globales del kernel 


						Cuando se llama a fork, el kernel toma el control y realiza lo siguiente:
							Separa espacio para la estructura del hijo
							llena la estructura del hijo como el padre
							separa espacio para el stack del hijo y area del usuario (variables con valores)
							llena el area de usuario como el padre
							asigna un pid al hijo
							setea al hijo para que comparta el mismo codigo (porque el codigo es el mismo)
							copia tablas de pagina de data y stack
							setea el uso de archivos abiertos
							copia los registros del padre al hijo 


						EJEMPLO 1:	
						fork simple

						pid_t pid; //declara variable pid
						printf("pid = %d, ppid = %d\n", getpid(), getppid()); //imprime pid y ppid
						fflush(stdout); //esto es para que, la salida del printf no se vuelva entrada del return de fork
						
						//aqui se crea el fork, el ==-1 es el assert a prueba de errores
						if ((pid=fork()) == -1) { 
							//en este instante, pid en el padre tiene el pid del hijo
							//pid en el hijo tiene 0, ojo pid es una variable, pero no es lo que imprimimos
							perror("fork failed");
							exit(1);
						}
						//esto en el padre imprimira su pid y el pid de su padre
						//esto en el hijo imprimira su pid de hijo, y el pid del proceso original
						printf("pid = %d, ppid = %d\n", getpid(), getppid());
						printf("that’s all\n");
						exit(0);

							pid = 21442, ppid = 18695
							pid = 21442, ppid = 18695
							that’s all
							pid = 21443, ppid = 21442 //notese que ppid del hijo es el pid del padre
							that’s all

						//EJEMPLO EN PYTHON ESTA EN CLASE 3, DIAPO 2, PAGINA 31

						EJEMPLO 2: 
						hijo hace pausa y el padre termina antes que el hijo, 
						por tanto el hijo es adoptado por init, el programa padre de todos, de pid 1

						pid_t pid, ppid;
						printf("pid = %d, ppid = %d\n", getpid(), getppid());
						fflush(stdout);
						if ((pid=fork() == -1) {
							perror("fork failed"); exit(1);
						}

						if (!pid) sleep(1); //el hijo hace una pausa, eso hara que su padre muera primero

						//esto en el padre imprimira su pid y el pid de su padre
						//esto en el hijo imprimira su pid de hijo, y el pid del proceso init
						printf("pid = %d, ppid = %d\n", getpid(), (ppid=getppid()));

						if (ppid == 1) printf("I have a new father - init\n");
						printf("that’s all\n");
						exit(0);
					
							pid = 21756, ppid =  18695
							pid = 21756, ppid =  18695
							that’s all
							$ pid = 21757, ppid  = 1 //hijo ya no apunta al padre, ahora apunta a init
							//ojo, aca aparece $ porque el padre acababa de terminar, 
							//pero en vez de recibir otra entrada, recibio el print del hijo
							I have a new father - init
							that’s all

						EJEMPLO 3:
						creando un daemon, sea el nombre del ejecutable prog7a

						if (daemon(0,0) == -1) {
							perror("daemon function failed");
							exit(1);
						}						
						while(1); //el proceso no termina

							$./prog7a			//no termina
							$ ps -l -C prog7a 	//cuando ps (process status) lista la info del programa
												//el ppid sera 1, porque el daemon se desvincula de su padre
												//el TTY (TeleTYpewriter, o Controler Terminal) 
												//sera ? porque es un bucle infinito, no tiene terminal asociado
							$ kill -KILL 3398	//matamos el proceso porque no iba a terminar
							$ ps -l -C prog7a   //los campos salen vacios porque se mato al daemon


						EJEMPLO 3:
						cadena de procesos

						int i, n=4;
						pid_t pid, ppid;
						for (i=0; i<n; i++) {
							if ((pid=fork() == -1) { perror("fork failed"); exit(1); }

							//aqui se rompe el for en el padre (pid != 0), pero sigue en el hijo (pid = 0  false)	
							if (pid) break;
						}
						printf("pid = %d, ppid = %d\n", getpid(), (ppid=getppid()));
						sleep(5); //se hace el sleep solo para tener el primer printf seguido de todos los programs
						printf("that’s all\n");
						exit(0);

							$ ./prog8
							pid = 21869, ppid = 18695 
							pid = 21870, ppid = 21869 //se puede ver que cada hijo tiene por ppid al proc anterior
							pid = 21871, ppid = 21870
							pid = 21872, ppid = 21871
							pid = 21873, ppid = 21872
							that’s all
							that’s all
							that’s all
							that’s all
							that’s all

						//tambien si se ejecuta ./prog8 &   (o sea en backgound) e inmediatamente se lanza ps -l
						//se ve que todos tienen el mismo CMD (command)	


						EJEMPLO 4:
						proceso zombieeeeeee

						Si en el ejercico anterior usamos if (pid) sleep(5);
						el padre entrara en sleep, el hijo no, por tanto el hijo hara un exit(), que no es recibido por el padre

						A eso se denomina zombie, y en la columna S de ps-l aparecera una Z

						como el padre esta dormido, todos los recursos asignados al hijo se limpian, pero el pid y el valor de return se conserva
						hasta que el padre despierte y reciba la senal de fin del proceso del hijo

						para evitar esto, en el padre se usa el syscall wait(), que espera la senal de exit() del hijo

						un zombie no se puede matar, alguien debe tomar su exit 


						ZOMBIE: padre detenido, no recibe exit del hijo. El hijo libera todos sus recursos, queda solo el exit. 
						HUERFANO: padre finalizado, hijo es adoptado por init. 

wait(); waitpid();		#include <sys/types.h>
						#include <sys/wait.h>

						pid_t wait(int *status); //suspende la ejecucion hasta que termine un hijo cualquiera

						pid_t waitpid(pid_t pid, int *status, int options); //suspende hasta que termine un hijo de pid especifico

						o hasta que se produce una señal cuya acción es terminar el  proceso  actual  o
       					llamar  a  la  función  manejadora  de la señal. Si un hijo ha salido cuando se produce la
       					llamada (lo que se entiende por proceso "zombie"), la función vuelve inmediatamente. Todos
       					los recursos del sistema reservados por el hijo son liberados.

       					en waitpid(), 
       					- el valor pid_t pid determina a quien se espera:

							pid<-1   significa cualquier proceso hijo cuyo pid es igual al valor absoluto de pid.
							pid=-1   significa cualquier proceso hijo; entrega el mismo comportamiento que tiene wait().
							pid=0    significa  cualquier proceso hijo cuyo pid es igual al del padre
							pid>0 	significa un proceso hijo de pid especifico.

						- el valor int options	es un OR de cero o más de las siguientes constantes:
							
							WNOHANG
              				que significa que vuelve inmediatamente si ningún hijo ha terminado.

       						WUNTRACED
				            que significa que también vuelve si hay hijos parados (pero no  rastreados),  y  de
				            cuyo  estado  no ha recibido notificación.  El estado para los hijos rastreados que
				            están parados también se proporciona sin esta opción.
						
						en waitpid() y wait(),
						- el valor int *status es un puntero	
						  	
						  	Si status no es NULL (o sea se desea almacenar el resultado del fin del proceso hijo),
						  	wait o waitpid almacena la información de estado en la memoria
       						apuntada por status. Debe haberse declarado como int nombrevariable.

       						Si  el  estado puede ser evaluado con las siguientes macros (dichas macros toman el buffer
       						stat (un int) como argumento — ¡no un puntero al buffer!) entonces:

       							WIFEXITED(status)
       							WEXITSTATUS(status)
       							WIFSIGNALED(status)
       							WTERMSIG(status)
       							WIFSTOPPED(status)
								WSTOPSIG(status)	//ver http://manpages.ubuntu.com/manpages/bionic/es/man2/wait.2.html

						- el valor de RETURN:	
							
							El pid del proceso del hijo que terminó
							0  si  se  utilizó  WNOHANG  y  no  hay  hijo disponible
							-1 en caso de error (en este caso, errno se pone a un valor apropiado).		


						EJEMPLO 1:
						waitpid() para esperar el fin del proceso hijo


						int i, n=4;
						pid_t childpid, mypid, ppid;
						
						//esta parte es igual a la anterior, se hace una cadena de procesos
						for (i=0; i<n; i++) {
							if ((childpid=fork() == -1) {perror("fork failed");exit(1);}
							if (childpid) break;
						}
						printf("pid = %d, ppid = %d\n", (mypid=getpid()), (ppid=getppid()));

						if (childpid) waitpid(childpid, NULL, 0); //si es el padre espera al hijo
						//el padre espera al hijo, childpid se obtuvo en el fork
						//en waitpid() se introduce un childpid > 0, o sea el pid del hijo especifico

						printf("%d: that’s all\n", mypid);
						exit(0);

						
						EJEMPLO 2:
						abanico de procesos, un padre con muchos hijos

						int i, n=4;
						pid_t childpid, mypid, ppid;
						for (i=0; i<n; i++) {
							if ((childpid=fork() == -1) {perror("fork failed");exit(1);}
							
							//a diferencia de la cadena, aca se hace break en el hijo, no en el padre
							//por eso el for continua en el padre, y se detiene en el hijo	
							if (!childpid) break;
						}
						printf("pid = %d, ppid = %d\n", (mypid=getpid()), (ppid=getppid()));
						
						//si estamos en el padre, 
						if (childpid) 

							//esperamos cualquier proceso hijo (pid_t pid = -1) que termine						
							for (i=0; i<n; i++) waitpid(-1, NULL, 0);

						//si estamos en el hijo, se duerme un segundo, 
						//esto solo es para probar que el waitpid funciona y espera	en el padre	
						else sleep(1);

						printf("%d: that’s all\n", mypid);
						exit(0);


						EJEMPLO 3: 
						arbol de procesos

						int i, n=3;
						pid_t childpid, mypid, ppid; /* no es el momento para getppid() */

						for (i=0; i<n; i++)
							
							//en el padre ORIGINAL, se hacen 3 fork, los tres primeros hijos
							//pero el valor de i para el primer hijo sera 0, para el segundo sera 1 y para el tercero 2
							//por ende el primer hijo, podra avanzar i 2 veces, o sea dos hijos
							//en el segundo hijo, i avanza una vez, tendra un hijo
							//en el tercer hijo, i no puede avanzar, no tendra hijos
							if ((childpid=fork() == -1) {perror("fork failed");exit(1);}
							
							//dentro del primer nieto, i sera 1, por lo que solo avanzara a 2, tendra un hijo
							//el resto no tiene hijos
							
														695
														 |
														781
													/	 |	 \
												782	    783	   785
											  / 	\     \
											784     786    788
											 |
											787
	
						
						printf("pid = %d, ppid = %d\n", (mypid=getpid()), (ppid=getppid()));

						//si es padre, esperara a todos sus hijos usando el while
						//waitpid retornara -1 por error cuando no tenga mas hijos
						if (childpid) while (waitpid(-1, NULL, 0) != -1);
						//si es hijo duerme un segundo para que el waitpid los pueda atrapar, antes de ser zombies
						else sleep(1);
						printf("%d: that’s all\n", mypid);
						exit(0);



execve();				#include <unistd.h>
      					int execve(const char *pathname, char *const argv[], char *const envp[]);

      					Ejecuta el programa indicado en el pathname
      					-	pathname puede ser un binario ejecutable, o un script
      					-	argv es una lista de argumentos, como si se escribiesen en la linea de comando
      						por convencion ***el primero de la lista es el archivo asociado al archivo que se ejecutara***
      						debe terminar en NULL
      					-	envp es otra lista de argumentos de la forma key = value, que se pasan como el enviroment
      						del nuevo programa

      					RETURN
      						Si se realiza con exito, no retorna
      						en error, retorna -1 setea errno para indicar el error

      						ver: https://man7.org/linux/man-pages/man2/execve.2.html

      					Cuando se ejecuta execve(), pasa el control al kernel para ejecutar las siguientes tareas:
      						- Ubica el programa ejecutable
      						- verifica los permisos de ejecucion, si es ejecutable
      						- lee y verifica el header del archivo que contiene el programa
      						- copia argumentos y ambiente al kernel
      						- libera espacio de direccion que tenia el programa anterior (se liberan todos esos espacios)
      						- asigna un nuevo espacio de direccion 
      						- copia argumentos, entorno al stack 
      						- reinicia las senales (porque el nuevo programa se ejecuta desde su inicio)
      						- inicializa los registros (porque el nuevo programa se ejecuta desde su inicio)

      						OJO: el programa nuevo tendra el mismo PID del anterior
      							- el nuevo programa tiene un exit
      							- el padre espera el exit del nuevo programa (porque es el mismo childpid)


      					Existen 6 formas de llamar al exec		
      					
      					#include <unistd.h>
      					int execl(const char *pathname, const char *arg0, ... /* (char *)0 */ );
      					execl:	pathname, lista de argumentos separada por coma y termina con caracter 0

      					int execv(const char *pathname, char *const argv []);
      					execv:	pathname, vector de argumentos

      					int execle(const char *pathname, const char *arg0, ... /* (char *)0*/, char *const envp[] );
      					execle:	igual que execl + vector CHAR* de envp de la forma key = value que termina en NULL
      					
      					int execve(const char *pathname, char *const argv[], char *const envp []);
      					execve: igual que execv + vector CHAR* de envp de la forma key = value que termina en NULL
      					
      					int execlp(const char *filename, const char *arg0, ... /* (char *)0 */ );
      					execlp:	igual que execl pero en vez de pathname, se usa el nombre de archivo, buscara en el PATH	
      					
      					int execvp(const char *filename, char *const argv []);
      					execvp: igual que execv pero en vez de pathname, se usa el nombre de archivo, buscara en el PATH 


      					Como funciona un pipe de fork+exec:
      						sea ps | cat1 | cat2
      							sh2  sh3    sh1
      						Hay que asumir que siempre se arranca desde el shell (aka sh0) asi que los pasos seran:
      						-	sh0 hace fork para crear otro sh(1), esto sucede POR DEFECTO
      						-   Aqui recien revisa cuantos procesos se tienen que realizar
      						-   Como son 3 procesos, decide reusarse para el ultimo proceso cat2, por tanto faltan dos procesos
      						-	sh1 hace dos fork sh2 y sh3, uno para ps y otro para cat1 
      						-	sh2 hace exec a ps, salida a pipe
      						-	sh3 hace exec a cat1, que recibe de entrada del pipe ps, y salida a pipe 
      						-	sh1 que esta desocupado, hace exec a cat2, recibe de entrada el pipe cat1
      						-	cat2 entrega la salida a sh0


      					EJEMPLO 1: 
      					exe con fork

      					#include <sys/types.h>
						#include <stdio.h>
						#include <unistd.h>
						int main()
						{
							pid_t pid;
							/* fork a child process */
							pid = fork();
							if (pid < 0) { /* error occurred */
								fprintf(stderr, "Fork Failed");
								return 1;
							}
							else if (pid == 0) { /* child process */
								execlp("/bin/ls", "ls", NULL);

									//pathname, argumento, entorno
								}
							else { /* parent process */
							/* parent will wait for the child to complete */
								wait (NULL);
								printf ("Child Complete");
							}
							return 0;
						}

*
dup()					int dup(int old_file_descriptor)
						dup() copia old_file_descriptor en el primer file_descriptor libre de la tabla de fds 
						para ello, primero se debe liberar uno  
						close(STD_OUTPUT); dup(fd[1]); copiara fd[1] a la entrada que libero STD_OUTPUT 
						close(STD_INPUT); dup(fd[0]); copiara fd[0] a la entrada que libero STD_INPUT 
 						
 						*******
 						EJEMPLO:  Programa que puede convertir a mayuscula el codigo del programa
 						permite usar de entrada el archivo mismo redirigido con  ./capital < archivolectura
 						o directamente desde el terminal  ./capital archivolectura

						#include <stdio.h>
						#include <ctype.h>
						#include <fcntl.h>
						#include <unistd.h>
						#define STD_IN  0

						int main(int argc, char *argv[])
						{
							int c, fd;
							/* si está dado el parámetro, lo consideramos como el nombre del archivo, lo abrimos para
							obtener su descriptor. Consideramos este archivo como la entrada estándar. */
							if (argc == 2) { 
								fd = open(argv[1], O_RDONLY); //crear descriptor con el primer argumento de la linea de comandos
								close(STD_IN); dup(fd); //al cerrar STD_IN, dup(fd) copiara ese filedescriptor en STD_IN
								//al ser el primero libre para uso
							}
							while ((c = getchar()) != EOF) putchar(toupper(c));
							return 0;
						}


*
dup2()					int dup2(int old_file_descriptor, int new_file_descriptor);

						dup2() function copies the old_file_descriptor into the new_file_descriptor. 
						If the new_file_descriptor already exists, then it’s automatically closed 
						and then the old_file_descriptor is copied to it.

*
pipe();					int pipe(int fds[2]);

						Parameters :
						fd[0] will be the fd(file descriptor) for the 
						read end of pipe (o sea el write en el parent).
						fd[1] will be the fd for the write end of pipe (o sea el read en child).

						Returns : 0 on Success. -1 on error.


						Parent read STDIN	---x						/ --- Pipe write 1  	STDIN Child read 
																	Pipe
						Parent write STDOUT	---		Pipe read 0 ---/ 				 x---		STDOUT Child read

						*******
						Ejemplo:


						#define STD_INPUT 0
						#define STD_OUTPUT 1
				
						char *process1, *process2;

						pipeline(process1, process2)
						{
							int fd[2];	//kernel colocara los fd de lectura y escritura del pipe aca
							//este es un arreglo local, por lo que se puede cerrar
							//sin afectar al otro proceso
							
							//creamos el pipe, guardamos sus fd en el arreglo
							pipe(&fd[0]);	//tambien sirve pipe(fd);

							/* 
							El pipe se crea con sus propios fd

										---		fd[0]	---
							pProcess	|					|  Pipe 
										---		fd[1]	---

							Ojo, el proceso sigue teniendo sus propios fd			

										---	STD_INPUT
							pProcess	|			
										---	STD_OUTPUT			

							
							Ojo, cuando se usa fork, padre e hijo apuntan al mismo pipe

							
										---		fd[0]	---				---		fd[0]	---
							pProcess	|					|  Pipe 	| 					| cProcess
										---		fd[1]	---				---		fd[1]	---


							Ojo, los procesos siguen teniendo sus propios fd			

										---	STD_INPUT
							pProcess	|			
										---	STD_OUTPUT		

										---	STD_INPUT
							cProcess	|			
										---	STD_OUTPUT	


							*/

							if (fork() != 0) {
							 /* El proceso padre */
								close(fd[0]); 

								//cerramos el fd[0] del pipe desde el proceso padre
								/*
											---		null	---				
								pProcess	|					|  Pipe 	
											---		fd[1]	---				

								fd[] = {null, fd[1]}
								*/

								dup2(fd[1],STD_OUTPUT);
								//duplicamos el fd[1] del pipe padre y lo ponemos al parent
								//PARA ESTE EJEMPLO, SE TRABAJARA DIRECTAMENTE CON STD_OUTPUT DEL PPROCESS

								/*
											---		null	---				
								pProcess	|					|  Pipe 	
											---		fd[1]	---	

											---	STD_INPUT
								pProcess	|			
											---	fd[1]												

								fd[] = {null, fd[1]}
								*/

								//al finalizar el programa	
								close(fd[1]);
								//cerramos fd[1]
								//fd[] = {null, null}

								execl(process1, process1, 0);

							} else {
							 /* El proceso hijo ejecuta esta parte */
								
								close(fd[1]); 
								//cerramos el fd[1] del pipe en el hijo
								/*
											---	fd[0]	---
								Pipe 	   |  				| cProcess
											---	null	---

								fd[] = {fd[0], null}								
								*/

								dup2(fd[0],STD_INPUT);
								//duplicamos el fd[0] del pipe y lo ponemos en el proceso child
								//PARA ESTE EJEMPLO SE TRBAAJA DIRECTAMENTE CON STD_INPUT DEL CPROCESS

								/*
											---	fd[0]		---
								Pipe 	   |  					| cProcess
											---	null		---

												fd[0]		---
																| cProcess	
												STD_OUTPUT	---											

								fd[] = {fd[0], null}
								*/

								//al finalizar el programa	
								close(fd[0]);
								//cerramos fd[0]
								//fd[] = {null, null}

								execl(process2, process2, 0);
								
							}

							//RESULTADO PARA ESTE EJEMPLO
							/*
								STD_INPUT ---					---	null					---	fd[0]	---
											  	|	pProcess	|				|  Pipe |					| cProcess |
																---	fd[1]	---					null	---				--- STD_OUTPUT
							*/
						}


						
