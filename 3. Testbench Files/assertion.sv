`ifndef ASSERTION_SV
`define ASSERTION_SV
module Assertion #(
    parameter DATA_WIDTH = 4,
    parameter ADDR_WIDTH = 4,
    parameter DEPTH = 16
    ) (
    input logic cs,
    input logic we,
    input logic clk,
    input logic [ADDR_WIDTH - 1 : 0] addr,
    input logic [DATA_WIDTH - 1 : 0] din,
    input logic [DATA_WIDTH - 1 : 0] dout
);

    logic written_map[logic [ADDR_WIDTH - 1 : 0]];

    always @(posedge clk) begin
        if (cs && we) begin
            written_map[addr] = 1'b1;
        end
    end

    property stable_din;
        @(posedge clk)
        (cs && we) |-> din <= 4'(DEPTH - 1);
    endproperty
    a_stable_din : assert property(stable_din);
    c_stable_din : cover property(stable_din);

    property valid_address;
        @(posedge clk)
        cs |-> addr <= 4'(DEPTH - 1);
    endproperty
    a_valid_address : assert property(valid_address);
    c_valid_address : cover property(valid_address);

    property read_latency;
        @(posedge clk)
        (cs && !we && written_map.exists(addr)) |-> ##1 dout <= 4'(DEPTH - 1);
    endproperty
    a_read_latency : assert property(read_latency);
    c_read_latency : cover property(read_latency);
        
endmodule
`endif // ASSERTION_SV