`ifndef TOP_SV
`define TOP_SV
module top #(parameter DATA_WIDTH = 4,
             parameter ADDR_WIDTH = 4
            )(
            input clk,
            input cs,
            input we, 
            input [ADDR_WIDTH - 1 : 0] addr,
            input [DATA_WIDTH - 1 : 0] din,
            output [DATA_WIDTH - 1 : 0] dout
            );

  simple_ram DUT (
        .clk (clk),
        .cs (cs),
        .we (we),
        .addr (addr),
        .din (din),
        .dout (dout)
        );

    bind simple_ram Assertion #(
    .ADDR_WIDTH(4), .DATA_WIDTH(4), .DEPTH(16)
  ) des_asst_bind (
    .clk (clk),
    .cs (cs),
    .we (we),
    .addr (addr),
    .din (din), 
    .dout (dout)
  );
endmodule
`endif // TOP_SV