

#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#include <iostream>
#include <string>

int main()
{
   /* auto excontext = eXosip_malloc();
    auto ret = eXosip_init(excontext);
	if (eXosip_init(excontext) < 0)
	{
		std::cout << "SIP��ʼ��ʧ��" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}

	if (eXosip_listen_addr(excontext, IPPROTO_UDP, nullptr, 5061, AF_INET, 0) != OSIP_SUCCESS)
	{
		std::cout << "SIP����ʧ��" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}
	*/
	
    struct eXosip_t* context_eXosip;

    eXosip_event_t* je;
    osip_message_t* reg = NULL;
    osip_message_t* invite = NULL;
    osip_message_t* ack = NULL;
    osip_message_t* info = NULL;
    osip_message_t* message = NULL;

    int call_id, dialog_id;
    int i, flag;
    int flag1 = 1;

    const char* identity = "sip:140@127.0.0.1";   //UAC1���˿���15060
    const  char* registar = "sip:133@127.0.0.1:15061"; //UAS,�˿���15061
    const  char* source_call = "sip:140@127.0.0.1";
    const  char* dest_call = "sip:133@127.0.0.1:15061";
    //identify��register��һ���ַ�Ǻ�source��destination��ַ��ͬ��
    //����������У�uac��uasͨ�ţ���source�����Լ��ĵ�ַ����Ŀ�ĵ�ַ����uac1�ĵ�ַ
    char command;
    char tmp[4096];

    printf("r   �������ע��\n\n");
    printf("c   ȡ��ע��\n\n");
    printf("i   �����������\n\n");
    printf("h   �Ҷ�\n\n");
    printf("q   �Ƴ�����\n\n");
    printf("s   ִ�з���INFO\n\n");
    printf("m   ִ�з���MESSAGE\n\n");

    //��ʼ��
    auto ctx = eXosip_malloc();
    i = eXosip_init(ctx);

    if (i != 0)
    {
        printf("Couldn't initialize eXosip!\n");
        return -1;
    }
    else
    {
        printf("eXosip_init successfully!\n");
    }

    //��uac�Լ��Ķ˿�15060�������ж˿ڼ���
    i = eXosip_listen_addr(ctx,IPPROTO_UDP, NULL, 15060, AF_INET, 0);
    if (i != 0)
    {
        eXosip_quit(ctx);
        fprintf(stderr, "Couldn't initialize transport layer!\n");
        return -1;
    }
    flag = 1;

    while (flag)
    {
        //��������
        printf("Please input the command:\n");
        scanf("%c", &command);
        getchar();

        switch (command)
        {
        case 'r'://ע��
        {
         
            osip_message_t* reg = NULL;
            int i;

            eXosip_lock(ctx);

            auto regid = eXosip_register_build_initial_register(ctx, source_call, dest_call, nullptr, 1800, &reg);

            if (regid < 1) {
                eXosip_unlock(ctx);
                std::cout << "eXosip_register_build_initial_register failed" << std::endl;
                exit(1);
            }
            osip_message_set_supported(reg, "100rel");
            osip_message_set_supported(reg, "path");
            i = eXosip_register_send_register(ctx,regid, reg);

            eXosip_unlock(ctx);
            break;
        }
        case 'i'://INVITE�������������
            i = eXosip_call_build_initial_invite(ctx,&invite, dest_call, source_call, NULL, "This is a call for conversation");
            if (i != 0)
            {
                printf("Initial INVITE failed!\n");
                break;
            }
            //����SDP��ʽ����������a���Զ����ʽ��Ҳ����˵���Դ���Լ�����Ϣ��
            //����ֻ�������У������ʻ���Ϣ
            //���Ǿ������ԣ���ʽvot�ز����٣�ԭ��δ֪��������Э��ջ�ڴ���ʱ��Ҫ����
            snprintf(tmp, 4096,
                "v=0\r\n"
                "o=anonymous 0 0 IN IP4 0.0.0.0\r\n"
                "t=1 10\r\n"
                "a=username:rainfish\r\n"
                "a=password:123\r\n");

            osip_message_set_body(invite, tmp, strlen(tmp));
            osip_message_set_content_type(invite, "application/sdp");

            eXosip_lock(ctx);
            i = eXosip_call_send_initial_invite(ctx,invite); //invite SIP INVITE message to send
            eXosip_unlock(ctx);

            //������INVITE��Ϣ���ȴ�Ӧ��
            flag1 = 1;
            while (flag1)
            {
                je = eXosip_event_wait(ctx,0, 2000); //Wait for an eXosip event
                //(��ʱʱ���룬��ʱʱ�����)
                if (je == NULL)
                {
                    printf("No response or the time is over!\n");
                    break;
                }
                switch (je->type)   //���ܻᵽ�����¼�����
                {
                case EXOSIP_CALL_INVITE:   //�յ�һ��INVITE����
                    printf("a new invite received!\n");
                    break;
                case EXOSIP_CALL_PROCEEDING: //�յ�100 trying��Ϣ����ʾ�������ڴ�����
                    printf("proceeding!\n");
                    break;
                case EXOSIP_CALL_RINGING:   //�յ�180 RingingӦ�𣬱�ʾ���յ�INVITE�����UAS�����򱻽��û�����
                    printf("ringing!\n");
                    printf("call_id is %d,dialog_id is %d \n", je->cid, je->did);
                    break;
                case EXOSIP_CALL_ANSWERED: //�յ�200 OK����ʾ�����Ѿ����ɹ����ܣ��û�Ӧ��
                    printf("ok!connected!\n");
                    call_id = je->cid;
                    dialog_id = je->did;
                    printf("call_id is %d,dialog_id is %d \n", je->cid, je->did);

                    //����ackӦ����Ϣ
                    eXosip_call_build_ack(ctx,je->did, &ack);
                    eXosip_call_send_ack(ctx,je->did, ack);
                    flag1 = 0; //�Ƴ�Whileѭ��
                    break;
                case EXOSIP_CALL_CLOSED: //a BYE was received for this call
                    printf("the other sid closed!\n");
                    break;
                case EXOSIP_CALL_ACK: //ACK received with 200ok to INVITE  (���Ƿ���INVITE���յ��˶Ը�INVITE��ACK)
                    printf("ACK received!\n");
                    break;
                default: //�յ�����Ӧ��
                    printf("other response!\n");
                    break;
                }
                eXosip_event_free(je); //Free ressource in an eXosip event
            }
            break;

        case 'h':   //�Ҷ�
            printf("Holded!\n");

            eXosip_lock(ctx);
            eXosip_call_terminate(ctx,call_id, dialog_id);
            eXosip_unlock(ctx);
            break;

        case 'c':
            printf("This modal is not commpleted!\n");
            break;

        case 's': //����INFO����
            eXosip_call_build_info(ctx,dialog_id, &info);
            snprintf(tmp, 4096, "\nThis is a sip message(Method:INFO)");
            osip_message_set_body(info, tmp, strlen(tmp));
            //��ʽ���������趨��text/plain�����ı���Ϣ;
            osip_message_set_content_type(info, "text/plain");
            eXosip_call_send_request(ctx,dialog_id, info);
            break;

        case 'm':
        {
            //����MESSAGE������Ҳ���Ǽ�ʱ��Ϣ����INFO������ȣ�����Ϊ��Ҫ�����ǣ�
           //MESSAGE���ý������ӣ�ֱ�Ӵ�����Ϣ����INFO��Ϣ�����ڽ���INVITE�Ļ����ϴ���
            printf("the method : MESSAGE\n");
            eXosip_message_build_request(ctx, &message, "MESSAGE", dest_call, source_call, NULL);
            //���ݣ�������      to       ��from      ��route
            snprintf(tmp, 4096, "This is a sip message(Method:MESSAGE)");
            osip_message_set_body(message, tmp, strlen(tmp));
            //�����ʽ��xml
            osip_message_set_content_type(message, "text/xml");
            auto a = eXosip_message_send_request(ctx, message);
            std::cout << "a: " << a << std::endl;
            break;
        }
        case 'q':
            eXosip_quit(ctx);
            printf("Exit the setup!\n");
            flag = 0;
            break;
        }
    }

    return 0;
}