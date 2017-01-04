# Matthieu PERRIN - License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.


Syschecker is a Host Monitoring Scheduler.


Description :

Syschecker is a host monitoring using scripts that are hosted on the local systems.
It run periodically local Nagios plugins.
The result is sent with send_nsca to the monitoring server.


Instalation :

- copy syschecker.cfg, syschecker.cmd and syschecker in /usr/local/syschecker
- install send_nsca
- install nagios plugins
- edit syschecker.cfg with send_nsca path, nagios plugins path, enter Nagios Poller IP and hostname  
- edit syschecker.cmd with plugins to execute
- copy init script in /etc/init.d/syschecker



Test with CentOS 5 / Nagios 3.2.3 / Centreon 2.6.1