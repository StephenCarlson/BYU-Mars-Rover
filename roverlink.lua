p_roverlink = Proto ("roverlink","BYU RoverLink")
local f_command = ProtoField.uint16("roverlink.command", "Command", base.HEX)
local f_data = ProtoField.string("roverlink.data", "Data", FT_STRING)
 
p_roverlink.fields = {f_command}
 
-- roverlink dissector function
function p_roverlink.dissector (buf, pkt, root)
  -- validate packet length is adequate, otherwise quit
  if buf:len() == 0 then return end
  pkt.cols.protocol = p_roverlink.name
 
  -- create subtree for roverlink
  subtree = root:add(p_roverlink, buf(0))
  -- add protocol fields to subtree
  subtree:add(f_command, buf(0,2)):append_text(" [Command text]")
 
  -- description of payload
  subtree:append_text(", Command details here or in the tree below")
end
 
-- Initialization routine
function p_roverlink.init()
end
 
-- register a chained dissector for port 8002
local tcp_dissector_table = DissectorTable.get("tcp.port")
dissector = tcp_dissector_table:get_dissector(8002)
  -- you can call dissector from function p_roverlink.dissector above
  -- so that the previous dissector gets called
tcp_dissector_table:add(8002, p_roverlink)

-- Put the following lines in the init.lua script found inside the Wireshark directory:
-- MYPROTO_SCRIPT_PATH="C:\Users\Steve\Desktop\Projects\MarsRover\BYU-Mars-Rover\"
-- dofile(MYPROTO_SCRIPT_PATH.."roverlink.lua")