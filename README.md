# ModWars

The attacker's goal is to modify a value in the defender's memory without being
detected.

The defender's goal is to detect and stop the attacker at all costs.

Note that the attacker must have root privileges, but the defender must not.
Run `setuid.sh` to take root ownership of the `attacker` executable and set the
setuid bit.

Run the `judge` to initiate the battle.
