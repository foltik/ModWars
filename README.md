# ModWars

In this game there are two roles. The attacker and the defender, both processes running in userspace.
The attacker's goal is to make changes and modify behavior in the defender's program without being
detected.

The defender's goal is to detect and stop the attacker at all costs.

Note that the attacker must have root privileges, but the defender must not.
Run `setuid.sh` to take root ownership of the `attacker` executable and set the
setuid bit.

Run the `judge` to initiate the battle.
