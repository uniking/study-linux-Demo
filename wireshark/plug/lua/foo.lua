-- @brief Foo Protocol dissector plugin
-- @author zzq
-- @date 2015.08.12

-- register this dissector
DissectorTable.get("udp.port"):add(PORT, foo)

-- create fields of foo
local fields = foo.fields
fields.type = ProtoField.uint8 (NAME .. ".type", "Type")
fields.flags = ProtoField.uint8 (NAME .. ".flags", "Flags")
fields.seqno = ProtoField.uint16(NAME .. ".seqno", "Seq No.")
fields.ipaddr = ProtoField.ipv4(NAME .. ".ipaddr", "IPv4 Address")

-- create a new dissector
local NAME = "foo"
local PORT = 9877
local foo = Proto(NAME, "Foo Protocol")

-- dissect packet
function foo.dissector (tvb, pinfo, tree)
    local subtree = tree:add(foo, tvb())
    local offset = 0
    
    -- show protocol name in protocol column
    pinfo.cols.protocol = foo.name
    
    -- dissect field one by one, and add to protocol tree
    local type = tvb(offset, 1)
    subtree:add(fields.type, type)
    subtree:append_text(", type: " .. type:uint())
    offset = offset + 1
    
    subtree:add(fields.flags, tvb(offset, 1))
    offset = offset + 1
    subtree:add(fields.seqno, tvb(offset, 2))
    offset = offset + 2
    subtree:add(fields.ipaddr, tvb(offset, 4))
end
