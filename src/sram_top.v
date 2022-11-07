module jar_sram_top 
# (
	parameter AW = 4, // address width
	parameter DW = 8, // data width
	parameter DEPTH = 8 // number of bytes
)
(
	input [DW-1:0] io_in,
	output [DW-1:0] io_out
);
	// Shared address and data input.
	// When writing, low data bits first, then high bits, then address
	wire [AW-1:0] addr_data = io_in[DW-1:DW-AW];
	wire oe  = io_in[3]; // Output Enable
	wire we  = io_in[2]; // Write Enable
	wire rst = io_in[1]; // Reset
	wire clk = io_in[0]; // Clock

	// Modes
	wire write  = !oe &  we;
	wire read   =  oe & !we;

	reg lo;
	reg hi;
	reg [DW-1:0] data_tmp;
	reg [DW-1:0] mem [DEPTH];

	wire [2:0] addr = addr_data[2:0];

	always @(posedge clk) begin
		if (rst) begin
			lo <= 0;
			hi <= 0;
		end
		else if (write) begin
			if (!lo) begin
				data_tmp[3:0] <= addr_data;
				lo <= 1;
			end
			else if (!hi) begin
				data_tmp[7:4] <= addr_data;
				hi <= 1;
			end
			else begin
				mem[addr] <= data_tmp;
				lo <= 0;
				hi <= 0;
			end
		end
		else if (read) begin
			data_tmp <= mem[addr];
		end
	end

	assign io_out = (read) ? data_tmp : 8'bz;
endmodule
