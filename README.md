# BE RESEAU
## TPs BE Reseau - 3 MIC


TAGS : 
- v1 -> Version 1 de mictcp
- v2 -> Version 2 de mictcp
- v3 -> Version 3 de mictcp

TESTS : 
- compiler -> make
- Test de la version texte : >> Machine 1 : ./tsock_texte -t mictcp -p
			     >> Machine 2 : ./tsock_texte -t mictcp -s
- Test de la version video : >> Machine 1 : ./tsock_video -t mictcp -p
			     >> Machine 2 : ./tsock_video -t mictcp -s
- (Remarque : Veuillez utiliser la vidéo Wildlife au lieu de Starwars pour les tests vidéo)
- Vous pouvez modifier la tolérance pour la fiabilité partielle en haut du fichier mictcp.c dans le dossier src
- Vous pouvez modifier le loss rate dans la fonction mic_tcp_socket en haut du fichier mictcp.c dans le dossier src			  
