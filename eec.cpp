#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include<unistd.h>
#include<iostream>

#include<stdbool.h>
#include<cstring>
#include<stdlib.h>
#include <signal.h>
#include <pwd.h>
#include <map>
#include <string>


using namespace std;
char *s[100];
int pipefd[1000][2];
int ind,outd,ind1,outd1;
char *filename;
int fd,chid,ampercet;
int l,pipeindex,status;
char *home,*user;
map<string,string> m1;
int flag_alias = 0;
bool quit(string str)
{
     if(str=="exit")
	return true;
}

void  change(char *dir)
{
if(strcmp(dir,"~") == 0)
	chdir(home);
	chdir(dir);
}

void ctrlchandler(int signo)
{
	printf("killed\n");
	kill(chid, SIGKILL);
}
void ctrlzhandler(int signo)
{
	printf("stopped\n");
	kill(chid, SIGSTOP);
	
}
void store(string k, string k1)
{
	
        m1[k] = k1;
	
}
string find(string temp)
{

	auto it = m1.find(temp);
	if(m1.end()==it)
	return "no";	
	else
	return it->second;
}
int read(char **s) 
{
 
		signal(SIGINT, ctrlchandler);
		signal(SIGTSTP, ctrlzhandler);
	 char* c, *c1;

 	 string input;
		
  
 	 int cont = 0,pid;
		while(1) 
		{
			cin>>input;
			
  			
	        		
			if (quit(input)) 
				{
					exit(1);
				}
			c = new char[input.size()+1];
			strcpy(c, input.c_str());
			s[cont] = c;
			cont++;
        	        if (cin.get() == '\n')
      			  break; 
			
  		}

  
 s[cont] = NULL;
   if(strcmp(s[0],"alias")==0)
	{
		store(string(s[1]),string(s[3]));
		flag_alias = 1;
        }
   else if("no" != find(string(s[0])))
	{
		string i = find(string(s[0]));
                c1 = new char[i.size()+1];
		strcpy(c1, i.c_str());
	        s[0] = c1; 
	        
	
	}
	return cont;

}


void  execute(char **s)
{

		signal(SIGINT, ctrlchandler);
		signal(SIGTSTP, ctrlzhandler);

			int pid;
                        pipe(pipefd[l]);
			pid = fork();
			
			if(strcmp(s[0], "fg") == 0)
			{
				ampercet = 0;
				kill(chid, SIGCONT);
			}
			if(strcmp(s[0], "bg") == 0)
			{
				ampercet = 1;
				kill(chid, SIGCONT);
			}
			if(pid==0)
			{


				if(l == 0 && pipeindex - 1 == 0)
				{
					
				
				}	
				else if(l == 0 && pipeindex - 1 != 0)
				{
					close(1);
					close(pipefd[l][0]);
					dup2(pipefd[l][1], 1);
					
				}
				else if(l != 0 && l < pipeindex - 1)
				{
					close(0);	
					close(pipefd[l - 1][1]);
					dup2(pipefd[l - 1][0], 0);
					close(1);
					close(pipefd[l][0]);
					dup2(pipefd[l][1], 1);
					
					
				}
				else if(l == pipeindex - 1)
				{
					close(0);
					close(pipefd[l - 1][1]);
					dup2(pipefd[l - 1][0], 0);
					
				}
				if(ind == 1)
				{
					close(0);
					fd = open(filename, O_RDONLY);
			
				}
				else if(outd == 1)
				{
					close(1);
                                        fd = open(filename, O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
				}
				else if(ind1 == 1)
				{
					close(0);
					fd = open(filename, O_RDONLY);
			
				}
				else if(outd1 == 1)
				{
					close(1);
                                        fd = open(filename, O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
				}
		               
					

				execvp(s[0],s);
			
			
			} 
		else {
				chid = pid;
				if (ampercet == 0)
					{
						waitpid(pid, &status, WUNTRACED);
					}
				else
				{
					printf("[1] %d\n", chid);
					waitpid(pid, &status, WNOHANG);
					
				}
				close(pipefd[l][1]);
			}
	
		



}
void initial()
{
	struct passwd *pwd =getpwuid(getuid());

	home = pwd->pw_dir;
	user = pwd->pw_name;
	setenv("HOME",home,1);
	setenv("USER",user,1);
	



}
void check(char **s)
{
	int i=0;

		signal(SIGINT, ctrlchandler);
		signal(SIGTSTP, ctrlzhandler);


                //c[i]=strtok(s," ");            
    		while(s[i] != NULL)
			{
				if(strcmp(s[i], "&") == 0)
				{
					s[i] = NULL;
					ampercet = 1;
				}
				else if(strcmp(s[i], "<") == 0)
				{
					filename=s[i+1];
					ind = 1;
					s[i] = NULL;
				}
				else if(strcmp(s[i], ">") == 0)
				{
					filename = s[i+1];
					outd = 1;
					s[i] = NULL;
				}
				else if(strcmp(s[i], ">>")==0)
                                {
					filename = s[i+1];
					outd1 = 1;
					s[i]  = NULL;
				}
				else if(strcmp(s[i], "<<")==0)
				{
					filename = s[i+1];
					ind1 =  1;
					s[i] = NULL;					
					
				}
				else if(strcmp(s[i],"cd")==0)
				{
					change(s[i+1]);
				}
				
				else
					i++;
					
			}

}
int main()
{
	initial();

	while(1)
	{
		char *s[1000];
		char *subcommand[1000];
		if (getuid() != 0)
		{
			cout<<"@gsmodi-Lenovo-IdeaPad-S540-15IWL-D:$";

		} 
		else 
		{
			cout<<"@gsmodi-Lenovo-IdeaPad-S540-15IWL-D:#";
		}
		


		signal(SIGINT, ctrlchandler);
		signal(SIGTSTP, ctrlzhandler);

	pipeindex = 1;
	l =0;
	int size =read(s);
	
if(flag_alias != 1)
{
	while(s[l] != NULL)
		{	
			if(strcmp(s[l],"|")==0)
			{	
				pipeindex++;
			}
			l++;	
		}
  	int j=0,i=0;
        for(l=0;l<pipeindex;l++)
	{	
                i =0;
          
		while( s[j] != NULL && strcmp(s[j],"|") != 0)
		{
		    	subcommand[i] = s[j];
				i++;
				j++;	
		}
		subcommand[i]=NULL;
		if(s[j] != NULL)
			{
			     j++;						
									
			}
		if(strcmp(subcommand[0],"cd")==0)
			change(subcommand[1]);
		else if(strcmp(subcommand[0],"alias")==0)
			store(subcommand[1],subcommand[3]);
		else
		{
		       check(subcommand);
		       execute(subcommand);
			cout<<endl;
			ind = 0;
			outd =0;
			ind1 = 0;
			outd1 =0;
			ampercet=0;
		}


	}

}
flag_alias=0;
 
}
return 0;
 }
