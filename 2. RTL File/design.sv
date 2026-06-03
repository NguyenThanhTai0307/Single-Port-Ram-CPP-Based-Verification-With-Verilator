module simple_ram #(
parameter ADDR_WIDTH = 4,
parameter DATA_WIDTH = 4,
parameter DEPTH = 16
)
(
input clk,
input cs, we, 
input [ADDR_WIDTH - 1 : 0] addr,
input [DATA_WIDTH - 1 : 0] din,
output reg [DATA_WIDTH - 1 : 0] dout
);

reg [DATA_WIDTH - 1 : 0] mem [DEPTH];

always @ (posedge clk) begin
if (cs) begin
    if (we) 
        mem[addr] <= din;
    else 
        dout <= mem[addr];
    end
end
endmodule
