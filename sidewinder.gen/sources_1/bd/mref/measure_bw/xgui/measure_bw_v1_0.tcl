# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "ADDRESS_OFFSET" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "AXI_DATA_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "MAX_OUTSTANDING_RREQ" -parent ${Page_0}
  ipgui::add_param $IPINST -name "MAX_OUTSTANDING_WREQ" -parent ${Page_0}


}

proc update_PARAM_VALUE.ADDRESS_OFFSET { PARAM_VALUE.ADDRESS_OFFSET } {
	# Procedure called to update ADDRESS_OFFSET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ADDRESS_OFFSET { PARAM_VALUE.ADDRESS_OFFSET } {
	# Procedure called to validate ADDRESS_OFFSET
	return true
}

proc update_PARAM_VALUE.AXI_ADDR_WIDTH { PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to update AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_ADDR_WIDTH { PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to validate AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.AXI_DATA_WIDTH { PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to update AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AXI_DATA_WIDTH { PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to validate AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.MAX_OUTSTANDING_RREQ { PARAM_VALUE.MAX_OUTSTANDING_RREQ } {
	# Procedure called to update MAX_OUTSTANDING_RREQ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MAX_OUTSTANDING_RREQ { PARAM_VALUE.MAX_OUTSTANDING_RREQ } {
	# Procedure called to validate MAX_OUTSTANDING_RREQ
	return true
}

proc update_PARAM_VALUE.MAX_OUTSTANDING_WREQ { PARAM_VALUE.MAX_OUTSTANDING_WREQ } {
	# Procedure called to update MAX_OUTSTANDING_WREQ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MAX_OUTSTANDING_WREQ { PARAM_VALUE.MAX_OUTSTANDING_WREQ } {
	# Procedure called to validate MAX_OUTSTANDING_WREQ
	return true
}


proc update_MODELPARAM_VALUE.ADDRESS_OFFSET { MODELPARAM_VALUE.ADDRESS_OFFSET PARAM_VALUE.ADDRESS_OFFSET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ADDRESS_OFFSET}] ${MODELPARAM_VALUE.ADDRESS_OFFSET}
}

proc update_MODELPARAM_VALUE.MAX_OUTSTANDING_RREQ { MODELPARAM_VALUE.MAX_OUTSTANDING_RREQ PARAM_VALUE.MAX_OUTSTANDING_RREQ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MAX_OUTSTANDING_RREQ}] ${MODELPARAM_VALUE.MAX_OUTSTANDING_RREQ}
}

proc update_MODELPARAM_VALUE.MAX_OUTSTANDING_WREQ { MODELPARAM_VALUE.MAX_OUTSTANDING_WREQ PARAM_VALUE.MAX_OUTSTANDING_WREQ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MAX_OUTSTANDING_WREQ}] ${MODELPARAM_VALUE.MAX_OUTSTANDING_WREQ}
}

proc update_MODELPARAM_VALUE.AXI_DATA_WIDTH { MODELPARAM_VALUE.AXI_DATA_WIDTH PARAM_VALUE.AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.AXI_ADDR_WIDTH { MODELPARAM_VALUE.AXI_ADDR_WIDTH PARAM_VALUE.AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.AXI_ADDR_WIDTH}
}

