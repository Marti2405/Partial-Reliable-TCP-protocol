#include <mictcp.h>
#include <api/mictcp_core.h>


//Declaration socket
mic_tcp_sock socket_m ;

//Init numero ack reception et envoi
int num_seq_send = 0, num_seq_rec = 0;

/*
 * Permet de créer un socket entre l’application et MIC-TCP
 * Retourne le descripteur du socket ou bien -1 en cas d'erreur
 */
int mic_tcp_socket(start_mode sm)
{
    int result = -1;

    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    if (initialize_components(sm)==-1 ) return result; /* Appel obligatoire */
    set_loss_rate(3);

    mic_tcp_sock sock;
    sock.fd = 0;
    //sock.state = IDLE;

    socket_m = sock;
    result = sock.fd;

    return result;
}

/*
 * Permet d’attribuer une adresse à un socket.
 * Retourne 0 si succès, et -1 en cas d’échec
 */
int mic_tcp_bind(int socket, mic_tcp_sock_addr addr)
{
   printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
   /*
   //retrouver socket puis :
   socket_m.fd = socket;
   socket_m.state = IDLE;
   socket_m.addr = addr;*/
   return 0;
   
   
}
   

/*
 * Met le socket en état d'acceptation de connexions
 * Retourne 0 si succès, -1 si erreur
 */
int mic_tcp_accept(int socket, mic_tcp_sock_addr* addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    return 0;
}

/*
 * Permet de réclamer l’établissement d’une connexion
 * Retourne 0 si la connexion est établie, et -1 en cas d’échec
 */
int mic_tcp_connect(int socket, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    return 0;
}

/*
 * Permet de réclamer l’envoi d’une donnée applicative
 * Retourne la taille des données envoyées, et -1 en cas d'erreur
 */
int mic_tcp_send (int mic_sock, char* mesg, int mesg_size)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    int result = -1;
    mic_tcp_pdu pdu;
    pdu.header.seq_num = num_seq_send; //On met le numero de sequence
    mic_tcp_sock_addr addr_dest = socket_m.addr; 

    
    pdu.header.dest_port = socket_m.addr.port;
    //pdu.header.source_port = 1234;//Pourt ou on envoie l'ack dans processpdu
    
    pdu.payload.data=mesg;
    pdu.payload.size=mesg_size;
    //int nb_transmission_max = 0;

    while (result==-1 ){//&& nb_transmission_max<10
        //on envoie le message
        if (IP_send(pdu,addr_dest)==-1) printf("Erreur de IP_send, ligne :%d\n",__LINE__);
        
        unsigned long timer = 1000; //Le temps qu'on attends avant de renvoyer un message

        //mic_tcp_pdu* pdu_recu = malloc(sizeof(mic_tcp_pdu));
        //mic_tcp_sock_addr* addr_autre = malloc(sizeof(mic_tcp_sock_addr));

        mic_tcp_pdu pdu_recu;
        mic_tcp_sock_addr addr_autre;

        result = IP_recv(&pdu_recu,&addr_autre,timer);

        if(result!=-1){ //si on a reçu un packet
            if (pdu_recu.header.ack){//si c'est un acquittement
                if(pdu_recu.header.ack_num!=num_seq_send){ //si c'est pas le bon acquittement
                    result = -1;
                }
                printf("Ack recu\n");
            }else{//si ce n'est pas un paquet d'acquittement
                result = -1;
                printf("Pas paquet ack\n");
            }

        }else{
            printf("Ack non recu\n");
        }
        //sinon alors on est bon et on peut sortir de la boucle
        //nb_transmission_max++;
    }
    num_seq_send = (num_seq_send+1)%2;
    


    return result;
}

/*
 * Permet à l’application réceptrice de réclamer la récupération d’une donnée
 * stockée dans les buffers de réception du socket
 * Retourne le nombre d’octets lu ou bien -1 en cas d’erreur
 * NB : cette fonction fait appel à la fonction app_buffer_get()
 */
int mic_tcp_recv (int socket, char* mesg, int max_mesg_size)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    int result = -1 ;
    //retrouver socket ici socket_m
    mic_tcp_payload data_buf ;
    data_buf.data = mesg;
    data_buf.size = max_mesg_size;
    /*
    mic_tcp_sock_addr addr_rec = socket_m.addr;
    mic_tcp_pdu pdu;
    IP_recv(&pdu,&addr_rec,0.01);//Voir quoi mettre pour timeout
    */
    result = app_buffer_get(data_buf);

    //*mesg = (data_buf).data;

    
    return result;
}

/*
 * Permet de réclamer la destruction d’un socket.
 * Engendre la fermeture de la connexion suivant le modèle de TCP.
 * Retourne 0 si tout se passe bien et -1 en cas d'erreur
*/
int mic_tcp_close (int socket)
{
    printf("[MIC-TCP] Appel de la fonction :  "); printf(__FUNCTION__); printf("\n");
    //retrouer socket puis close dans state
    //socket_m.state = CLOSED;
    return 0;
}

/*
 * Traitement d’un PDU MIC-TCP reçu (mise à jour des numéros de séquence
 * et d'acquittement, etc.) puis insère les données utiles du PDU dans
 * le buffer de réception du socket. Cette fonction utilise la fonction
 * app_buffer_put().
 */
void process_received_PDU(mic_tcp_pdu pdu, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");

    mic_tcp_header header =pdu.header; 
    mic_tcp_payload payload = pdu.payload;

    //Check ack num du paquet reçu est correct
    if (header.seq_num ==num_seq_rec){
        app_buffer_put(payload);
        //creation d'un ack a envoyer
        mic_tcp_pdu pduack;
        mic_tcp_sock_addr addr_dest = addr; 
        //pduack.header.dest_port = pdu.header.source_port;
        pduack.header.ack = 1; //On active flag d'acquittement pour dire que c'est un ack
        pduack.header.ack_num = num_seq_rec; //On met numero qu'on acquitte

        pduack.payload.data="";
        pduack.payload.size=0;
        
        IP_send(pduack,addr_dest); // Envoi ack

        num_seq_rec = (num_seq_rec+1)%2; //Incréméntation num ack
    }

    //Si c'est pas le bon numéro on ne fait rien
    

}