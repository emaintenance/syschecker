
# Matthieu PERRIN - License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "syschecker_utils.h"



FILE* ouvrir(char* chemin)
{
        FILE * fp;
        fp = fopen(chemin, "r");
        return fp;
}

int lire_ligne(char* ligne, int max, FILE* fp)
{

//        ssize_t read;
//      int max = sizeof ligne;
        int i=0;

        //read = getline(&ligne, &len, fp);

        do
        {
                ligne[i] = (char)fgetc(fp);
                if ( ligne[i] == EOF )
                        {
                        ligne[i+1] = '\0';
                        return -1;
                        }
                i++;
        }
        while ( (i<max) && (ligne[i-1]!='\n') );
        ligne[i] = '\0';
        ligne[i-1] = '\0'; // remove return line
        return 0;

}


void fermer(FILE* fp)
{
        fclose(fp);
}

int get_param(char* ligne, char* param, char* res)
{
        //int i=0;
        char* sub;
        sub = strstr(ligne, param);
        if ( sub != NULL )
                {
                sub = strstr(ligne, "=");
                //printf("%s", sub+1);
                strcpy( res, sub+1);
                //printf(" --- %s --- ", res);
                return 0;
                }
        return 1;
}

void get_config()
{

        if( access( config , F_OK ) != -1 )
        {
        FILE* f=ouvrir(config);
        char ligne[256];
        int len = sizeof ligne;

        while ( lire_ligne(ligne, len, f) != -1 )
                {
                //printf("%s", ligne);
                get_param(ligne, "IP_POLLER", ip_poller);
                get_param(ligne, "HOSTNAME", hostname);
                get_param(ligne, "CONDUITE_HOST", conduite_host);
                get_param(ligne, "PATH_LIBS", path_libs);
                get_param(ligne, "BIN_NSCA", bin_nsca);
                get_param(ligne, "NSCA_CFG", nsca_cfg);
                get_param(ligne, "OPT_NSCA", nsca_opt);
                get_param(ligne, "SEND_NSCA", send_nsca);
                }

        fermer(f);

        }
}


// copie dans res, l'element n de line separe par ch
void strcut(char *line, char *res, char ch, int n)
{
        char ligne[256];
        //strcpy(ligne, line);
        strncpy(ligne, line, sizeof ligne);
        const char c[2] = { ch, '\0' };

        char *token;

        token = strtok(ligne, c);

        int i=1;
        while ( token != NULL )
        {
                if ( i == n )
                {
                        strcpy(res, token);
                        //strncpy(res, token, sizeof *res);
                        //printf( " %d\n", sizeof *res);
                }
                token = strtok(NULL, c);
        i++;
        }
}

// copie dans res, l'element n de line separe par ch
void strcut_end(char *line, char *res, char ch, int n)
{
        char ligne[256];
        strncpy(ligne, line, sizeof ligne);
        const char c[2] = { ch, '\0' };

//printf("%s", ligne);
        char *token;
        token = strtok(ligne, c);
//printf("%s", ligne);

        int i=1;
        while ( token != NULL )
        {
                if ( i == n )
                {
                        strcpy(res, token);
                        //strcpy(res, ligne);
                }
                else if ( i > n )
                {
                        strcat(res, " ");
                        strcat(res, token);
                }
                token = strtok(NULL, c);
        i++;
        }
}

void strmulticat(char *out, int size, char **in)
{
        out[0] = '\0';
        int i=0;
        while ( in[i] != NULL )
        {
                if ( size - strlen(out) > strlen( in[i]) )
                strcat(out, in[i]);
                i++;
        }
}


FILE* pouvrir(char* chemin)
{
        FILE * fp;
        fp = popen(chemin, "r");
        return fp;
}

FILE* pouvrir_w(char* chemin)
{
        FILE * fp;
        fp = popen(chemin, "w");
        return fp;
}

int pfermer(FILE* fp)
{
        return WEXITSTATUS(pclose(fp));
	return 3;
}

int procout(char *cmd, char *sortie)
{
        char resultat[256];

        FILE* f=pouvrir(cmd);

        fgets(resultat, sizeof(resultat)-1, f);

        strcpy(sortie, resultat);

        return (pfermer(f) >> 8);
}

int procout_w(char *cmd, char *msg)
{
        //char resultat[256];

        FILE* f=pouvrir_w(cmd);

        //fgets(resultat, sizeof(resultat)-1, f);
        fputs(msg, f);

        //strcpy(sortie, resultat);

        return (pfermer(f) >> 8);
}

void process_result(int code, char *resultat, char *cmd)
{


        if ( test_mode == 1)
        {
        // mode affichage ecran
        int len = strlen(resultat);
        if ( resultat[len-1] == '\n' )  resultat[len-1] = '\0';

                printf("%s\n", cmd);
                if ( code == 0 )
                {
                        printf("[  OK  ] ");
                }
                else
                {
                        printf("[ ERROR ] ");
                }
                printf(" %s\n", resultat);
        }
        else
        {
        // mode envoi serveur

                char prog[256];

                char *list[10] = {  bin_nsca, " -H ", ip_poller, " -c ", nsca_cfg, " -d \";\"" };
                strmulticat(prog, sizeof(prog), list);


                //printf("MSG : %s\n", msg);
                //char r[256];
                procout_w(prog, resultat);
                //printf("%s\n", resultat);
                //printf("%s\n", prog);
        }
}

