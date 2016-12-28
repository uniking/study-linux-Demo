/* packet-foo.c
 * Routines for Foo protocol packet disassembly
 * By zzq
 */

#include "config.h"

#include <epan/packet.h>
#include <epan/prefs.h>
//#include <epan/dissectors/packet-tcp.h>
#include "packet-foo.h"


#define FOO_PORT        9877
#define FOO_NAME        "Foo Protocol"
#define FOO_SHORT_NAME  "Foo"
#define FOO_ABBREV      "foo"


static int proto_foo = -1;

static int hf_foo_pdu_type = -1;
static int hf_foo_flags = -1;
static int hf_foo_seqno = -1;
static int hf_foo_ip = -1;
static gint ett_foo = -1;

static const value_string pkt_type_names[] = 
{
    {1, "Initilize"},
    {2, "Terminate"},
    {3, "Data"},
    {0, NULL}
};

#define FOO_START_FLAG  0x01
#define FOO_END_FLAG    0x02
#define FOO_PRIOR_FLAG  0x04

static int hf_foo_start_flag    = -1;
static int hf_foo_end_flag      = -1;
static int hf_foo_prior_flag    = -1;



void proto_register_foo(void);
void proto_reg_handoff_foo(void);
static int dissect_foo(tvbuff_t*, packet_info*, proto_tree*, void*);



void
proto_register_foo(void)
{
    static hf_register_info hf[] = 
    {
        {
            &hf_foo_pdu_type,
            {
                "Type", "foo.type",
                FT_UINT8, BASE_DEC,
                VALS(pkt_type_names), 0x0, 
                NULL, HFILL
            }
        },
        {
            &hf_foo_flags,
            {
                "Flags", "foo.flags",
                FT_UINT8, BASE_HEX, NULL, 0x0, NULL, HFILL
            }
        },
        {
            &hf_foo_start_flag,
            {
                "Start Flag", "foo.flags.start",
                FT_BOOLEAN, 8,
                NULL, FOO_START_FLAG, NULL, HFILL
            }
        },
        {
            &hf_foo_end_flag,
            {
                "End Flag", "foo.flags.end",
                FT_BOOLEAN, 8,
                NULL, FOO_END_FLAG, NULL, HFILL
            }
        },
        {
            &hf_foo_prior_flag,
            {
                "Priority Flag", "foo.flags.prior",
                FT_BOOLEAN, 8,
                NULL, FOO_PRIOR_FLAG, NULL, HFILL
            }
        },
        {
            &hf_foo_seqno,
            {
                "Sequence Number", "foo.seq",
                FT_UINT16, BASE_DEC,
                NULL, 0x0, NULL, HFILL
            }
        },
        {
            &hf_foo_ip,
            {
                "IP Address", "foo.ip",
                FT_IPv4, BASE_NONE,
                NULL, 0x0, NULL, HFILL
            }
        }
    };
    
    static gint *ett[] = { &ett_foo };

    
    proto_foo = proto_register_protocol (
            FOO_NAME,
            FOO_SHORT_NAME,
            FOO_ABBREV);
            
    proto_register_field_array(proto_foo, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));
}



static int
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data)
{
    guint8 packet_type = tvb_get_guint8(tvb, 0);
    
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
    /* Clear out stuff in the info column */
    col_clear(pinfo->cinfo,COL_INFO);
    col_add_fstr(pinfo->cinfo, COL_INFO, "Type %s",
        val_to_str(packet_type, pkt_type_names, "Unknown (0x%02x)"));
    
    /* proto details display */
    if(tree)
    {
        proto_item* ti = NULL;
        proto_tree* foo_tree = NULL;
        gint offset = 0;
        
        ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
        proto_item_append_text(ti, ", Type %s",
            val_to_str(packet_type, pkt_type_names, "Unknown (0x%02x)"));
        foo_tree = proto_item_add_subtree(ti, ett_foo);
        proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset += 1;
        proto_tree_add_item(foo_tree, hf_foo_flags, tvb, offset, 1, ENC_BIG_ENDIAN);
        proto_tree_add_item(foo_tree, hf_foo_start_flag, tvb, offset, 1, ENC_BIG_ENDIAN);
        proto_tree_add_item(foo_tree, hf_foo_end_flag, tvb, offset, 1, ENC_BIG_ENDIAN);
        proto_tree_add_item(foo_tree, hf_foo_prior_flag, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset += 1;
        proto_tree_add_item(foo_tree, hf_foo_seqno, tvb, offset, 2, ENC_BIG_ENDIAN);
        offset += 2;
        proto_tree_add_item(foo_tree, hf_foo_ip, tvb, offset, 4, ENC_BIG_ENDIAN);
        offset += 4;
    }

    return tvb_reported_length(tvb);
}

void
proto_reg_handoff_foo(void)
{
    static dissector_handle_t foo_handle;

    foo_handle = new_create_dissector_handle(dissect_foo, proto_foo);
    dissector_add_uint("udp.port", FOO_PORT, foo_handle);
}
