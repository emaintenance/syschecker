
# Matthieu PERRIN - License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.


#ifndef SYSCHECKER_UTILS_H_INCLUDED
#define SYSCHECKER_UTILS_H_INCLUDED

char config[128];
char cmd[128];

char ip_poller[32];
char hostname[128];
char conduite_host[128];
char path_libs[128];
char bin_nsca[128];
char nsca_cfg[128];
char nsca_opt[128];
char send_nsca[128];

int minute;
int test_mode;
int debug_mode;



FILE* ouvrir(char* chemin);
int lire_ligne(char* ligne, int max, FILE* fp);
void fermer(FILE* fp);
int get_param(char* ligne, char* param, char* res);
void get_config();
void strcut(char *line, char *res, char ch, int n);
void strcut_end(char *line, char *res, char ch, int n);
void strmulticat(char *out, int size, char **in);
FILE* pouvrir(char* chemin);
FILE* pouvrir_w(char* chemin);
int pfermer(FILE* fp);
int procout(char *cmd, char *sortie);
int procout_w(char *cmd, char *msg);
void process_result(int code, char *resultat, char *cmd);
void run_command(char *frequence, char *actif, char *plugin, char *hote, char *service, char *param);
void process_command();
void ptr_debug();
int min_now();
void usage(char *nom);
void opt(int argc, char** argv);


#endif
