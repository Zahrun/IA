# IA

##### Instructions pour pouvoir jouer avec la dernière version du prof
sudo apt-get install ocaml ocaml-findlib libcurses-ocaml-dev xterm  
virer ce qu'il y a devant ocamlfind et ocamlbuild dans client/Makefile et server/Makefile  
sudo ln /bin/bash /usr/bin/bash

script/execute-ia-player.sh 1

##### Instructions pour lancer le jeu avec notre IA
1) Faire un raccourcis de l'éxecutable de notre projet dans le dossier empire-captain (ou le copier)  
ln -s /wathever/IA /wathever/empire-captain/IA  
2) ai42.py à copier dans le dossier empire-captain  
3) dans empire-scripts, exécuter "./execute-ia-player.sh 42" pour jouer à la main contre notre IA ou "./execute-ia.sh 42 1" pour faire jouer contre l'IA du prof

