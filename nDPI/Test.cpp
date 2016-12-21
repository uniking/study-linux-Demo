#include <pcap.h>
#include <stdlib.h>
#include <ndpi_main.h>

typedef struct _process_arg
{
	struct ndpi_detection_module_struct *pndpi_struct;
}process_arg, *pprocess_arg;

static void on_protocol_discovered(struct ndpi_workflow * workflow,
				   struct ndpi_flow_info * flow,
				   void * udata)
{
}

/**
 * @brief Check pcap packet
 */
static void pcap_packet_callback_checked(u_char *args,
					 const struct pcap_pkthdr *header,
					 const u_char *packet)
{
	pprocess_arg pa = (pprocess_arg)args;
  /* allocate an exact size buffer to check overflows */
  uint8_t *packet_checked = (uint8_t *)malloc(header->caplen);
  memcpy(packet_checked, packet, header->caplen);
  

	/*
	  ndpi_protocol ndpi_detection_process_packet(struct ndpi_detection_module_struct *ndpi_struct,
							  struct ndpi_flow_struct *flow,
							  const unsigned char *packet,
							  const unsigned short packetlen,
							  const u_int64_t current_tick,
							  struct ndpi_id_struct *src,
							  struct ndpi_id_struct *dst);
	*/

	struct ndpi_flow_struct flow;
	const unsigned short packetlen=10;
	const u_int64_t current_tick=10;
	struct ndpi_id_struct src;
	struct ndpi_id_struct dst;
	ndpi_protocol np = ndpi_detection_process_packet(pa->pndpi_struct,
							  &flow,
							  packet,
							  packetlen,
							  current_tick,
							  &src,
							  &dst);
}


int main(int argn, char* argv[])
{
	NDPI_PROTOCOL_BITMASK all;
	//struct ndpi_detection_module_struct *ndpi_init_detection_module(void);
	/*
		struct ndpi_detection_module_struct {
		  NDPI_PROTOCOL_BITMASK detection_bitmask;
		  NDPI_PROTOCOL_BITMASK generic_http_packet_bitmask;

		  u_int32_t current_ts;
		  u_int32_t ticks_per_second;

		#ifdef NDPI_ENABLE_DEBUG_MESSAGES
		  void *user_data;
		#endif
	*/
	struct ndpi_detection_module_struct* ndpi_struct;
	ndpi_struct = ndpi_init_detection_module();

	u_int16_t thread_id;
	ndpi_struct->http_dont_dissect_response = 0;
	ndpi_struct->dns_dissect_response = 0;


	/*
	  void ndpi_set_protocol_detection_bitmask2(struct ndpi_detection_module_struct *ndpi_struct,
					    const NDPI_PROTOCOL_BITMASK * detection_bitmask);
	*/
	NDPI_BITMASK_SET_ALL(all);
	ndpi_set_protocol_detection_bitmask2(ndpi_struct, &all);

	
	char pcap_error_buffer[PCAP_ERRBUF_SIZE];
	const u_char * pcap_file = (const u_char *)"test.pcap";
	//pcap_t *pcap_handle = pcap_open_live((char*)pcap_file, snaplen, promisc, 500, pcap_error_buffer)
	pcap_t *pcap_handle = pcap_open_offline((char*)pcap_file, pcap_error_buffer);
	pcap_loop(pcap_handle, -1, &pcap_packet_callback_checked, (u_char*)&thread_id);






	/*
	void ndpi_exit_detection_module(struct ndpi_detection_module_struct *ndpi_struct);
	*/
	ndpi_exit_detection_module(ndpi_struct);


	return 0;
}