void run_command(char *frequence, char *actif, char *plugin, char *hote, char *service, char *param)
{
        int freq = ((int)(char)frequence[0])-48;
        int a = ((int)(char)actif[0])-48;

        int reste = minute%freq;
        //printf("M:%d, F:%d, R:%d\n", minute, freq, reste);

        int code=3;
        char resultat[256];
        char check[256];
        char cmd[256];

        char *list[10] = { plugin, " ", hote, " ", service, " ", param };
        strmulticat(check, sizeof(check), list);

        // Si minute d'execution
        if ( reste == 0 )
        {

                        if ( a == 1 )
                        {
                        // plugin actif
                        strcpy(cmd, path_libs);
                        strcat(cmd, "/activ2passiv ");
                        strcat(cmd, check);
                        }
                        else if ( strcmp(plugin, "echo") != 0 )
                        {
                        // plugin passif
                        strcpy(cmd, path_libs);
                        strcat(cmd, "/");
                        strcat(cmd, check);
                        }
                        else
                        {
                        // echo
                        strcpy(cmd, "/bin/");
                        strcat(cmd, plugin);
                        strcat(cmd, " \"");
                        strcat(cmd, hote);
                        strcat(cmd, service);
                        strcat(cmd, "\"");
                        }
                code = procout(cmd, resultat);
				
				char c[8]="";
				strcut(resultat, c, ';', 3);
				code = atoi(c);
                //printf("Cmd : %s\n", cmd);
                if ( debug_mode )
                printf("Code retour : %i, Sortie : %s\n", code, resultat);

                process_result(code, resultat, cmd);
        }

}

void process_command()
{

if( access( cmd, F_OK ) != -1 )
        {
        FILE* f=ouvrir(cmd);
        char ligne[256];
        int len = sizeof ligne;

        //char resultat[128]="";
        char frequence[128]="";
        char actif[128]="";
        char plugin[128]="";
        char hote[128]="";
        char service[128]="";
        char param[128]="";

        while ( lire_ligne(ligne, len, f) != -1 )
                {
                //printf("%s\n", ligne);
                strcut(ligne, frequence, ' ', 1);
                strcut(ligne, actif, ' ', 2);
                strcut(ligne, plugin, ' ', 3);
                strcut(ligne, hote, ' ', 4);
                strcut(ligne, service, ' ', 5);
                strcut_end(ligne, param, ' ', 6);

                if ( debug_mode )
                printf("Freq:%s, Actif:%s, Plugin:%s, Hote:%s, Service%s, Param: %s\n", frequence, actif, plugin, hote, service, param);

                run_command(frequence, actif, plugin, hote, service, param);
                }

        fermer(f);
        }
else
        {
        printf("File not found : %s", cmd);
        exit(3);
        }
}

void ptr_debug()
{
        printf("Poller : %s", ip_poller);
        printf("Hostname : %s", hostname);
        printf("Conduite : %s", conduite_host);
        printf("Path libs :  %s", path_libs);
        printf("Bin NSCA : %s", bin_nsca);
        printf("NSCA cfg : %s", nsca_cfg);
        printf("Send NSCA : %s", send_nsca);


}

int min_now()
{
        time_t theTime = time(NULL);
        struct tm *tm_struct = localtime(&theTime);
        return tm_struct->tm_min;
}

void usage(char *nom)
{
        printf("Usage : %s [-c config.cfg] [-e list.cmd] [-t] [-f] [-v]\n", nom);
        printf("\n\t-c \tfichier de configuration \n\t-e \tfichier de commandes (execute)\n\t-t \ttest mode \n\t-f \tForce l execution de tout les scripts \n\t-v \tVerbose\n");
        exit(1);
}

void opt(int argc, char** argv)
{

        int i=0;
        for ( i=1; i < argc; i++ )
        {
//      printf("OPT: %s\n" , argv[i]);
                if ( strcmp(argv[i], "-c")== 0 )
                {
                        // config file
                        i++;
                        if ( i < argc )
                        {
                                strncpy(config, argv[i], sizeof(config));
                                //printf("Config file : %s\n", config);
                        }
                }
                else if ( strcmp(argv[i], "-e")== 0 )
                {
                        // command file
                        i++;
                        if ( i < argc )
                        {
                                strncpy(cmd, argv[i], sizeof(cmd));
                                //printf("Cmd file : %s\n", cmd);
                        }
                }
                else if ( strcmp(argv[i], "-v")== 0 )
                {
                        // mode verbose
                        debug_mode=1;
                        //printf("Verbose on\n");
                }
                else if ( strcmp(argv[i], "-t")== 0 )
                {
                        // mode test
                        test_mode=1;
                        minute=0;
                        //printf("Test mode\n");
                }
                else if ( strcmp(argv[i], "-f")== 0 )
                {
                        // force
                        minute=0;
                        //printf("Force\n");
                }
                else
                {
                        usage(argv[0]);
                }
        }
}

