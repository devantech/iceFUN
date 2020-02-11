/*
* (c) https://www.fpga4fun.com
*
  * ported to iceFun FPGA by Hirosh Dabui <hirosh@dabui.de>
  */
module top (
 input clk12MHz,
 output spkp,
 output spkm
);

wire clk;
assign spkp = speaker;
assign spkm = ~speaker;
// 25 MHz PLL
SB_PLL40_CORE #(
 .FEEDBACK_PATH("SIMPLE"),
 .DIVR(4'b0000),         // DIVR =  0
 .DIVF(7'b1000010),      // DIVF = 66
 .DIVQ(3'b101),          // DIVQ =  5
 .FILTER_RANGE(3'b001)   // FILTER_RANGE = 1
) uut (
 .LOCK(locked),
 .RESETB(1'b1),
 .BYPASS(1'b0),
 .REFERENCECLK(clk12MHz),
 .PLLOUTCORE(clk)
);


reg [27:0] tone;
always @(posedge clk) tone <= tone+1;

wire [6:0] fastsweep = (tone[22] ? tone[21:15] : ~tone[21:15]);
wire [6:0] slowsweep = (tone[25] ? tone[24:18] : ~tone[24:18]);
wire [14:0] clkdivider = {2'b01, (tone[27] ? slowsweep : fastsweep), 6'b000000};

reg [14:0] counter;
always @(posedge clk) if(counter==0) counter <= clkdivider; else counter <= counter-1;

reg speaker;
always @(posedge clk) if(counter==0) speaker <= ~speaker;

endmodule
