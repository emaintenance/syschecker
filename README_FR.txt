# Matthieu PERRIN - License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.
# AUCUNE GARANTIT / AUCUN SUPPORT


Syschecker est un ordonnaceur de supervision d'hote.

Description :

Syschecker est un logiciels pour les hotes.
Il execute periodiquement des plugins local.
Le résultat est envoyé avec send_nsca au serveur de supervsion.


Instalation :

- copier syschecker.cfg, syschecker.cmd et syschecker dans /usr/local/syschecker
- installer send_nsca
- installer les plugins nagios 
- editer syschecker.cfg avec les chemins de send_nsca, des plugins nagios , saisir l ip du serveur Nagios et le hostname  
- editer syschecker.cmd avec les plugins à execute
- copier le script init dans /etc/init.d/syschecker



Test with CentOS 5 / Nagios 3.2.3 / Centreon 2.6.1

