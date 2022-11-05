module jar_sram_top 
# (
	parameter AW = 4, // address width
	parameter DW = 8, // data width
	parameter DEPTH = 1 << AW // number of bytes
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
	wire stream =  oe &  we;

	reg [AW-1:0] cnt;
	reg [DW-1:0] data_tmp;
	reg [DW-1:0] mem [DEPTH];

	always @(posedge clk) begin
		if (rst) begin
			cnt <= 0;
			//data_tmp <= 8'b0;
		end
		else if (write) begin
			case(cnt[1:0])
				2'b00: begin
					data_tmp[3:0] <= addr_data;
					cnt <= cnt + 1;
				end
				2'b01: begin
					data_tmp[7:4] <= addr_data;
					cnt <= cnt + 1;
				end
				2'b10: begin
					mem[addr_data] <= data_tmp;
					cnt <= 0;
				end
				default:;
			endcase
		end
		else if (read) begin
			data_tmp <= mem[addr_data];
		end
		else if (stream) begin
			data_tmp <= mem[cnt];
			cnt <= cnt + 1;
		end
	end

	assign io_out = (oe) ? data_tmp : 8'bz;
endmodule
