module jar_sram_top 
# (
	parameter AW = 4, // address width
	parameter DW = 8, // data width
	parameter DEPTH = 8 // number of bytes
)
(
	input  [DW-1:0] io_in,
	output [DW-1:0] io_out
);
	// Shared address and data input.
	// When writing, low data bits first, then high bits, then address
	wire [AW-1:0] addr_data = io_in[DW-1:DW-AW];
	wire             commit = io_in[3]; // Commit to memory
	wire             oe     = io_in[2]; // Output Enable
	wire             we     = io_in[1]; // Write Enable
	wire             clk    = io_in[0]; // Clock

	reg [DW-1:0] data_tmp;
	reg [DW-1:0] mem [DEPTH];

	wire    [2:0] addr = addr_data[2:0];
	wire [AW-1:0] data = addr_data[AW-1:0];

	always @(posedge clk) begin
		if (we) begin
			data_tmp <= {addr_data, data_tmp[DW-1:AW]};
		end
		if (commit) begin
			mem[addr] <= data_tmp;
		end
		if (oe) begin
			data_tmp <= mem[addr];
		end
	end

	assign io_out = (oe) ? data_tmp : 8'bz;
endmodule
