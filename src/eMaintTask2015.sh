#!/bin/bash
# Matthieu PERRIN (OAB) - Mai 2015 - eMaintenance
# License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.

source eMaintTask.cfg

# Format : Frequence actif plugin hote service arg1 arg2 arg3 etc...
cmd=$(cat eMaintTask.cmd)

#Mesure de la minute
MINUTE=$( echo -n $(/bin/date +%M) )

arg1="$1"
VERB=N
# Mode test et debug
if [ "$1" == "test" ] || [ "$1" = "v" ] || [ "$1" == "force" ]
then
        . /etc/init.d/functions
        [ "$1" = "v" ] && VERB=O
        MINUTE=0
fi

#Traitement de donnees
process()
{
        [ $VERB == O ] && echo "Fonction ${FUNCNAME[0]} de ${BASH_SOURCE[0]}"

        if [ "$arg1" == "test" ] && [ "$plugins" != "echo" ]
        then
                retour=$(echo $resultat | cut -d";" -f3)
                echo -n $service
                [ "$retour" -eq 0 ] && echo_success || echo_failure
                echo -e "\n"
        else
                echo "$resultat" | $SEND_NSCA
        fi
}

# Execution du plugin
execute()
{
        [ $VERB == O ] && echo "Fonction ${FUNCNAME[0]} de ${BASH_SOURCE[0]}"

        if [ "$actif" -eq "1" ]
        then
                resultat=$(bash $PATH_LIBS/activ2passiv $plugins $hote $service $arg)
                [ "$arg1" == "test" ] && echo "bash $PATH_LIBS/activ2passiv $plugins $hote $service $arg"
        else
                if [ "$plugins" == "echo" ]
                then
                        resultat=$($plugins $hote $service $arg)
                else
                        resultat=$(bash $PATH_LIBS/$plugins $hote $service $arg)
                        [ "$arg1" == "test" ] && echo "bash $PATH_LIBS/$plugins $hote $service $arg"
                fi
        fi

        process
}

# --- PROGRAMME ---

printf %s "$cmd" | while IFS= read -r line
do
   #echo "$line"
   min=$(echo $line | cut -d" " -f 1)
   actif=$(echo $line | cut -d" " -f 2)
   plugins=$(echo $line | cut -d" " -f 3)
   hote=$(echo $line | cut -d" " -f 4)
   service=$(echo $line | cut -d" " -f 5)
   arg=$(echo $line | cut -d" " -f6-)
   [ $VERB == O ] && echo -e " --- \nMin : $min, Plugin : $plugins, Hote : $hote, Service : $service, Arg : $arg"

   #[ $(((10#$min)%$MINUTE)) -eq 0 ] && execute
   [ $(((10#$MINUTE)%$min)) -eq 0 ] && execute

done
