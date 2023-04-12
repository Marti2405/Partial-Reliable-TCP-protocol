#include <mictcp.h>
#include <api/mictcp_core.h>
#define TOLERANCE 5

//Declaration socket
mic_tcp_sock socket_m ;

//Init numero ack reception et envoi
int num_seq_send , num_seq_rec ,connecok=-1;


int tolerance_perte = TOLERANCE; //-> tous les combien de paquets on accepte la perte
//compteur fenentre pour fiabilité partielle
int compteur = TOLERANCE; 




/*
 * Permet de créer un socket entre l’application et MIC-TCP
 * Retourne le descripteur du socket ou bien -1 en cas d'erreur
 */
int mic_tcp_socket(start_mode sm)
{
    int result = -1;

    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    if (initialize_components(sm)==-1 ) return result; /* Appel obligatoire */
    set_loss_rate(10);

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
    
    int result = -1;

    while (result==-1){
        if (connecok == 1){
            result = 0;
        }
        sleep(0.2);
    }
    printf("Connexion aceptee\n");

    return 0;
}

/*
 * Permet de réclamer l’établissement d’une connexion
 * Retourne 0 si la connexion est établie, et -1 en cas d’échec
 */
int mic_tcp_connect(int socket, mic_tcp_sock_addr addr)
{   

    mic_tcp_pdu pdu_recu;
    mic_tcp_sock_addr addr_autre;

    num_seq_send = 0;

    //creation d'un Syn a envoyer
    mic_tcp_pdu pdusyn;

    
    pdusyn.payload.data="";
    pdusyn.payload.size=0;

    pdusyn.header.ack_num = num_seq_send; 
    pdusyn.header.seq_num = 0;
    pdusyn.header.syn = 1;
    pdusyn.header.ack = 0;
    

    pdusyn.header.dest_port = htons(API_CS_Port);
    pdusyn.header.source_port = htons(API_CS_Port);
    

    
    int result = -1;

    sleep(1);

    while (result==-1){
        //on envoie le message
        if (IP_send(pdusyn,addr) == -1) printf("Erreur send ack\n"); // Envoi syn
        printf("Syn envoyé\n");


        unsigned long timer = 500; //Le temps qu'on attends avant de renvoyer un message


        

        result = IP_recv(&pdu_recu,&addr_autre,timer); //reception du syn+ack

        if (result!=-1){ //si on a reçu 

            
            if(pdu_recu.header.seq_num!=0 && pdu_recu.header.ack_num==(num_seq_send+1)%2){//si on a pas reçu le bon ack
                result = -1;
            }else {
                printf("Synack reçu\n");
            }

        }
  
    }

    
    //creation d'un Ack a envoyer
    
    
    pdusyn.payload.data="";
    pdusyn.payload.size=0;

    pdusyn.header.ack_num = pdu_recu.header.ack_num; 
    pdusyn.header.seq_num = pdu_recu.header.seq_num+1;
    pdusyn.header.ack = 1;

    pdusyn.header.dest_port = htons(API_CS_Port);
    pdusyn.header.source_port = htons(API_CS_Port);
    
    if (IP_send(pdusyn,addr) == -1) printf("Erreur send ack\n"); // Envoi ack


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
    addr_dest.ip_addr = "localhost";
    addr_dest.ip_addr_size = strlen(addr_dest.ip_addr) +1;
    addr_dest.port = htons(API_CS_Port);

    pdu.header.dest_port = htons(API_CS_Port);
    pdu.header.source_port = htons(API_CS_Port);
    
    pdu.header.dest_port = socket_m.addr.port;
    //pdu.header.source_port = 1234;//Pourt ou on envoie l'ack dans processpdu
    
    pdu.payload.data=mesg;
    pdu.payload.size=mesg_size;
    

    
    

    while (result==-1){
        //on envoie le message
        if (IP_send(pdu,addr_dest)==-1) printf("Erreur de IP_send, ligne :%d\n",__LINE__);
        
        unsigned long timer = 10; //Le temps qu'on attends avant de renvoyer un message


        mic_tcp_pdu pdu_recu;
        mic_tcp_sock_addr addr_autre;

        result = IP_recv(&pdu_recu,&addr_autre,timer);

        if(result!=-1 && pdu_recu.header.ack_num!=num_seq_send){ //si on a reçu un packet mais c'est pas le bon ack
            result = -1;
        }

        

        if (result==-1 ){//si on a une perte de paquet
            
            if (compteur==tolerance_perte){
                compteur =0;
                result = 0;
                printf("Perte tolérée\n");
                num_seq_send = (num_seq_send+1)%2;
            } else {
                printf("Perte non tolérée\n");
            }
        }else{//sinon on augmente le compteur si il est en dessous de tolerance
            
            if(compteur<tolerance_perte){
                compteur++;
                //printf("Bien reçu, on incrémente le compteur\n");
            } else if(compteur==tolerance_perte){
                printf("On est prets a tolerer\n");
            }
        }
        //sinon alors on est bon et on peut sortir de la boucle
    }
    
    
    


    return result;
}

/*
 * Permet à l’application réceptrice de réclamer la récupération d’une donnée
 * stockée dans les buffers de réception du socket
 * Retourne le nombre d’octets lu ou bien -1 en cas d’erreur
 * NB : cette fonction fait appel à la fonction app_buffer_get()
 */
int mic_tcp_recv (int socket, char* mesg, int max_mesg_size){
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

    
    if (pdu.header.syn==1 && pdu.header.ack==0){

        printf("Syn reçu\n");


        //creation d'un Syn+Ack a envoyer
        mic_tcp_pdu pdusynack;

        
        pdusynack.payload.data="";
        pdusynack.payload.size=0;


        pdusynack.header.dest_port = htons(API_CS_Port);
        pdusynack.header.source_port = htons(API_CS_Port);
        

        pdusynack.header.ack_num = pdu.header.ack_num+1; 
        pdusynack.header.seq_num = pdu.header.seq_num;
        pdusynack.header.ack = 1;
        pdusynack.header.syn = 1;

        //Convention pour commencer par le numero d'ack reçu
        num_seq_rec = pdu.header.ack_num;

        
        //envoi synAck
        if (IP_send(pdusynack,addr) == -1) printf("Erreur send Synack\n"); 
        printf("SynAck envoyé\n");

        connecok = 1;
        

        
        
        

    }else{
        //Check ack num du paquet reçu est correct
        if (pdu.header.seq_num ==num_seq_rec){
            
            app_buffer_put(pdu.payload);
            num_seq_rec = (num_seq_rec+1)%2; //Incréméntation num ack
            
        }

        //creation d'un ack a envoyer
        mic_tcp_pdu pduack;

        pduack.payload.data="";
        pduack.payload.size=0;

        pduack.header.ack_num = pdu.header.seq_num;

        pduack.header.dest_port = htons(API_CS_Port);
        pduack.header.source_port = htons(API_CS_Port);

        if (IP_send(pduack,addr) == -1) printf("Erreur send ack\n"); // Envoi ack
    }

    
    

    

}